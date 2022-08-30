AM_CFLAGS = --std=c17

bin_PROGRAMS = sound zelda
sound_SOURCES = src/sound.c
zelda_SOURCES = src/zelda.c

man1_MANS = docs/sound.man

clean-local:
	@rm configure.scan configure.ac autoscan.log 
	@rm config.status configure config.log
	@rm Makefile
	@rm -r autom4te.cache/ .deps/
	@rm aclocal.m4
	@rm compile install-sh missing Makefile.in
	@rm config.h config.h.in COPYING depcomp INSTALL stamp-h1
	@rm -f a.out sound sound.o zelda zelda.o