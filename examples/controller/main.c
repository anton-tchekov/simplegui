#include <stdio.h>

#define SIMPLEGUI_IMPLEMENTATION
#include "../../simplegui.h"

#define MAX_CHAR_SIZE 64
#define FONT_SIZE     32

#define FONT_FILE       "../fonts/arial.ttf"

int main(void)
{
	sg_init(sg_size(640, 480), "Controller");

	SgFont font = sg_font_load(FONT_FILE, FONT_SIZE);
	if(!font)
	{
		fprintf(stderr, "Failed to open font: \"%s\"\n", FONT_FILE);
		return 1;
	}

	SgFontAtlas fontatlas = sg_font_atlas_create(MAX_CHAR_SIZE, FONT_SIZE);
	sg_fontatlas_add_ascii(fontatlas, font);
	sg_fontatlas_add_icon(fontatlas, "up_left.png", 200);
	sg_fontatlas_add_icon(fontatlas, "up.png", 201);
	sg_fontatlas_add_icon(fontatlas, "up_right.png", 202);
	sg_fontatlas_add_icon(fontatlas, "left.png", 203);
	sg_fontatlas_add_icon(fontatlas, "center.png", 204);
	sg_fontatlas_add_icon(fontatlas, "right.png", 205);
	sg_fontatlas_add_icon(fontatlas, "down_left.png", 206);
	sg_fontatlas_add_icon(fontatlas, "down.png", 207);
	sg_fontatlas_add_icon(fontatlas, "down_right.png", 208);

	sg_fontatlas_add_default_icons(fontatlas);
	sg_fontatlas_update(fontatlas);
	sg_font_destroy(font);
	sg_set_fontatlas(fontatlas);

	SgTheme theme =
	{
		/* Window */
		.WindowBackgroundColor = 0xf0f0f0,

		/* Label */
		.LabelTextColor = 0x000000,

		/* Button */
		.ButtonTextColor = { 0x000000, 0x000000, 0x000000 },
		.ButtonInnerColor = { 0xe1e1e1, 0xdddddd, 0xcccccc },
		.ButtonBorderColor ={ 0xadadad, 0x777777, 0x52a9fd },
		.ButtonBorderThickness = { 1, 1, 2 },

		/* Checkbox */
		.CheckboxIconColor = { SG_GREEN, SG_GREEN, SG_GREEN },
		.CheckboxInnerColor = { 0xFFFFFF, 0xFFFFFF, 0xFFFFFF },
		.CheckboxBorderColor = { 0xadadad, 0x777777, 0x52a9fd },
		.CheckboxBorderThickness = { 1, 1, 2 },

		/* Textbox */
		.TextboxTextColor = { 0x000000, 0x000000, 0x000000 },
		.TextboxInnerColor = { 0xFFFFFF, 0xFFFFFF, 0xFFFFFF },
		.TextboxBorderColor = { 0xadadad, 0x777777, 0x52a9fd },
		.TextboxBorderThickness = { 1, 1, 2 },
		.TextboxPaddingX = 6,
		.Cursor = { -1, -1, 1, 2 },
		.CursorColor = 0x000000,
		.SelectionColor = 0x3399ff,
		.SelectionTextColor = 0xffffff,

		/* Slider */
		.SliderThumbColor = { 0x999999, 0x888888, 0x52a9fd },
		.SliderRailColor = { 0xcccccc, 0xbbbbbb, 0xaaaaaa },
		.SliderThumbWidth = 6,
		.SliderRailHeight = 4,

		/* Select */
		.SelectTextColor = { 0x000000, 0x000000, 0x000000 },
		.SelectInnerColor = { 0xe1e1e1, 0xdddddd, 0xcccccc },
		.SelectBorderColor = { 0xadadad, 0x777777, 0x52a9fd },
		.SelectBorderThickness = { 1, 1, 2 },
		.SelectPageBorder = 0x333333,
		.SelectItemInnerColor = { 0xFFFFFF, 0x3399ff },
		.SelectItemTextColor = { 0x000000, 0xFFFFFF },
		.SelectItemPadding = 2,
		.SelectPageItems = 5,
		.SelectPaddingX = 10,
	};

	sg_theme = &theme;

	int D = 10;
	int S = 80;
	int O = S + D;

	while(sg_running())
	{
		sg_begin();

		sg_label(sg_rect(D, D, 0, 0), "Controller", SG_TOP_LEFT);

		for(int y = 0; y < 3; ++y)
		{
			for(int x = 0; x < 3; ++x)
			{
				char text[] = { 200 + 3*y + x, '\0' };
				if(sg_button(sg_rect(D + x * O, 2*D+FONT_SIZE + y * O, S, S), text))
				{
					printf("%d %d clicked\n", x, y);
				}
			}
		}

		sg_update();
	}

	sg_fontatlas_destroy(fontatlas);
	sg_destroy();
	return 0;
}
