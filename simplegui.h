/* ========================================================================== */
/* HEADER */

#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

/* ========================================================================== */
/* sdl2 includes */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

/* SgColor */
typedef uint32_t SgColor;
SgColor sg_color(int r, int g, int b);

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

int sg_rect_contains_point(SgRect rect, SgPoint p);

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

SgTexture sg_load_texture(const char *file);
void sg_destroy_texture(SgTexture texture);
SgSize sg_texture_size(SgTexture texture);
SgRect sg_texture_bounds(SgTexture texture);
void sg_draw_texture(SgTexture texture, SgRect src, SgRect dst);

/* SgFont */
typedef TTF_Font *SgFont;

SgFont sg_font_load(const char *file, int size);
void sg_font_destroy(SgFont font);

/* SgFontAtalas */
typedef struct
{
	int MaxCharSize;
	SgSize CharDim[256];
	SgSurface Surface;
	SgTexture Texture;
} SgFontAtlasInternal;

typedef SgFontAtlasInternal *SgFontAtlas;

SgFontAtlas sg_font_atlas_create(int max_char_size);
void sg_set_fontatlas(SgFontAtlas atlas);
void sg_fontatlas_destroy(SgFontAtlas atlas);

int sg_fontatlas_add_ascii(SgFontAtlas atlas, SgFont font);
int sg_fontatlas_add_char(SgFontAtlas atlas, SgFont font, const char *utf8, uint8_t index);
int sg_fontatlas_add_icon(SgFontAtlas atlas, const char *file, uint8_t index);
int sg_fontatlas_blit_surface(SgFontAtlas atlas, SgSurface surface, SgRect src, uint8_t index);
void sg_fontatlas_add_binary(SgFontAtlas atlas, const uint8_t *bytes, SgSize size, uint8_t index);
void sg_fontatlas_update(SgFontAtlas atlas);

/* font rendering */
int sg_char_width(uint8_t c);
int sg_string_width(const char *s);

/* allocation */
void *sg_malloc(size_t size);
void *sg_calloc(size_t count, size_t size);
void *sg_realloc(void *p, size_t size);
void sg_free(void *p);
void sg_alloc_report(void);

/* window */
void sg_init(int width, int height, const char *title);
void sg_destroy(void);
void sg_set_title(const char *title);
void sg_begin(void);
void sg_clear(SgColor color);
void sg_update(void);
int sg_running(void);
SgSize sg_get_window_size(void);

/* mouse handling */
int sg_is_mouse_button_down(int button);
int sg_is_mouse_button_pressed(int button);
int sg_is_mouse_button_released(int button);
SgPoint sg_mouse_position(void);
SgPoint sg_mouse_wheel(void);

/* keyboard handling */
int sg_is_key_down(int key);
int sg_is_key_pressed(int key);
int sg_is_key_released(int key);

/* draw and fill rect */
void sg_fill_rect(SgRect rect, SgColor color);
void sg_draw_rect(SgRect rect, int border, SgColor color);

/* Controls */
/* All Controls are styled based on the same global theme */
/* Controls return a bit field of events */

/* SgControlState */
enum
{
	SG_CLICKED = 1,
	SG_CHANGED = 2,
	SG_ENTER_PRESSED = 4
};

void sg_label(SgRect dimensions, const char *text, int flags);
int sg_button(SgRect dimensions, const char *text);
int sg_checkbox(SgRect dimensions, const char *text, bool *checked);
int sg_slider(SgRect dimensions, float *value, float min, float max);
int sg_textbox(SgRect dimensions, char *text, size_t capacity);
int sg_select(SgRect dimensions, const char *text, int *active);

/* ========================================================================== */
/* IMPLEMENTATION */

#ifdef SIMPLEGUI_IMPLEMENTATION

/* ========================================================================== */
/* graphics global state */

extern const uint8_t _sg_default_font[];

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

#define SG_FONTDEBUG 1

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

