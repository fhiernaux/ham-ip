bin_PROGRAMS = hamip
HAMIP_SRC = \
	src/hamip.cpp \
	src/packet.h \
	src/Packetizer.cpp \
	src/Packetizer.h \
	src/Depacketizer.cpp \
	src/Depacketizer.h \
	src/TunInterface.cpp \
	src/TunInterface.h \
	src/MinimodemSender.cpp \
	src/MinimodemSender.h \
	src/Configuration.cpp \
	src/Configuration.h

SIMPLEAUDIO_SRC = \
	minimodem/src/simpleaudio.h		\
	minimodem/src/simpleaudio_internal.h	\
	minimodem/src/simpleaudio.c		\
	minimodem/src/simple-tone-generator.c \
	minimodem/src/simpleaudio-pulse.c	\
	minimodem/src/simpleaudio-alsa.c	\
	minimodem/src/simpleaudio-benchmark.c	\
	minimodem/src/simpleaudio-sndfile.c

hamip_SOURCES = $(HAMIP_SRC) $(SIMPLEAUDIO_SRC)
hamip_LDADD = $(DEPS_LIBS) $(BOOST_THREAD_LIBS) $(BOOST_PROGRAM_OPTIONS_LIBS)
hamip_CFLAGS = $(DEPS_CFLAGS)
hamip_CPPFLAGS = $(BOOST_CPPFLAGS) -Wall
hamip_LDFLAGS = $(BOOST_THREAD_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LDFLAGS)

ACLOCAL_AMFLAGS = -I m4 -I build-aux --install
EXTRA_DIST = m4/NOTES
