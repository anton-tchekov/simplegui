#include <stdio.h>

#define SIMPLEGUI_IMPLEMENTATION
#include "../../simplegui.h"

#define MAX_CHAR_SIZE 64
#define FONT_SIZE     14

#define FONT_FILE       "../fonts/arial.ttf"

int main(void)
{
	sg_init(sg_size(640, 480), "Power Usage Calculator");

	SgFont font = sg_font_load(FONT_FILE, FONT_SIZE);
	if(!font)
	{
		fprintf(stderr, "Failed to open font: \"%s\"\n", FONT_FILE);
		return 1;
	}

	SgFontAtlas fontatlas = sg_font_atlas_create(MAX_CHAR_SIZE, FONT_SIZE);
	sg_fontatlas_add_ascii(fontatlas, font);
	sg_fontatlas_add_utf8(fontatlas, font, "€", 0xFF);
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

	const char *power_unit[] = { "watt [W]", "kilowatt [kW]" };
	size_t power_unit_sel = 0;

	const char *time_unit[] =
	{
		"minutes per day",
		"hours per day",
		"hours per week",
		"hours per month",
		"days per week",
		"days per month",
		"days per year",
		"months per year"
	};

	size_t time_unit_sel = 1;

	const char *appliance[] =
	{
		"Define your own",
		"Light bulb (LED)",
		"Light bulb (Incandescent)",
		"Refrigerator",
		"Television",
		"Microwave oven",
		"Dishwasher"
	};

	size_t appliance_sel = 0;

	char buf_power[64];
	SgStringBuffer sb_power = { buf_power, 0, sizeof(buf_power) };

	char buf_capacity[64];
	SgStringBuffer sb_capacity = { buf_capacity, 0, sizeof(buf_capacity) };

	char buf_usage[64];
	SgStringBuffer sb_usage = { buf_usage, 0, sizeof(buf_usage) };

	char buf_price[64];
	SgStringBuffer sb_price = { buf_price, 0, sizeof(buf_price) };

	int X0 = 10;
	int X1 = 220;
	int X2 = 430;
	int H = 30;
	int W = 200;
	int YS = 40;

	while(sg_running())
	{
		sg_begin();

		int Y = 10;

		sg_label(sg_rect(X0, Y, W, H), "Typical Appliance:", SG_CENTER_RIGHT);
		sg_select(sg_rect(X1, Y, W, H), appliance, SG_ARRLEN(appliance), &appliance_sel);
		Y += YS;

		sg_label(sg_rect(X0, Y, W, H), "Appliance Power:", SG_CENTER_RIGHT);
		sg_textbox(sg_rect(X1, Y, W, H), &sb_power);
		sg_select(sg_rect(X2, Y, W, H), power_unit, SG_ARRLEN(power_unit), &power_unit_sel);
		Y += YS;

		sg_label(sg_rect(X0, Y, W, H), "Use/run at:", SG_CENTER_RIGHT);
		sg_textbox(sg_rect(X1, Y, W, H), &sb_capacity);
		sg_label(sg_rect(X2, Y, W, H), "% capacity", SG_CENTER_LEFT);
		Y += YS;

		sg_label(sg_rect(X0, Y, W, H), "Usage:", SG_CENTER_RIGHT);
		sg_textbox(sg_rect(X1, Y, W, H), &sb_usage);
		sg_select(sg_rect(X2, Y, W, H), time_unit, SG_ARRLEN(time_unit), &time_unit_sel);
		Y += YS;

		sg_label(sg_rect(X0, Y, W, H), "Electricity Price:", SG_CENTER_RIGHT);
		sg_textbox(sg_rect(X1, Y, W, H), &sb_price);
		sg_label(sg_rect(X2, Y, W, H), "\xFF per kWh", SG_CENTER_LEFT);
		Y += YS;

		sg_button(sg_rect(X1, Y, W, H), "Calculate");
		sg_button(sg_rect(X2, Y, W, H), "Clear");

		sg_update();
	}

	sg_fontatlas_destroy(fontatlas);
	sg_destroy();
	return 0;
}
