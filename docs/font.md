# Fonts

SimpleGUI uses a Texture Atlas approach (called `SgFontAtlas`)
to render text. Each font atlas has 16x16 equally-sized
square cells.

## SgFont

Alias for `TTF_Font *`.

## Load font from .ttf file

```c
SgFont sg_font_load(const char *file, int size);
```

## Free font

```c
void sg_font_destroy(SgFont font);
```

## SgFontAtlas

Pointer to struct that stores font atlas information.

## Create font atlas


SgFontAtlas sg_font_atlas_create(int max_char_size, int font_height);
void sg_set_fontatlas(SgFontAtlas atlas);

## Free font atlas

```c
void sg_fontatlas_destroy(SgFontAtlas atlas);
```

## Add printable ASCII characters from font to font atlas

```c
int sg_fontatlas_add_ascii(SgFontAtlas atlas, SgFont font);
```

## Add specific character from font to font atlas

```c
int sg_fontatlas_add_char(SgFontAtlas atlas, SgFont font, uint8_t c);
```

## Add UTF-8 character from font to font atlas

```c
int sg_fontatlas_add_utf8(SgFontAtlas atlas, SgFont font, const char *utf8, uint8_t index);
```

The character is remapped to the specified byte index.

## Add icon from image file to font atlas

```c
int sg_fontatlas_add_icon(SgFontAtlas atlas, const char *file, uint8_t index);
```

The icon is mapped to the specified byte index.

## Blit (part of) SgSurface (SDL_Surface *) to font atlas

```c
int sg_fontatlas_blit_surface(SgFontAtlas atlas, SgSurface surface, SgRect src, uint8_t index);
```

## Add bitmap to font atlas

```c
void sg_fontatlas_add_binary(SgFontAtlas atlas, const uint8_t *bytes, SgSize size, uint8_t index);
```

Add icon/character described by raw bytes (1-bit per pixel) to font atlas.

## Add default checkmark icon to new font atlas

```c
void sg_fontatlas_add_default_checkmark(SgFontAtlas atlas);
```

Used for checkbox control.

## Add default select icon to new font atlas

```c
void sg_fontatlas_add_default_select(SgFontAtlas atlas);
```

Used for select (list box) control.

## Add all default icons to new font atlas

```c
void sg_fontatlas_add_default_icons(SgFontAtlas atlas);
```

## Update font atlas

```c
void sg_fontatlas_update(SgFontAtlas atlas);
```

Make changes made to font atlas take into effect.
