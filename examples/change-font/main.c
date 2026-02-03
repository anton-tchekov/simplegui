#include <stdio.h>

#define SIMPLEGUI_IMPLEMENTATION
#include "../../simplegui.h"

#define MAX_CHAR_SIZE 64
#define FONT_SIZE     32

#define FONT_FILE       "arial.ttf"

int main(void)
{
	sg_init(sg_size(640, 480), "SimpleGUI Change Font");

	SgFont font = sg_font_load(FONT_FILE, FONT_SIZE);
	if(!font)
	{
		fprintf(stderr, "Failed to open font: \"%s\"\n", FONT_FILE);
		return 1;
	}

	SgFontAtlas fontatlas = sg_font_atlas_create(MAX_CHAR_SIZE, FONT_SIZE);
	sg_fontatlas_add_ascii(fontatlas, font);
	sg_fontatlas_update(fontatlas);
	sg_font_destroy(font);
	sg_set_fontatlas(fontatlas);

	while(sg_running())
	{
		sg_begin();

		sg_render_string(sg_point(10, 10), "Hello World!", SG_WHITE);
		sg_button(sg_rect(10, 100, 200, 50), "Click Me!");

		sg_update();
	}

	sg_fontatlas_destroy(fontatlas);
	sg_destroy();
	return 0;
}
