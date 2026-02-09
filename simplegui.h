/* ========================================================================== */
/* HEADER */

#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>

/* ========================================================================== */
/* sdl2 includes */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

/* Util */
#define SG_ARRLEN(X) (sizeof(X) / sizeof(*X))

static inline int sg_min(int a, int b)
{
	return a < b ? a : b;
}

static inline int sg_max(int a, int b)
{
	return a > b ? a : b;
}

static inline double sg_fclamp(double x, double min, double max)
{
	if(x < min) { return min; }
	if(x > max) { return max; }
	return x;
}

/* SgColor */
typedef uint32_t SgColor;
SgColor sg_color(int r, int g, int b);

enum
{
	SG_ALICEBLUE = 0xF0F8FF,
	SG_ANTIQUEWHITE = 0xFAEBD7,
	SG_AQUA = 0x00FFFF,
	SG_AQUAMARINE = 0x7FFFD4,
	SG_AZURE = 0xF0FFFF,
	SG_BEIGE = 0xF5F5DC,
	SG_BISQUE = 0xFFE4C4,
	SG_BLACK = 0x000000,
	SG_BLANCHEDALMOND = 0xFFEBCD,
	SG_BLUE = 0x0000FF,
	SG_BLUEVIOLET = 0x8A2BE2,
	SG_BROWN = 0xA52A2A,
	SG_BURLYWOOD = 0xDEB887,
	SG_CADETBLUE = 0x5F9EA0,
	SG_CHARTREUSE = 0x7FFF00,
	SG_CHOCOLATE = 0xD2691E,
	SG_CORAL = 0xFF7F50,
	SG_CORNFLOWERBLUE = 0x6495ED,
	SG_CORNSILK = 0xFFF8DC,
	SG_CRIMSON = 0xDC143C,
	SG_CYAN = 0x00FFFF,
	SG_DARKBLUE = 0x00008B,
	SG_DARKCYAN = 0x008B8B,
	SG_DARKGOLDENROD = 0xB8860B,
	SG_DARKGRAY = 0xA9A9A9,
	SG_DARKGREY = 0xA9A9A9,
	SG_DARKGREEN = 0x006400,
	SG_DARKKHAKI = 0xBDB76B,
	SG_DARKMAGENTA = 0x8B008B,
	SG_DARKOLIVEGREEN = 0x556B2F,
	SG_DARKORANGE = 0xFF8C00,
	SG_DARKORCHID = 0x9932CC,
	SG_DARKRED = 0x8B0000,
	SG_DARKSALMON = 0xE9967A,
	SG_DARKSEAGREEN = 0x8FBC8F,
	SG_DARKSLATEBLUE = 0x483D8B,
	SG_DARKSLATEGRAY = 0x2F4F4F,
	SG_DARKSLATEGREY = 0x2F4F4F,
	SG_DARKTURQUOISE = 0x00CED1,
	SG_DARKVIOLET = 0x9400D3,
	SG_DEEPPINK = 0xFF1493,
	SG_DEEPSKYBLUE = 0x00BFFF,
	SG_DIMGRAY = 0x696969,
	SG_DIMGREY = 0x696969,
	SG_DODGERBLUE = 0x1E90FF,
	SG_FIREBRICK = 0xB22222,
	SG_FLORALWHITE = 0xFFFAF0,
	SG_FORESTGREEN = 0x228B22,
	SG_FUCHSIA = 0xFF00FF,
	SG_GAINSBORO = 0xDCDCDC,
	SG_GHOSTWHITE = 0xF8F8FF,
	SG_GOLD = 0xFFD700,
	SG_GOLDENROD = 0xDAA520,
	SG_GRAY = 0x808080,
	SG_GREY = 0x808080,
	SG_GREEN = 0x008000,
	SG_GREENYELLOW = 0xADFF2F,
	SG_HONEYDEW = 0xF0FFF0,
	SG_HOTPINK = 0xFF69B4,
	SG_INDIANRED = 0xCD5C5C,
	SG_INDIGO = 0x4B0082,
	SG_IVORY = 0xFFFFF0,
	SG_KHAKI = 0xF0E68C,
	SG_LAVENDER = 0xE6E6FA,
	SG_LAVENDERBLUSH = 0xFFF0F5,
	SG_LAWNGREEN = 0x7CFC00,
	SG_LEMONCHIFFON = 0xFFFACD,
	SG_LIGHTBLUE = 0xADD8E6,
	SG_LIGHTCORAL = 0xF08080,
	SG_LIGHTCYAN = 0xE0FFFF,
	SG_LIGHTGOLDENRODYELLOW = 0xFAFAD2,
	SG_LIGHTGRAY = 0xD3D3D3,
	SG_LIGHTGREY = 0xD3D3D3,
	SG_LIGHTGREEN = 0x90EE90,
	SG_LIGHTPINK = 0xFFB6C1,
	SG_LIGHTSALMON = 0xFFA07A,
	SG_LIGHTSEAGREEN = 0x20B2AA,
	SG_LIGHTSKYBLUE = 0x87CEFA,
	SG_LIGHTSLATEGRAY = 0x778899,
	SG_LIGHTSLATEGREY = 0x778899,
	SG_LIGHTSTEELBLUE = 0xB0C4DE,
	SG_LIGHTYELLOW = 0xFFFFE0,
	SG_LIME = 0x00FF00,
	SG_LIMEGREEN = 0x32CD32,
	SG_LINEN = 0xFAF0E6,
	SG_MAGENTA = 0xFF00FF,
	SG_MAROON = 0x800000,
	SG_MEDIUMAQUAMARINE = 0x66CDAA,
	SG_MEDIUMBLUE = 0x0000CD,
	SG_MEDIUMORCHID = 0xBA55D3,
	SG_MEDIUMPURPLE = 0x9370DB,
	SG_MEDIUMSEAGREEN = 0x3CB371,
	SG_MEDIUMSLATEBLUE = 0x7B68EE,
	SG_MEDIUMSPRINGGREEN = 0x00FA9A,
	SG_MEDIUMTURQUOISE = 0x48D1CC,
	SG_MEDIUMVIOLETRED = 0xC71585,
	SG_MIDNIGHTBLUE = 0x191970,
	SG_MINTCREAM = 0xF5FFFA,
	SG_MISTYROSE = 0xFFE4E1,
	SG_MOCCASIN = 0xFFE4B5,
	SG_NAVAJOWHITE = 0xFFDEAD,
	SG_NAVY = 0x000080,
	SG_OLDLACE = 0xFDF5E6,
	SG_OLIVE = 0x808000,
	SG_OLIVEDRAB = 0x6B8E23,
	SG_ORANGE = 0xFFA500,
	SG_ORANGERED = 0xFF4500,
	SG_ORCHID = 0xDA70D6,
	SG_PALEGOLDENROD = 0xEEE8AA,
	SG_PALEGREEN = 0x98FB98,
	SG_PALETURQUOISE = 0xAFEEEE,
	SG_PALEVIOLETRED = 0xDB7093,
	SG_PAPAYAWHIP = 0xFFEFD5,
	SG_PEACHPUFF = 0xFFDAB9,
	SG_PERU = 0xCD853F,
	SG_PINK = 0xFFC0CB,
	SG_PLUM = 0xDDA0DD,
	SG_POWDERBLUE = 0xB0E0E6,
	SG_PURPLE = 0x800080,
	SG_REBECCAPURPLE = 0x663399,
	SG_RED = 0xFF0000,
	SG_ROSYBROWN = 0xBC8F8F,
	SG_ROYALBLUE = 0x4169E1,
	SG_SADDLEBROWN = 0x8B4513,
	SG_SALMON = 0xFA8072,
	SG_SANDYBROWN = 0xF4A460,
	SG_SEAGREEN = 0x2E8B57,
	SG_SEASHELL = 0xFFF5EE,
	SG_SIENNA = 0xA0522D,
	SG_SILVER = 0xC0C0C0,
	SG_SKYBLUE = 0x87CEEB,
	SG_SLATEBLUE = 0x6A5ACD,
	SG_SLATEGRAY = 0x708090,
	SG_SLATEGREY = 0x708090,
	SG_SNOW = 0xFFFAFA,
	SG_SPRINGGREEN = 0x00FF7F,
	SG_STEELBLUE = 0x4682B4,
	SG_TAN = 0xD2B48C,
	SG_TEAL = 0x008080,
	SG_THISTLE = 0xD8BFD8,
	SG_TOMATO = 0xFF6347,
	SG_TURQUOISE = 0x40E0D0,
	SG_VIOLET = 0xEE82EE,
	SG_WHEAT = 0xF5DEB3,
	SG_WHITE = 0xFFFFFF,
	SG_WHITESMOKE = 0xF5F5F5,
	SG_YELLOW = 0xFFFF00,
	SG_YELLOWGREEN = 0x9ACD32
};

static inline int sg_color_r(SgColor color)
{
	return (color >> 16) & 0xFF;
}

static inline int sg_color_g(SgColor color)
{
	return (color >> 8) & 0xFF;
}

static inline int sg_color_b(SgColor color)
{
	return color & 0xFF;
}

/* SgPoint */
typedef SDL_Point SgPoint;

static inline SgPoint sg_point(int x, int y)
{
	SgPoint point = { x, y };
	return point;
}

static inline bool sg_point_eq(SgPoint a, SgPoint b)
{
	return a.x == b.x && a.y == b.y;
}

/* SgSize */
typedef struct
{
	int w, h;
} SgSize;

static inline SgSize sg_size(int w, int h)
{
	SgSize size = { w, h };
	return size;
}

/* SgRect */
typedef SDL_Rect SgRect;

static inline SgRect sg_rect(int x, int y, int w, int h)
{
	SgRect rect = { x, y, w, h };
	return rect;
}

bool sg_rect_contains_point(SgRect rect, SgPoint p);
bool sg_rect_contains_mouse(SgRect rect);

/* SgAlign */
#define SG_VALIGN_MASK 0x03
enum
{
	SG_VALIGN_TOP = 0,
	SG_VALIGN_CENTER = 1,
	SG_VALIGN_BOTTOM = 2
};

#define SG_HALIGN_MASK 0x0C
enum
{
	SG_HALIGN_LEFT = 0,
	SG_HALIGN_CENTER = 4,
	SG_HALIGN_RIGHT = 8
};

