cmake_minimum_required(VERSION 3.10)

project(CppAutoGUI)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_library(cppautogui STATIC
  src/cppautogui.cpp
  src/cppautogui.h
  src/bmp.cpp
  src/data.cpp
  src/data.h
  src/image.cpp
  src/image.h
  src/jpeg.cpp
  src/pfile.h
  src/png.cpp
)

find_package(JPEG REQUIRED)
include_directories(${JPEG_INCLUDE_DIR})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
    find_library(QUARTZ_FRAMEWORK Quartz)
    find_library(COREFRAMEWORK CoreFoundation)
    find_library(COREGRAPHICS CoreGraphics)

target_link_libraries(cppautogui
        "${QUARTZ_FRAMEWORK}"
        "${COREFRAMEWORK}"
        "${COREGRAPHICS}"
    )

elseif(UNIX)
    find_package(X11 REQUIRED)
    find_library(XTEST_LIB Xtst)

    include_directories(${X11_INCLUDE_DIR})
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
        ${JPEG_LIBRARIES}
        ${PNG_LIBRARIES}
    )
elseif(UNIX)
    target_link_libraries(test_cppautogui
        cppautogui
        ${X11_LIBRARIES}
        ${XTEST_LIB}
        ${JPEG_LIBRARIES}
        ${PNG_LIBRARIES}    )
else()
    target_link_libraries(test_cppautogui cppautogui
    ${JPEG_LIBRARIES}
    ${PNG_LIBRARIES})
endif()

add_test(NAME test_cppautogui COMMAND test_cppautogui)

