# Process this file with autoconf to produce a configure script

AC_PREREQ(2.65)
AC_INIT([xorgxnest], [0.2.0], [jay.sorg@gmail.com])
AC_CONFIG_HEADERS(config_ac.h:config_ac-h.in)
AM_INIT_AUTOMAKE([1.11 foreign parallel-tests])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_PROG_CC
AC_C_CONST
AC_PROG_LIBTOOL

AC_CONFIG_MACRO_DIR([m4])
AX_CFLAGS_WARN_ALL
AX_APPEND_COMPILE_FLAGS([-Wwrite-strings])
AX_GCC_FUNC_ATTRIBUTE([format])

PKG_CHECK_MODULES([XORG_SERVER], [xorg-server >= 0], [],
  [AC_MSG_ERROR([please install xserver-xorg-dev, xorg-x11-server-sdk or xorg-x11-server-devel])])

AC_ARG_ENABLE([strict-locations],
  [AS_HELP_STRING([--enable-strict-locations], [Use standard installation directories])])

if test "x$enable_strict_locations" = "xyes"; then
  moduledir='${libdir}/xorg/modules'
else
  moduledir=`pkg-config xorg-server --variable=moduledir`
  sysconfdir="/etc"
fi

AC_SUBST([moduledir])

AC_CONFIG_FILES([Makefile
                 module/Makefile
                 xnestdev/Makefile
                 xnestkeyb/Makefile
                 xnestmouse/Makefile
])

AC_OUTPUT