#define SG_TOP_LEFT      (SG_VALIGN_TOP | SG_HALIGN_LEFT)
#define SG_TOP_CENTER    (SG_VALIGN_TOP | SG_HALIGN_CENTER)
#define SG_TOP_RIGHT     (SG_VALIGN_TOP | SG_HALIGN_RIGHT)

#define SG_CENTER_LEFT   (SG_VALIGN_CENTER | SG_HALIGN_LEFT)
#define SG_CENTER        (SG_VALIGN_CENTER | SG_HALIGN_CENTER)
#define SG_CENTER_RIGHT  (SG_VALIGN_CENTER | SG_HALIGN_RIGHT)

#define SG_BOTTOM_LEFT   (SG_VALIGN_BOTTOM | SG_HALIGN_LEFT)
#define SG_BOTTOM_CENTER (SG_VALIGN_BOTTOM | SG_HALIGN_CENTER)
#define SG_BOTTOM_RIGHT  (SG_VALIGN_BOTTOM | SG_HALIGN_RIGHT)

enum
{
	SG_BUTTON_LEFT = 1,
	SG_BUTTON_MIDDLE = 2,
	SG_BUTTON_RIGHT = 4
};

/* SgSurface */
typedef SDL_Surface *SgSurface;

/* SgTexture */
typedef SDL_Texture *SgTexture;

SgTexture sg_texture_load(const char *file);
void sg_texture_destroy(SgTexture texture);
SgSize sg_texture_size(SgTexture texture);
SgRect sg_texture_bounds(SgTexture texture);
void sg_texture_draw(SgTexture texture, SgRect src, SgRect dst);

/* SgFont */
typedef TTF_Font *SgFont;

SgFont sg_font_load(const char *file, int size);
void sg_font_destroy(SgFont font);

/* SgFontAtalas */
typedef struct
{
	int FontHeight;
	int MaxCharSize;
	SgSize CharDim[256];
	SgSurface Surface;
	SgTexture Texture;
} SgFontAtlasInternal;

typedef SgFontAtlasInternal *SgFontAtlas;

SgFontAtlas sg_font_atlas_create(int max_char_size, int font_height);
void sg_set_fontatlas(SgFontAtlas atlas);
void sg_fontatlas_destroy(SgFontAtlas atlas);

int sg_fontatlas_add_ascii(SgFontAtlas atlas, SgFont font);
int sg_fontatlas_add_char(SgFontAtlas atlas, SgFont font, uint8_t c);
int sg_fontatlas_add_utf8(SgFontAtlas atlas, SgFont font, const char *utf8, uint8_t index);
int sg_fontatlas_add_icon(SgFontAtlas atlas, const char *file, uint8_t index);
int sg_fontatlas_blit_surface(SgFontAtlas atlas, SgSurface surface, SgRect src, uint8_t index);
void sg_fontatlas_add_binary(SgFontAtlas atlas, const uint8_t *bytes, SgSize size, uint8_t index);
void sg_fontatlas_add_default_checkmark(SgFontAtlas atlas);
void sg_fontatlas_add_default_select(SgFontAtlas atlas);
void sg_fontatlas_add_default_icons(SgFontAtlas atlas);
void sg_fontatlas_update(SgFontAtlas atlas);

/* font rendering */
int sg_char_height(uint8_t c);
int sg_char_width(uint8_t c);
int sg_string_width(const char *s);
int sg_string_width_len(const char *s, size_t len);

int sg_render_char(SgPoint p, uint8_t c, SgColor color);
int sg_render_char_align(SgPoint p, uint8_t c, int flags, SgColor color);
int sg_render_char_in_rect(SgRect p, uint8_t c, int flags, SgColor color);

int sg_render_string(SgPoint p, const char *s, SgColor color);
int sg_render_string_align(SgPoint p, const char *s, int flags, SgColor color);
int sg_render_string_in_rect(SgRect p, const char *s, int flags, SgColor color);

int sg_render_string_len(SgPoint p, const char *s, size_t len, SgColor color);

/* allocation */
void *sg_malloc(size_t size);
void *sg_calloc(size_t count, size_t size);
void *sg_realloc(void *p, size_t size);
void sg_free(void *p);
void sg_alloc_report(void);

/* window */
enum
{
	SG_INDEX_DEFAULT,
	SG_INDEX_HOVER,
	SG_INDEX_ACTIVE,
	SG_INDEX_SELECTED
};

typedef struct
{
	/* Window */
	SgColor WindowBackgroundColor;

	/* Label */
	SgColor LabelTextColor;

	/* Button */
	SgColor ButtonTextColor[3];
	SgColor ButtonInnerColor[3];
	SgColor ButtonBorderColor[3];
	int ButtonBorderThickness[3];

	/* Checkbox */
	SgColor CheckboxIconColor[3];
	SgColor CheckboxInnerColor[3];
	SgColor CheckboxBorderColor[3];
	int CheckboxBorderThickness[3];

	/* Textbox */
	SgColor TextboxTextColor[3];
	SgColor TextboxInnerColor[3];
	SgColor TextboxBorderColor[3];
	int TextboxBorderThickness[3];
	int TextboxPaddingX;
	SgRect Cursor;
	SgColor CursorColor;
	SgColor SelectionColor;
	SgColor SelectionTextColor;

	/* Slider */
	SgColor SliderThumbColor[3];
	SgColor SliderRailColor[3];
	int SliderThumbWidth;
	int SliderRailHeight;

	/* Select */
	SgColor SelectTextColor[3];
	SgColor SelectInnerColor[3];
	SgColor SelectBorderColor[3];
	int SelectBorderThickness[3];
	int SelectPageItems;
	int SelectPaddingX;
} SgTheme;

void sg_init(SgSize size, const char *title);
void sg_destroy(void);
void sg_set_title(const char *title);
void sg_begin(void);
void sg_update(void);
bool sg_running(void);
SgSize sg_get_window_size(void);

/* mouse handling */
int sg_is_mouse_button_down(int button);
int sg_is_mouse_button_pressed(int button);
int sg_is_mouse_button_released(int button);
SgPoint sg_mouse_position(void);
SgPoint sg_mouse_wheel(void);

/* keyboard handling */
bool sg_is_key_down(SDL_Scancode key);
bool sg_is_key_pressed(SDL_Scancode key);
bool sg_is_key_released(SDL_Scancode key);

/* keyboard layouts */
#define MOD_SHIFT          0x8000
#define MOD_CTRL           0x4000
#define MOD_OS             0x2000
#define MOD_ALT            0x1000
#define MOD_ALT_GR         0x800

int sg_key_convert(int scancode, int mod);
int sg_key_to_codepoint_german(int k);

extern int (*sg_key_to_codepoint)(int);

/* draw and fill rect */
void sg_fill_rect(SgRect rect, SgColor color);
void sg_draw_rect(SgRect rect, int border, SgColor color);

/* Controls */
/* All Controls are styled based on the same global theme */
void sg_set_checkmark_char(uint8_t index);
void sg_set_select_char(uint8_t index);

/* StringBuffer */
typedef struct
{
	char *buffer;
	size_t length;
	size_t capacity;
} SgStringBuffer;

void sg_label(SgRect dimensions, const char *text, int flags);
bool sg_button(SgRect dimensions, const char *text);
bool sg_checkbox(SgRect dimensions, bool *checked);
int sg_slider(SgRect dimensions, double *value, double min, double max);
int sg_textbox(SgRect dimensions, SgStringBuffer *text);
int sg_select(SgRect dimensions, const char *items[], size_t count, size_t *active);

/* ========================================================================== */
/* IMPLEMENTATION */

#ifdef SIMPLEGUI_IMPLEMENTATION

/* ========================================================================== */
/* graphics global state */

#define SG_MAX_KEYS 32

extern SgTheme sg_default_theme;
extern SgTheme *sg_theme;

static uint32_t _sg_keys[SG_MAX_KEYS];
static int _sg_keys_top = 0;

uint8_t _sg_char_checkmark = 1;
uint8_t _sg_char_select = 2;

extern const uint8_t _sg_default_font[];
extern const uint8_t _sg_default_checkmark[];
extern const uint8_t _sg_default_select[];

int (*sg_key_to_codepoint)(int) = sg_key_to_codepoint_german;

SgFontAtlas _sg_fontatlas_default = NULL;

SDL_Window *_sg_window = NULL;
SDL_Renderer *_sg_renderer = NULL;
SgFontAtlas _sg_fontatlas = NULL;

bool _sg_running = false;

SgPoint _sg_scroll;

int _sg_mouse_button_pressed = 0;
int _sg_mouse_button_released = 0;

int _sg_num_keys = 0;

const uint8_t *_sg_key_state = NULL;
uint8_t *_sg_key_pressed = NULL;
uint8_t *_sg_key_released = NULL;

SgPoint _sg_selected_point;
bool _sg_selected = false;
bool _sg_drag = false;

int sg_double_click_ms = 500;
SgPoint sg_multi_click_point;
uint32_t sg_double_click_time;
uint32_t sg_triple_click_time;

bool sg_double_click;
bool sg_triple_click;
bool _sg_tb_multi_clicked = false;

#define SG_FONTDEBUG 0

/* ========================================================================== */
/* font rendering */
SgFont sg_font_load(const char *file, int size)
{
	return TTF_OpenFont(file, size);
}

void sg_font_destroy(SgFont font)
{
	TTF_CloseFont(font);
}

SgFontAtlas sg_font_atlas_create(int max_char_size, int font_height)
{
	SgSurface surface = SDL_CreateRGBSurface(0,
		max_char_size * 16, max_char_size * 16,
		32, 0xff, 0xff00, 0xff0000, 0xff000000);

	SgFontAtlasInternal *atlas = sg_calloc(1, sizeof(SgFontAtlasInternal));
	memset(atlas->CharDim, 0, sizeof(atlas->CharDim));
	atlas->FontHeight = font_height;
	atlas->MaxCharSize = max_char_size;
	atlas->Surface = surface;
	atlas->Texture = NULL;
	return atlas;
}

void sg_set_fontatlas(SgFontAtlas atlas)
{
	_sg_fontatlas = atlas;
}

