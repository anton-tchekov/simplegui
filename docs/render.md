# 2D Rendering functions

## SgPoint

An alias for `SDL_Point`, a struct containing
two ints: `x` and `y`.

## SgSize

A struct containing two ints: `w` and `h`.

## SgRect

An alias for `SDL_Rect`, a struct containing
four ints: `x`, `y`, `w`, `h`.

## Fill a rectangle

`void sg_fill_rect(SgRect rect, SgColor color);`

## Draw a rectangle

`void sg_draw_rect(SgRect rect, int border, SgColor color);`

The border extends inwards.

## Draw a character

`int sg_render_char(int x, int y, uint8_t c, SgColor color);`

Uses the currently selected font atlas.
Returns the width of the character.

## Draw a string

`void sg_render_string(int x, int y, const char *s, SgColor color);`

Uses the currently selected font atlas.
Returns the width of the string.

## Draw a string with maximum length

`void sg_render_string_len(int x, int y, const char *s, size_t len, SgColor color);`

Uses the currently selected font atlas.
Returns the width of the string.

## Get the width of a character

`int sg_char_width(uint8_t c);`

## Get the height of a character

`int sg_char_height(uint8_t c);`

## Get the width of a string

`int sg_string_width(const char *s);`

## Get the width of a string with maximum length

`int sg_string_width_len(const char *s, size_t len);`
