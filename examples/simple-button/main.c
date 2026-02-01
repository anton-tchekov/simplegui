#include <stdio.h>

#define SIMPLEGUI_IMPLEMENTATION
#include "../../simplegui.h"

int main(void)
{
	char buf[64];
	const char *msg = "Click Me!";
	int click_count = 0;

	sg_init(sg_size(640, 480), "Simple Button");
	while(sg_running())
	{
		sg_begin(_sg_window_bg);

		if(click_count > 0)
		{
			snprintf(buf, sizeof(buf), "Clicked %d times", click_count);
			msg = buf;
		}

		if(sg_button(sg_rect(50, 50, 200, 50), msg))
		{
			++click_count;
		}

		sg_update();
	}

	sg_destroy();
	return 0;
}
