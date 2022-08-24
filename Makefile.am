
AM_CFLAGS = --std=c17 -Wall -Wextra -Werror

AC_CHECK_HEADER([alsa/asoundlib.h])
AC_CHECK_LIB([asound], [snd_device_name_hint], [], [AC_MSG_ERROR([Unable to find libasound])])

bin_PROGRAMS=sound
sound_SOURCES=sound.c

clean-local:
	@rm configure.scan configure.ac autoscan.log 
	@rm config.status configure config.log
	@rm Makefile
	@rm -r autom4te.cache/ .deps/
	@rm aclocal.m4
	@rm compile install-sh missing Makefile.in
	@rm config.h config.h.in COPYING depcomp INSTALL stamp-h1