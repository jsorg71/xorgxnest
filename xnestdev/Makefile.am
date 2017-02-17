EXTRA_DIST =

AM_CFLAGS = \
  $(XORG_SERVER_CFLAGS) \
  -I$(top_srcdir)/module

xnestdev_drv_la_LTLIBRARIES = xnestdev_drv.la

xnestdev_drv_la_LDFLAGS = -module -avoid-version

xnestdev_drv_ladir = $(moduledir)/drivers

xnestdev_drv_la_SOURCES = xnestdev.c

xnestdev_drv_la_LIBADD =

xnestdevsysconfdir=$(sysconfdir)/X11/xnest

dist_xnestdevsysconf_DATA = \
  xorg.conf

