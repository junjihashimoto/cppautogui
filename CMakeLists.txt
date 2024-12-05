cmake_minimum_required(VERSION 3.10)

project(CppAutoGUI)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    include_directories(${CMAKE_SOURCE_DIR}/cppautogui)
    add_library(cppautogui STATIC cppautogui/cppautogui_win.cpp cppautogui/cppautogui_win.h cppautogui/cppautogui.cpp cppautogui/cppautogui.h)

elseif(APPLE)
    find_library(QUARTZ_FRAMEWORK Quartz)
    find_library(COREFRAMEWORK CoreFoundation)
    find_library(COREGRAPHICS CoreGraphics)

    include_directories(${CMAKE_SOURCE_DIR}/cppautogui)
    add_library(cppautogui STATIC cppautogui/cppautogui_osx.cpp cppautogui/cppautogui_osx.h cppautogui/cppautogui.cpp cppautogui/cppautogui.h)

    target_link_libraries(cppautogui
        "${QUARTZ_FRAMEWORK}"
        "${COREFRAMEWORK}"
        "${COREGRAPHICS}"
    )

elseif(UNIX)
    find_package(X11 REQUIRED)
    find_library(XTEST_LIB Xtst)

    include_directories(${X11_INCLUDE_DIR})
    include_directories(${CMAKE_SOURCE_DIR}/cppautogui)

    add_library(cppautogui STATIC cppautogui/cppautogui_x11.cpp cppautogui/cppautogui_x11.h cppautogui/cppautogui.cpp cppautogui/cppautogui.h)

    target_link_libraries(cppautogui
        ${X11_LIBRARIES}
        ${XTEST_LIB}
    )
endif()

enable_testing()

add_executable(test_cppautogui tests/test_cppautogui.cpp)

if(APPLE)
    target_link_libraries(test_cppautogui
        cppautogui
        "${QUARTZ_FRAMEWORK}"
        "${COREFRAMEWORK}"
        "${COREGRAPHICS}"
    )
elseif(UNIX)
    target_link_libraries(test_cppautogui
        cppautogui
        ${X11_LIBRARIES}
        ${XTEST_LIB}
    )
else()
    target_link_libraries(test_cppautogui cppautogui)
endif()

add_test(NAME test_cppautogui COMMAND test_cppautogui)

