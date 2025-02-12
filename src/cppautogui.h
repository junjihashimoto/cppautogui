#ifndef CPPAUTOGUI_H
#define CPPAUTOGUI_H

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

#include "image.h"

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

class CppAutoGUI {
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
    static std::shared_ptr<image> screenshot();
    static std::tuple<int, int, int, int> locateOnScreen(const std::string& imagePath);
    static std::tuple<int, int> locateCenterOnScreen(const std::string& imagePath);
    static std::tuple<int, int, int> pixel(int x, int y);
    static bool pixelMatchesColor(int x, int y, const std::tuple<int, int, int>& color, int tolerance = 0);
};

#endif // CPPAUTOGUI_H
