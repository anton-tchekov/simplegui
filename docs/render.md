# 2D Rendering functions

## SgColor

A `uint32_t` representing a RGB color.

Definitions of all CSS colors are provided. For example:
`SG_YELLOW = 0xFFFF00`, `SG_DARKRED = 0x8B0000`,
`SG_TURQUOISE = 0x40E0D0` etc.

### Get the red color component of a color (0-255)

```c
static inline int sg_color_r(SgColor color);
```

### Get the green color component of a color (0-255)

```c
static inline int sg_color_g(SgColor color);
```

### Get the blue color component of a color (0-255)

```c
static inline int sg_color_b(SgColor color);
```

## SgPoint

An alias for `SDL_Point`, a struct containing
two ints: `x` and `y`.

### Create a SgPoint

```c
static inline SgPoint sg_point(int x, int y);
```

Constructor function, useful for creating a `SgPoint` struct
as a parameter in a function call.

## SgSize

A struct containing two ints: `w` and `h`.

### Create a SgSize

```c
static inline SgSize sg_size(int w, int h)
```

Constructor function, useful for creating a `SgSize` struct
as a parameter in a function call.

## SgRect

An alias for `SDL_Rect`, a struct containing
four ints: `x`, `y`, `w`, `h`.

### Create a SgRect

```c
static inline SgRect sg_rect(int x, int y, int w, int h);
```

Constructor function, useful for creating a `SgRect` struct
as a parameter in a function call.

### Check if a point is within a rectangle

```c
bool sg_rect_contains_point(SgRect rect, SgPoint p);
```

### Check if the mouse cursor is within a rectangle

```c
bool sg_rect_contains_mouse(SgRect rect);
```

## Fill a rectangle

```c
void sg_fill_rect(SgRect rect, SgColor color);
```

## Draw a rectangle

```c
void sg_draw_rect(SgRect rect, int border, SgColor color);
```

The border extends inwards.

## Draw a character

```c
int sg_render_char(SgPoint p, uint8_t c, SgColor color);
```

Point is top left point of the character.
Uses the currently selected font atlas.
Returns the width of the character.

## Draw a string

```c
int sg_render_string(SgPoint p, const char *s, SgColor color);
```

Point is top left point of the string.
Uses the currently selected font atlas.
Returns the width of the string.

## Draw a string with maximum length

```c
int sg_render_string_len(SgPoint p, const char *s, size_t len, SgColor color);
```

Point is top left point of the string.
Uses the currently selected font atlas.
Returns the width of the string.

## Get the width of a character

```c
int sg_char_width(uint8_t c);
```

## Get the height of a character

```c
int sg_char_height(uint8_t c);
```

## Get the width of a string

```c
int sg_string_width(const char *s);
```

## Get the width of a string with maximum length

```c
int sg_string_width_len(const char *s, size_t len);
```
