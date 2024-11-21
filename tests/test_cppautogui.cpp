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

class CppAutoGUITest {
public:
    static void test_getPosition() {
        auto [x, y] = CppAutoGUI::getPosition();
        std::cout << "Mouse position: (" << x << ", " << y << ")" << std::endl;
    }

    static void test_getSize() {
        auto [width, height] = CppAutoGUI::getSize();
        std::cout << "Screen size: (" << width << ", " << height << ")" << std::endl;
    }

    static void test_moveTo() {
        CppAutoGUI::moveTo(100, 100);
        auto [x, y] = CppAutoGUI::getPosition();
        std::cout << "Moved to: (" << x << ", " << y << ")" << std::endl;
    }

    static void test_mouseDown() {
        CppAutoGUI::mouseDown(100, 100, "left");
        std::cout << "Mouse down at (100, 100)" << std::endl;
    }

    static void test_mouseUp() {
        CppAutoGUI::mouseUp(100, 100, "left");
        std::cout << "Mouse up at (100, 100)" << std::endl;
    }

    static void test_click() {
        CppAutoGUI::click(100, 100, "left");
        std::cout << "Clicked at (100, 100)" << std::endl;
    }

    static void test_scroll() {
        CppAutoGUI::scroll(3, 100, 100);
        std::cout << "Scrolled at (100, 100)" << std::endl;
    }

    static void test_keyDown() {
        CppAutoGUI::keyDown("a");
        std::cout << "Key down: a" << std::endl;
    }

    static void test_keyUp() {
        CppAutoGUI::keyUp("a");
        std::cout << "Key up: a" << std::endl;
    }
};

int main() {
    CppAutoGUITest::test_getPosition();
    CppAutoGUITest::test_getSize();
    CppAutoGUITest::test_moveTo();
    CppAutoGUITest::test_mouseDown();
    CppAutoGUITest::test_mouseUp();
    CppAutoGUITest::test_click();
    CppAutoGUITest::test_scroll();
    CppAutoGUITest::test_keyDown();
    CppAutoGUITest::test_keyUp();

    return 0;
}
