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

GC related calls

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

#include <mipointer.h>
#include <fb.h>
#include <micmap.h>
#include <mi.h>

#include "nest.h"
#include "nestFillSpans.h"
#include "nestSetSpans.h"
#include "nestPutImage.h"
#include "nestCopyArea.h"
#include "nestCopyPlane.h"
#include "nestPolyPoint.h"
#include "nestPolylines.h"
#include "nestPolySegment.h"
#include "nestPolyRectangle.h"
#include "nestPolyArc.h"
#include "nestFillPolygon.h"
#include "nestPolyFillRect.h"
#include "nestPolyFillArc.h"
#include "nestPolyText8.h"
#include "nestPolyText16.h"
#include "nestImageText8.h"
#include "nestImageText16.h"
#include "nestImageGlyphBlt.h"
#include "nestPolyGlyphBlt.h"
#include "nestPushPixels.h"
#include "nestDraw.h"
#include "nestGC.h"

/******************************************************************************/
#define LOG_LEVEL 1
#define LLOGLN(_level, _args) \
    do { if (_level < LOG_LEVEL) { ErrorF _args ; ErrorF("\n"); } } while (0)

static void
nestValidateGC(GCPtr pGC, unsigned long changes, DrawablePtr d);
static void
nestChangeGC(GCPtr pGC, unsigned long mask);
static void
nestCopyGC(GCPtr src, unsigned long mask, GCPtr dst);
static void
nestDestroyGC(GCPtr pGC);
static void
nestChangeClip(GCPtr pGC, int type, pointer pValue, int nrects);
static void
nestDestroyClip(GCPtr pGC);
static void
nestCopyClip(GCPtr dst, GCPtr src);

GCFuncs g_nestGCFuncs =
{
    nestValidateGC, nestChangeGC, nestCopyGC, nestDestroyGC, nestChangeClip,
    nestDestroyClip, nestCopyClip
};

GCOps g_nestGCOps =
{
    nestFillSpans, nestSetSpans, nestPutImage, nestCopyArea, nestCopyPlane,
    nestPolyPoint, nestPolylines, nestPolySegment, nestPolyRectangle,
    nestPolyArc, nestFillPolygon, nestPolyFillRect, nestPolyFillArc,
    nestPolyText8, nestPolyText16, nestImageText8, nestImageText16,
    nestImageGlyphBlt, nestPolyGlyphBlt, nestPushPixels
};

/******************************************************************************/
static void
nestValidateGC(GCPtr pGC, unsigned long changes, DrawablePtr d)
{
    LLOGLN(0, ("nestValidateGC:"));
}

/******************************************************************************/
static void
nestChangeGC(GCPtr pGC, unsigned long mask)
{
    LLOGLN(0, ("nestChangeGC:"));
}

/******************************************************************************/
static void
nestCopyGC(GCPtr src, unsigned long mask, GCPtr dst)
{
    LLOGLN(0, ("nestCopyGC:"));
}

/******************************************************************************/
static void
nestDestroyGC(GCPtr pGC)
{
    LLOGLN(0, ("nestDestroyGC:"));
}

/******************************************************************************/
static void
nestChangeClip(GCPtr pGC, int type, pointer pValue, int nrects)
{
    LLOGLN(0, ("nestChangeClip:"));
}

/******************************************************************************/
static void
nestDestroyClip(GCPtr pGC)
{
    LLOGLN(0, ("nestDestroyClip:"));
}

/******************************************************************************/
static void
nestCopyClip(GCPtr dst, GCPtr src)
{
    LLOGLN(0, ("nestCopyClip:"));
}

/*****************************************************************************/
Bool
nestCreateGC(GCPtr pGC)
{
    nestPtr dev;
    nestGCPtr priv;

    LLOGLN(0, ("nestCreateGC:"));
    LLOGLN(0, ("nestCreateGC: pGC %p pScreen %p", pGC, pGC->pScreen));
    if (pGC->pScreen == NULL)
    {
        return FALSE;
    }
    dev = nestGetDevFromScreen(pGC->pScreen);
    if (dev == NULL)
    {
        return FALSE;
    }
    priv = nestGetGCPrivate(pGC, dev->privateKeyRecGC);
    if (priv == NULL)
    {
        return FALSE;
    }
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 16, 99, 901, 0)
    pGC->clientClipType = CT_NONE;
#endif
    pGC->clientClip = NULL;
    pGC->funcs = &g_nestGCFuncs;
    pGC->ops = &g_nestGCOps;
    pGC->miTranslate = 1;
    priv->gc = 0;
    priv->nClipRects = 0;
    LLOGLN(0, ("nestCreateGC: ok"));
    return TRUE;
}

/*****************************************************************************/
void
nestQueryBestSize(int class, unsigned short *pWidth, unsigned short *pHeight,
                  ScreenPtr pScreen)
{
    unsigned int width;
    unsigned int height;

    LLOGLN(0, ("nestQueryBestSize:"));
    LLOGLN(0, ("nestQueryBestSize: width %d height %d", *pWidth, *pHeight));
    width = *pWidth;
    height = *pHeight;

    //XQueryBestSize(xnestDisplay, class,
    //       xnestDefaultWindows[pScreen->myNum],
    //       width, height, &width, &height);

    *pWidth = width;
    *pHeight = height;
}
