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

#include "cppautogui.h"

#ifdef _WIN32
std::tuple<int, int> WindowsPlatform::getPosition() {
  POINT p;
  if (GetCursorPos(&p)) {
    return std::make_tuple(p.x, p.y);
  }
  throw CppAutoGUIException("Failed to get cursor position");
}

std::tuple<int, int> WindowsPlatform::getSize() {
  return std::make_tuple(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

void WindowsPlatform::moveTo(int x, int y) {
  SetCursorPos(x, y);
}

void WindowsPlatform::mouseDown(int x, int y, const std::string& button) {
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

void WindowsPlatform::mouseUp(int x, int y, const std::string& button) {
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

void WindowsPlatform::click(int x, int y, const std::string& button) {
  mouseDown(x, y, button);
  mouseUp(x, y, button);
}

void WindowsPlatform::scroll(int clicks, int x, int y) {
  INPUT input = {0};
  input.type = INPUT_MOUSE;
  input.mi.dwFlags = MOUSEEVENTF_WHEEL;
  input.mi.mouseData = clicks * WHEEL_DELTA;
  SendInput(1, &input, sizeof(INPUT));
}

void WindowsPlatform::keyDown(const std::string& key) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = VkKeyScan(key[0]);
  SendInput(1, &input, sizeof(INPUT));
}

void WindowsPlatform::keyUp(const std::string& key) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = VkKeyScan(key[0]);
  input.ki.dwFlags = KEYEVENTF_KEYUP;
  SendInput(1, &input, sizeof(INPUT));
}
#elif __APPLE__
std::tuple<int, int> MacOSPlatform::getPosition() {
  CGEventRef event = CGEventCreate(NULL);
  CGPoint point = CGEventGetLocation(event);
  CFRelease(event);
  return std::make_tuple(static_cast<int>(point.x), static_cast<int>(point.y));
}

std::tuple<int, int> MacOSPlatform::getSize() {
  CGRect displayBounds = CGDisplayBounds(CGMainDisplayID());
  return std::make_tuple(static_cast<int>(displayBounds.size.width), static_cast<int>(displayBounds.size.height));
}

void MacOSPlatform::moveTo(int x, int y) {
  CGEventRef move = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, CGPointMake(x, y), kCGMouseButtonLeft);
  CGEventPost(kCGHIDEventTap, move);
  CFRelease(move);
}

void MacOSPlatform::mouseDown(int x, int y, const std::string& button) {
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

void MacOSPlatform::mouseUp(int x, int y, const std::string& button) {
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

void MacOSPlatform::click(int x, int y, const std::string& button) {
  mouseDown(x, y, button);
  mouseUp(x, y, button);
}

void MacOSPlatform::scroll(int clicks, int x, int y) {
  CGEventRef event = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitLine, 1, clicks);
  CGEventPost(kCGHIDEventTap, event);
  CFRelease(event);
}

void MacOSPlatform::keyDown(const std::string& key) {
  CGEventRef event = CGEventCreateKeyboardEvent(NULL, 0, true);
  CGEventPost(kCGHIDEventTap, event);
  CFRelease(event);
}

void MacOSPlatform::keyUp(const std::string& key) {
  CGEventRef event = CGEventCreateKeyboardEvent(NULL, 0, false);
  CGEventPost(kCGHIDEventTap, event);
  CFRelease(event);
}
#elif __linux__
void LinuxPlatform::initialize() {
  display = XOpenDisplay(NULL);
  root = DefaultRootWindow(display);
}

std::tuple<int, int> LinuxPlatform::getPosition() {
  XEvent event;
  XQueryPointer(display, root, &event.xbutton.root, &event.xbutton.window,
		&event.xbutton.x_root, &event.xbutton.y_root,
		&event.xbutton.x, &event.xbutton.y,
		&event.xbutton.state);
  return std::make_tuple(event.xbutton.x_root, event.xbutton.y_root);
}

std::tuple<int, int> LinuxPlatform::getSize() {
  Screen* screen = DefaultScreenOfDisplay(display);
  return std::make_tuple(screen->width, screen->height);
}

void LinuxPlatform::moveTo(int x, int y) {
  XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
  XFlush(display);
}

void LinuxPlatform::mouseDown(int x, int y, const std::string& button) {
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

void LinuxPlatform::mouseUp(int x, int y, const std::string& button) {
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

void LinuxPlatform::click(int x, int y, const std::string& button) {
  mouseDown(x, y, button);
  mouseUp(x, y, button);
}

void LinuxPlatform::scroll(int clicks, int x, int y) {
  int buttonCode = (clicks > 0) ? Button4 : Button5;
  for (int i = 0; i < std::abs(clicks); ++i) {
    XTestFakeButtonEvent(display, buttonCode, True, CurrentTime);
    XTestFakeButtonEvent(display, buttonCode, False, CurrentTime);
  }
  XFlush(display);
}

void LinuxPlatform::keyDown(const std::string& key) {
  KeyCode keyCode = XKeysymToKeycode(display, XStringToKeysym(key.c_str()));
  XTestFakeKeyEvent(display, keyCode, True, CurrentTime);
  XFlush(display);
}

void LinuxPlatform::keyUp(const std::string& key) {
  KeyCode keyCode = XKeysymToKeycode(display, XStringToKeysym(key.c_str()));
  XTestFakeKeyEvent(display, keyCode, False, CurrentTime);
  XFlush(display);
}

Display* LinuxPlatform::display = nullptr;
Window LinuxPlatform::root = 0;
#endif

std::tuple<int, int> CppAutoGUI::getPosition() {
#ifdef _WIN32
  return WindowsPlatform::getPosition();
#elif __APPLE__
  return MacOSPlatform::getPosition();
#elif __linux__
  return LinuxPlatform::getPosition();
#endif
}

std::tuple<int, int> CppAutoGUI::getSize() {
#ifdef _WIN32
  return WindowsPlatform::getSize();
#elif __APPLE__
  return MacOSPlatform::getSize();
#elif __linux__
  return LinuxPlatform::getSize();
#endif
}

void CppAutoGUI::moveTo(int x, int y) {
#ifdef _WIN32
  WindowsPlatform::moveTo(x, y);
#elif __APPLE__
  MacOSPlatform::moveTo(x, y);
#elif __linux__
  LinuxPlatform::moveTo(x, y);
#endif
}

void CppAutoGUI::mouseDown(int x, int y, const std::string& button) {
#ifdef _WIN32
  WindowsPlatform::mouseDown(x, y, button);
#elif __APPLE__
  MacOSPlatform::mouseDown(x, y, button);
#elif __linux__
  LinuxPlatform::mouseDown(x, y, button);
#endif
}

void CppAutoGUI::mouseUp(int x, int y, const std::string& button) {
#ifdef _WIN32
  WindowsPlatform::mouseUp(x, y, button);
#elif __APPLE__
  MacOSPlatform::mouseUp(x, y, button);
#elif __linux__
  LinuxPlatform::mouseUp(x, y, button);
#endif
}

void CppAutoGUI::click(int x, int y, const std::string& button) {
#ifdef _WIN32
  WindowsPlatform::click(x, y, button);
#elif __APPLE__
  MacOSPlatform::click(x, y, button);
#elif __linux__
  LinuxPlatform::click(x, y, button);
#endif
}

void CppAutoGUI::scroll(int clicks, int x, int y) {
#ifdef _WIN32
  WindowsPlatform::scroll(clicks, x, y);
#elif __APPLE__
  MacOSPlatform::scroll(clicks, x, y);
#elif __linux__
  LinuxPlatform::scroll(clicks, x, y);
#endif
}

void CppAutoGUI::keyDown(const std::string& key) {
#ifdef _WIN32
  WindowsPlatform::keyDown(key);
#elif __APPLE__
  MacOSPlatform::keyDown(key);
#elif __linux__
  LinuxPlatform::keyDown(key);
#endif
}

void CppAutoGUI::keyUp(const std::string& key) {
#ifdef _WIN32
  WindowsPlatform::keyUp(key);
#elif __APPLE__
  MacOSPlatform::keyUp(key);
#elif __linux__
  LinuxPlatform::keyUp(key);
#endif
}

#ifdef _WIN32
HBITMAP screenshot() {
  HDC hdcScreen = GetDC(NULL);
  HDC hdcMemDC = CreateCompatibleDC(hdcScreen);
  int width = GetSystemMetrics(SM_CXSCREEN);
  int height = GetSystemMetrics(SM_CYSCREEN);
  HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
  SelectObject(hdcMemDC, hBitmap);
  BitBlt(hdcMemDC, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);
  DeleteDC(hdcMemDC);
  ReleaseDC(NULL, hdcScreen);
  return hBitmap;
}
#elif __APPLE__
CGImageRef screenshot() {
  CGDirectDisplayID displayId = CGMainDisplayID();
  return CGDisplayCreateImage(displayId);
}
#elif __linux__
XImage* screenshot() {
  Display* display = XOpenDisplay(NULL);
  Window root = DefaultRootWindow(display);
  XWindowAttributes attributes = {0};
  XGetWindowAttributes(display, root, &attributes);
  return XGetImage(display, root, 0, 0, attributes.width, attributes.height, AllPlanes, ZPixmap);
}
#endif

#ifdef _WIN32
std::tuple<int, int, int, int> locateOnScreen(const std::string& imagePath) {
  // Implement Windows-specific code to locate an image on the screen
  // Return (left, top, width, height) of the located image
  return std::make_tuple(0, 0, 0, 0);
}
#elif __APPLE__
std::tuple<int, int, int, int> locateOnScreen(const std::string& imagePath) {
  // Implement macOS-specific code to locate an image on the screen
  // Return (left, top, width, height) of the located image
  return std::make_tuple(0, 0, 0, 0);
}
#elif __linux__
std::tuple<int, int, int, int> locateOnScreen(const std::string& imagePath) {
  // Implement Linux-specific code to locate an image on the screen
  // Return (left, top, width, height) of the located image
  return std::make_tuple(0, 0, 0, 0);
}
#endif

std::tuple<int, int> locateCenterOnScreen(const std::string& imagePath) {
  auto [left, top, width, height] = locateOnScreen(imagePath);
  return std::make_tuple(left + width / 2, top + height / 2);
}

#ifdef _WIN32
std::tuple<int, int, int> pixel(int x, int y) {
  HDC hdcScreen = GetDC(NULL);
  COLORREF color = GetPixel(hdcScreen, x, y);
  ReleaseDC(NULL, hdcScreen);
  return std::make_tuple(GetRValue(color), GetGValue(color), GetBValue(color));
}
#elif __APPLE__
std::tuple<int, int, int> pixel(int x, int y) {
  CGImageRef image = screenshot();
  CFDataRef data = CGDataProviderCopyData(CGImageGetDataProvider(image));
  const UInt8* buffer = CFDataGetBytePtr(data);
  size_t bytesPerRow = CGImageGetBytesPerRow(image);
  size_t bytesPerPixel = CGImageGetBitsPerPixel(image) / 8;
  const UInt8* pixel = buffer + (y * bytesPerRow) + (x * bytesPerPixel);
  int red = pixel[0];
  int green = pixel[1];
  int blue = pixel[2];
  CFRelease(data);
  CGImageRelease(image);
  return std::make_tuple(red, green, blue);
}
#elif __linux__
std::tuple<int, int, int> pixel(int x, int y) {
  XImage* image = screenshot();
  unsigned long pixel = XGetPixel(image, x, y);
  int red = (pixel & image->red_mask) >> 16;
  int green = (pixel & image->green_mask) >> 8;
  int blue = pixel & image->blue_mask;
  XDestroyImage(image);
  return std::make_tuple(red, green, blue);
}
#endif

bool pixelMatchesColor(int x, int y, const std::tuple<int, int, int>& color, int tolerance = 0) {
  auto [red, green, blue] = pixel(x, y);
  auto [targetRed, targetGreen, targetBlue] = color;
  return std::abs(red - targetRed) <= tolerance &&
         std::abs(green - targetGreen) <= tolerance &&
         std::abs(blue - targetBlue) <= tolerance;
}