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
};

#endif // CPPAUTOGUI_H
