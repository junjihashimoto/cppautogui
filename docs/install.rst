.. default-role:: code

============
Installation
============

To install CppAutoGUI, follow the instructions below for your operating system.

Windows
-------

On Windows, you can use CMake to build the project:

    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

macOS
-----

On macOS, you can use CMake to build the project. Make sure you have the Quartz framework installed:

    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

Linux
-----

On Linux, you can use CMake to build the project. Make sure you have the X11 and XTest libraries installed:

    ```sh
    sudo apt-get install libx11-dev libxtst-dev
    mkdir build
    cd build
    cmake ..
    make
    ```

CppAutoGUI will install the necessary modules it depends on, including the Quartz framework on macOS and the X11 and XTest libraries on Linux.
