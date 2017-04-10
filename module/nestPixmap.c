/*
Copyright 2017 Jay Sorg

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

pixmap calls

*/

#if defined(HAVE_CONFIG_H)
#include "config_ac.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* this should be before all X11 .h files */
#include <xorg-server.h>
#include <xorgVersion.h>

/* all driver need this */
#include <xf86.h>
#include <xf86_OSproc.h>

#include "nest.h"
#include "nestDraw.h"
#include "nestPixmap.h"

#ifndef XNEST_PIX
#warning XNEST_PIX not defined
#endif

/******************************************************************************/
#define LOG_LEVEL 1
#define LLOGLN(_level, _args) \
    do { if (_level < LOG_LEVEL) { ErrorF _args ; ErrorF("\n"); } } while (0)

#if XNEST_PIX == 2

/*****************************************************************************/
PixmapPtr
nestCreatePixmap(ScreenPtr pScreen, int width, int height, int depth,
                 unsigned usage_hint)
{
    nestPtr dev;
    PixmapPtr rv;

    LLOGLN(10, ("nestCreatePixmap: width %d height %d depth %d",
           width, height, depth));
    dev = nestGetDevFromScreen(pScreen);
    pScreen->CreatePixmap = dev->CreatePixmap;
    rv = pScreen->CreatePixmap(pScreen, width, height, depth, usage_hint);
    pScreen->CreatePixmap = nestCreatePixmap;
    return rv;
}

#else

/*****************************************************************************/
PixmapPtr
nestCreatePixmap(ScreenPtr pScreen, int width, int height, int depth)
{
    nestPtr dev;
    PixmapPtr rv;

    LLOGLN(10, ("nestCreatePixmap: width %d height %d depth %d",
           width, height, depth));
    dev = nestGetDevFromScreen(pScreen);
    pScreen->CreatePixmap = dev->CreatePixmap;
    rv = pScreen->CreatePixmap(pScreen, width, height, depth);
    pScreen->CreatePixmap = nestCreatePixmap;
    return rv;
}

#endif

/******************************************************************************/
Bool
nestDestroyPixmap(PixmapPtr pPixmap)
{
    Bool rv;
    ScreenPtr pScreen;
    nestPtr dev;

    LLOGLN(10, ("nestDestroyPixmap: refcnt %d", pPixmap->refcnt));
    pScreen = pPixmap->drawable.pScreen;
    dev = nestGetDevFromScreen(pScreen);
    pScreen->DestroyPixmap = dev->DestroyPixmap;
    rv = pScreen->DestroyPixmap(pPixmap);
    pScreen->DestroyPixmap = nestDestroyPixmap;
    return rv;
}

/******************************************************************************/
Bool
nestModifyPixmapHeader(PixmapPtr pPixmap, int width, int height, int depth,
                      int bitsPerPixel, int devKind, pointer pPixData)
{
    Bool rv;
    ScreenPtr pScreen;
    nestPtr dev;

    LLOGLN(10, ("nestModifyPixmapHeader:"));
    pScreen = pPixmap->drawable.pScreen;
    dev = nestGetDevFromScreen(pScreen);
    pScreen->ModifyPixmapHeader = dev->ModifyPixmapHeader;
    rv = pScreen->ModifyPixmapHeader(pPixmap, width, height, depth, bitsPerPixel,
                                     devKind, pPixData);
    pScreen->ModifyPixmapHeader = nestModifyPixmapHeader;
    return rv;
}
