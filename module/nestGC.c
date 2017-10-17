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

/******************************************************************************/
#define GC_FUNC_VARS nestPtr dev; nestGCPtr priv;

/******************************************************************************/
#define GC_FUNC_PROLOGUE(_pGC) \
    do { \
        dev = nestGetDevFromScreen((_pGC)->pScreen); \
        priv = (nestGCPtr)nestGetGCPrivate(_pGC, dev->privateKeyRecGC); \
        (_pGC)->funcs = priv->funcs; \
        if (priv->ops != 0) \
        { \
            (_pGC)->ops = priv->ops; \
        } \
    } while (0)

/******************************************************************************/
#define GC_FUNC_EPILOGUE(_pGC) \
    do { \
        priv->funcs = (_pGC)->funcs; \
        (_pGC)->funcs = &g_nestGCFuncs; \
        if (priv->ops != 0) \
        { \
            priv->ops = (_pGC)->ops; \
            (_pGC)->ops = &g_nestGCOps; \
        } \
    } while (0)

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
    GC_FUNC_VARS;

    LLOGLN(10, ("nestValidateGC:"));
    GC_FUNC_PROLOGUE(pGC);
    pGC->funcs->ValidateGC(pGC, changes, d);
    priv->ops = pGC->ops;
    GC_FUNC_EPILOGUE(pGC);
}

/******************************************************************************/
static void
nestChangeGC(GCPtr pGC, unsigned long mask)
{
    GC_FUNC_VARS;

    LLOGLN(10, ("nestChangeGC:"));
    GC_FUNC_PROLOGUE(pGC);
    pGC->funcs->ChangeGC(pGC, mask);
    GC_FUNC_EPILOGUE(pGC);
}

/******************************************************************************/
static void
nestCopyGC(GCPtr src, unsigned long mask, GCPtr dst)
{
    GC_FUNC_VARS;

    LLOGLN(10, ("nestCopyGC:"));
    GC_FUNC_PROLOGUE(dst);
    dst->funcs->CopyGC(src, mask, dst);
    GC_FUNC_EPILOGUE(dst);
}

/******************************************************************************/
static void
nestDestroyGC(GCPtr pGC)
{
    GC_FUNC_VARS;

    LLOGLN(10, ("nestDestroyGC:"));
    GC_FUNC_PROLOGUE(pGC);
    pGC->funcs->DestroyGC(pGC);
    GC_FUNC_EPILOGUE(pGC);
}

/******************************************************************************/
static void
nestChangeClip(GCPtr pGC, int type, pointer pValue, int nrects)
{
    GC_FUNC_VARS;

    LLOGLN(10, ("nestChangeClip:"));
    GC_FUNC_PROLOGUE(pGC);
    pGC->funcs->ChangeClip(pGC, type, pValue, nrects);
    GC_FUNC_EPILOGUE(pGC);
}

/******************************************************************************/
static void
nestDestroyClip(GCPtr pGC)
{
    GC_FUNC_VARS;

    LLOGLN(10, ("nestDestroyClip:"));
    GC_FUNC_PROLOGUE(pGC);
    pGC->funcs->DestroyClip(pGC);
    GC_FUNC_EPILOGUE(pGC);
}

/******************************************************************************/
static void
nestCopyClip(GCPtr dst, GCPtr src)
{
    GC_FUNC_VARS;

    LLOGLN(10, ("nestCopyClip:"));
    GC_FUNC_PROLOGUE(dst);
    dst->funcs->CopyClip(dst, src);
    GC_FUNC_EPILOGUE(dst);
}

/*****************************************************************************/
Bool
nestCreateGC(GCPtr pGC)
{
    Bool rv;
    nestPtr dev;
    ScreenPtr pScreen;
    nestGCPtr priv;

    LLOGLN(0, ("nestCreateGC:"));
    pScreen = pGC->pScreen;
    dev = nestGetDevFromScreen(pScreen);
    priv = (nestGCPtr)nestGetGCPrivate(pGC, dev->privateKeyRecGC);
    pScreen->CreateGC = dev->CreateGC;
    rv = pScreen->CreateGC(pGC);
    if (rv)
    {
        priv->funcs = pGC->funcs;
        priv->ops = 0;
        pGC->funcs = &g_nestGCFuncs;
    }
    pScreen->CreateGC = nestCreateGC;
    LLOGLN(0, ("nestCreateGC: out"));
    return rv;
}
