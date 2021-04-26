bin_PROGRAMS = gym

gym_SOURCES = main.c registry.c game.c
gym_CPPFLAGS = -I$(includedir)
gym_LDADD = $(libdir)/libmem.la \
	$(libdir)/liberror.la \
	$(libdir)/libcstrings.la \
	$(libdir)/libdict.la \
	$(libdir)/libconfigparse.la
