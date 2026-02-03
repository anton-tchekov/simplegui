#include <stdio.h>

#define SIMPLEGUI_IMPLEMENTATION
#include "../../simplegui.h"

int main(void)
{
	double value = 50.0;

	sg_init(sg_size(640, 480), "Simple Slider");
	while(sg_running())
	{
		sg_begin();

		if(sg_slider(sg_rect(50, 50, 800, 32), &value, 50.0, 120.0))
		{
			printf("Slider changed value!\n");
		}

		char buf[64];
		snprintf(buf, sizeof(buf), "%8.2f", value);
		sg_label(sg_rect(50, 100, 0, 0), buf, 0);
		sg_update();
	}

	sg_destroy();
	return 0;
}
