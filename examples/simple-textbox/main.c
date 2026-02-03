#include <stdio.h>

#define SIMPLEGUI_IMPLEMENTATION
#include "../../simplegui.h"

int main(void)
{
	char buf[64] = "Hello world";
	SgStringBuffer sb =
	{
		.buffer = buf,
		.length = 11,
		.capacity = sizeof(buf)
	};

	sg_init(sg_size(640, 480), "Simple Textbox");
	while(sg_running())
	{
		sg_begin();

		if(sg_textbox(sg_rect(50, 50, 800, 32), &sb))
		{
			printf("Textbox changed value!\n");
		}

		sg_update();
	}

	sg_destroy();
	return 0;
}
