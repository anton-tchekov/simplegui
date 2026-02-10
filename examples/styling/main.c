#include <stdio.h>

#define SIMPLEGUI_IMPLEMENTATION
#include "../../simplegui.h"

#define MAX_CHAR_SIZE 64
#define FONT_SIZE     14

#define FONT_FILE       "../fonts/arial.ttf"

int main(void)
{
	sg_init(sg_size(640, 480), "SimpleGUI Styling Elements");

	SgFont font = sg_font_load(FONT_FILE, FONT_SIZE);
	if(!font)
	{
		fprintf(stderr, "Failed to open font: \"%s\"\n", FONT_FILE);
		return 1;
	}

	SgFontAtlas fontatlas = sg_font_atlas_create(MAX_CHAR_SIZE, FONT_SIZE);
	sg_fontatlas_add_ascii(fontatlas, font);
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
		.SelectPageBorderColor = 0x333333,
		.SelectPageBorderThickness = 1,
		.SelectItemInnerColor = { 0xFFFFFF, 0x3399ff },
		.SelectItemTextColor = { 0x000000, 0xFFFFFF },
		.SelectItemPadding = 3,
		.SelectPageItems = 5,
		.SelectPaddingX = 10,
	};

	sg_theme = &theme;

	char buf[64] = "Hello world";
	SgStringBuffer sb =
	{
		.buffer = buf,
		.length = 11,
		.capacity = sizeof(buf)
	};

	bool checked = true;
	double value = 0.0;

	size_t cur = 1;
	const char *items[] =
	{
		"January",
		"February",
		"March",
		"April",
		"May",
		"June",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December"
	};

	while(sg_running())
	{
		sg_begin();

		sg_render_string(sg_point(10, 10), "Hello World!", SG_BLACK);

		sg_select(sg_rect(10, 50, 200, 30), items, SG_ARRLEN(items), &cur);
		sg_textbox(sg_rect(10, 100, 200, 30), &sb);
		sg_slider(sg_rect(10, 150, 800, 24), &value, 0.0, 100.0);
		sg_checkbox(sg_rect(10, 200, 22, 22), &checked);
		sg_button(sg_rect(10, 250, 200, 30), "Click Me!");

		sg_update();
	}

	sg_fontatlas_destroy(fontatlas);
	sg_destroy();
	return 0;
}
