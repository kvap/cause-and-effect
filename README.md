Cause and Effect
================

This is a puzzle platformer game where you control two characters at the same
time with the same controller. Since the characters move in opposite directions
the challenge is to put them both into some target area.

The scenes are expected to be in SVG format and can be created with Inkscape or
other SVG editor. The loader is pretty simple and only supports rectangles.
Please see the example scenes in `res/scenes/`.

Dependencies
------------

Cause and Effect requires these libraries:

- `freetype2`
- `glew`
- `libxml++-2.6`
- `libpng`
- `glfw3`
- `libxml-2.0`
- `Box2D`
- `OpenGL`
- `OpenAL`

`OpenGL` and `OpenAL` come preinstalled on macOS, and the rest can be installed
with `brew`, except `Box2D`.

![cause-and-effect optimized](https://cloud.githubusercontent.com/assets/663660/26762235/24ae3ab8-493f-11e7-825b-6013b876cdc7.gif)
