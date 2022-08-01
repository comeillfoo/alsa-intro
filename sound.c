#include <stdio.h>
#include <alsa/global.h>
#include <alsa/input.h>
#include <alsa/output.h>
#include <alsa/conf.h>
#include <alsa/control.h>


int main(void) {
	char** names;
	int err = snd_device_name_hint(-1, "ctl", (void***) &names);
	int sz = 0;
	for (char** it = names; *it != NULL; ++it)
		printf("%d: %s\n", ++sz, *it);
	return 0;
}
