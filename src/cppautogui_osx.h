#ifndef CPPAUTOGUI_OSX_H
#define CPPAUTOGUI_OSX_H

#include <ApplicationServices/ApplicationServices.h>
#include <tuple>
#include <string>
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

#endif // CPPAUTOGUI_OSX_H
