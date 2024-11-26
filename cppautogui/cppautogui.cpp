#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <tuple>
#include <cmath>
#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cassert>

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#elif __linux__
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif

class CppAutoGUIException : public std::runtime_error {
public:
    explicit CppAutoGUIException(const std::string& message) : std::runtime_error(message) {}
};

class FailSafeException : public CppAutoGUIException {
public:
    explicit FailSafeException(const std::string& message) : CppAutoGUIException(message) {}
};

class ImageNotFoundException : public CppAutoGUIException {
public:
    explicit ImageNotFoundException(const std::string& message) : CppAutoGUIException(message) {}
};

#ifdef _WIN32
class WindowsPlatform {
public:
    static std::tuple<int, int> getPosition() {
        POINT p;
        if (GetCursorPos(&p)) {
            return std::make_tuple(p.x, p.y);
        }
        throw CppAutoGUIException("Failed to get cursor position");
    }

    static std::tuple<int, int> getSize() {
        return std::make_tuple(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    }

    static void moveTo(int x, int y) {
        SetCursorPos(x, y);
    }

    static void mouseDown(int x, int y, const std::string& button) {
        INPUT input = {0};
        input.type = INPUT_MOUSE;
        if (button == "left") {
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        } else if (button == "middle") {
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
        } else if (button == "right") {
            input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        }
        SendInput(1, &input, sizeof(INPUT));
    }

    static void mouseUp(int x, int y, const std::string& button) {
        INPUT input = {0};
        input.type = INPUT_MOUSE;
        if (button == "left") {
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        } else if (button == "middle") {
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
        } else if (button == "right") {
            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        }
        SendInput(1, &input, sizeof(INPUT));
    }

    static void click(int x, int y, const std::string& button) {
        mouseDown(x, y, button);
        mouseUp(x, y, button);
    }

    static void scroll(int clicks, int x, int y) {
        INPUT input = {0};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        input.mi.mouseData = clicks * WHEEL_DELTA;
        SendInput(1, &input, sizeof(INPUT));
    }

    static void keyDown(const std::string& key) {
        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = VkKeyScan(key[0]);
        SendInput(1, &input, sizeof(INPUT));
    }

    static void keyUp(const std::string& key) {
        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = VkKeyScan(key[0]);
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }
};
#elif __APPLE__
class MacOSPlatform {
public:
    static std::tuple<int, int> getPosition() {
        CGEventRef event = CGEventCreate(NULL);
        CGPoint point = CGEventGetLocation(event);
        CFRelease(event);
        return std::make_tuple(static_cast<int>(point.x), static_cast<int>(point.y));
    }

    static std::tuple<int, int> getSize() {
        CGRect displayBounds = CGDisplayBounds(CGMainDisplayID());
        return std::make_tuple(static_cast<int>(displayBounds.size.width), static_cast<int>(displayBounds.size.height));
    }

    static void moveTo(int x, int y) {
        CGEventRef move = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, CGPointMake(x, y), kCGMouseButtonLeft);
        CGEventPost(kCGHIDEventTap, move);
        CFRelease(move);
    }

    static void mouseDown(int x, int y, const std::string& button) {
        CGEventType eventType;
        if (button == "left") {
            eventType = kCGEventLeftMouseDown;
        } else if (button == "middle") {
            eventType = kCGEventOtherMouseDown;
        } else if (button == "right") {
            eventType = kCGEventRightMouseDown;
        }
        CGEventRef event = CGEventCreateMouseEvent(NULL, eventType, CGPointMake(x, y), kCGMouseButtonLeft);
        CGEventPost(kCGHIDEventTap, event);
        CFRelease(event);
    }

    static void mouseUp(int x, int y, const std::string& button) {
        CGEventType eventType;
        if (button == "left") {
            eventType = kCGEventLeftMouseUp;
        } else if (button == "middle") {
            eventType = kCGEventOtherMouseUp;
        } else if (button == "right") {
            eventType = kCGEventRightMouseUp;
        }
        CGEventRef event = CGEventCreateMouseEvent(NULL, eventType, CGPointMake(x, y), kCGMouseButtonLeft);
        CGEventPost(kCGHIDEventTap, event);
        CFRelease(event);
    }

    static void click(int x, int y, const std::string& button) {
        mouseDown(x, y, button);
        mouseUp(x, y, button);
    }

    static void scroll(int clicks, int x, int y) {
        CGEventRef event = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitLine, 1, clicks);
        CGEventPost(kCGHIDEventTap, event);
        CFRelease(event);
    }

    static void keyDown(const std::string& key) {
        CGEventRef event = CGEventCreateKeyboardEvent(NULL, 0, true);
        CGEventPost(kCGHIDEventTap, event);
        CFRelease(event);
    }

    static void keyUp(const std::string& key) {
        CGEventRef event = CGEventCreateKeyboardEvent(NULL, 0, false);
        CGEventPost(kCGHIDEventTap, event);
        CFRelease(event);
    }
};
#elif __linux__
class LinuxPlatform {
public:
    static Display* display;
    static Window root;

    static void initialize() {
        display = XOpenDisplay(NULL);
        root = DefaultRootWindow(display);
    }

