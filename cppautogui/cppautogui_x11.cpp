#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <map>
#include <cstdlib>
#include <unistd.h>

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

class X11Platform {
public:
    static Display* display;
    static Window root;
    static std::map<std::string, KeyCode> keyboardMapping;

    static void initialize() {
        display = XOpenDisplay(NULL);
        if (display == NULL) {
            throw CppAutoGUIException("Failed to open X display");
        }
        root = DefaultRootWindow(display);
        initializeKeyboardMapping();
    }

    static void initializeKeyboardMapping() {
        keyboardMapping = {
            {"backspace", XKeysymToKeycode(display, XK_BackSpace)},
            {"\b", XKeysymToKeycode(display, XK_BackSpace)},
            {"tab", XKeysymToKeycode(display, XK_Tab)},
            {"enter", XKeysymToKeycode(display, XK_Return)},
            {"return", XKeysymToKeycode(display, XK_Return)},
            {"shift", XKeysymToKeycode(display, XK_Shift_L)},
            {"ctrl", XKeysymToKeycode(display, XK_Control_L)},
            {"alt", XKeysymToKeycode(display, XK_Alt_L)},
            {"pause", XKeysymToKeycode(display, XK_Pause)},
            {"capslock", XKeysymToKeycode(display, XK_Caps_Lock)},
            {"esc", XKeysymToKeycode(display, XK_Escape)},
            {"escape", XKeysymToKeycode(display, XK_Escape)},
            {"pgup", XKeysymToKeycode(display, XK_Page_Up)},
            {"pgdn", XKeysymToKeycode(display, XK_Page_Down)},
            {"pageup", XKeysymToKeycode(display, XK_Page_Up)},
            {"pagedown", XKeysymToKeycode(display, XK_Page_Down)},
            {"end", XKeysymToKeycode(display, XK_End)},
            {"home", XKeysymToKeycode(display, XK_Home)},
            {"left", XKeysymToKeycode(display, XK_Left)},
            {"up", XKeysymToKeycode(display, XK_Up)},
            {"right", XKeysymToKeycode(display, XK_Right)},
            {"down", XKeysymToKeycode(display, XK_Down)},
            {"select", XKeysymToKeycode(display, XK_Select)},
            {"print", XKeysymToKeycode(display, XK_Print)},
            {"execute", XKeysymToKeycode(display, XK_Execute)},
            {"prtsc", XKeysymToKeycode(display, XK_Print)},
            {"prtscr", XKeysymToKeycode(display, XK_Print)},
            {"prntscrn", XKeysymToKeycode(display, XK_Print)},
            {"printscreen", XKeysymToKeycode(display, XK_Print)},
            {"insert", XKeysymToKeycode(display, XK_Insert)},
            {"del", XKeysymToKeycode(display, XK_Delete)},
            {"delete", XKeysymToKeycode(display, XK_Delete)},
            {"help", XKeysymToKeycode(display, XK_Help)},
            {"win", XKeysymToKeycode(display, XK_Super_L)},
            {"winleft", XKeysymToKeycode(display, XK_Super_L)},
            {"winright", XKeysymToKeycode(display, XK_Super_R)},
            {"apps", XKeysymToKeycode(display, XK_Menu)},
            {"num0", XKeysymToKeycode(display, XK_KP_0)},
            {"num1", XKeysymToKeycode(display, XK_KP_1)},
            {"num2", XKeysymToKeycode(display, XK_KP_2)},
            {"num3", XKeysymToKeycode(display, XK_KP_3)},
            {"num4", XKeysymToKeycode(display, XK_KP_4)},
            {"num5", XKeysymToKeycode(display, XK_KP_5)},
            {"num6", XKeysymToKeycode(display, XK_KP_6)},
            {"num7", XKeysymToKeycode(display, XK_KP_7)},
            {"num8", XKeysymToKeycode(display, XK_KP_8)},
            {"num9", XKeysymToKeycode(display, XK_KP_9)},
            {"multiply", XKeysymToKeycode(display, XK_KP_Multiply)},
            {"add", XKeysymToKeycode(display, XK_KP_Add)},
            {"separator", XKeysymToKeycode(display, XK_KP_Separator)},
            {"subtract", XKeysymToKeycode(display, XK_KP_Subtract)},
            {"decimal", XKeysymToKeycode(display, XK_KP_Decimal)},
            {"divide", XKeysymToKeycode(display, XK_KP_Divide)},
            {"f1", XKeysymToKeycode(display, XK_F1)},
            {"f2", XKeysymToKeycode(display, XK_F2)},
            {"f3", XKeysymToKeycode(display, XK_F3)},
            {"f4", XKeysymToKeycode(display, XK_F4)},
            {"f5", XKeysymToKeycode(display, XK_F5)},
            {"f6", XKeysymToKeycode(display, XK_F6)},
            {"f7", XKeysymToKeycode(display, XK_F7)},
            {"f8", XKeysymToKeycode(display, XK_F8)},
            {"f9", XKeysymToKeycode(display, XK_F9)},
            {"f10", XKeysymToKeycode(display, XK_F10)},
            {"f11", XKeysymToKeycode(display, XK_F11)},
            {"f12", XKeysymToKeycode(display, XK_F12)},
            {"f13", XKeysymToKeycode(display, XK_F13)},
            {"f14", XKeysymToKeycode(display, XK_F14)},
            {"f15", XKeysymToKeycode(display, XK_F15)},
            {"f16", XKeysymToKeycode(display, XK_F16)},
            {"f17", XKeysymToKeycode(display, XK_F17)},
            {"f18", XKeysymToKeycode(display, XK_F18)},
            {"f19", XKeysymToKeycode(display, XK_F19)},
            {"f20", XKeysymToKeycode(display, XK_F20)},
            {"f21", XKeysymToKeycode(display, XK_F21)},
            {"f22", XKeysymToKeycode(display, XK_F22)},
            {"f23", XKeysymToKeycode(display, XK_F23)},
            {"f24", XKeysymToKeycode(display, XK_F24)},
            {"numlock", XKeysymToKeycode(display, XK_Num_Lock)},
            {"scrolllock", XKeysymToKeycode(display, XK_Scroll_Lock)},
            {"shiftleft", XKeysymToKeycode(display, XK_Shift_L)},
            {"shiftright", XKeysymToKeycode(display, XK_Shift_R)},
            {"ctrlleft", XKeysymToKeycode(display, XK_Control_L)},
            {"ctrlright", XKeysymToKeycode(display, XK_Control_R)},
            {"altleft", XKeysymToKeycode(display, XK_Alt_L)},
            {"altright", XKeysymToKeycode(display, XK_Alt_R)},
            {" ", XKeysymToKeycode(display, XK_space)},
            {"space", XKeysymToKeycode(display, XK_space)},
            {"\t", XKeysymToKeycode(display, XK_Tab)},
            {"\n", XKeysymToKeycode(display, XK_Return)},
            {"\r", XKeysymToKeycode(display, XK_Return)},
            {"\e", XKeysymToKeycode(display, XK_Escape)},
            {"!", XKeysymToKeycode(display, XK_exclam)},
            {"#", XKeysymToKeycode(display, XK_numbersign)},
            {"%", XKeysymToKeycode(display, XK_percent)},
            {"$", XKeysymToKeycode(display, XK_dollar)},
            {"&", XKeysymToKeycode(display, XK_ampersand)},
            {"\"", XKeysymToKeycode(display, XK_quotedbl)},
            {"'", XKeysymToKeycode(display, XK_apostrophe)},
            {"(", XKeysymToKeycode(display, XK_parenleft)},
            {")", XKeysymToKeycode(display, XK_parenright)},
            {"*", XKeysymToKeycode(display, XK_asterisk)},
            {"=", XKeysymToKeycode(display, XK_equal)},
            {"+", XKeysymToKeycode(display, XK_plus)},
            {",", XKeysymToKeycode(display, XK_comma)},
            {"-", XKeysymToKeycode(display, XK_minus)},
            {".", XKeysymToKeycode(display, XK_period)},
            {"/", XKeysymToKeycode(display, XK_slash)},
            {":", XKeysymToKeycode(display, XK_colon)},
            {";", XKeysymToKeycode(display, XK_semicolon)},
            {"<", XKeysymToKeycode(display, XK_less)},
            {">", XKeysymToKeycode(display, XK_greater)},
            {"?", XKeysymToKeycode(display, XK_question)},
            {"@", XKeysymToKeycode(display, XK_at)},
            {"[", XKeysymToKeycode(display, XK_bracketleft)},
            {"]", XKeysymToKeycode(display, XK_bracketright)},
            {"\\", XKeysymToKeycode(display, XK_backslash)},
            {"^", XKeysymToKeycode(display, XK_asciicircum)},
            {"_", XKeysymToKeycode(display, XK_underscore)},
            {"`", XKeysymToKeycode(display, XK_grave)},
            {"{", XKeysymToKeycode(display, XK_braceleft)},
            {"|", XKeysymToKeycode(display, XK_bar)},
            {"}", XKeysymToKeycode(display, XK_braceright)},
            {"~", XKeysymToKeycode(display, XK_asciitilde)}
        };

        for (char c = 'a'; c <= 'z'; ++c) {
            keyboardMapping[std::string(1, c)] = XKeysymToKeycode(display, XStringToKeysym(std::string(1, c).c_str()));
        }

        for (char c = 'A'; c <= 'Z'; ++c) {
            keyboardMapping[std::string(1, c)] = XKeysymToKeycode(display, XStringToKeysym(std::string(1, c).c_str()));
        }

        for (char c = '0'; c <= '9'; ++c) {
            keyboardMapping[std::string(1, c)] = XKeysymToKeycode(display, XStringToKeysym(std::string(1, c).c_str()));
        }
    }