void sg_fontatlas_destroy(SgFontAtlas atlas)
{
	SDL_FreeSurface(atlas->Surface);
	if(atlas->Texture)
	{
		sg_texture_destroy(atlas->Texture);
	}

	sg_free(atlas);
}

static uint32_t sg_get_pixel(SgSurface s, int x, int y)
{
	uint32_t *pixels = s->pixels;
	return pixels[y * (s->pitch / 4) + x];
}

static void sg_set_pixel(SgSurface s, int x, int y, uint32_t color)
{
	uint32_t *pixels = s->pixels;
	pixels[y * (s->pitch / 4) + x] = color;
}

int sg_surface_color_to_binary(uint32_t color)
{
	return (color >> 24) > 127;
}

int sg_fontatlas_blit_surface(SgFontAtlas atlas, SgSurface s, SgRect src, uint8_t index)
{
	SgRect dst =
	{
		(index & 0x0F) * atlas->MaxCharSize,
		(index >> 4) * atlas->MaxCharSize,
		src.w,
		src.h
	};

	if(SG_FONTDEBUG)
	{
		/* print c array */
		if(isprint(index))
		{
			printf("\t/* Char %d ('%c') */\n", index, index);
		}
		else
		{
			printf("\t/* Char %d */\n", index);
		}

		for(int y = 0; y < src.h; ++y)
		{
			printf("\t");

			int byte = 0;
			int bit = 7;
			for(int x = 0; x < src.w; ++x)
			{
				uint32_t color = sg_get_pixel(s, src.x + x, src.y + y);
				if(sg_surface_color_to_binary(color))
				{
					byte |= (1 << bit);
				}

				--bit;
				if(bit < 0)
				{
					printf("0x%02X, ", byte);
					bit = 7;
					byte = 0;
				}
			}

			if((src.w & 3) != 0)
			{
				printf("0x%02X, ", byte);
			}

			printf("/* ");
			for(int x = 0; x < src.w; ++x)
			{
				uint32_t color = sg_get_pixel(s, src.x + x, src.y + y);
				printf("%c", sg_surface_color_to_binary(color) ? '#' : ' ');
			}

			printf(" */\n");
		}

		printf("\n");
	}

	atlas->CharDim[index] = sg_size(src.w, src.h);
	return SDL_BlitSurface(s, &src, atlas->Surface, &dst);
}

int sg_fontatlas_add_char(SgFontAtlas atlas, SgFont font, uint8_t c)
{
	SDL_Color color = { 255, 255, 255, 0 };
	char letter[2] = { c, '\0' };
	SDL_Surface *s = TTF_RenderText_Blended(font, letter, color);
	SDL_SetSurfaceBlendMode(s, SDL_BLENDMODE_NONE);
	int status = sg_fontatlas_blit_surface(atlas, s, sg_rect(0, 0, s->w, s->h), c);
	SDL_FreeSurface(s);
	return status;
}

int sg_fontatlas_add_ascii(SgFontAtlas atlas, SgFont font)
{
	int status = 0;
	for(int i = 32; i <= 126; ++i)
	{
		if(sg_fontatlas_add_char(atlas, font, i))
		{
			status = 1;
		}
	}

	return status;
}

int sg_fontatlas_add_utf8(SgFontAtlas atlas, SgFont font, const char *utf8, uint8_t index)
{
	SDL_Color color = { 255, 255, 255, 0 };
	SDL_Surface *s = TTF_RenderUTF8_Blended(font, utf8, color);
	int status = sg_fontatlas_blit_surface(atlas, s, sg_rect(0, 0, s->w, s->h), index);
	SDL_FreeSurface(s);
	return status;
}

int sg_fontatlas_add_icon(SgFontAtlas atlas, const char *file, uint8_t index)
{
	SDL_Surface *s = IMG_Load(file);
	if(!s)
	{
		return 1;
	}

	return sg_fontatlas_blit_surface(atlas, s, sg_rect(0, 0, s->w, s->h), index);
}

void sg_fontatlas_add_binary(SgFontAtlas atlas, const uint8_t *bytes, SgSize size, uint8_t index)
{
	int i = 0;
	for(int y = 0; y < size.h; ++y)
	{
		int byte = bytes[i];
		int bit = 7;
		for(int x = 0; x < size.w; ++x)
		{
			if(byte & (1 << bit))
			{
				sg_set_pixel(atlas->Surface,
					(index & 0x0F) * atlas->MaxCharSize + x,
					(index >> 4) * atlas->MaxCharSize + y, 0xFFFFFFFF);
			}

			--bit;
			if(bit < 0)
			{
				bit = 7;
				++i;
				byte = bytes[i];
			}
		}
	}

	atlas->CharDim[index] = size;
}

void sg_fontatlas_add_default_checkmark(SgFontAtlas atlas)
{
	sg_fontatlas_add_binary(atlas,
		_sg_default_checkmark,
		sg_size(16, 16), _sg_char_checkmark);
}

void sg_fontatlas_add_default_select(SgFontAtlas atlas)
{
	sg_fontatlas_add_binary(atlas,
		_sg_default_select,
		sg_size(16, 16), _sg_char_select);
}

void sg_fontatlas_add_default_icons(SgFontAtlas atlas)
{
	sg_fontatlas_add_default_checkmark(atlas);
	sg_fontatlas_add_default_select(atlas);
}

void sg_fontatlas_update(SgFontAtlas atlas)
{
	if(SG_FONTDEBUG)
	{
		SDL_SaveBMP(atlas->Surface, "fontatlas.bmp");
	}

	if(atlas->Texture)
	{
		sg_texture_destroy(atlas->Texture);
	}

	atlas->Texture = SDL_CreateTextureFromSurface(_sg_renderer, atlas->Surface);
}

int sg_render_char(SgPoint p, uint8_t c, SgColor color)
{
	int stride = _sg_fontatlas->MaxCharSize;
	SgSize size = _sg_fontatlas->CharDim[c];
	SDL_Rect src = { (c & 0x0F) * stride, (c >> 4) * stride, size.w, size.h };
	SDL_Rect dst = { p.x, p.y, size.w, size.h };
	SDL_SetTextureColorMod(_sg_fontatlas->Texture,
		sg_color_r(color), sg_color_g(color), sg_color_b(color));
	SDL_RenderCopy(_sg_renderer, _sg_fontatlas->Texture, &src, &dst);
	return size.w;
}

int sg_render_char_align(SgPoint p, uint8_t c, int flags, SgColor color)
{
	int valign = flags & SG_VALIGN_MASK;
	int halign = flags & SG_HALIGN_MASK;

	if(halign == SG_HALIGN_CENTER)
	{
		p.x -= sg_char_width(c) / 2;
	}
	else if(halign == SG_HALIGN_RIGHT)
	{
		p.x -= sg_char_width(c);
	}

	if(valign == SG_VALIGN_CENTER)
	{
		p.y -= sg_char_height(c) / 2;
	}
	else if(valign == SG_VALIGN_BOTTOM)
	{
		p.y -= sg_char_height(c);
	}

	return sg_render_char(p, c, color);
}

int sg_render_char_in_rect(SgRect d, uint8_t c, int flags, SgColor color)
{
	int valign = flags & SG_VALIGN_MASK;
	int halign = flags & SG_HALIGN_MASK;

	SgPoint r = { d.x, d.y };
	if(halign == SG_HALIGN_CENTER)
	{
		r.x += d.w / 2 - sg_char_width(c) / 2;
	}
	else if(halign == SG_HALIGN_RIGHT)
	{
		r.x += d.w - sg_char_width(c);
	}

	if(valign == SG_VALIGN_CENTER)
	{
		r.y += d.h / 2 - sg_char_height(c) / 2;
	}
	else if(valign == SG_VALIGN_BOTTOM)
	{
		r.y += d.h - sg_char_height(c);
	}

	return sg_render_char(r, c, color);
}

int sg_render_string(SgPoint p, const char *s, SgColor color)
{
	int w = 0;
	for(uint8_t c; (c = *s); ++s)
	{
		w += sg_render_char(sg_point(p.x + w, p.y), c, color);
	}

	return w;
}

int sg_render_string_align(SgPoint p, const char *s, int flags, SgColor color)
{
	int valign = flags & SG_VALIGN_MASK;
	int halign = flags & SG_HALIGN_MASK;

	if(halign == SG_HALIGN_CENTER)
	{
		p.x -= sg_string_width(s) / 2;
	}
	else if(halign == SG_HALIGN_RIGHT)
	{
		p.x -= sg_string_width(s);
	}

	if(valign == SG_VALIGN_CENTER)
	{
		p.y -= _sg_fontatlas->FontHeight / 2;
	}
	else if(valign == SG_VALIGN_BOTTOM)
	{
		p.y -= _sg_fontatlas->FontHeight;
	}

	return sg_render_string(p, s, color);
}

int sg_render_string_in_rect(SgRect d, const char *s, int flags, SgColor color)
{
	int valign = flags & SG_VALIGN_MASK;
	int halign = flags & SG_HALIGN_MASK;

	SgPoint r = { d.x, d.y };
	if(halign == SG_HALIGN_CENTER)
	{
		r.x += d.w / 2 - sg_string_width(s) / 2;
	}
	else if(halign == SG_HALIGN_RIGHT)
	{
		r.x += d.w - sg_string_width(s);
	}

	if(valign == SG_VALIGN_CENTER)
	{
		r.y += d.h / 2 - _sg_fontatlas->FontHeight / 2;
	}
	else if(valign == SG_VALIGN_BOTTOM)
	{
		r.y += d.h - _sg_fontatlas->FontHeight;
	}

	return sg_render_string(r, s, color);
}

int sg_render_string_len(SgPoint p, const char *s, size_t len, SgColor color)
{
	int w = 0;
	size_t i = 0;
	for(uint8_t c; (c = s[i]) && i < len; ++i)
	{
		w += sg_render_char(sg_point(p.x + w, p.y), c, color);
	}

	return w;
}

int sg_char_width(uint8_t c)
{
	return _sg_fontatlas->CharDim[c].w;
}

int sg_char_height(uint8_t c)
{
	return _sg_fontatlas->CharDim[c].h;
}

int sg_string_width(const char *s)
{
	int w = 0;
	for(uint8_t c; (c = *s); ++s)
	{
		w += sg_char_width(c);
	}

	return w;
}

