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
#include "cppautogui.h"

#ifdef _WIN32
class WindowsPlatform {
public:
    static std::tuple<int, int> getPosition();
    static std::tuple<int, int> getSize();
    static void moveTo(int x, int y);
    static void mouseDown(int x, int y, const std::string& button);
    static void mouseUp(int x, int y, const std::string& button);
    static void click(int x, int y, const std::string& button);
    static void scroll(int clicks, int x, int y);
    static void keyDown(const std::string& key);
    static void keyUp(const std::string& key);
};
#elif __APPLE__
class MacOSPlatform {
public:
    static std::tuple<int, int> getPosition();
    static std::tuple<int, int> getSize();
    static void moveTo(int x, int y);
    static void mouseDown(int x, int y, const std::string& button);
    static void mouseUp(int x, int y, const std::string& button);
    static void click(int x, int y, const std::string& button);
    static void scroll(int clicks, int x, int y);
    static void keyDown(const std::string& key);
    static void keyUp(const std::string& key);
};
#elif __linux__
class LinuxPlatform {
public:
    static Display* display;
    static Window root;

    static void initialize();
    static std::tuple<int, int> getPosition();
    static std::tuple<int, int> getSize();
    static void moveTo(int x, int y);
    static void mouseDown(int x, int y, const std::string& button);
    static void mouseUp(int x, int y, const std::string& button);
    static void click(int x, int y, const std::string& button);
    static void scroll(int clicks, int x, int y);
    static void keyDown(const std::string& key);
    static void keyUp(const std::string& key);
};
#endif

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

void hbitmap2image(HBITMAP hBitmap, BITMAP& bitmap, BYTE*& bits) {
  GetObject(hBitmap, sizeof(BITMAP), &bitmap);
  BITMAPINFOHEADER bi = {0};
  bi.biSize = sizeof(BITMAPINFOHEADER);
  bi.biWidth = bitmap.bmWidth;
  bi.biHeight = bitmap.bmHeight;
  bi.biPlanes = 1;
  bi.biBitCount = 32;
  bi.biCompression = BI_RGB;
  bi.biSizeImage = 0;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;
  bi.biClrUsed = 0;
  bi.biClrImportant = 0;
  HDC hdc = GetDC(NULL);
  GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, NULL, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);
  bits = new BYTE[bi.biSizeImage];
  GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, bits, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);
  DeleteDC(hdc);
}

std::shared_ptr<image> CppAutoGUI::screenshot() {
  HDC hdcScreen = GetDC(NULL);
  HDC hdcMemDC = CreateCompatibleDC(hdcScreen);
  int width = GetSystemMetrics(SM_CXSCREEN);
  int height = GetSystemMetrics(SM_CYSCREEN);
  HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
  SelectObject(hdcMemDC, hBitmap);
  BitBlt(hdcMemDC, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);
  DeleteDC(hdcMemDC);
  ReleaseDC(NULL, hdcScreen);
  HBITMAP bmp = hBitmap;
  std::shared_ptr<image> img(new image, [](image* img) {
    image_free(img);
    delete img;
  });
  image_init(*img, width, height);
  // Convert HBITMAP to image
  BITMAP bitmap = {0};
  BYTE* bits = nullptr;
  hbitmap2image(bmp, bitmap, bits);
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int i = y * width + x;
      int j = (height - y - 1) * width + x;
      (*img)[y][x].r = bits[i * 4 + 2];
      (*img)[y][x].g = bits[i * 4 + 1];
      (*img)[y][x].b = bits[i * 4];
      (*img)[y][x].alpha = bits[i * 4 + 3];
    }
  }
  delete[] bits;
  return img;
}

#elif __APPLE__
void cgimage2bitmap(CGImageRef image, size_t& width, size_t& height, size_t& bytesPerRow, size_t& bytesPerPixel, UInt8*& buffer) {
  width = CGImageGetWidth(image);
  height = CGImageGetHeight(image);
  bytesPerRow = CGImageGetBytesPerRow(image);
  bytesPerPixel = CGImageGetBitsPerPixel(image) / 8;
  buffer = new UInt8[bytesPerRow * height];
  CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
  CGContextRef context = CGBitmapContextCreate(buffer, width, height, 8, bytesPerRow, colorSpace, kCGImageAlphaPremultipliedFirst);
  CGContextDrawImage(context, CGRectMake(0, 0, width, height), image);
  CGContextRelease(context);
  CGColorSpaceRelease(colorSpace);
}