    static std::tuple<int, int> getPosition() {
        XEvent event;
        XQueryPointer(display, root, &event.xbutton.root, &event.xbutton.window,
                      &event.xbutton.x_root, &event.xbutton.y_root,
                      &event.xbutton.x, &event.xbutton.y,
                      &event.xbutton.state);
        return std::make_tuple(event.xbutton.x_root, event.xbutton.y_root);
    }

    static std::tuple<int, int> getSize() {
        Screen* screen = DefaultScreenOfDisplay(display);
        return std::make_tuple(screen->width, screen->height);
    }

    static void moveTo(int x, int y) {
        XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
        XFlush(display);
    }

    static void mouseDown(int x, int y, const std::string& button) {
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

    static void mouseUp(int x, int y, const std::string& button) {
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

    static void click(int x, int y, const std::string& button) {
        mouseDown(x, y, button);
        mouseUp(x, y, button);
    }

    static void scroll(int clicks, int x, int y) {
        int buttonCode = (clicks > 0) ? Button4 : Button5;
        for (int i = 0; i < std::abs(clicks); ++i) {
            XTestFakeButtonEvent(display, buttonCode, True, CurrentTime);
            XTestFakeButtonEvent(display, buttonCode, False, CurrentTime);
        }
        XFlush(display);
    }

    static void keyDown(const std::string& key) {
        KeyCode keyCode = keyboardMapping[key];
        XTestFakeKeyEvent(display, keyCode, True, CurrentTime);
        XFlush(display);
    }

    static void keyUp(const std::string& key) {
        KeyCode keyCode = keyboardMapping[key];
        XTestFakeKeyEvent(display, keyCode, False, CurrentTime);
        XFlush(display);
    }
};

Display* X11Platform::display = nullptr;
Window X11Platform::root = 0;
std::map<std::string, KeyCode> X11Platform::keyboardMapping;

class CppAutoGUI {
public:
    static std::tuple<int, int> getPosition() {
        return X11Platform::getPosition();
    }

    static std::tuple<int, int> getSize() {
        return X11Platform::getSize();
    }

    static void moveTo(int x, int y) {
        X11Platform::moveTo(x, y);
    }

    static void mouseDown(int x, int y, const std::string& button) {
        X11Platform::mouseDown(x, y, button);
    }

    static void mouseUp(int x, int y, const std::string& button) {
        X11Platform::mouseUp(x, y, button);
    }

    static void click(int x, int y, const std::string& button) {
        X11Platform::click(x, y, button);
    }

    static void scroll(int clicks, int x, int y) {
        X11Platform::scroll(clicks, x, y);
    }

    static void keyDown(const std::string& key) {
        X11Platform::keyDown(key);
    }

    static void keyUp(const std::string& key) {
        X11Platform::keyUp(key);
    }
};
