# Simple GUI Library

SimpleGUI is an Immediate Mode GUI Library based on SDL2.
It is a Single Header Library, which makes it easy to
add to a project.

## TODO

This project is work in progress, most of the functionality
is still missing:

- Select
- Textbox
- Styling functions

- Add Asserts / Error checks where missing
- Create example projects
- Write documentation
- Add screenshots

## Features

The following elements are (will be) supported:

- Label
- Button
- Textbox
- Checkbox
- Slider
- Select

Elements can be styled (also independently from each other).
You can change text color, background color, border color,
border thickness, font and font size, etc.
You can also add custom characters and icons to existing fonts.

SimpleGUI has a fallback font built into the source code, but can also
load fonts from `.ttf` files (via SDL2_ttf)

## Installation

Add `simplegui.h` to your project directory and add the following
two lines to one source file.

```c
#define SIMPLEGUI_IMPLEMENTATION
#include "simplegui.h"
```

All other source files that use SimpleGUI should only include the header.
```c
#include "simplegui.h"`
```

## Screenshots



## Dependencies

You will need to link `SDL2`, `SDL2_image` and `SDL2_ttf` into your
project.

## Directory structure

The `examples/` directory contains code examples for how to use
SimpleGUI.

The `docs/` directory contains [Documentation](docs/README.md) for the functions
of SimpleGUI.
