AC_INIT([ham-ip], [0.01])

AC_PREREQ([2.68])
AC_CONFIG_SRCDIR([src/hamip.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.11 subdir-objects -Wall -Werror])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_FILES([Makefile])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

# Library Checks
AC_SEARCH_LIBS([lroundf], [m])

deps_packages="fftw3f"

#   ALSA
AC_ARG_WITH([alsa], [AS_HELP_STRING([--without-alsa],
            [build without ALSA support])])
AS_IF([test "x$with_alsa" == "xno"], [
    use_alsa=0
], [
    use_alsa=1
    deps_packages="$deps_packages alsa"
])
AC_DEFINE_UNQUOTED([USE_ALSA], [$use_alsa],
                   [Define to 1 to enable ALSA support])

#   pulseaudio
AC_ARG_WITH([pulseaudio], AS_HELP_STRING([--without-pulseaudio],
            [build without pulseaudio support]))
AS_IF([test "x$with_pulseaudio" == "xno"], [
    use_pulseaudio=0
], [
    use_pulseaudio=1
    deps_packages="$deps_packages libpulse-simple"
])
AC_DEFINE_UNQUOTED([USE_PULSEAUDIO], [$use_pulseaudio],
                   [Define to 1 to enable pulseaudio support])

# Checks for libraries.
PKG_CHECK_MODULES(DEPS, [$deps_packages])

AC_MSG_RESULT([
option summary:
    alsa           $with_alsa ($use_alsa)
    pulseaudio     $with_pulseaudio ($use_pulseaudio)
])

BOOST_REQUIRE([1.22])
BOOST_CRC

AC_OUTPUT