    static std::tuple<int, int> getPosition() {
        XEvent event;
        XQueryPointer(display, root, &event.xbutton.root, &event.xbutton.window,
                      &event.xbutton.x_root, &event.xbutton.y_root,
                      &event.xbutton.x, &event.xbutton.y,
                      &event.xbutton.state);
        return std::make_tuple(event.xbutton.x_root, event.xbutton.y_root);
    }

    static std::tuple<int, int> getSize() {
        Screen* screen = DefaultScreenOfDisplay(display);
        return std::make_tuple(screen->width, screen->height);
    }

    static void moveTo(int x, int y) {
        XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
        XFlush(display);
    }

    static void mouseDown(int x, int y, const std::string& button) {
        int buttonCode;
        if (button == "left") {
            buttonCode = Button1;
        } else if (button == "middle") {
            buttonCode = Button2;
        } else if (button == "right") {
            buttonCode = Button3;
        }
        XTestFakeButtonEvent(display, buttonCode, True, CurrentTime);
        XFlush(display);
    }

    static void mouseUp(int x, int y, const std::string& button) {
        int buttonCode;
        if (button == "left") {
            buttonCode = Button1;
        } else if (button == "middle") {
            buttonCode = Button2;
        } else if (button == "right") {
            buttonCode = Button3;
        }
        XTestFakeButtonEvent(display, buttonCode, False, CurrentTime);
        XFlush(display);
    }

    static void click(int x, int y, const std::string& button) {
        mouseDown(x, y, button);
        mouseUp(x, y, button);
    }

    static void scroll(int clicks, int x, int y) {
        int buttonCode = (clicks > 0) ? Button4 : Button5;
        for (int i = 0; i < std::abs(clicks); ++i) {
            XTestFakeButtonEvent(display, buttonCode, True, CurrentTime);
            XTestFakeButtonEvent(display, buttonCode, False, CurrentTime);
        }
        XFlush(display);
    }

    static void keyDown(const std::string& key) {
        KeyCode keyCode = XKeysymToKeycode(display, XStringToKeysym(key.c_str()));
        XTestFakeKeyEvent(display, keyCode, True, CurrentTime);
        XFlush(display);
    }

    static void keyUp(const std::string& key) {
        KeyCode keyCode = XKeysymToKeycode(display, XStringToKeysym(key.c_str()));
        XTestFakeKeyEvent(display, keyCode, False, CurrentTime);
        XFlush(display);
    }
};

Display* LinuxPlatform::display = nullptr;
Window LinuxPlatform::root = 0;
#endif

class CppAutoGUI {
public:
    static std::tuple<int, int> getPosition() {
#ifdef _WIN32
        return WindowsPlatform::getPosition();
#elif __APPLE__
        return MacOSPlatform::getPosition();
#elif __linux__
        return LinuxPlatform::getPosition();
#endif
    }

    static std::tuple<int, int> getSize() {
#ifdef _WIN32
        return WindowsPlatform::getSize();
#elif __APPLE__
        return MacOSPlatform::getSize();
#elif __linux__
        return LinuxPlatform::getSize();
#endif
    }

    static void moveTo(int x, int y) {
#ifdef _WIN32
        WindowsPlatform::moveTo(x, y);
#elif __APPLE__
        MacOSPlatform::moveTo(x, y);
#elif __linux__
        LinuxPlatform::moveTo(x, y);
#endif
    }

    static void mouseDown(int x, int y, const std::string& button) {
#ifdef _WIN32
        WindowsPlatform::mouseDown(x, y, button);
#elif __APPLE__
        MacOSPlatform::mouseDown(x, y, button);
#elif __linux__
        LinuxPlatform::mouseDown(x, y, button);
#endif
    }

    static void mouseUp(int x, int y, const std::string& button) {
#ifdef _WIN32
        WindowsPlatform::mouseUp(x, y, button);
#elif __APPLE__
        MacOSPlatform::mouseUp(x, y, button);
#elif __linux__
        LinuxPlatform::mouseUp(x, y, button);
#endif
    }

    static void click(int x, int y, const std::string& button) {
#ifdef _WIN32
        WindowsPlatform::click(x, y, button);
#elif __APPLE__
        MacOSPlatform::click(x, y, button);
#elif __linux__
        LinuxPlatform::click(x, y, button);
#endif
    }

    static void scroll(int clicks, int x, int y) {
#ifdef _WIN32
        WindowsPlatform::scroll(clicks, x, y);
#elif __APPLE__
        MacOSPlatform::scroll(clicks, x, y);
#elif __linux__
        LinuxPlatform::scroll(clicks, x, y);
#endif
    }

    static void keyDown(const std::string& key) {
#ifdef _WIN32
        WindowsPlatform::keyDown(key);
#elif __APPLE__
        MacOSPlatform::keyDown(key);
#elif __linux__
        LinuxPlatform::keyDown(key);
#endif
    }

    static void keyUp(const std::string& key) {
#ifdef _WIN32
        WindowsPlatform::keyUp(key);
#elif __APPLE__
        MacOSPlatform::keyUp(key);
#elif __linux__
        LinuxPlatform::keyUp(key);
#endif
    }
};
