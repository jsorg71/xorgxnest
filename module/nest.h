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

*/

#ifndef _NEST_H
#define _NEST_H

#include <xorg-server.h>
#include <xorgVersion.h>
#include <xf86.h>

#include <scrnintstr.h>
#include <gcstruct.h>
#include <mipointer.h>
#include <randrstr.h>

#include "nestPri.h"

#define XNEST_MODULE_NAME "XORGXNEST"
#define XNEST_DRIVER_NAME "XNESTDEV"
#define XNEST_MOUSE_NAME "XNESTMOUSE"
#define XNEST_KEYB_NAME "XNESTKEYB"
#define XNEST_VERSION 1000

#define PACKAGE_VERSION_MAJOR 1
#define PACKAGE_VERSION_MINOR 0
#define PACKAGE_VERSION_PATCHLEVEL 0

#define PixelToMM(_size, _dpi) (((_size) * 254 + (_dpi) * 5) / ((_dpi) * 10))

#define NESTMIN(_val1, _val2) ((_val1) < (_val2) ? (_val1) : (_val2))
#define NESTMAX(_val1, _val2) ((_val1) < (_val2) ? (_val2) : (_val1))
#define NESTCLAMP(_val, _lo, _hi) \
    (_val) < (_lo) ? (_lo) : (_val) > (_hi) ? (_hi) : (_val)
#define NESTALIGN(_val, _al) ((((long)(_val)) + ((_al) - 1)) & ~((_al) - 1))

#if 0
#define RegionCopy DONOTUSE
#define RegionTranslate DONOTUSE
#define RegionNotEmpty DONOTUSE
#define RegionIntersect DONOTUSE
#define RegionContainsRect DONOTUSE
#define RegionInit DONOTUSE
#define RegionUninit DONOTUSE
#define RegionFromRects DONOTUSE
#define RegionDestroy DONOTUSE
#define RegionCreate DONOTUSE
#define RegionUnion DONOTUSE
#define RegionSubtract DONOTUSE
#define RegionInverse DONOTUSE
#define RegionExtents DONOTUSE
#define RegionReset DONOTUSE
#define RegionBreak DONOTUSE
#define RegionUnionRect DONOTUSE
#endif

struct _nestCounts
{
    CARD32 nestFillSpansCallCount; /* 1 */
    CARD32 nestSetSpansCallCount;
    CARD32 nestPutImageCallCount;
    CARD32 nestCopyAreaCallCount;
    CARD32 nestCopyPlaneCallCount;
    CARD32 nestPolyPointCallCount;
    CARD32 nestPolylinesCallCount;
    CARD32 nestPolySegmentCallCount;
    CARD32 nestPolyRectangleCallCount;
    CARD32 nestPolyArcCallCount; /* 10 */
    CARD32 nestFillPolygonCallCount;
    CARD32 nestPolyFillRectCallCount;
    CARD32 nestPolyFillArcCallCount;
    CARD32 nestPolyText8CallCount;
    CARD32 nestPolyText16CallCount;
    CARD32 nestImageText8CallCount;
    CARD32 nestImageText16CallCount;
    CARD32 nestImageGlyphBltCallCount;
    CARD32 nestPolyGlyphBltCallCount;
    CARD32 nestPushPixelsCallCount; /* 20 */
    CARD32 nestCompositeCallCount;
    CARD32 nestCopyWindowCallCount; /* 22 */
    CARD32 nestTrapezoidsCallCount;
    CARD32 callCount[64 - 23];
};

/* move this to common header */
struct _nestRec
{
    int width;
    int height;
    int depth;
    int paddedWidthInBytes;
    int sizeInBytes;
    int num_modes;
    int bitsPerPixel;
    int Bpp;
    int Bpp_mask;
    char *pfbMemory_alloc;
    char *pfbMemory;
    ScreenPtr pScreen;
    nestDevPrivateKey privateKeyRecGC;
    nestDevPrivateKey privateKeyRecPixmap;

    CopyWindowProcPtr CopyWindow;
    CreateGCProcPtr CreateGC;
    CreatePixmapProcPtr CreatePixmap;
    DestroyPixmapProcPtr DestroyPixmap;
    ModifyPixmapHeaderProcPtr ModifyPixmapHeader;
    CloseScreenProcPtr CloseScreen;
    CompositeProcPtr Composite;
    GlyphsProcPtr Glyphs;
    TrapezoidsProcPtr Trapezoids;

    /* keyboard and mouse */
    miPointerScreenFuncPtr pCursorFuncs;
    /* mouse */
    nestPointer pointer;
    /* keyboard */
    nestKeyboard keyboard;

    /* RandR */
    RRSetConfigProcPtr rrSetConfig;
    RRGetInfoProcPtr rrGetInfo;
    RRScreenSetSizeProcPtr rrScreenSetSize;
    RRCrtcSetProcPtr rrCrtcSet;
    RRCrtcSetGammaProcPtr rrCrtcSetGamma;
    RRCrtcGetGammaProcPtr rrCrtcGetGamma;
    RROutputSetPropertyProcPtr rrOutputSetProperty;
    RROutputValidateModeProcPtr rrOutputValidateMode;
    RRModeDestroyProcPtr rrModeDestroy;
    RROutputGetPropertyProcPtr rrOutputGetProperty;
    RRGetPanningProcPtr rrGetPanning;
    RRSetPanningProcPtr rrSetPanning;

    nestPixmapRec screenPriv;

    struct _nestCounts counts;

    /* multimon */
    int extra_outputs;
    RRCrtcPtr crtc[16];
    RROutputPtr output[16];
    struct monitor_info minfo[16]; /* client monitor data */
    int doMultimon;
    int monitorCount;

};
typedef struct _nestRec nestRec;
typedef struct _nestRec * nestPtr;
#define XNESTPTR(_p) ((nestPtr)((_p)->driverPrivate))

struct _nestGCRec
{
/* changed to const in d89b42b */
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 15, 99, 901, 0)
    GCFuncs *funcs;
    GCOps *ops;
#else
    const GCFuncs *funcs;
    const GCOps *ops;
#endif
};
typedef struct _nestGCRec nestGCRec;
typedef struct _nestGCRec * nestGCPtr;

#endif
