#include <stdio.h>

#define _POSIX_SOURCE // otherwise there is timespec redefinition error
#include <alsa/asoundlib.h>

int main(void) {
    char **names;
    int err = snd_device_name_hint(-1, "ctl", (void ***)&names);
    if (err != 0) {
        fprintf(stderr, "snd_device_name_hint returned non-zero code\n");
        return -1;
    }
    int sz = 0;
    for (char **it = names; *it != NULL; ++it)
        printf("%d: %s\n", ++sz, *it);
    return 0;
}