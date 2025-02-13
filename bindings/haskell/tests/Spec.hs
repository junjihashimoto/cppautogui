{-# LANGUAGE QuasiQuotes #-}
{-# LANGUAGE TemplateHaskell #-}

module Main where

import Test.Hspec
import qualified Graphics.UI.AutoGUI as AG

main :: IO ()
main = hspec $ do
  describe "CppAutoGUI Haskell bindings" $ do
    it "getPosition" $ do
      (x, y) <- AG.getPosition
      x `shouldSatisfy` (>= 0)
      y `shouldSatisfy` (>= 0)

    it "getSize" $ do
      (width, height) <- AG.getSize
      width `shouldSatisfy` (>= 0)
      height `shouldSatisfy` (>= 0)

    it "moveTo" $ do
      AG.moveTo 100 100
      (x, y) <- AG.getPosition
      x `shouldBe` 100
      y `shouldBe` 100

    it "mouseDown" $ do
      AG.mouseDown 100 100 "left"
      -- No assertion, just ensure it doesn't throw an exception

    it "mouseUp" $ do
      AG.mouseUp 100 100 "left"
      -- No assertion, just ensure it doesn't throw an exception

    it "click" $ do
      AG.click 100 100 "left"
      -- No assertion, just ensure it doesn't throw an exception

    it "scroll" $ do
      AG.scroll 3 100 100
      -- No assertion, just ensure it doesn't throw an exception

    it "keyDown" $ do
      AG.keyDown "a"
      -- No assertion, just ensure it doesn't throw an exception

    it "keyUp" $ do
      AG.keyUp "a"
      -- No assertion, just ensure it doesn't throw an exception

    it "screenshot" $ do
      _ <- AG.screenshot
      -- No assertion, just ensure it doesn't throw an exception

    it "locateOnScreen" $ do
      (left, top, width, height) <- AG.locateOnScreen "button.png"
      left `shouldSatisfy` (>= 0)
      top `shouldSatisfy` (>= 0)
      width `shouldSatisfy` (>= 0)
      height `shouldSatisfy` (>= 0)

    it "locateCenterOnScreen" $ do
      (x, y) <- AG.locateCenterOnScreen "button.png"
      x `shouldSatisfy` (>= 0)
      y `shouldSatisfy` (>= 0)

    it "pixel" $ do
      (r, g, b) <- AG.pixel 100 100
      r `shouldSatisfy` (>= 0)
      g `shouldSatisfy` (>= 0)
      b `shouldSatisfy` (>= 0)

    it "pixelMatchesColor" $ do
      matches <- AG.pixelMatchesColor 100 100 (255, 255, 255) 0
      matches `shouldBe` True
