EXTRA_DIST = nasm_lt.sh

EXTRA_SOURCES =
EXTRA_FLAGS =

AM_CFLAGS = \
  $(XORG_SERVER_CFLAGS) \
  -I$(top_srcdir)/module \
  $(EXTRA_FLAGS)

noinst_HEADERS = \
  nestComposite.h \
  nestCopyArea.h \
  nestCopyPlane.h \
  nestCursor.h \
  nestDraw.h \
  nestFillPolygon.h \
  nestFillSpans.h \
  nestGC.h \
  nestGlyphs.h \
  nest.h \
  nestImageGlyphBlt.h \
  nestImageText16.h \
  nestImageText8.h \
  nestInput.h \
  nestMain.h \
  nestMisc.h \
  nestPixmap.h \
  nestPolyArc.h \
  nestPolyFillArc.h \
  nestPolyFillRect.h \
  nestPolyGlyphBlt.h \
  nestPolylines.h \
  nestPolyPoint.h \
  nestPolyRectangle.h \
  nestPolySegment.h \
  nestPolyText16.h \
  nestPolyText8.h \
  nestPri.h \
  nestPushPixels.h \
  nestPutImage.h \
  nestRandR.h \
  nestReg.h \
  nestSetSpans.h \
  nestTrapezoids.h \
  nestXv.h \
  nestXClient.h

libxorgxnest_la_LTLIBRARIES = libxorgxnest.la

libxorgxnest_la_LDFLAGS = -module -avoid-version -lX11

libxorgxnest_ladir = $(moduledir)

libxorgxnest_la_SOURCES = nestDraw.c nestPri.c nestGC.c nestFillSpans.c \
nestSetSpans.c nestPutImage.c nestCopyArea.c nestCopyPlane.c nestPolyPoint.c \
nestPolylines.c nestPolySegment.c nestPolyRectangle.c nestPolyArc.c \
nestFillPolygon.c nestPolyFillRect.c nestPolyFillArc.c nestPolyText8.c \
nestPolyText16.c nestImageText8.c nestImageText16.c nestImageGlyphBlt.c \
nestPolyGlyphBlt.c nestPushPixels.c nestCursor.c nestMain.c nestRandR.c \
nestMisc.c nestReg.c nestComposite.c nestGlyphs.c nestPixmap.c nestInput.c \
nestTrapezoids.c nestXv.c nestXClient.c \
$(EXTRA_SOURCES)

libxorgxnest_la_LIBADD =