SgFontAtlas sg_font_atlas_create(int max_char_size)
{
	SgSurface surface = SDL_CreateRGBSurface(0,
		max_char_size * 16, max_char_size * 16,
		32, 0xff, 0xff00, 0xff0000, 0xff000000);

	SgFontAtlasInternal *atlas = sg_calloc(1, sizeof(SgFontAtlasInternal));
	memset(atlas->CharDim, 0, sizeof(atlas->CharDim));
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
		sg_destroy_texture(atlas->Texture);
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
					printf("\t0x%02X, ", byte);
					bit = 7;
					byte = 0;
				}
			}

			if((src.w & 3) != 0)
			{
				printf("\t0x%02X, ", byte);
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

int sg_fontatlas_add_ascii(SgFontAtlas atlas, SgFont font)
{
	SDL_Color color = { 255, 255, 255, 0 };
	char letter[2];
	letter[1] = '\0';
	int status = 0;
	for(int i = 32; i <= 126; ++i)
	{
		letter[0] = i;
		SDL_Surface *s = TTF_RenderText_Blended(font, letter, color);
		if(sg_fontatlas_blit_surface(atlas, s, sg_rect(0, 0, s->w, s->h), i))
		{
			status = 1;
		}

		SDL_FreeSurface(s);
	}

	return status;
}

int sg_fontatlas_add_char(SgFontAtlas atlas, SgFont font, const char *utf8, uint8_t index)
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

void sg_fontatlas_update(SgFontAtlas atlas)
{
	if(SG_FONTDEBUG)
	{
		SDL_SaveBMP(atlas->Surface, "fontatlas.bmp");
	}

	if(atlas->Texture)
	{
		sg_destroy_texture(atlas->Texture);
	}

	atlas->Texture = SDL_CreateTextureFromSurface(_sg_renderer, atlas->Surface);
}

int sg_render_char(int x, int y, uint8_t c, SgColor color)
{
	int stride = _sg_fontatlas->MaxCharSize;
	SgSize size = _sg_fontatlas->CharDim[c];
	SDL_Rect src = { (c & 0x0F) * stride, (c >> 4) * stride, size.w, size.h };
	SDL_Rect dst = { x, y, size.w, size.h };
	SDL_SetTextureColorMod(_sg_fontatlas->Texture,
		sg_color_r(color), sg_color_g(color), sg_color_b(color));
	SDL_RenderCopy(_sg_renderer, _sg_fontatlas->Texture, &src, &dst);
	return size.w;
}

void sg_render_string(int x, int y, const char *s, SgColor color)
{
	for(uint8_t c; (c = *s); ++s)
	{
		x += sg_render_char(x, y, c, color);
	}
}

int sg_char_width(uint8_t c)
{
	return _sg_fontatlas->CharDim[c].w;
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

void sg_init(int width, int height, const char *title)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Error initializing SDL; SDL_Init: %s\n", SDL_GetError());
		sg_destroy();
		exit(1);
	}

	if(!(_sg_window = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_RESIZABLE)))
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

	SDL_SetRenderDrawBlendMode(_sg_renderer, SDL_BLENDMODE_BLEND);

	_sg_key_state = SDL_GetKeyboardState(&_sg_num_keys);
	_sg_key_pressed = sg_calloc(_sg_num_keys, 1);
	_sg_key_released = sg_calloc(_sg_num_keys, 1);

	_sg_running = true;

	_sg_fontatlas_default = sg_font_atlas_create(32);
	for(int c = 32; c <= 126; ++c)
	{
		sg_fontatlas_add_binary(_sg_fontatlas_default,
			_sg_default_font + (c - 32) * 18,
			sg_size(8, 18), c);
	}

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

void sg_clear(SgColor color)
{
	sg_set_color(color);
	SDL_RenderClear(_sg_renderer);
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
/* draw and fill rectangle */
SgTexture sg_load_texture(const char *file)
{
	return IMG_LoadTexture(_sg_renderer, file);
}

void sg_destroy_texture(SgTexture texture)
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

void sg_draw_texture(SgTexture texture, SgRect src, SgRect dst)
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
	_sg_mouse_button_pressed |= SDL_BUTTON(e->button.button);
}

static void sg_handle_mouse_button_up(SDL_Event *e)
{
	_sg_mouse_button_released |= SDL_BUTTON(e->button.button);
}

