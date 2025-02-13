.. CppAutoGUI documentation master file, created by
   sphinx-quickstart on Sun Jul 20 12:59:43 2014.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to CppAutoGUI's documentation!
=====================================


CppAutoGUI lets your C++ programs control the mouse and keyboard to automate interactions with other applications. The API is designed to be simple. CppAutoGUI works on Windows, macOS, and Linux.

To install, follow the instructions in the :doc:`install` page.

The source code is available on: https://github.com/junjihashimoto/cppautogui

CppAutoGUI has several features:

* Moving the mouse and clicking in the windows of other applications.
* Sending keystrokes to applications (for example, to fill out forms).
* Take screenshots, and given an image (for example, of a button or checkbox), and find it on the screen.
* Locate an application's window, and move, resize, maximize, minimize, or close it (Windows-only, currently).
* Display alert and message boxes.

Here's `a YouTube video of a bot automatically playing the game Sushi Go Round <https://www.youtube.com/watch?v=lfk_T6VKhTE>`_. The bot watches the game's application window and searches for images of sushi orders. When it finds one, it clicks the ingredient buttons to make the sushi. It also clicks the phone in the game to order more ingredients as needed. The bot is completely autonomous and can finish all seven days of the game. This is the kind of automation that CppAutoGUI is capable of.

Examples
========

.. code:: cpp

    #include "cppautogui.h"

    int main() {
        try {
            auto [screenWidth, screenHeight] = CppAutoGUI::getSize(); // Get the size of the primary monitor.
            std::cout << screenWidth << ", " << screenHeight << std::endl;

            auto [currentMouseX, currentMouseY] = CppAutoGUI::getPosition(); // Get the XY position of the mouse.
            std::cout << currentMouseX << ", " << currentMouseY << std::endl;

            CppAutoGUI::moveTo(100, 150); // Move the mouse to XY coordinates.

            CppAutoGUI::click(100, 200, "left");  // Move the mouse to XY coordinates and click it.
            CppAutoGUI::click("button.png"); // Find where button.png appears on the screen and click it.

            CppAutoGUI::move(400, 0);      // Move the mouse 400 pixels to the right of its current position.
            CppAutoGUI::doubleClick(500, 500, "left");     // Double click the mouse.
            CppAutoGUI::moveTo(500, 500, 2, CppAutoGUI::easeInOutQuad);  // Use tweening/easing function to move mouse over 2 seconds.

            CppAutoGUI::write("Hello world!", 0.25);  // type with quarter-second pause in between each key
            CppAutoGUI::press("esc");     // Press the Esc key.

            CppAutoGUI::keyDown("shift");  // Press the Shift key down and hold it.
            CppAutoGUI::write({"left", "left", "left", "left"});  // Press the left arrow key 4 times.
            CppAutoGUI::keyUp("shift");  // Shift key is released automatically.

            CppAutoGUI::hotkey({"ctrl", "c"}); // Press the Ctrl-C hotkey combination.

            CppAutoGUI::alert("This is the message to display."); // Make an alert box appear and pause the program until OK is clicked.
        } catch (const CppAutoGUIException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        return 0;
    }

This example drags the mouse in a square spiral shape in MS Paint (or any graphics drawing program):

.. code:: cpp

    #include "cppautogui.h"

    int main() {
        try {
            int distance = 200;
            while (distance > 0) {
                CppAutoGUI::drag(distance, 0, 0.5);   // move right
                distance -= 5;
                CppAutoGUI::drag(0, distance, 0.5);   // move down
                CppAutoGUI::drag(-distance, 0, 0.5);  // move left
                distance -= 5;
                CppAutoGUI::drag(0, -distance, 0.5);  // move up
            }
        } catch (const CppAutoGUIException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        return 0;
    }

.. image:: square_spiral.png

The benefit of using CppAutoGUI, as opposed to a script that directly generates the image file, is that you can use the brush tools that MS Paint provides.

FAQ: Frequently Asked Questions
===============================

Send questions to al@inventwithpython.com

**Q: Can CppAutoGUI work on Android, iOS, or tablet/smartphone apps.**

A: Unfortunately no. CppAutoGUI only runs on Windows, macOS, and Linux.

**Q: Does CppAutoGUI work on multi-monitor setups.**

A: No, right now CppAutoGUI only handles the primary monitor.

**Q: Does CppAutoGUI do OCR?**

A: No, but this is a feature that's on the roadmap.

**Q: Can CppAutoGUI do keylogging, or detect if a key is currently pressed down?**

A: No, CppAutoGUI cannot do this currently.


Fail-Safes
==========

.. image:: sorcerers_apprentice_brooms.png

Like the enchanted brooms from the Sorcerer’s Apprentice programmed to keep filling (and then overfilling) the bath with water, a bug in your program could make it go out of control. It's hard to use the mouse to close a program if the mouse cursor is moving around on its own.

As a safety feature, a fail-safe feature is enabled by default. When a CppAutoGUI function is called, if the mouse is in any of the four corners of the primary monitor, they will raise a ``CppAutoGUI::FailSafeException``. There is a one-tenth second delay after calling every CppAutoGUI functions to give the user time to slam the mouse into a corner to trigger the fail safe.

You can disable this failsafe by setting ``CppAutoGUI::FAILSAFE = false``. **I HIGHLY RECOMMEND YOU DO NOT DISABLE THE FAILSAFE.**

The tenth-second delay is set by the ``CppAutoGUI::PAUSE`` setting, which is ``0.1`` by default. You can change this value. There is also a ``CppAutoGUI::DARWIN_CATCH_UP_TIME`` setting which adds an additional delay on macOS after keyboard and mouse events, since the operating system appears to need a delay after CppAutoGUI issues these events. It is set to ``0.01`` by default, adding an additional hundredth-second delay.


Contents:

.. toctree::
   :maxdepth: 2

   install.rst
   quickstart.rst
   mouse.rst
   keyboard.rst
   msgbox.rst
   screenshot.rst
   tests.rst
   roadmap.rst

   source/modules.rst

This documentation is still a work in progress.


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

