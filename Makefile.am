
AM_CFLAGS = --std=c17 -Wall -Wextra -Werror

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