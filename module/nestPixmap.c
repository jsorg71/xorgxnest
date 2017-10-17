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
#include <mi.h>

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
    PixmapPtr pPixmap;

    LLOGLN(0, ("nestCreatePixmap: width %d height %d depth %d",
           width, height, depth));
    pPixmap = AllocatePixmap(pScreen, 0);
    if (pPixmap == NULL)
    {
        return NullPixmap;
    }
    pPixmap->drawable.type = DRAWABLE_PIXMAP;
    pPixmap->drawable.class = 0;
    pPixmap->drawable.depth = depth;
    pPixmap->drawable.bitsPerPixel = depth;
    pPixmap->drawable.id = 0;
    pPixmap->drawable.x = 0;
    pPixmap->drawable.y = 0;
    pPixmap->drawable.width = width;
    pPixmap->drawable.height = height;
    pPixmap->drawable.pScreen = pScreen;
    pPixmap->drawable.serialNumber = NEXT_SERIAL_NUMBER;
    pPixmap->refcnt = 1;
    pPixmap->devKind = PixmapBytePad(width, depth);
    pPixmap->usage_hint = usage_hint;
    LLOGLN(0, ("nestCreatePixmap: pPixmap %p", pPixmap));
    return pPixmap;
}

#else

/*****************************************************************************/
PixmapPtr
nestCreatePixmap(ScreenPtr pScreen, int width, int height, int depth)
{
    PixmapPtr pPixmap;

    LLOGLN(0, ("nestCreatePixmap: width %d height %d depth %d",
           width, height, depth));
    pPixmap = AllocatePixmap(pScreen, 0);
    if (pPixmap == NULL)
    {
        return NullPixmap;
    }
    pPixmap->drawable.type = DRAWABLE_PIXMAP;
    pPixmap->drawable.class = 0;
    pPixmap->drawable.depth = depth;
    pPixmap->drawable.bitsPerPixel = depth;
    pPixmap->drawable.id = 0;
    pPixmap->drawable.x = 0;
    pPixmap->drawable.y = 0;
    pPixmap->drawable.width = width;
    pPixmap->drawable.height = height;
    pPixmap->drawable.pScreen = pScreen;
    pPixmap->drawable.serialNumber = NEXT_SERIAL_NUMBER;
    pPixmap->refcnt = 1;
    pPixmap->devKind = PixmapBytePad(width, depth);
    pPixmap->usage_hint = usage_hint;
    LLOGLN(0, ("nestCreatePixmap: pPixmap %p", pPixmap));
    return pPixmap;
}

#endif

/******************************************************************************/
Bool
nestDestroyPixmap(PixmapPtr pPixmap)
{
    LLOGLN(0, ("nestDestroyPixmap: pPixmap %p refcnt %d", pPixmap,
           pPixmap->refcnt));
    pPixmap->refcnt--;
    if (pPixmap->refcnt != 0)
    {
        return TRUE;
    }
    FreePixmap(pPixmap);
    return TRUE;
}

/******************************************************************************/
Bool
nestModifyPixmapHeader(PixmapPtr pPixmap, int width, int height, int depth,
                      int bitsPerPixel, int devKind, pointer pPixData)
{
    Bool rv;
    //ScreenPtr pScreen;
    //nestPtr dev;

    LLOGLN(0, ("nestModifyPixmapHeader: pPixmap %p width %d "
           "height %d depth %d bitsPerPixel %d devKind %d pPixData %p",
           pPixmap, width, height, depth, bitsPerPixel, devKind, pPixData));

    rv = miModifyPixmapHeader(pPixmap, width, height, depth, bitsPerPixel, devKind, pPixData);

    //pScreen = pPixmap->drawable.pScreen;
    //dev = nestGetDevFromScreen(pScreen);
    //pScreen->ModifyPixmapHeader = dev->ModifyPixmapHeader;
    //rv = pScreen->ModifyPixmapHeader(pPixmap, width, height, depth, bitsPerPixel,
    //                                 devKind, pPixData);
    //pScreen->ModifyPixmapHeader = nestModifyPixmapHeader;
    //return rv;

    LLOGLN(0, ("nestModifyPixmapHeader: out"));

    return rv;
}
