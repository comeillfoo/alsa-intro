#include <stdio.h>

#include <alsa/asoundlib.h>

int main(void) {
    char **names;
    int err = snd_device_name_hint(-1, "ctl", (void ***)&names);
    int sz = 0;
    for (char **it = names; *it != NULL; ++it)
        printf("%d: %s\n", ++sz, *it);
    return 0;
}