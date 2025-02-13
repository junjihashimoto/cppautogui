.. default-role:: code

===========
Cheat Sheet
===========

This is a quickstart reference to using CppAutoGUI. CppAutoGUI is a cross-platform GUI automation library that works with C++. You can control the mouse and keyboard as well as perform basic image recognition to automate tasks on your computer.

**All the keyword arguments in the examples on this page are optional.**

    #include "cppautogui.h"

CppAutoGUI works on Windows/Mac/Linux. To build the project, use CMake:

    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

General Functions
-----------------

    auto [x, y] = CppAutoGUI::getPosition();  // current mouse x and y
    auto [width, height] = CppAutoGUI::getSize();  // current screen resolution width and height
    bool onScreen = CppAutoGUI::onScreen(x, y);  // True if x & y are within the screen.

Fail-Safes
----------

Set up a 2.5 second pause after each CppAutoGUI call:

    CppAutoGUI::PAUSE = 2.5;

When fail-safe mode is `true`, moving the mouse to the upper-left will raise a `CppAutoGUI::FailSafeException` that can abort your program:

    CppAutoGUI::FAILSAFE = true;

Mouse Functions
---------------

XY coordinates have 0, 0 origin at top left corner of the screen. X increases going right, Y increases going down.

    CppAutoGUI::moveTo(x, y, duration);  // move mouse to XY coordinates over duration seconds
    CppAutoGUI::moveRel(xOffset, yOffset, duration);  // move mouse relative to its current position

If `duration` is 0 or unspecified, movement is immediate. Note: dragging on Mac can't be immediate.

    CppAutoGUI::dragTo(x, y, duration);  // drag mouse to XY
    CppAutoGUI::dragRel(xOffset, yOffset, duration);  // drag mouse relative to its current position

Calling `click()` just clicks the mouse once with the left button at the mouse's current location, but the keyword arguments can change that:

    CppAutoGUI::click(x, y, clicks, interval, button);

The `button` keyword argument can be `'left'`, `'middle'`, or `'right'`.

All clicks can be done with `click()`, but these functions exist for readability. Keyword args are optional:

    CppAutoGUI::rightClick(x, y);
    CppAutoGUI::middleClick(x, y);
    CppAutoGUI::doubleClick(x, y);
    CppAutoGUI::tripleClick(x, y);

Positive scrolling will scroll up, negative scrolling will scroll down:

    CppAutoGUI::scroll(amount, x, y);

Individual button down and up events can be called separately:

    CppAutoGUI::mouseDown(x, y, button);
    CppAutoGUI::mouseUp(x, y, button);

Keyboard Functions
------------------

Key presses go to wherever the keyboard cursor is at function-calling time.

    CppAutoGUI::write("Hello world!\n", interval);  // useful for entering text, newline is Enter

A list of key names can be passed too:

    CppAutoGUI::write({"a", "b", "c", "left", "backspace", "enter", "f1"}, interval);

The full list of key names is in `CppAutoGUI::KEYBOARD_KEYS`.

Keyboard hotkeys like Ctrl-S or Ctrl-Shift-1 can be done by passing a list of key names to `hotkey()`:

    CppAutoGUI::hotkey({"ctrl", "c"});  // ctrl-c to copy
    CppAutoGUI::hotkey({"ctrl", "v"});  // ctrl-v to paste

Individual button down and up events can be called separately:

    CppAutoGUI::keyDown(key_name);
    CppAutoGUI::keyUp(key_name);

Message Box Functions
---------------------

If you need to pause the program until the user clicks OK on something, or want to display some information to the user, the message box functions have similar names that JavaScript has:

    CppAutoGUI::alert("This displays some text with an OK button.");
    auto result = CppAutoGUI::confirm("This displays text and has an OK and Cancel button.");
    // result will be "OK" or "Cancel"
    auto input = CppAutoGUI::prompt("This lets the user type in a string and press OK.");
    // input will be the string entered or None if Cancel was clicked

Screenshot Functions
--------------------

CppAutoGUI uses an external library for its image-related data.

On Linux, you must run `sudo apt-get install scrot` to use the screenshot features.

    auto screenshot = CppAutoGUI::screenshot();  // returns an image object
    screenshot.save("foo.png");  // saves the screenshot to a file

If you have an image file of something you want to click on, you can find it on the screen with `locateOnScreen()`.

    auto location = CppAutoGUI::locateOnScreen("looksLikeThis.png");  // returns (left, top, width, height) of first place it is found

The `locateAllOnScreen()` function will return a generator for all the locations it is found on the screen:

    for (auto location : CppAutoGUI::locateAllOnScreen("looksLikeThis.png")) {
        // process each location
    }

    auto locations = CppAutoGUI::locateAllOnScreen("looksLikeThis.png");
    // locations will be a list of (left, top, width, height) tuples

The `locateCenterOnScreen()` function just returns the XY coordinates of the middle of where the image is found on the screen:

    auto [centerX, centerY] = CppAutoGUI::locateCenterOnScreen("looksLikeThis.png");  // returns center x and y

These functions return `None` if the image couldn't be found on the screen.

Note: The locate functions are slow and can take a full second or two.
