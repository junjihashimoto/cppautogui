#include <ApplicationServices/ApplicationServices.h>
#include <tuple>
#include <string>
#include <map>
#include <thread>
#include <chrono>
#include <stdexcept>

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

class CppAutoGUI {
public:
    static std::tuple<int, int> getPosition() {
        return MacOSPlatform::getPosition();
    }

    static std::tuple<int, int> getSize() {
        return MacOSPlatform::getSize();
    }

    static void moveTo(int x, int y) {
        MacOSPlatform::moveTo(x, y);
    }

    static void mouseDown(int x, int y, const std::string& button) {
        MacOSPlatform::mouseDown(x, y, button);
    }

    static void mouseUp(int x, int y, const std::string& button) {
        MacOSPlatform::mouseUp(x, y, button);
    }

    static void click(int x, int y, const std::string& button) {
        MacOSPlatform::click(x, y, button);
    }

    static void scroll(int clicks, int x, int y) {
        MacOSPlatform::scroll(clicks, x, y);
    }

    static void keyDown(const std::string& key) {
        MacOSPlatform::keyDown(key);
    }

    static void keyUp(const std::string& key) {
        MacOSPlatform::keyUp(key);
    }
};
