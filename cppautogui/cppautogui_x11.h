#ifndef CPPAUTOGUI_X11_H
#define CPPAUTOGUI_X11_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <string>
#include <vector>

namespace cppautogui {

class X11 {
public:
    X11();
    ~X11();

    void moveMouse(int x, int y);
    void clickMouse(int button);
    void typeKey(const std::string& key);
    void pressKey(const std::string& key);
    void releaseKey(const std::string& key);
    std::vector<int> getMousePosition();

private:
    Display* display;
    Window rootWindow;
    int screen;
};

} // namespace cppautogui

#endif // CPPAUTOGUI_X11_H
