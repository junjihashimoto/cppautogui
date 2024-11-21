CppAutoGUI
=========

CppAutoGUI is a cross-platform GUI automation C++ library for human beings. Used to programmatically control the mouse & keyboard.

To build the project, use CMake:

```sh
mkdir build
cd build
cmake ..
make
```

Full documentation available at https://cppautogui.readthedocs.org

Source code available at https://github.com/junjihashimoto/cppautogui

Dependencies
============

CppAutoGUI supports Windows, macOS, and Linux. If you are building CppAutoGUI from source:

Windows has no dependencies.

macOS needs the Quartz framework.

Linux needs the X11 and XTest libraries.

Example Usage
=============

Keyboard and Mouse Control
--------------------------

The x, y coordinates used by CppAutoGUI have the 0, 0 origin coordinates in the top left corner of the screen. The x coordinates increase going to the right (just as in mathematics) but the y coordinates increase going down (the opposite of mathematics). On a screen that is 1920 x 1080 pixels in size, coordinates 0, 0 are for the top left while 1919, 1079 is for the bottom right.

Currently, CppAutoGUI only works on the primary monitor. CppAutoGUI isn't reliable for the screen of a second monitor (the mouse functions may or may not work on multi-monitor setups depending on your operating system and version).

All keyboard presses done by CppAutoGUI are sent to the window that currently has focus, as if you had pressed the physical keyboard key.

```cpp
#include "cppautogui.h"

int main() {
    try {
        CppAutoGUI::moveTo(100, 150); // Move the mouse to the x, y coordinates 100, 150.
        CppAutoGUI::click(100, 150, "left"); // Click the mouse at the x, y coordinates 100, 150.
        CppAutoGUI::moveTo(200, 220); // Move the mouse to the x, y coordinates 200, 220.
        CppAutoGUI::click(200, 220, "left"); // Click the mouse at the x, y coordinates 200, 220.
        CppAutoGUI::move(0, 10); // Move mouse 10 pixels down, that is, move the mouse relative to its current position.
        CppAutoGUI::doubleClick(500, 500, "left"); // Double click the mouse at the x, y coordinates 500, 500.
        CppAutoGUI::moveTo(500, 500, 2, CppAutoGUI::easeInOutQuad); // Use tweening/easing function to move mouse over 2 seconds.
        CppAutoGUI::write("Hello world!", 0.25); // Type with quarter-second pause in between each key.
        CppAutoGUI::press("esc"); // Simulate pressing the Escape key.
        CppAutoGUI::keyDown("shift");
        CppAutoGUI::write({"left", "left", "left", "left", "left", "left"});
        CppAutoGUI::keyUp("shift");
        CppAutoGUI::hotkey({"ctrl", "c"});
    } catch (const CppAutoGUIException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
```

Screenshot Functions
--------------------

CppAutoGUI uses an external library for image-related features. You can also locate where an image is on the screen:

```cpp
#include "cppautogui.h"

int main() {
    try {
        auto buttonLocation = CppAutoGUI::locateOnScreen("button.png"); // returns (left, top, width, height) of matching region
        auto [buttonX, buttonY] = CppAutoGUI::center(buttonLocation);
        CppAutoGUI::click(buttonX, buttonY, "left"); // clicks the center of where the button was found
    } catch (const CppAutoGUIException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
```

The locateCenterOnScreen() function returns the center of this match region:

```cpp
#include "cppautogui.h"

int main() {
    try {
        auto [buttonX, buttonY] = CppAutoGUI::locateCenterOnScreen("button.png"); // returns (x, y) of matching region
        CppAutoGUI::click(buttonX, buttonY, "left"); // clicks the center of where the button was found
    } catch (const CppAutoGUIException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
```

How Does CppAutoGUI Work?
========================

The three major operating systems (Windows, macOS, and Linux) each have different ways to programmatically control the mouse and keyboard. This can often involve confusing, obscure, and deeply technical details. The job of CppAutoGUI is to hide all of this complexity behind a simple API.

* On Windows, CppAutoGUI accesses the Windows API (also called the WinAPI or win32 API) through the built-in `windows.h` header.

* On macOS, CppAutoGUI uses the Quartz framework to access the Cocoa API.

* On Linux, CppAutoGUI uses the X11 and XTest libraries to access the X Window System.

Support
-------

If you find this project helpful and would like to support its development, consider donating to its creator.
