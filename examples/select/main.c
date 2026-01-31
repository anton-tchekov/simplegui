#include <stdio.h>

#define SIMPLEGUI_IMPLEMENTATION
#include "../../simplegui.h"

#define ARRLEN(X) (sizeof(X) / sizeof(*X))

int main(void)
{
	int cur = 0;
	const char *items[] =
	{
		"Red",
		"Green",
		"Blue",
		"Yellow",
		"Orange"
	};

	sg_init(640, 480, "Select");
	while(sg_running())
	{
		sg_begin();
		sg_clear(_sg_window_bg);

		sg_label(sg_rect(50, 50, 0, 0), "Select Color:", 0);
		if(sg_select(sg_rect(50, 70, 200, 32), items, ARRLEN(items), &cur))
		{
			printf("Select changed value!\n");
		}

		char buf[64];
		snprintf(buf, sizeof(buf), "Selected value: %s", items[cur]);
		sg_label(sg_rect(50, 120, 0, 0), buf, 0);
		sg_update();
	}

	sg_destroy();
	return 0;
}
