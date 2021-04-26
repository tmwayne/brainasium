gamesdir = $(pkglibdir)/games

games_LTLIBRARIES = sample_game.la

sample_game_la_SOURCES = sample-game.c ../src/game.c ../src/registry.c
sample_game_la_CPPFLAGS = -I$(top_srcdir)/src -I$(includedir)
sample_game_la_LDFLAGS = -module
sample_game_la_LIBADD = -L$(libdir) -lerror
# sample_game_la_LIBADD = $(libdir)/liberror.la