std::shared_ptr<image> CppAutoGUI::screenshot() {
  CGDirectDisplayID displayId = CGMainDisplayID();
  CGImageRef bmp = CGDisplayCreateImage(displayId);
  std::shared_ptr<image> img(new image, [](image* img) {
    image_free(*img);
    delete img;
  });
  size_t width, height, bytesPerRow, bytesPerPixel;
  UInt8* buffer = nullptr;
  cgimage2bitmap(bmp, width, height, bytesPerRow, bytesPerPixel, buffer);
  image_init(*img, width, height);
  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      size_t i = y * width + x;
      size_t j = (height - y - 1) * width + x;
      (*img)[y][x].r = buffer[i * bytesPerPixel + 2];
      (*img)[y][x].g = buffer[i * bytesPerPixel + 1];
      (*img)[y][x].b = buffer[i * bytesPerPixel];
      (*img)[y][x].alpha = buffer[i * bytesPerPixel + 3];
    }
  }
  delete[] buffer;
  CGImageRelease(bmp);
  return img;
}

#elif __linux__
void ximage2bitmap(XImage* image, size_t& width, size_t& height, size_t& bytesPerRow, size_t& bytesPerPixel, unsigned char*& buffer) {
  width = image->width;
  height = image->height;
  bytesPerRow = image->bytes_per_line;
  bytesPerPixel = image->bits_per_pixel / 8;
  buffer = new unsigned char[bytesPerRow * height];
  memcpy(buffer, image->data, bytesPerRow * height);
}

std::shared_ptr<image> CppAutoGUI::screenshot() {
  Display* display = XOpenDisplay(NULL);
  Window root = DefaultRootWindow(display);
  XWindowAttributes attributes = {0};
  XGetWindowAttributes(display, root, &attributes);
  XImage* bmp = XGetImage(display, root, 0, 0, attributes.width, attributes.height, AllPlanes, ZPixmap);
  std::shared_ptr<image> img(new image, [](image* img) {
    image_free(img);
    delete img;
  });
  size_t width, height, bytesPerRow, bytesPerPixel;
  unsigned char* buffer = nullptr;
  ximage2bitmap(bmp, width, height, bytesPerRow, bytesPerPixel, buffer);
  image_init(*img, width, height);
  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      size_t i = y * width + x;
      size_t j = (height - y - 1) * width + x;
      (*img)[y][x].r = buffer[i * bytesPerPixel + 2];
      (*img)[y][x].g = buffer[i * bytesPerPixel + 1];
      (*img)[y][x].b = buffer[i * bytesPerPixel];
      (*img)[y][x].alpha = 0;
    }
  }
  delete[] buffer;
  XDestroyImage(bmp);
  XCloseDisplay(display);
  return img;
}

#endif

std::tuple<int, int, int, int> CppAutoGUI::locateOnScreen(const std::string& imagePath) {
  // Implement Windows-specific code to locate an image on the screen
  // Return (left, top, width, height) of the located image
  image img;
  ::data data;
  data_load(data,imagePath.c_str());
  //Check the suffix of the image path
  std::string suffix = imagePath.substr(imagePath.find_last_of(".") + 1);
  if (suffix == "bmp") {
    bmp2image(data, img);
  } else if (suffix == "png") {
    png2image(data, img);
  } else if (suffix == "jpg" || suffix == "jpeg") {
    jpeg2image(data, img);
  } else {
    throw CppAutoGUIException("Unsupported image format");
  }

  std::shared_ptr<image> screen = screenshot();
  int width = screen->width - img.width + 1;
  int height = screen->height - img.height + 1;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      bool found = true;
      for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
          if ((*screen)[y + i][x + j].r != img[i][j].r ||
              (*screen)[y + i][x + j].g != img[i][j].g ||
              (*screen)[y + i][x + j].b != img[i][j].b) {
            found = false;
            break;
          }
        }
        if (!found) {
          break;
        }
      }
      if (found) {
        return std::make_tuple(x, y, img.width, img.height);
      }
    }
  }
  return std::make_tuple(0, 0, 0, 0);
}

std::tuple<int, int> CppAutoGUI::locateCenterOnScreen(const std::string& imagePath) {
  auto [left, top, width, height] = locateOnScreen(imagePath);
  return std::make_tuple(left + width / 2, top + height / 2);
}

std::tuple<int, int, int> CppAutoGUI::pixel(int x, int y) {
  auto image = CppAutoGUI::screenshot();
  return std::make_tuple((*image)[y][x].r, (*image)[y][x].g, (*image)[y][x].b);
}

bool CppAutoGUI::pixelMatchesColor(int x, int y, const std::tuple<int, int, int>& color, int tolerance) {
  auto [red, green, blue] = pixel(x, y);
  auto [targetRed, targetGreen, targetBlue] = color;
  return std::abs(red - targetRed) <= tolerance &&
         std::abs(green - targetGreen) <= tolerance &&
         std::abs(blue - targetBlue) <= tolerance;
}
