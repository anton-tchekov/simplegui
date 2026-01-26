#include <stdio.h>

#define SIMPLEGUI_IMPLEMENTATION
#include "../../simplegui.h"

int main(void)
{
	bool checked = false;

	sg_init(640, 480, "Simple Button");
	while(sg_running())
	{
		sg_begin();
		sg_clear(_sg_window_bg);

		if(sg_checkbox(sg_rect(50, 50, 32, 32), &checked))
		{
			printf("Checkbox changed state!\n");
		}

		sg_label(sg_rect(92, 58, 0, 0), checked ? "Checked" : "Unchecked", 0);
		sg_update();
	}

	sg_destroy();
	return 0;
}