int sg_string_width_len(const char *s, size_t len)
{
	int w = 0;
	size_t i = 0;
	for(uint8_t c; (c = s[i]) && i < len; ++i)
	{
		w += sg_char_width(c);
	}

	return w;
}

/* ========================================================================== */
/* allocation utils */
static size_t _sg_alloc_count, _sg_free_count;

void *sg_malloc(size_t size)
{
	void *p = malloc(size);
	if(!p)
	{
		fprintf(stderr, "[sg_malloc] Memory allocation failed (%zu bytes)\n", size);
		exit(1);
	}

	++_sg_alloc_count;
	return p;
}

void *sg_calloc(size_t count, size_t size)
{
	void *ptr = calloc(count, size);
	if(!ptr)
	{
		fprintf(stderr, "[sg_calloc] Memory allocation failed (%zu * %zu bytes)", count, size);
		exit(1);
	}

	++_sg_alloc_count;
	return ptr;
}

void *sg_realloc(void *p, size_t size)
{
	if(!p)
	{
		++_sg_alloc_count;
	}

	p = realloc(p, size);
	if(!p)
	{
		fprintf(stderr, "[sg_realloc] Memory allocation failed (%zu bytes)", size);
		exit(1);
	}

	return p;
}

void sg_free(void *p)
{
	if(p)
	{
		free(p);
		++_sg_free_count;
	}
}

void sg_alloc_report(void)
{
	printf("%zu allocs, %zu frees\n", _sg_alloc_count, _sg_free_count);
}

/* ========================================================================== */
/* window */
void sg_destroy(void)
{
	if(_sg_renderer)
	{
		SDL_DestroyRenderer(_sg_renderer);
		_sg_renderer = NULL;
	}

	if(_sg_window)
	{
		SDL_DestroyWindow(_sg_window);
		_sg_window = NULL;
	}

	if(SDL_WasInit(SDL_INIT_VIDEO))
	{
		SDL_Quit();
	}

	if(TTF_WasInit())
	{
		TTF_Quit();
	}

	sg_fontatlas_destroy(_sg_fontatlas_default);

	sg_free(_sg_key_pressed);
	sg_free(_sg_key_released);
}

void sg_init(SgSize size, const char *title)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Error initializing SDL; SDL_Init: %s\n", SDL_GetError());
		sg_destroy();
		exit(1);
	}

	if(!(_sg_window = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		size.w, size.h, SDL_WINDOW_RESIZABLE)))
	{
		fprintf(stderr, "Error creating SDL_Window: %s\n", SDL_GetError());
		sg_destroy();
		exit(1);
	}

	if(!(_sg_renderer = SDL_CreateRenderer(_sg_window, -1,
		SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED)))
	{
		fprintf(stderr, "Error creating SDL_Renderer: %s\n", SDL_GetError());
		sg_destroy();
		exit(1);
	}

	if(TTF_Init())
	{
		printf("Loading initializing TTF: %s\n", TTF_GetError());
		sg_destroy();
		exit(1);
	}

	SDL_SetRenderDrawBlendMode(_sg_renderer, SDL_BLENDMODE_NONE);

	_sg_key_state = SDL_GetKeyboardState(&_sg_num_keys);
	_sg_key_pressed = sg_calloc(_sg_num_keys, 1);
	_sg_key_released = sg_calloc(_sg_num_keys, 1);

	_sg_running = true;

	_sg_fontatlas_default = sg_font_atlas_create(32, 16);
	for(int c = 32; c <= 126; ++c)
	{
		sg_fontatlas_add_binary(_sg_fontatlas_default,
			_sg_default_font + (c - 32) * 18,
			sg_size(8, 18), c);
	}

	sg_fontatlas_add_default_icons(_sg_fontatlas_default);
	sg_fontatlas_update(_sg_fontatlas_default);
	_sg_fontatlas = _sg_fontatlas_default;
}

void sg_set_title(const char *title)
{
	SDL_SetWindowTitle(_sg_window, title);
}

static void sg_set_color(SgColor color)
{
	SDL_SetRenderDrawColor(_sg_renderer,
		sg_color_r(color), sg_color_g(color), sg_color_b(color), 255);
}

SgSize sg_get_window_size(void)
{
	SgSize size;
	SDL_GetWindowSize(_sg_window, &size.w, &size.h);
	return size;
}

/* ========================================================================== */
/* draw and fill rectangle */
void sg_fill_rect(SgRect rect, SgColor color)
{
	sg_set_color(color);
	SDL_RenderFillRect(_sg_renderer, &rect);
}

void sg_draw_rect(SgRect rect, int border, SgColor color)
{
	sg_fill_rect(sg_rect(rect.x, rect.y, rect.w, border), color);
	sg_fill_rect(sg_rect(rect.x, rect.y + rect.h - border, rect.w, border), color);

	sg_fill_rect(sg_rect(rect.x, rect.y + border, border, rect.h - 2 * border), color);
	sg_fill_rect(sg_rect(rect.x + rect.w - border, rect.y + border, border, rect.h - 2 * border), color);
}

/* ========================================================================== */
/* textures */
SgTexture sg_texture_load(const char *file)
{
	return IMG_LoadTexture(_sg_renderer, file);
}

void sg_texture_destroy(SgTexture texture)
{
	SDL_DestroyTexture(texture);
}

SgSize sg_texture_size(SgTexture texture)
{
	SgSize size;
	SDL_QueryTexture(texture, NULL, NULL, &size.w, &size.h);
	return size;
}

SgRect sg_texture_bounds(SgTexture texture)
{
	SgSize size = sg_texture_size(texture);
	return sg_rect(0, 0, size.w, size.h);
}

void sg_texture_draw(SgTexture texture, SgRect src, SgRect dst)
{
	SDL_RenderCopy(_sg_renderer, texture, &src, &dst);
}

/* ========================================================================== */
/* event handling */
static void sg_handle_mouse_scroll(SDL_Event *e)
{
	_sg_scroll = sg_point(e->wheel.x, e->wheel.y);
}

static void sg_handle_mouse_button_down(SDL_Event *e)
{
	if(e->button.button == SDL_BUTTON_LEFT)
	{
		_sg_selected = false;

		SgPoint click_point = sg_point(e->button.x, e->button.y);

		uint32_t time = SDL_GetTicks();
		if(time < sg_triple_click_time + sg_double_click_ms &&
			sg_point_eq(sg_multi_click_point, click_point))
		{
			sg_double_click_time = 0;
			sg_triple_click_time = 0;
			sg_triple_click = true;
		}
		else if(time < sg_double_click_time + sg_double_click_ms &&
			sg_point_eq(sg_multi_click_point, click_point))
		{
			sg_triple_click_time = sg_double_click_time;
			sg_double_click_time = 0;
			sg_double_click = true;
		}
		else
		{
			sg_double_click_time = time;
			sg_multi_click_point = click_point;
		}
	}

	_sg_mouse_button_pressed |= SDL_BUTTON(e->button.button);
}

static void sg_handle_mouse_button_up(SDL_Event *e)
{
	if(e->button.button == SDL_BUTTON_LEFT)
	{
		_sg_drag = false;
		_sg_tb_multi_clicked = false;
	}

	_sg_mouse_button_released |= SDL_BUTTON(e->button.button);
}

static void sg_handle_key_down(SDL_Event *e)
{
	_sg_key_pressed[e->key.keysym.scancode] = 1;

	if(_sg_keys_top < SG_MAX_KEYS)
	{
		int key = sg_key_convert(e->key.keysym.scancode, e->key.keysym.mod);
		_sg_keys[_sg_keys_top++] = key;
	}
}

static void sg_handle_key_up(SDL_Event *e)
{
	_sg_key_released[e->key.keysym.scancode] = 1;
}

void sg_begin(void)
{
	sg_double_click = false;
	sg_triple_click = false;

	_sg_keys_top = 0;
	_sg_scroll = sg_point(0, 0);
	_sg_mouse_button_pressed = 0;
	_sg_mouse_button_released = 0;

	memset(_sg_key_pressed, 0, _sg_num_keys);
	memset(_sg_key_released, 0, _sg_num_keys);

	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_QUIT:
			_sg_running = false;
			break;

		case SDL_MOUSEWHEEL:
			sg_handle_mouse_scroll(&e);
			break;

		case SDL_MOUSEBUTTONDOWN:
			sg_handle_mouse_button_down(&e);
			break;

		case SDL_MOUSEBUTTONUP:
			sg_handle_mouse_button_up(&e);
			break;

		case SDL_KEYDOWN:
			sg_handle_key_down(&e);
			break;

		case SDL_KEYUP:
			sg_handle_key_up(&e);
			break;
		}
	}

	sg_set_color(sg_theme->WindowBackgroundColor);
	SDL_RenderClear(_sg_renderer);
}

void sg_update(void)
{
	SDL_RenderPresent(_sg_renderer);
}

bool sg_running(void)
{
	return _sg_running;
}

/* ========================================================================== */
/* mouse handling */
int sg_is_mouse_button_down(int button)
{
	return SDL_GetMouseState(NULL, NULL) & button;
}

int sg_is_mouse_button_pressed(int button)
{
	return _sg_mouse_button_pressed & button;
}

int sg_is_mouse_button_released(int button)
{
	return _sg_mouse_button_released & button;
}

SgPoint sg_mouse_position(void)
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	return sg_point(x, y);
}

SgPoint sg_mouse_wheel(void)
{
	return _sg_scroll;
}

/* ========================================================================== */
/* keyboard handling */
bool sg_is_key_down(SDL_Scancode key)
{
	return _sg_key_state[key];
}

bool sg_is_key_pressed(SDL_Scancode key)
{
	return _sg_key_pressed[key];
}

bool sg_is_key_released(SDL_Scancode key)
{
	return _sg_key_released[key];
}

/* ========================================================================== */
/* keyboard layouts */
int sg_key_convert(int scancode, int mod)
{
	int key = scancode;
	if(mod & (KMOD_LCTRL | KMOD_RCTRL))
	{
		key |= MOD_CTRL;
	}

	if(mod & KMOD_LALT)
	{
		key |= MOD_ALT;
	}

	if(mod & KMOD_RALT)
	{
		key |= MOD_ALT_GR;
	}

	if(mod & (KMOD_LGUI | KMOD_RGUI))
	{
		key |= MOD_OS;
	}

	if(mod & (KMOD_LSHIFT | KMOD_RSHIFT))
	{
		key |= MOD_SHIFT;
	}

	return key;
}