static void sg_handle_key_down(SDL_Event *e)
{
	_sg_key_pressed[e->key.keysym.scancode] = 1;

	/*int key = key_convert(e.key.keysym.scancode, e.key.keysym.mod);
	gui_event_key(key, key_to_codepoint(key),
		e.key.repeat ? KEYSTATE_REPEAT : KEYSTATE_PRESSED);*/
}

static void sg_handle_key_up(SDL_Event *e)
{
	_sg_key_released[e->key.keysym.scancode] = 1;

	/*int key = key_convert(e.key.keysym.scancode, e.key.keysym.mod);
	gui_event_key(key, key_to_codepoint(key), KEYSTATE_RELEASED);*/
}

void sg_begin(void)
{
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
			_sg_running = 0;
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
}

void sg_update(void)
{
	SDL_RenderPresent(_sg_renderer);
}

int sg_running(void)
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
int sg_is_key_down(int key)
{
	return _sg_key_state[key];
}

int sg_is_key_pressed(int key)
{
	return _sg_key_pressed[key];
}

int sg_is_key_released(int key)
{
	return _sg_key_released[key];
}

/* ========================================================================== */
/* GUI GLOBAL STATE */
SgColor _sg_window_bg = 0x100000;

SgColor _sg_color_text = 0xff8200;
SgColor _sg_color_text_hover = 0xff8200;
SgColor _sg_color_text_active = 0xff8200;

SgColor _sg_color_sel_fg = 0x310000;
SgColor _sg_color_sel_bg = 0xff8200;

SgColor _sg_color_bg = 0x310000;
SgColor _sg_color_bg_hover = 0x7b0000;
SgColor _sg_color_bg_active = 0x510000;

SgColor _sg_color_border = 0x7b0000;
SgColor _sg_color_border_hover = 0xff8200;
SgColor _sg_color_border_active = 0xff8200;

int _sg_border_thickness = 2;
int _font_height = 16;

/* ========================================================================== */
/* sg_color */
SgColor sg_color(int r, int g, int b)
{
	return (r << 16) | (g << 8) | b;
}

/* ========================================================================== */
/* sg_rect */
int sg_rect_contains_point(SgRect rect, SgPoint p)
{
	return p.x >= rect.x && p.y >= rect.y &&
		p.x < rect.x + rect.w && p.y < rect.y + rect.h;
}

int sg_rect_mouse(SgRect rect)
{
	return sg_rect_contains_point(rect, sg_mouse_position());
}

/* ========================================================================== */
/* common utility functions for all elements */


/* ========================================================================== */
/* sg_label */
void sg_label(SgRect d, const char *text, int flags)
{
	int valign = flags & SG_VALIGN_MASK;
	int halign = flags & SG_HALIGN_MASK;

	int rx = d.x;
	if(halign == SG_HALIGN_CENTER)
	{
		rx += d.w / 2 - sg_string_width(text) / 2;
	}
	else if(halign == SG_HALIGN_RIGHT)
	{
		rx += d.w - sg_string_width(text);
	}

	int ry = d.y;
	if(valign == SG_VALIGN_CENTER)
	{
		ry += d.h / 2 - _font_height / 2;
	}
	else if(valign == SG_VALIGN_BOTTOM)
	{
		ry += d.h - _font_height;
	}

	sg_render_string(rx, ry, text, _sg_color_text);
}

/* ========================================================================== */
/* sg_button */
int sg_button(SgRect d, const char *text)
{
	int hover = sg_rect_mouse(d);
	int active = sg_is_mouse_button_down(SG_BUTTON_LEFT);

	sg_fill_rect(d,
		hover ? (active ? _sg_color_bg_active : _sg_color_bg_hover) : _sg_color_bg);

	sg_draw_rect(d, _sg_border_thickness,
		hover ? (active ? _sg_color_border_active : _sg_color_border_hover) : _sg_color_border);

	sg_render_string(
		d.x + d.w / 2 - sg_string_width(text) / 2,
		d.y + d.h / 2 - _font_height / 2,
		text, hover ? (active ? _sg_color_text_active : _sg_color_text_hover) : _sg_color_text);

	return hover && sg_is_mouse_button_pressed(SG_BUTTON_LEFT);
}

/* ========================================================================== */
/* sg_checkbox */


/* ========================================================================== */
/* Default (fallback) font */
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
