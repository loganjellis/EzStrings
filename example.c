#include "vibrant_logs.h"
#include "ez_strings.h"

int main(void)
{
	vl_init();

	EzString s = ez_string_create("hello world!");

	ez_string_print(&s);

	ez_string_append_s(&s, " goodbye world!");

	ez_string_print(&s);

	ez_string_destroy(&s);

	return 0;
}