int sg_key_to_codepoint_german(int k)
{
	int nomods = k & 0xFF;

	if(nomods == SDL_SCANCODE_TAB)                             { return '\t'; }
	else if(nomods == SDL_SCANCODE_BACKSPACE)                  { return '\b'; }
	else if(nomods == SDL_SCANCODE_RETURN)                     { return '\n'; }
	else if(nomods == SDL_SCANCODE_SPACE)                      { return ' '; }
	else if(k == (SDL_SCANCODE_COMMA | MOD_SHIFT))             { return ';'; }
	else if(k == (SDL_SCANCODE_COMMA))                         { return ','; }
	else if(k == (SDL_SCANCODE_PERIOD | MOD_SHIFT))            { return ':'; }
	else if(k == (SDL_SCANCODE_PERIOD))                        { return '.'; }
	else if(k == (SDL_SCANCODE_SLASH | MOD_SHIFT))             { return '_'; }
	else if(k == (SDL_SCANCODE_SLASH))                         { return '-'; }
	else if(k == (SDL_SCANCODE_BACKSLASH | MOD_SHIFT))         { return '\''; }
	else if(k == (SDL_SCANCODE_BACKSLASH))                     { return '#'; }
	else if(k == (SDL_SCANCODE_RIGHTBRACKET | MOD_SHIFT))      { return '*'; }
	else if(k == (SDL_SCANCODE_RIGHTBRACKET | MOD_ALT_GR))     { return '~'; }
	else if(k == (SDL_SCANCODE_RIGHTBRACKET))                  { return '+'; }
	else if(k == (SDL_SCANCODE_NONUSBACKSLASH | MOD_SHIFT))    { return '>'; }
	else if(k == (SDL_SCANCODE_NONUSBACKSLASH | MOD_ALT_GR))   { return '|'; }
	else if(k == SDL_SCANCODE_NONUSBACKSLASH)                  { return '<'; }
	else if(k == (SDL_SCANCODE_MINUS | MOD_SHIFT))             { return '?'; }
	else if(k == (SDL_SCANCODE_MINUS | MOD_ALT_GR))            { return '\\'; }
	else if(k == (SDL_SCANCODE_EQUALS | MOD_SHIFT))            { return '`'; }
	else if(k == SDL_SCANCODE_GRAVE)                           { return '^'; }
	else if(nomods >= SDL_SCANCODE_A && nomods <= SDL_SCANCODE_Z)
	{
		int c = nomods - SDL_SCANCODE_A + 'a';

		if(c == 'z') { c = 'y'; }
		else if(c == 'y') { c = 'z'; }

		if(k & MOD_ALT_GR)
		{
			if(c == 'q') { return '@'; }
		}

		if(k & MOD_SHIFT)
		{
			c = toupper(c);
		}

		return c;
	}
	else if(nomods >= SDL_SCANCODE_1 && nomods <= SDL_SCANCODE_0)
	{
		static const uint8_t numbers[] =
			{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

		static const uint8_t numbers_shift[] =
			{ '!', '\"', 0, '$', '%', '&', '/', '(', ')', '=' };

		static const uint8_t numbers_altgr[] =
			{ 0, 0, 0, 0, 0, 0, '{', '[', ']', '}' };

		int idx = nomods - SDL_SCANCODE_1;

		if(k & MOD_SHIFT)
		{
			return numbers_shift[idx];
		}
		else if(k & MOD_ALT_GR)
		{
			return numbers_altgr[idx];
		}
		else
		{
			return numbers[idx];
		}
	}

	return 0;
}

/* ========================================================================== */
/* GUI GLOBAL STATE */

SgTheme sg_default_theme =
{
	/* Window */
	.WindowBackgroundColor = 0x100000,

	/* Label */
	.LabelTextColor = 0xff8200,

	/* Button */
	.ButtonTextColor = { 0xff8200, 0xff8200, 0xff8200 },
	.ButtonInnerColor = { 0x310000, 0x7b0000, 0x510000 },
	.ButtonBorderColor = { 0x7b0000, 0xff8200, 0xff8200 },
	.ButtonBorderThickness = { 2, 2, 1 },

	/* Checkbox */
	.CheckboxIconColor = { 0xff8200, 0xff8200, 0xff8200 },
	.CheckboxInnerColor = { 0x310000, 0x7b0000, 0x510000 },
	.CheckboxBorderColor = { 0x7b0000, 0xff8200, 0xff8200 },
	.CheckboxBorderThickness = { 2, 2, 1 },

	/* Textbox */
	.TextboxTextColor = { 0xff8200, 0xff8200, 0xff8200 },
	.TextboxInnerColor = { 0x310000, 0x7b0000, 0x510000 },
	.TextboxBorderColor = { 0x7b0000, 0xff8200, 0xff8200 },
	.TextboxBorderThickness = { 2, 2, 1 },
	.TextboxPaddingX = 5,
	.Cursor = { 0, 0, 2, 0 },
	.CursorColor = 0xff8200,
	.SelectionColor = 0xff8200,
	.SelectionTextColor = 0x310000,

	/* Slider */
	.SliderThumbColor = { 0x7b0000, 0xff8200, 0xffb200 },
	.SliderRailColor = { 0x510000, 0x7b0000, 0x9b0000 },
	.SliderThumbWidth = 6,
	.SliderRailHeight = 6,

	/* Select */
	.SelectTextColor = { 0xff8200, 0xff8200, 0xff8200 },
	.SelectInnerColor = { 0x310000, 0x7b0000, 0x510000 },
	.SelectBorderColor = { 0x7b0000, 0xff8200, 0xff8200 },
	.SelectBorderThickness = { 2, 2, 1 },
	.SelectPageItems = 5,
	.SelectPaddingX = 10,
};

SgTheme *sg_theme = &sg_default_theme;

/* ========================================================================== */
/* sg_color */
SgColor sg_color(int r, int g, int b)
{
	return (r << 16) | (g << 8) | b;
}

/* ========================================================================== */
/* sg_rect */
bool sg_rect_contains_point(SgRect rect, SgPoint p)
{
	return p.x >= rect.x && p.y >= rect.y &&
		p.x < rect.x + rect.w && p.y < rect.y + rect.h;
}

bool sg_rect_contains_mouse(SgRect rect)
{
	return sg_rect_contains_point(rect, sg_mouse_position());
}

/* ========================================================================== */
/* common functions for controls */
void sg_box(SgRect d, int index, SgColor *inner_color,
	SgColor *border_color, int *border_thickness)
{
	int border = border_thickness[index];
	SgRect inner = { d.x + border, d.y + border, d.w - 2 * border, d.h - 2 * border };

	sg_fill_rect(inner, inner_color[index]);
	sg_draw_rect(d, border, border_color[index]);
}

bool sg_clicked(SgRect d, int *index)
{
	bool hover = sg_rect_contains_mouse(d);
	bool active = sg_is_mouse_button_down(SG_BUTTON_LEFT);
	*index = (hover ? (active ? SG_INDEX_ACTIVE : SG_INDEX_HOVER) : SG_INDEX_DEFAULT);
	return hover && sg_is_mouse_button_pressed(SG_BUTTON_LEFT);
}

bool sg_selected(SgRect d, int *index, bool *state)
{
	bool hover = sg_rect_contains_mouse(d);
	if(hover && sg_is_mouse_button_down(SG_BUTTON_LEFT))
	{
		*state = true;
		_sg_selected_point = sg_mouse_position();
	}

	bool sel = *state && sg_rect_contains_point(d, _sg_selected_point);
	*index = sel ? SG_INDEX_ACTIVE : (hover ? SG_INDEX_HOVER : SG_INDEX_DEFAULT);
	return sel;
}

/* ========================================================================== */
/* sg_label */
void sg_label(SgRect d, const char *text, int flags)
{
	sg_render_string_in_rect(d, text, flags, sg_theme->LabelTextColor);
}

/* ========================================================================== */
/* sg_button */
bool sg_button(SgRect d, const char *text)
{
	int index;
	bool clicked = sg_clicked(d, &index);

	sg_box(d, index, sg_theme->ButtonInnerColor,
		sg_theme->ButtonBorderColor, sg_theme->ButtonBorderThickness);
	sg_render_string_in_rect(d, text, SG_CENTER, sg_theme->ButtonTextColor[index]);

	return clicked;
}

/* ========================================================================== */
/* sg_checkbox */
void sg_set_checkmark_char(uint8_t index)
{
	_sg_char_checkmark = index;
}

uint8_t sg_get_checkmark_char(void)
{
	return sg_char_width(_sg_char_checkmark) > 0 ? _sg_char_checkmark : 'X';
}

bool sg_checkbox(SgRect d, bool *checked)
{
	int index;
	bool clicked = sg_clicked(d, &index);

	sg_box(d, index, sg_theme->CheckboxInnerColor,
		sg_theme->CheckboxBorderColor, sg_theme->CheckboxBorderThickness);
	if(*checked)
	{
		sg_render_char_in_rect(d, sg_get_checkmark_char(),
			SG_CENTER, sg_theme->CheckboxIconColor[index]);
	}

	if(clicked)
	{
		*checked = !*checked;
	}

	return clicked;
}

/* ========================================================================== */
/* sg_slider */
int sg_slider(SgRect d, double *value, double min, double max)
{
	int index;
	bool active = sg_selected(d, &index, &_sg_drag);

	sg_fill_rect(sg_rect(d.x, d.y + d.h / 2 - sg_theme->SliderRailHeight / 2, d.w, sg_theme->SliderRailHeight),
		sg_theme->SliderRailColor[index]);

	int thumb_pos = d.x + (*value - min) / (max - min) * (d.w - sg_theme->SliderThumbWidth);
	sg_fill_rect(sg_rect(thumb_pos, d.y, sg_theme->SliderThumbWidth, d.h),
		sg_theme->SliderThumbColor[index]);

	double prev_value = *value;
	if(active)
	{
		SgPoint mouse = sg_mouse_position();
		double percent = (mouse.x - (d.x + sg_theme->SliderThumbWidth / 2)) /
			(double)(d.w - sg_theme->SliderThumbWidth);
		*value = min + sg_fclamp(percent, 0.0, 1.0) * (max - min);
	}

	return *value != prev_value;
}

/* ========================================================================== */
/* sg_select */
void sg_set_select_char(uint8_t index)
{
	_sg_char_select = index;
}

uint8_t sg_get_select_char(void)
{
	return sg_char_width(_sg_char_select) > 0 ? _sg_char_select : 'V';
}

int sg_select(SgRect d, const char *items[], size_t count, size_t *current)
{
	assert(count > 0);

	int index;
	bool selected = sg_selected(d, &index, &_sg_selected);

	sg_box(d, index, sg_theme->SelectInnerColor,
		sg_theme->SelectBorderColor, sg_theme->SelectBorderThickness);

	sg_render_string(
		sg_point(d.x + sg_theme->SelectPaddingX, d.y + d.h / 2 - _sg_fontatlas->FontHeight / 2),
		items[*current], sg_theme->SelectTextColor[index]);

	sg_render_char_align(
		sg_point(d.x + d.w - sg_theme->SelectPaddingX, d.y + d.h / 2),
		sg_get_select_char(), SG_CENTER_RIGHT, sg_theme->SelectTextColor[index]);

	if(selected)
	{
		int elems = sg_min(sg_theme->SelectPageItems, count);
		int step_y = d.h - sg_theme->SelectBorderThickness[SG_INDEX_DEFAULT];
		int total_h = elems * step_y;

		if(d.x + d.h + total_h > sg_get_window_size().h && d.x - total_h > 0)
		{
			step_y = -step_y;
		}

		SgRect b = sg_rect(d.x, d.y, d.w, d.h);
		for(int i = 0; i < elems; ++i)
		{
			b.y += step_y;

			sg_box(b, index, sg_theme->SelectInnerColor,
				sg_theme->SelectBorderColor, sg_theme->SelectBorderThickness);
			sg_render_string(
				sg_point(b.x + sg_theme->SelectPaddingX,
					b.y + b.h / 2 - _sg_fontatlas->FontHeight / 2),
				items[i], sg_theme->SelectTextColor[index]);
		}
	}

	return 0;
}

/* ========================================================================== */
/* sg_textbox */
static int _sg_tb_selection;
static int _sg_tb_position;

static void sg_textbox_replace(SgStringBuffer *sb,
	uint32_t index, uint32_t count,
	const void *elems, uint32_t new_count)
{
	uint32_t new_length = sb->length - count + new_count;
	uint32_t last_bytes = (sb->length - index - count);
	memmove(sb->buffer + index + new_count,
		sb->buffer + index + count,
		last_bytes);

	memcpy(sb->buffer + index, elems, new_count);
	sb->length = new_length;
}

static void sg_textbox_remove(SgStringBuffer *sb, uint32_t index)
{
	sg_textbox_replace(sb, index, 1, NULL, 0);
}

static void sg_textbox_selection_replace(SgStringBuffer *sb,
	const char *str, int len)
{
	int sel_start = sg_min(_sg_tb_selection, _sg_tb_position);
	int sel_len = sg_max(_sg_tb_selection, _sg_tb_position) - sel_start;

	sg_textbox_replace(sb, sel_start, sel_len, str, len);
	_sg_tb_position = sel_start + len;
	_sg_tb_selection = _sg_tb_position;
}

static void sg_textbox_backspace(SgStringBuffer *sb)
{
	if(_sg_tb_selection != _sg_tb_position)
	{
		sg_textbox_selection_replace(sb, NULL, 0);
	}
	else if(_sg_tb_position > 0)
	{
		--_sg_tb_position;
		_sg_tb_selection = _sg_tb_position;
		sg_textbox_remove(sb, _sg_tb_position);
	}
}

static void sg_textbox_delete(SgStringBuffer *sb)
{
	if(_sg_tb_selection != _sg_tb_position)
	{
		sg_textbox_selection_replace(sb, NULL, 0);
	}
	else if(_sg_tb_position < (int)sb->length)
	{
		sg_textbox_remove(sb, _sg_tb_position);
	}
}

static void sg_textbox_char(SgStringBuffer *sb, uint32_t chr)
{
	char ins = chr;
	sg_textbox_selection_replace(sb, &ins, 1);
}

static void sg_tb_selection_save(SgStringBuffer *sb)
{
	int sel_start = sg_min(_sg_tb_selection, _sg_tb_position);
	int sel_len = sg_max(_sg_tb_selection, _sg_tb_position) - sel_start;
	char *p = sb->buffer + sel_start + sel_len;
	int chr = *p;
	*p = '\0';
	SDL_SetClipboardText(sb->buffer + sel_start);
	*p = chr;
}

static void sg_textbox_copy(SgStringBuffer *sb)
{
	sg_tb_selection_save(sb);
}

static void sg_textbox_cut(SgStringBuffer *sb)
{
	sg_tb_selection_save(sb);
	sg_textbox_selection_replace(sb, NULL, 0);
}

static void sg_textbox_paste(SgStringBuffer *sb)
{
	char *p = SDL_GetClipboardText();
	sg_textbox_selection_replace(sb, p, strlen(p));
	free(p);
}

static int sg_textbox_click(SgStringBuffer *sb, int x, int tb_x)
{
	tb_x += sg_theme->TextboxPaddingX;
	if(x < tb_x)
	{
		return 0;
	}

	size_t i = 0;
	for(uint8_t c; (c = sb->buffer[i]) && i < sb->length; ++i)
	{
		int w = sg_char_width(c);
		if(x < tb_x + w / 2)
		{
			return i;
		}

		tb_x += w;
	}

	return sb->length;
}

static void sg_textbox_left(void)
{
	if(_sg_tb_selection != _sg_tb_position)
	{
		_sg_tb_position = sg_min(_sg_tb_position, _sg_tb_selection);
		_sg_tb_selection = _sg_tb_position;
	}
	else if(_sg_tb_position > 0)
	{
		--_sg_tb_position;
		_sg_tb_selection = _sg_tb_position;
	}
}

static void sg_textbox_select_left(void)
{
	if(_sg_tb_position > 0)
	{
		--_sg_tb_position;
	}
}

static void sg_textbox_right(SgStringBuffer *sb)
{
	if(_sg_tb_selection != _sg_tb_position)
	{
		_sg_tb_position = sg_max(_sg_tb_position, _sg_tb_selection);
		_sg_tb_selection = _sg_tb_position;
	}
	else if(_sg_tb_position < (int)sb->length)
	{
		++_sg_tb_position;
		_sg_tb_selection = _sg_tb_position;
	}
}

static void sg_textbox_select_right(SgStringBuffer *sb)
{
	if(_sg_tb_position < (int)sb->length)
	{
		++_sg_tb_position;
	}
}

static void sg_textbox_home(void)
{
	_sg_tb_selection = 0;
	_sg_tb_position = 0;
}

static void sg_textbox_select_home(void)
{
	_sg_tb_position = 0;
}

static void sg_textbox_end(SgStringBuffer *sb)
{
	_sg_tb_position = sb->length;
	_sg_tb_selection = _sg_tb_position;
}

static void sg_textbox_select_end(SgStringBuffer *sb)
{
	_sg_tb_position = sb->length;
}

static void sg_textbox_select_all(SgStringBuffer *sb)
{
	_sg_tb_selection = 0;
	_sg_tb_position = sb->length;
}

int sg_textbox_event_key(SgStringBuffer *sb, uint32_t key, uint32_t chr)
{
	uint32_t nomods = key & 0xFF;
	if(key == SDL_SCANCODE_HOME)
	{
		sg_textbox_home();
	}
	else if(key == (SDL_SCANCODE_HOME | MOD_SHIFT))
	{
		sg_textbox_select_home();
	}
	else if(key == SDL_SCANCODE_END)
	{
		sg_textbox_end(sb);
	}
	else if(key == (SDL_SCANCODE_END | MOD_SHIFT))
	{
		sg_textbox_select_end(sb);
	}
	else if(key == SDL_SCANCODE_LEFT)
	{
		sg_textbox_left();
	}
	else if(key == (SDL_SCANCODE_LEFT | MOD_SHIFT))
	{
		sg_textbox_select_left();
	}
	else if(key == SDL_SCANCODE_RIGHT)
	{
		sg_textbox_right(sb);
	}
	else if(key == (SDL_SCANCODE_RIGHT | MOD_SHIFT))
	{
		sg_textbox_select_right(sb);
	}
	else if(nomods == SDL_SCANCODE_BACKSPACE)
	{
		sg_textbox_backspace(sb);
	}
	else if(nomods == SDL_SCANCODE_DELETE)
	{
		sg_textbox_delete(sb);
	}
	else if(key == (SDL_SCANCODE_A | MOD_CTRL))
	{
		sg_textbox_select_all(sb);
	}
	else if(key == (SDL_SCANCODE_C | MOD_CTRL))
	{
		sg_textbox_copy(sb);
	}
	else if(key == (SDL_SCANCODE_X | MOD_CTRL))
	{
		sg_textbox_cut(sb);
	}
	else if(key == (SDL_SCANCODE_V | MOD_CTRL))
	{
		sg_textbox_paste(sb);
	}
	else if(nomods == SDL_SCANCODE_RETURN)
	{
		return '\n';
	}
	else if(isprint(chr))
	{
		sg_textbox_char(sb, chr);
	}

	return 0;
}

int sg_textbox_key_events(SgStringBuffer *sb)
{
	int result = 0;
	for(int i = 0; i < _sg_keys_top; ++i)
	{
		int key = _sg_keys[i];
		int chr = sg_key_to_codepoint(key);
		int ret = sg_textbox_event_key(sb, key, chr);
		if(result != '\n')
		{
			result = ret;
		}
	}

	return result;
}

bool sg_shift_down(void)
{
	return sg_is_key_down(SDL_SCANCODE_LSHIFT) ||
		sg_is_key_down(SDL_SCANCODE_RSHIFT);
}

bool sg_char_stop(int c)
{
	return c == ' ' || (ispunct(c) && c != '_');
}

int sg_textbox(SgRect d, SgStringBuffer *sb)
{
	int result = 0;
	int index;
	bool selected = sg_selected(d, &index, &_sg_selected);
	SgColor text_color = sg_theme->TextboxTextColor[index];

	if(selected)
	{
		if(sg_rect_contains_mouse(d))
		{
			if(sg_triple_click)
			{
				_sg_tb_position = sb->length;
				_sg_tb_selection = 0;
				_sg_tb_multi_clicked = true;
			}
			else if(sg_double_click)
			{
				_sg_tb_position = sg_textbox_click(sb, sg_mouse_position().x, d.x);

				for(; _sg_tb_position < (int)sb->length && !sg_char_stop(sb->buffer[_sg_tb_position]);
					++_sg_tb_position)
				{
				}

				for(_sg_tb_selection = _sg_tb_position;
					_sg_tb_selection > 0 && !sg_char_stop(sb->buffer[_sg_tb_selection - 1]);
					--_sg_tb_selection)
				{
				}

				_sg_tb_multi_clicked = true;
			}
			else
			{
				if(sg_is_mouse_button_pressed(SG_BUTTON_LEFT) && !sg_shift_down())
				{
					_sg_tb_position = sg_textbox_click(sb, sg_mouse_position().x, d.x);
					_sg_tb_selection = _sg_tb_position;
				}
				else if(sg_is_mouse_button_down(SG_BUTTON_LEFT) && !_sg_tb_multi_clicked)
				{
					_sg_tb_position = sg_textbox_click(sb, sg_mouse_position().x, d.x);
				}
			}
		}

		result = sg_textbox_key_events(sb);
	}

	sg_box(d, index, sg_theme->TextboxInnerColor,
		sg_theme->TextboxBorderColor, sg_theme->TextboxBorderThickness);

	int text_y = d.y + d.h / 2 - _sg_fontatlas->FontHeight / 2;

	if(_sg_tb_selection == _sg_tb_position || !selected)
	{
		sg_render_string_len(
			sg_point(d.x + sg_theme->TextboxPaddingX, text_y),
			sb->buffer, sb->length,
			text_color);
	}
	else
	{
		int sel_start = sg_min(_sg_tb_selection, _sg_tb_position);
		int sel_len = sg_max(_sg_tb_selection, _sg_tb_position) - sel_start;

		sg_render_string_len(
			sg_point(d.x + sg_theme->TextboxPaddingX, text_y),
			sb->buffer, sel_start,
			text_color);

		int sel_x = sg_string_width_len(sb->buffer, sel_start);

		sg_fill_rect(sg_rect(
			d.x + sg_theme->TextboxPaddingX + sel_x,
			text_y + sg_theme->Cursor.y,
			sg_string_width_len(sb->buffer + sel_start, sel_len),
			_sg_fontatlas->FontHeight + sg_theme->Cursor.h),
			sg_theme->SelectionColor);

		sg_render_string_len(
			sg_point(d.x + sg_theme->TextboxPaddingX + sel_x, text_y),
			sb->buffer + sel_start, sel_len,
			sg_theme->SelectionTextColor);

		sg_render_string_len(
			sg_point(d.x + sg_theme->TextboxPaddingX +
				sg_string_width_len(sb->buffer, sel_start + sel_len),
				text_y),
			sb->buffer + sel_start + sel_len,
			sb->length - sel_start - sel_len,
			text_color);
	}

	if(selected)
	{
		sg_fill_rect(sg_rect(d.x + sg_theme->TextboxPaddingX +
			sg_string_width_len(sb->buffer, _sg_tb_position) + sg_theme->Cursor.x,
			text_y + sg_theme->Cursor.y,
			sg_theme->Cursor.w,
			_sg_fontatlas->FontHeight + sg_theme->Cursor.h),
			sg_theme->CursorColor);
	}

	return result;
}

/* ========================================================================== */
/* Default font */
const uint8_t _sg_default_checkmark[] =
{
	0x00, 0x00, /*                  */
	0x00, 0x00, /*                  */
	0x00, 0x01, /*                # */
	0x00, 0x03, /*               ## */
	0x00, 0x07, /*              ### */
	0x00, 0x0E, /*             ###  */
	0x00, 0x1C, /*            ###   */
	0x80, 0x38, /* #         ###    */
	0xC0, 0x70, /* ##       ###     */
	0xE0, 0xE0, /* ###     ###      */
	0x71, 0xC0, /*  ###   ###       */
	0x3B, 0x80, /*   ### ###        */
	0x1F, 0x00, /*    #####         */
	0x0E, 0x00, /*     ###          */
	0x04, 0x00, /*      #           */
	0x00, 0x00, /*                  */
};

const uint8_t _sg_default_select[] =
{
	0x00, 0x00, /*                  */
	0x00, 0x00, /*                  */
	0x00, 0x00, /*                  */
	0x00, 0x00, /*                  */
	0x7F, 0xFE, /*  ##############  */
	0x3F, 0xFC, /*   ############   */
	0x1F, 0xF8, /*    ##########    */
	0x0F, 0xF0, /*     ########     */
	0x07, 0xE0, /*      ######      */
	0x03, 0xC0, /*       ####       */
	0x01, 0x80, /*        ##        */
	0x00, 0x00, /*                  */
	0x00, 0x00, /*                  */
	0x00, 0x00, /*                  */
	0x00, 0x00, /*                  */
	0x00, 0x00, /*                  */
};

const uint8_t _sg_default_font[] =
{
	/* Char 32 (' ') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 33 ('!') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 34 ('"') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x24, /*   #  #   */
	0x24, /*   #  #   */
	0x24, /*   #  #   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 35 ('#') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x24, /*   #  #   */
	0x24, /*   #  #   */
	0x24, /*   #  #   */
	0x7E, /*  ######  */
	0x24, /*   #  #   */
	0x24, /*   #  #   */
	0x7E, /*  ######  */
	0x24, /*   #  #   */
	0x24, /*   #  #   */
	0x24, /*   #  #   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 36 ('$') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x10, /*    #     */
	0x7C, /*  #####   */
	0x92, /* #  #  #  */
	0x90, /* #  #     */
	0x90, /* #  #     */
	0x7C, /*  #####   */
	0x12, /*    #  #  */
	0x12, /*    #  #  */
	0x92, /* #  #  #  */
	0x7C, /*  #####   */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x00, /*          */

	/* Char 37 ('%') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x64, /*  ##  #   */
	0x94, /* #  # #   */
	0x68, /*  ## #    */
	0x08, /*     #    */
	0x10, /*    #     */
	0x10, /*    #     */
	0x20, /*   #      */
	0x2C, /*   # ##   */
	0x52, /*  # #  #  */
	0x4C, /*  #  ##   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 38 ('&') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x18, /*    ##    */
	0x24, /*   #  #   */
	0x24, /*   #  #   */
	0x18, /*    ##    */
	0x30, /*   ##     */
	0x4A, /*  #  # #  */
	0x44, /*  #   #   */
	0x44, /*  #   #   */
	0x44, /*  #   #   */
	0x3A, /*   ### #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 39 (''') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 40 ('(') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x08, /*     #    */
	0x10, /*    #     */
	0x20, /*   #      */
	0x20, /*   #      */
	0x20, /*   #      */
	0x20, /*   #      */
	0x20, /*   #      */
	0x20, /*   #      */
	0x10, /*    #     */
	0x08, /*     #    */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 41 (')') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x20, /*   #      */
	0x10, /*    #     */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x10, /*    #     */
	0x20, /*   #      */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 42 ('*') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x24, /*   #  #   */
	0x18, /*    ##    */
	0x7E, /*  ######  */
	0x18, /*    ##    */
	0x24, /*   #  #   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 43 ('+') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x10, /*    #     */
	0x7C, /*  #####   */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 44 (',') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x10, /*    #     */
	0x20, /*   #      */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 45 ('-') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7E, /*  ######  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 46 ('.') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 47 ('/') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x04, /*      #   */
	0x04, /*      #   */
	0x08, /*     #    */
	0x08, /*     #    */
	0x10, /*    #     */
	0x10, /*    #     */
	0x20, /*   #      */
	0x20, /*   #      */
	0x40, /*  #       */
	0x40, /*  #       */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 48 ('0') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x46, /*  #   ##  */
	0x4A, /*  #  # #  */
	0x52, /*  # #  #  */
	0x62, /*  ##   #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3C, /*   ####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 49 ('1') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x08, /*     #    */
	0x18, /*    ##    */
	0x28, /*   # #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x3E, /*   #####  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 50 ('2') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x02, /*       #  */
	0x04, /*      #   */
	0x08, /*     #    */
	0x10, /*    #     */
	0x20, /*   #      */
	0x40, /*  #       */
	0x7E, /*  ######  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 51 ('3') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x02, /*       #  */
	0x1C, /*    ###   */
	0x02, /*       #  */
	0x02, /*       #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3C, /*   ####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 52 ('4') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x02, /*       #  */
	0x06, /*      ##  */
	0x0A, /*     # #  */
	0x12, /*    #  #  */
	0x22, /*   #   #  */
	0x42, /*  #    #  */
	0x7E, /*  ######  */
	0x02, /*       #  */
	0x02, /*       #  */
	0x02, /*       #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 53 ('5') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7E, /*  ######  */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x7C, /*  #####   */
	0x02, /*       #  */
	0x02, /*       #  */
	0x02, /*       #  */
	0x42, /*  #    #  */
	0x3C, /*   ####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 54 ('6') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x1C, /*    ###   */
	0x20, /*   #      */
	0x40, /*  #       */
	0x40, /*  #       */
	0x7C, /*  #####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3C, /*   ####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 55 ('7') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7E, /*  ######  */
	0x02, /*       #  */
	0x02, /*       #  */
	0x04, /*      #   */
	0x04, /*      #   */
	0x08, /*     #    */
	0x08, /*     #    */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 56 ('8') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3C, /*   ####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 57 ('9') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3E, /*   #####  */
	0x02, /*       #  */
	0x02, /*       #  */
	0x04, /*      #   */
	0x38, /*   ###    */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 58 (':') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 59 (';') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x10, /*    #     */
	0x20, /*   #      */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 60 ('<') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x04, /*      #   */
	0x08, /*     #    */
	0x10, /*    #     */
	0x20, /*   #      */
	0x40, /*  #       */
	0x20, /*   #      */
	0x10, /*    #     */
	0x08, /*     #    */
	0x04, /*      #   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 61 ('=') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7E, /*  ######  */
	0x00, /*          */
	0x00, /*          */
	0x7E, /*  ######  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 62 ('>') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x40, /*  #       */
	0x20, /*   #      */
	0x10, /*    #     */
	0x08, /*     #    */
	0x04, /*      #   */
	0x08, /*     #    */
	0x10, /*    #     */
	0x20, /*   #      */
	0x40, /*  #       */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 63 ('?') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x04, /*      #   */
	0x08, /*     #    */
	0x08, /*     #    */
	0x00, /*          */
	0x08, /*     #    */
	0x08, /*     #    */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 64 ('@') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7C, /*  #####   */
	0x82, /* #     #  */
	0x9E, /* #  ####  */
	0xA2, /* # #   #  */
	0xA2, /* # #   #  */
	0xA2, /* # #   #  */
	0xA6, /* # #  ##  */
	0x9A, /* #  ## #  */
	0x80, /* #        */
	0x7E, /*  ######  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 65 ('A') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x7E, /*  ######  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 66 ('B') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7C, /*  #####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x7C, /*  #####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x7C, /*  #####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 67 ('C') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3C, /*   ####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 68 ('D') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x78, /*  ####    */
	0x44, /*  #   #   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x44, /*  #   #   */
	0x78, /*  ####    */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 69 ('E') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7E, /*  ######  */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x78, /*  ####    */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x7E, /*  ######  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 70 ('F') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7E, /*  ######  */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x78, /*  ####    */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 71 ('G') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x40, /*  #       */
	0x40, /*  #       */
	0x4E, /*  #  ###  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3C, /*   ####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 72 ('H') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x7E, /*  ######  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 73 ('I') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x38, /*   ###    */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x38, /*   ###    */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 74 ('J') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x0E, /*     ###  */
	0x04, /*      #   */
	0x04, /*      #   */
	0x04, /*      #   */
	0x04, /*      #   */
	0x04, /*      #   */
	0x04, /*      #   */
	0x44, /*  #   #   */
	0x44, /*  #   #   */
	0x38, /*   ###    */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 75 ('K') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x42, /*  #    #  */
	0x44, /*  #   #   */
	0x48, /*  #  #    */
	0x50, /*  # #     */
	0x60, /*  ##      */
	0x60, /*  ##      */
	0x50, /*  # #     */
	0x48, /*  #  #    */
	0x44, /*  #   #   */
	0x42, /*  #    #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 76 ('L') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x7E, /*  ######  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 77 ('M') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x82, /* #     #  */
	0xC6, /* ##   ##  */
	0xAA, /* # # # #  */
	0x92, /* #  #  #  */
	0x92, /* #  #  #  */
	0x82, /* #     #  */
	0x82, /* #     #  */
	0x82, /* #     #  */
	0x82, /* #     #  */
	0x82, /* #     #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 78 ('N') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x62, /*  ##   #  */
	0x52, /*  # #  #  */
	0x4A, /*  #  # #  */
	0x46, /*  #   ##  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 79 ('O') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3C, /*   ####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 80 ('P') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7C, /*  #####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x7C, /*  #####   */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 81 ('Q') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x4A, /*  #  # #  */
	0x3C, /*   ####   */
	0x02, /*       #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 82 ('R') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7C, /*  #####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x7C, /*  #####   */
	0x50, /*  # #     */
	0x48, /*  #  #    */
	0x44, /*  #   #   */
	0x42, /*  #    #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 83 ('S') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x40, /*  #       */
	0x40, /*  #       */
	0x3C, /*   ####   */
	0x02, /*       #  */
	0x02, /*       #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3C, /*   ####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 84 ('T') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0xFE, /* #######  */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 85 ('U') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3C, /*   ####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 86 ('V') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x24, /*   #  #   */
	0x24, /*   #  #   */
	0x24, /*   #  #   */
	0x18, /*    ##    */
	0x18, /*    ##    */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 87 ('W') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x82, /* #     #  */
	0x82, /* #     #  */
	0x82, /* #     #  */
	0x82, /* #     #  */
	0x82, /* #     #  */
	0x92, /* #  #  #  */
	0x92, /* #  #  #  */
	0xAA, /* # # # #  */
	0xC6, /* ##   ##  */
	0x82, /* #     #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 88 ('X') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x24, /*   #  #   */
	0x24, /*   #  #   */
	0x18, /*    ##    */
	0x18, /*    ##    */
	0x24, /*   #  #   */
	0x24, /*   #  #   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 89 ('Y') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x82, /* #     #  */
	0x82, /* #     #  */
	0x44, /*  #   #   */
	0x44, /*  #   #   */
	0x28, /*   # #    */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 90 ('Z') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7E, /*  ######  */
	0x02, /*       #  */
	0x02, /*       #  */
	0x04, /*      #   */
	0x08, /*     #    */
	0x10, /*    #     */
	0x20, /*   #      */
	0x40, /*  #       */
	0x40, /*  #       */
	0x7E, /*  ######  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 91 ('[') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x38, /*   ###    */
	0x20, /*   #      */
	0x20, /*   #      */
	0x20, /*   #      */
	0x20, /*   #      */
	0x20, /*   #      */
	0x20, /*   #      */
	0x20, /*   #      */
	0x20, /*   #      */
	0x38, /*   ###    */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 92 ('\') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x40, /*  #       */
	0x40, /*  #       */
	0x20, /*   #      */
	0x20, /*   #      */
	0x10, /*    #     */
	0x10, /*    #     */
	0x08, /*     #    */
	0x08, /*     #    */
	0x04, /*      #   */
	0x04, /*      #   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 93 (']') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x38, /*   ###    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x38, /*   ###    */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 94 ('^') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x28, /*   # #    */
	0x44, /*  #   #   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 95 ('_') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7E, /*  ######  */
	0x00, /*          */
	0x00, /*          */

	/* Char 96 ('`') */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x08, /*     #    */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 97 ('a') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x02, /*       #  */
	0x3E, /*   #####  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3E, /*   #####  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 98 ('b') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x7C, /*  #####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x7C, /*  #####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 99 ('c') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x42, /*  #    #  */
	0x3C, /*   ####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 100 ('d') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x02, /*       #  */
	0x02, /*       #  */
	0x02, /*       #  */
	0x3E, /*   #####  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3E, /*   #####  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 101 ('e') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x7E, /*  ######  */
	0x40, /*  #       */
	0x40, /*  #       */
	0x3C, /*   ####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 102 ('f') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x0E, /*     ###  */
	0x10, /*    #     */
	0x10, /*    #     */
	0x7C, /*  #####   */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 103 ('g') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3E, /*   #####  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3E, /*   #####  */
	0x02, /*       #  */
	0x02, /*       #  */
	0x3C, /*   ####   */
	0x00, /*          */

	/* Char 104 ('h') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x7C, /*  #####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 105 ('i') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x30, /*   ##     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x38, /*   ###    */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 106 ('j') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x04, /*      #   */
	0x04, /*      #   */
	0x00, /*          */
	0x0C, /*     ##   */
	0x04, /*      #   */
	0x04, /*      #   */
	0x04, /*      #   */
	0x04, /*      #   */
	0x04, /*      #   */
	0x04, /*      #   */
	0x44, /*  #   #   */
	0x44, /*  #   #   */
	0x38, /*   ###    */
	0x00, /*          */

	/* Char 107 ('k') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x42, /*  #    #  */
	0x44, /*  #   #   */
	0x48, /*  #  #    */
	0x70, /*  ###     */
	0x48, /*  #  #    */
	0x44, /*  #   #   */
	0x42, /*  #    #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 108 ('l') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x30, /*   ##     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x38, /*   ###    */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 109 ('m') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0xFC, /* ######   */
	0x92, /* #  #  #  */
	0x92, /* #  #  #  */
	0x92, /* #  #  #  */
	0x92, /* #  #  #  */
	0x92, /* #  #  #  */
	0x92, /* #  #  #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 110 ('n') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7C, /*  #####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 111 ('o') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3C, /*   ####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3C, /*   ####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 112 ('p') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7C, /*  #####   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x7C, /*  #####   */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x00, /*          */

	/* Char 113 ('q') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3E, /*   #####  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3E, /*   #####  */
	0x02, /*       #  */
	0x02, /*       #  */
	0x02, /*       #  */
	0x00, /*          */

	/* Char 114 ('r') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x5E, /*  # ####  */
	0x60, /*  ##      */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x40, /*  #       */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 115 ('s') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x3E, /*   #####  */
	0x40, /*  #       */
	0x40, /*  #       */
	0x3C, /*   ####   */
	0x02, /*       #  */
	0x02, /*       #  */
	0x7C, /*  #####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 116 ('t') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x7C, /*  #####   */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x0E, /*     ###  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 117 ('u') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3E, /*   #####  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 118 ('v') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x24, /*   #  #   */
	0x24, /*   #  #   */
	0x18, /*    ##    */
	0x18, /*    ##    */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 119 ('w') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x82, /* #     #  */
	0x82, /* #     #  */
	0x92, /* #  #  #  */
	0x92, /* #  #  #  */
	0x92, /* #  #  #  */
	0x92, /* #  #  #  */
	0x7C, /*  #####   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 120 ('x') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x24, /*   #  #   */
	0x18, /*    ##    */
	0x24, /*   #  #   */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 121 ('y') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x42, /*  #    #  */
	0x3E, /*   #####  */
	0x02, /*       #  */
	0x02, /*       #  */
	0x3C, /*   ####   */
	0x00, /*          */

	/* Char 122 ('z') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x7E, /*  ######  */
	0x04, /*      #   */
	0x08, /*     #    */
	0x10, /*    #     */
	0x20, /*   #      */
	0x40, /*  #       */
	0x7E, /*  ######  */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 123 ('{') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x0C, /*     ##   */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x20, /*   #      */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x0C, /*     ##   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 124 ('|') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x10, /*    #     */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 125 ('}') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x30, /*   ##     */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x04, /*      #   */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x08, /*     #    */
	0x30, /*   ##     */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */

	/* Char 126 ('~') */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x62, /*  ##   #  */
	0x92, /* #  #  #  */
	0x8C, /* #   ##   */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
	0x00, /*          */
};

#endif
