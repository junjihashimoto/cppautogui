#include <windows.h>
#include <stdexcept>
#include <string>
#include <tuple>

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

class WindowsPlatform {
public:
    static std::tuple<int, int> getPosition() {
        POINT p;
        if (GetCursorPos(&p)) {
            return std::make_tuple(p.x, p.y);
        }
        throw CppAutoGUIException("Failed to get cursor position");
    }

    static std::tuple<int, int> getSize() {
        return std::make_tuple(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    }

    static void moveTo(int x, int y) {
        SetCursorPos(x, y);
    }

    static void mouseDown(int x, int y, const std::string& button) {
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

    static void mouseUp(int x, int y, const std::string& button) {
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

    static void click(int x, int y, const std::string& button) {
        mouseDown(x, y, button);
        mouseUp(x, y, button);
    }

    static void scroll(int clicks, int x, int y) {
        INPUT input = {0};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        input.mi.mouseData = clicks * WHEEL_DELTA;
        SendInput(1, &input, sizeof(INPUT));
    }

    static void keyDown(const std::string& key) {
        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = VkKeyScan(key[0]);
        SendInput(1, &input, sizeof(INPUT));
    }

    static void keyUp(const std::string& key) {
        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = VkKeyScan(key[0]);
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }
};

class CppAutoGUI {
public:
    static std::tuple<int, int> getPosition() {
        return WindowsPlatform::getPosition();
    }

    static std::tuple<int, int> getSize() {
        return WindowsPlatform::getSize();
    }

    static void moveTo(int x, int y) {
        WindowsPlatform::moveTo(x, y);
    }

    static void mouseDown(int x, int y, const std::string& button) {
        WindowsPlatform::mouseDown(x, y, button);
    }

    static void mouseUp(int x, int y, const std::string& button) {
        WindowsPlatform::mouseUp(x, y, button);
    }

    static void click(int x, int y, const std::string& button) {
        WindowsPlatform::click(x, y, button);
    }

    static void scroll(int clicks, int x, int y) {
        WindowsPlatform::scroll(clicks, x, y);
    }

    static void keyDown(const std::string& key) {
        WindowsPlatform::keyDown(key);
    }

    static void keyUp(const std::string& key) {
        WindowsPlatform::keyUp(key);
    }
};

int main() {
    try {
        CppAutoGUI::moveTo(100, 100);
        CppAutoGUI::click(100, 100, "left");
        CppAutoGUI::scroll(3, 100, 100);
        CppAutoGUI::keyDown("a");
        CppAutoGUI::keyUp("a");
    } catch (const CppAutoGUIException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
