{-# LANGUAGE QuasiQuotes #-}
{-# LANGUAGE TemplateHaskell #-}

module CppAutoGUI where

import Language.C.Inline.Cpp
import Language.C.Inline.Cpp.Exceptions
import qualified Language.C.Inline.Cpp as C

C.context C.cppCtx
C.include "<iostream>"
C.include "<stdexcept>"
C.include "<string>"
C.include "<vector>"
C.include "<chrono>"
C.include "<thread>"
C.include "<tuple>"
C.include "<cmath>"
C.include "<algorithm>"
C.include "<functional>"
C.include "<map>"
C.include "<memory>"
C.include "<ctime>"
C.include "<sstream>"
C.include "<iomanip>"
C.include "<fstream>"
C.include "<cassert>"
C.include "<cppautogui.h>"

getPosition :: IO (Int, Int)
getPosition = [C.block| std::tuple<int, int> {
    return CppAutoGUI::getPosition();
  } |]

getSize :: IO (Int, Int)
getSize = [C.block| std::tuple<int, int> {
    return CppAutoGUI::getSize();
  } |]

moveTo :: Int -> Int -> IO ()
moveTo x y = [C.block| void {
    CppAutoGUI::moveTo($(int x), $(int y));
  } |]

mouseDown :: Int -> Int -> String -> IO ()
mouseDown x y button = [C.block| void {
    CppAutoGUI::mouseDown($(int x), $(int y), std::string($(const char* button)));
  } |]

mouseUp :: Int -> Int -> String -> IO ()
mouseUp x y button = [C.block| void {
    CppAutoGUI::mouseUp($(int x), $(int y), std::string($(const char* button)));
  } |]

click :: Int -> Int -> String -> IO ()
click x y button = [C.block| void {
    CppAutoGUI::click($(int x), $(int y), std::string($(const char* button)));
  } |]

scroll :: Int -> Int -> Int -> IO ()
scroll clicks x y = [C.block| void {
    CppAutoGUI::scroll($(int clicks), $(int x), $(int y));
  } |]

keyDown :: String -> IO ()
keyDown key = [C.block| void {
    CppAutoGUI::keyDown(std::string($(const char* key)));
  } |]

keyUp :: String -> IO ()
keyUp key = [C.block| void {
    CppAutoGUI::keyUp(std::string($(const char* key)));
  } |]

screenshot :: IO (Ptr Image)
screenshot = [C.block| std::shared_ptr<image> {
    return CppAutoGUI::screenshot();
  } |]

locateOnScreen :: String -> IO (Int, Int, Int, Int)
locateOnScreen imagePath = [C.block| std::tuple<int, int, int, int> {
    return CppAutoGUI::locateOnScreen(std::string($(const char* imagePath)));
  } |]

locateCenterOnScreen :: String -> IO (Int, Int)
locateCenterOnScreen imagePath = [C.block| std::tuple<int, int> {
    return CppAutoGUI::locateCenterOnScreen(std::string($(const char* imagePath)));
  } |]

pixel :: Int -> Int -> IO (Int, Int, Int)
pixel x y = [C.block| std::tuple<int, int, int> {
    return CppAutoGUI::pixel($(int x), $(int y));
  } |]

pixelMatchesColor :: Int -> Int -> (Int, Int, Int) -> Int -> IO Bool
pixelMatchesColor x y (r, g, b) tolerance = [C.block| bool {
    return CppAutoGUI::pixelMatchesColor($(int x), $(int y), std::make_tuple($(int r), $(int g), $(int b)), $(int tolerance));
  } |]
