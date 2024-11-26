cmake_minimum_required(VERSION 3.10)

project(CppAutoGUI)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    include_directories(${CMAKE_SOURCE_DIR}/cppautogui)
    add_library(libcppautogui STATIC cppautogui/cppautogui_win.cpp cppautogui/cppautogui_win.h)
    add_executable(cppautogui cppautogui/cppautogui.cpp)
    target_link_libraries(cppautogui libcppautogui)
elseif(APPLE)
    find_library(QUARTZ_FRAMEWORK Quartz)
    find_library(COREFRAMEWORK CoreFoundation)
    find_library(COREGRAPHICS CoreGraphics)
    include_directories(${CMAKE_SOURCE_DIR}/cppautogui)
    add_library(libcppautogui STATIC cppautogui/cppautogui_osx.cpp cppautogui/cppautogui_osx.h)
    add_executable(cppautogui cppautogui/cppautogui.cpp)
    target_link_libraries(cppautogui libcppautogui ${QUARTZ_FRAMEWORK} ${COREFRAMEWORK} ${COREGRAPHICS})
elseif(UNIX)
    find_package(X11 REQUIRED)
    include_directories(${CMAKE_SOURCE_DIR}/cppautogui)
    add_library(libcppautogui STATIC cppautogui/cppautogui_x11.cpp cppautogui/cppautogui_x11.h)
    add_executable(cppautogui cppautogui/cppautogui.cpp)
    target_link_libraries(cppautogui libcppautogui ${X11_LIBRARIES})
endif()

enable_testing()
add_executable(test_cppautogui tests/test_cppautogui.cpp)
target_link_libraries(test_cppautogui libcppautogui)
add_test(NAME test_cppautogui COMMAND test_cppautogui)
