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

    static void test_screenshot() {
        auto screenshot = CppAutoGUI::screenshot();
        std::cout << "Screenshot taken" << std::endl;
    }

    static void test_locateOnScreen() {
        auto [left, top, width, height] = CppAutoGUI::locateOnScreen("button.png");
        std::cout << "Located on screen: (" << left << ", " << top << ", " << width << ", " << height << ")" << std::endl;
    }

    static void test_locateCenterOnScreen() {
        auto [x, y] = CppAutoGUI::locateCenterOnScreen("button.png");
        std::cout << "Located center on screen: (" << x << ", " << y << ")" << std::endl;
    }

    static void test_pixel() {
        auto [red, green, blue] = CppAutoGUI::pixel(100, 100);
        std::cout << "Pixel color at (100, 100): (" << red << ", " << green << ", " << blue << ")" << std::endl;
    }

    static void test_pixelMatchesColor() {
        bool matches = CppAutoGUI::pixelMatchesColor(100, 100, std::make_tuple(255, 255, 255));
        std::cout << "Pixel matches color at (100, 100): " << (matches ? "true" : "false") << std::endl;
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
    CppAutoGUITest::test_screenshot();
    CppAutoGUITest::test_locateOnScreen();
    CppAutoGUITest::test_locateCenterOnScreen();
    CppAutoGUITest::test_pixel();
    CppAutoGUITest::test_pixelMatchesColor();

    return 0;
}
