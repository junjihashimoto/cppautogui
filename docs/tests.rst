.. default-role:: code

=======
Testing
=======

The unit tests for CppAutoGUI are currently not comprehensive. The tests (in basicTests.cpp) cover the following:

- onScreen()
- size()
- position()
- moveTo()
- moveRel()
- typewrite()
- PAUSE

Platforms Tested
================

- C++17
- Windows
- macOS
- Linux

The keyboard functions do not work on Ubuntu when run in VirtualBox on Windows.
