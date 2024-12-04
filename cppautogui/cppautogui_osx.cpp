#include <ApplicationServices/ApplicationServices.h>
#include <tuple>
#include <string>
#include <map>
#include <thread>
#include <chrono>
#include <stdexcept>
#include "cppautogui_osx.h"

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

std::tuple<int, int> CppAutoGUI::getPosition() {
  return MacOSPlatform::getPosition();
}

std::tuple<int, int> CppAutoGUI::getSize() {
  return MacOSPlatform::getSize();
}

void CppAutoGUI::moveTo(int x, int y) {
  MacOSPlatform::moveTo(x, y);
}

void CppAutoGUI::mouseDown(int x, int y, const std::string& button) {
  MacOSPlatform::mouseDown(x, y, button);
}

void CppAutoGUI::mouseUp(int x, int y, const std::string& button) {
  MacOSPlatform::mouseUp(x, y, button);
}

void CppAutoGUI::click(int x, int y, const std::string& button) {
  MacOSPlatform::click(x, y, button);
}

void CppAutoGUI::scroll(int clicks, int x, int y) {
  MacOSPlatform::scroll(clicks, x, y);
}

void CppAutoGUI::keyDown(const std::string& key) {
  MacOSPlatform::keyDown(key);
}

void CppAutoGUI::keyUp(const std::string& key) {
  MacOSPlatform::keyUp(key);
}
