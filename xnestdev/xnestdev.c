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

This is the main driver file

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

#define USE_FB 0

#include <mipointer.h>
#if USE_FB
#include <fb.h>
#endif
#include <micmap.h>
#include <mi.h>

#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 14, 0, 0, 0)
/* including fb.h includes mibstore.h but if we do not use fb.h ... */
#include <mibstore.h>
#endif

#ifdef RANDR
#include <randrstr.h>
#endif

#include <xf86Modes.h>

#include "nest.h"
#include "nestPri.h"
#include "nestDraw.h"
#include "nestGC.h"
#include "nestCursor.h"
#include "nestRandR.h"
#include "nestMisc.h"
#include "nestComposite.h"
#include "nestTrapezoids.h"
#include "nestGlyphs.h"
#include "nestPixmap.h"
#include "nestXv.h"
#include "nestXClient.h"

#define LLOG_LEVEL 1
#define LLOGLN(_level, _args) \
  do \
  { \
    if (_level < LLOG_LEVEL) \
    { \
      ErrorF _args ; \
      ErrorF("\n"); \
    } \
  } \
  while (0)

static int g_setup_done = 0;
static OsTimerPtr g_timer = 0;

static char g_xnest_driver_name[] = XNEST_DRIVER_NAME;

/* Supported "chipsets" */
static SymTabRec g_Chipsets[] =
{
    { 0, XNEST_DRIVER_NAME },
    { -1, 0 }
};

static XF86ModuleVersionInfo g_VersRec =
{
    XNEST_DRIVER_NAME,
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    PACKAGE_VERSION_MAJOR,
    PACKAGE_VERSION_MINOR,
    PACKAGE_VERSION_PATCHLEVEL,
    ABI_CLASS_VIDEODRV,
    ABI_VIDEODRV_VERSION,
    0,
    { 0, 0, 0, 0 }
};

/*****************************************************************************/
static Bool
nestAllocRec(ScrnInfoPtr pScrn)
{
    LLOGLN(10, ("nestAllocRec:"));
    if (pScrn->driverPrivate != 0)
    {
        return TRUE;
    }
    /* xnfcalloc exits if alloc failed */
    pScrn->driverPrivate = xnfcalloc(sizeof(nestRec), 1);
    return TRUE;
}

/*****************************************************************************/
static void
nestFreeRec(ScrnInfoPtr pScrn)
{
    LLOGLN(10, ("nestFreeRec:"));
    if (pScrn->driverPrivate == 0)
    {
        return;
    }
    free(pScrn->driverPrivate);
    pScrn->driverPrivate = 0;
}

/*****************************************************************************/
static Bool
nestPreInit(ScrnInfoPtr pScrn, int flags)
{
    rgb zeros1;
    Gamma zeros2;
    int got_res_match;
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 16, 0, 0, 0)
    char **modename;
#else
    const char **modename;
#endif
    DisplayModePtr mode;
    nestPtr dev;

    LLOGLN(0, ("nestPreInit:"));
    if (flags & PROBE_DETECT)
    {
        return FALSE;
    }
    if (pScrn->numEntities == 0)
    {
        return FALSE;
    }

    nestAllocRec(pScrn);
    dev = XNESTPTR(pScrn);

    dev->width = 800;
    dev->height = 600;

    pScrn->monitor = pScrn->confScreen->monitor;
    pScrn->bitsPerPixel = 32;
    pScrn->virtualX = dev->width;
    pScrn->displayWidth = dev->width;
    pScrn->virtualY = dev->height;
    pScrn->progClock = 1;
    pScrn->rgbBits = 8;
    pScrn->depth = 24;
    pScrn->chipset = g_xnest_driver_name;
    pScrn->currentMode = pScrn->modes;

    pScrn->offset.blue = 0;
    pScrn->offset.green = 8;
    pScrn->offset.red = 16;
    pScrn->mask.blue = ((1 << 8) - 1) << pScrn->offset.blue;
    pScrn->mask.green = ((1 << 8) - 1) << pScrn->offset.green;
    pScrn->mask.red = ((1 << 8) - 1) << pScrn->offset.red;

    if (!xf86SetDepthBpp(pScrn, pScrn->depth, pScrn->bitsPerPixel,
                         pScrn->bitsPerPixel,
                         Support24bppFb | Support32bppFb |
                         SupportConvert32to24 | SupportConvert24to32))
    {
        LLOGLN(0, ("nestPreInit: xf86SetDepthBpp failed"));
        nestFreeRec(pScrn);
        return FALSE;
    }
    xf86PrintDepthBpp(pScrn);
    g_memset(&zeros1, 0, sizeof(zeros1));
    if (!xf86SetWeight(pScrn, zeros1, zeros1))
    {
        LLOGLN(0, ("nestPreInit: xf86SetWeight failed"));
        nestFreeRec(pScrn);
        return FALSE;
    }
    g_memset(&zeros2, 0, sizeof(zeros2));
    if (!xf86SetGamma(pScrn, zeros2))
    {
        LLOGLN(0, ("nestPreInit: xf86SetGamma failed"));
        nestFreeRec(pScrn);
        return FALSE;
    }
    if (!xf86SetDefaultVisual(pScrn, -1))
    {
        LLOGLN(0, ("nestPreInit: xf86SetDefaultVisual failed"));
        nestFreeRec(pScrn);
        return FALSE;
    }
    xf86SetDpi(pScrn, 0, 0);
    if (0 == pScrn->display->modes)
    {
        LLOGLN(0, ("nestPreInit: modes error"));
        nestFreeRec(pScrn);
        return FALSE;
    }

    pScrn->virtualX = pScrn->display->virtualX;
    pScrn->virtualY = pScrn->display->virtualY;

    got_res_match = 0;
    for (modename = pScrn->display->modes; *modename != 0; modename++)
    {
        for (mode = pScrn->monitor->Modes; mode != 0; mode = mode->next)
        {
            LLOGLN(10, ("%s %s", mode->name, *modename));
            if (0 == strcmp(mode->name, *modename))
            {
                break;
            }
        }
        if (0 == mode)
        {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO, "\tmode \"%s\" not found\n",
                       *modename);
            continue;
        }
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "\tmode \"%s\" ok\n", *modename);
        LLOGLN(10, ("%d %d %d %d", mode->HDisplay, dev->width,
               mode->VDisplay, dev->height));
        if ((mode->HDisplay == dev->width) && (mode->VDisplay == dev->height))
        {
            pScrn->virtualX = mode->HDisplay;
            pScrn->virtualY = mode->VDisplay;
            got_res_match = 1;
        }
        if (got_res_match)
        {
            pScrn->modes = xf86DuplicateMode(mode);
            pScrn->modes->next = pScrn->modes;
            pScrn->modes->prev = pScrn->modes;
            dev->num_modes = 1;
            break;
        }
    }
    pScrn->currentMode = pScrn->modes;
    xf86PrintModes(pScrn);
    LLOGLN(10, ("nestPreInit: out fPtr->num_modes %d", dev->num_modes));
    if (!got_res_match)
    {
        LLOGLN(0, ("nestPreInit: could not find screen resolution %dx%d",
               dev->width, dev->height));
        return FALSE;
    }
    return TRUE;
}

/******************************************************************************/
static miPointerSpriteFuncRec g_nestSpritePointerFuncs =
{
    /* these are in nestCursor.c */
    nestSpriteRealizeCursor,
    nestSpriteUnrealizeCursor,
    nestSpriteSetCursor,
    nestSpriteMoveCursor,
    nestSpriteDeviceCursorInitialize,
    nestSpriteDeviceCursorCleanup
};

/******************************************************************************/
static Bool
nestSaveScreen(ScreenPtr pScreen, int on)
{
    LLOGLN(0, ("nestSaveScreen:"));
    return TRUE;
}

/******************************************************************************/
static Bool
nestResizeSession(nestPtr dev, int width, int height)
{
    int mmwidth;
    int mmheight;
    ScrnInfoPtr pScrn;
    Bool ok;

    LLOGLN(0, ("nestResizeSession: width %d height %d", width, height));
    pScrn = xf86Screens[dev->pScreen->myNum];
    mmwidth = PixelToMM(width, pScrn->xDpi);
    mmheight = PixelToMM(height, pScrn->yDpi);

    ok = TRUE;
    if ((dev->width != width) || (dev->height != height))
    {
        LLOGLN(0, ("  calling RRScreenSizeSet"));
        ok = RRScreenSizeSet(dev->pScreen, width, height, mmwidth, mmheight);
        LLOGLN(0, ("  RRScreenSizeSet ok %d", ok));
    }
    return ok;
}

/******************************************************************************/
/* returns error */
static CARD32
nestDeferredRandR(OsTimerPtr timer, CARD32 now, pointer arg)
{
    ScreenPtr pScreen;
    rrScrPrivPtr pRRScrPriv;
    nestPtr dev;
    char *envvar;
    int width;
    int height;

    pScreen = (ScreenPtr) arg;
    dev = nestGetDevFromScreen(pScreen);
    LLOGLN(0, ("nestDeferredRandR:"));
    pRRScrPriv = rrGetScrPriv(pScreen);
    if (pRRScrPriv == 0)
    {
        LLOGLN(0, ("nestDeferredRandR: rrGetScrPriv failed"));
        return 1;
    }

    dev->rrSetConfig          = pRRScrPriv->rrSetConfig;
    dev->rrGetInfo            = pRRScrPriv->rrGetInfo;
    dev->rrScreenSetSize      = pRRScrPriv->rrScreenSetSize;
    dev->rrCrtcSet            = pRRScrPriv->rrCrtcSet;
    dev->rrCrtcSetGamma       = pRRScrPriv->rrCrtcSetGamma;
    dev->rrCrtcGetGamma       = pRRScrPriv->rrCrtcGetGamma;
    dev->rrOutputSetProperty  = pRRScrPriv->rrOutputSetProperty;
    dev->rrOutputValidateMode = pRRScrPriv->rrOutputValidateMode;
    dev->rrModeDestroy        = pRRScrPriv->rrModeDestroy;
    dev->rrOutputGetProperty  = pRRScrPriv->rrOutputGetProperty;
    dev->rrGetPanning         = pRRScrPriv->rrGetPanning;
    dev->rrSetPanning         = pRRScrPriv->rrSetPanning;

    LLOGLN(10, ("  rrSetConfig = %p", dev->rrSetConfig));
    LLOGLN(10, ("  rrGetInfo = %p", dev->rrGetInfo));
    LLOGLN(10, ("  rrScreenSetSize = %p", dev->rrScreenSetSize));
    LLOGLN(10, ("  rrCrtcSet = %p", dev->rrCrtcSet));
    LLOGLN(10, ("  rrCrtcSetGamma = %p", dev->rrCrtcSetGamma));
    LLOGLN(10, ("  rrCrtcGetGamma = %p", dev->rrCrtcGetGamma));
    LLOGLN(10, ("  rrOutputSetProperty = %p", dev->rrOutputSetProperty));
    LLOGLN(10, ("  rrOutputValidateMode = %p", dev->rrOutputValidateMode));
    LLOGLN(10, ("  rrModeDestroy = %p", dev->rrModeDestroy));
    LLOGLN(10, ("  rrOutputGetProperty = %p", dev->rrOutputGetProperty));
    LLOGLN(10, ("  rrGetPanning = %p", dev->rrGetPanning));
    LLOGLN(10, ("  rrSetPanning = %p", dev->rrSetPanning));

    pRRScrPriv->rrSetConfig          = nestRRSetConfig;
    pRRScrPriv->rrGetInfo            = nestRRGetInfo;
    pRRScrPriv->rrScreenSetSize      = nestRRScreenSetSize;
    pRRScrPriv->rrCrtcSet            = nestRRCrtcSet;
    pRRScrPriv->rrCrtcSetGamma       = nestRRCrtcSetGamma;
    pRRScrPriv->rrCrtcGetGamma       = nestRRCrtcGetGamma;
    pRRScrPriv->rrOutputSetProperty  = nestRROutputSetProperty;
    pRRScrPriv->rrOutputValidateMode = nestRROutputValidateMode;
    pRRScrPriv->rrModeDestroy        = nestRRModeDestroy;
    pRRScrPriv->rrOutputGetProperty  = nestRROutputGetProperty;
    pRRScrPriv->rrGetPanning         = nestRRGetPanning;
    pRRScrPriv->rrSetPanning         = nestRRSetPanning;


    nestResizeSession(dev, 1024, 768);

    envvar = getenv("XNEST_START_WIDTH");
    if (envvar != 0)
    {
        width = atoi(envvar);
        if ((width >= 16) && (width < 8192))
        {
            envvar = getenv("XNEST_START_HEIGHT");
            if (envvar != 0)
            {
                height = atoi(envvar);
                if ((height >= 16) && (height < 8192))
                {
                    nestResizeSession(dev, width, height);
                }
            }
        }
    }

    RRScreenSetSizeRange(pScreen, 256, 256, 16 * 1024, 16 * 1024);
    RRTellChanged(pScreen);

    return 0;
}

/******************************************************************************/
static void
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 18, 5, 0, 0)
nestBlockHandler1(pointer blockData, OSTimePtr pTimeout, pointer pReadmask)
#else
nestBlockHandler1(void *blockData, void *pTimeout)
#endif
{
}

/******************************************************************************/
static void
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 18, 5, 0, 0)
nestWakeupHandler1(pointer blockData, int result, pointer pReadmask)
#else
nestWakeupHandler1(void *blockData, int result)
#endif
{
    //nestClientConCheck((ScreenPtr)blockData);
}

/*****************************************************************************/
static Bool
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 13, 0, 0, 0)
nestScreenInit(int scrnIndex, ScreenPtr pScreen, int argc, char **argv)
#else
nestScreenInit(ScreenPtr pScreen, int argc, char **argv)
#endif
{
    ScrnInfoPtr pScrn;
    nestPtr dev;
    VisualPtr vis;
    Bool vis_found;
    PictureScreenPtr ps;

    pScrn = xf86Screens[pScreen->myNum];
    dev = XNESTPTR(pScrn);

    dev->pScreen = pScreen;

    dev->client = nestXClientCreate(NULL);

    miClearVisualTypes();
    miSetVisualTypes(pScrn->depth, miGetDefaultVisualMask(pScrn->depth),
                     pScrn->rgbBits, TrueColor);
    miSetPixmapDepths();
    LLOGLN(0, ("nestScreenInit: virtualX %d virtualY %d rgbBits %d depth %d",
           pScrn->virtualX, pScrn->virtualY, pScrn->rgbBits, pScrn->depth));
    LLOGLN(0, ("nestScreenInit: width %d height %d", dev->width, dev->height));
    dev->depth = pScrn->depth;
    dev->paddedWidthInBytes = PixmapBytePad(dev->width, dev->depth);
    dev->bitsPerPixel = nestBitsPerPixel(dev->depth);
    dev->sizeInBytes = dev->paddedWidthInBytes * dev->height;
#if USE_FB
    LLOGLN(0, ("nestScreenInit: pfbMemory bytes %d", dev->sizeInBytes));
    dev->pfbMemory_alloc = g_new0(char, dev->sizeInBytes + 16);
    dev->pfbMemory = (char *) NESTALIGN(dev->pfbMemory_alloc, 16);
    LLOGLN(0, ("nestScreenInit: pfbMemory %p", dev->pfbMemory));
    if (!fbScreenInit(pScreen, dev->pfbMemory,
                      pScrn->virtualX, pScrn->virtualY,
                      pScrn->xDpi, pScrn->yDpi, pScrn->displayWidth,
                      pScrn->bitsPerPixel))
    {
        LLOGLN(0, ("nestScreenInit: fbScreenInit failed"));
        return FALSE;
    }
#else
    nestXClientSetupScreen(dev);
#endif
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 14, 0, 0, 0)
    /* 1.13 has this function, 1.14 and up does not */
    miInitializeBackingStore(pScreen);
#endif

    /* try to init simd functions */
    //nestSimdInit(pScreen, pScrn);

#if defined(XvExtension) && XvExtension
    /* XVideo */
    if (!nestXvInit(pScreen, pScrn))
    {
        LLOGLN(0, ("nestScreenInit: nestXvInit failed"));
    }
#endif

    LLOGLN(0, ("1"));

    vis = pScreen->visuals + (pScreen->numVisuals - 1);
    while (vis >= pScreen->visuals)
    {
        if ((vis->class | DynamicClass) == DirectColor)
        {
            vis->offsetBlue = pScrn->offset.blue;
            vis->blueMask = pScrn->mask.blue;
            vis->offsetGreen = pScrn->offset.green;
            vis->greenMask = pScrn->mask.green;
            vis->offsetRed = pScrn->offset.red;
            vis->redMask = pScrn->mask.red;
        }
        vis--;
    }
#if USE_FB
    fbPictureInit(pScreen, 0, 0);
#endif

    LLOGLN(0, ("2"));

    xf86SetBlackWhitePixels(pScreen);
    xf86SetBackingStore(pScreen);

    LLOGLN(0, ("3"));

#if 1
    /* hardware cursor */
    dev->pCursorFuncs = xf86GetPointerScreenFuncs();
    miPointerInitialize(pScreen, &g_nestSpritePointerFuncs,
                        dev->pCursorFuncs, 0);
#else
    /* software cursor */
    dev->pCursorFuncs = xf86GetPointerScreenFuncs();
    miDCInitialize(pScreen, dev->pCursorFuncs);
#endif
#if USE_FB
    fbCreateDefColormap(pScreen);
#endif
    /* must assign this one */
    pScreen->SaveScreen = nestSaveScreen;

    vis_found = FALSE;
    vis = pScreen->visuals + (pScreen->numVisuals - 1);
    while (vis >= pScreen->visuals)
    {
        if (vis->vid == pScreen->rootVisual)
        {
            vis_found = TRUE;
        }
        vis--;
    }
    if (!vis_found)
    {
        LLOGLN(0, ("nestScreenInit: no root visual"));
        return FALSE;
    }

    dev->privateKeyRecGC = nestAllocateGCPrivate(pScreen, sizeof(nestGCRec));
    dev->privateKeyRecPixmap = nestAllocatePixmapPrivate(pScreen, sizeof(nestPixmapRec));

    dev->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = nestCloseScreen;

    dev->CopyWindow = pScreen->CopyWindow;
    pScreen->CopyWindow = nestCopyWindow;

    dev->CreateGC = pScreen->CreateGC;
    pScreen->CreateGC = nestCreateGC;

    dev->CreatePixmap = pScreen->CreatePixmap;
    pScreen->CreatePixmap = nestCreatePixmap;

    dev->DestroyPixmap = pScreen->DestroyPixmap;
    pScreen->DestroyPixmap = nestDestroyPixmap;

    dev->ModifyPixmapHeader = pScreen->ModifyPixmapHeader;
    pScreen->ModifyPixmapHeader = nestModifyPixmapHeader;

    ps = GetPictureScreenIfSet(pScreen);
    if (ps != 0)
    {
        /* composite */
        dev->Composite = ps->Composite;
        ps->Composite = nestComposite;
        /* glyphs */
        dev->Glyphs = ps->Glyphs;
        ps->Glyphs = nestGlyphs;
        /* trapezoids */
        dev->Trapezoids = ps->Trapezoids;
        ps->Trapezoids = nestTrapezoids;
    }

    RegisterBlockAndWakeupHandlers(nestBlockHandler1, nestWakeupHandler1, pScreen);

    g_timer = TimerSet(g_timer, 0, 10, nestDeferredRandR, pScreen);

    //if (nestClientConInit(dev) != 0)
    //{
    //    LLOGLN(0, ("nestScreenInit: nestClientConInit failed"));
    //}

    dev->Bpp_mask = 0x00FFFFFF;
    dev->Bpp = 4;
    dev->bitsPerPixel = 32;

    LLOGLN(0, ("nestScreenInit: out"));
    return TRUE;
}

/*****************************************************************************/
static Bool
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 13, 0, 0, 0)
nestSwitchMode(int a, DisplayModePtr b, int c)
#else
nestSwitchMode(ScrnInfoPtr a, DisplayModePtr b)
#endif
{
    LLOGLN(0, ("nestSwitchMode:"));
    return TRUE;
}

/*****************************************************************************/
static void
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 13, 0, 0, 0)
nestAdjustFrame(int a, int b, int c, int d)
#else
nestAdjustFrame(ScrnInfoPtr a, int b, int c)
#endif
{
    LLOGLN(10, ("nestAdjustFrame:"));
}

/*****************************************************************************/
static Bool
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 13, 0, 0, 0)
nestEnterVT(int a, int b)
#else
nestEnterVT(ScrnInfoPtr a)
#endif
{
    LLOGLN(0, ("nestEnterVT:"));
    return TRUE;
}

/*****************************************************************************/
static void
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 13, 0, 0, 0)
nestLeaveVT(int a, int b)
#else
nestLeaveVT(ScrnInfoPtr a)
#endif
{
    LLOGLN(0, ("nestLeaveVT:"));
}

/*****************************************************************************/
static ModeStatus
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 13, 0, 0, 0)
nestValidMode(int a, DisplayModePtr b, Bool c, int d)
#else
nestValidMode(ScrnInfoPtr a, DisplayModePtr b, Bool c, int d)
#endif
{
    LLOGLN(0, ("nestValidMode:"));
    return 0;
}

/*****************************************************************************/
static void
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 13, 0, 0, 0)
nestFreeScreen(int a, int b)
#else
nestFreeScreen(ScrnInfoPtr a)
#endif
{
    LLOGLN(0, ("nestFreeScreen:"));
}

/*****************************************************************************/
static Bool
nestProbe(DriverPtr drv, int flags)
{
    int num_dev_sections;
    int i;
    int entity;
    GDevPtr *dev_sections;
    Bool found_screen;
    ScrnInfoPtr pscrn;

    LLOGLN(0, ("nestProbe:"));
    if (flags & PROBE_DETECT)
    {
        return FALSE;
    }
#if USE_FB
    /* fbScreenInit, fbPictureInit, ... */
    if (!xf86LoadDrvSubModule(drv, "fb"))
    {
        LLOGLN(0, ("nestProbe: xf86LoadDrvSubModule for fb failed"));
        return FALSE;
    }
#endif
    num_dev_sections = xf86MatchDevice(XNEST_DRIVER_NAME, &dev_sections);
    if (num_dev_sections <= 0)
    {
        LLOGLN(0, ("nestProbe: xf86MatchDevice failed"));
        return FALSE;
    }

    pscrn = 0;
    found_screen = FALSE;
    for (i = 0; i < num_dev_sections; i++)
    {
        entity = xf86ClaimFbSlot(drv, 0, dev_sections[i], 1);
        pscrn = xf86ConfigFbEntity(pscrn, 0, entity, 0, 0, 0, 0);
        if (pscrn)
        {
            LLOGLN(10, ("nestProbe: found screen"));
            found_screen = 1;
            pscrn->driverVersion = XNEST_VERSION;
            pscrn->driverName    = g_xnest_driver_name;
            pscrn->name          = g_xnest_driver_name;
            pscrn->Probe         = nestProbe;
            pscrn->PreInit       = nestPreInit;
            pscrn->ScreenInit    = nestScreenInit;
            pscrn->SwitchMode    = nestSwitchMode;
            pscrn->AdjustFrame   = nestAdjustFrame;
            pscrn->EnterVT       = nestEnterVT;
            pscrn->LeaveVT       = nestLeaveVT;
            pscrn->ValidMode     = nestValidMode;
            pscrn->FreeScreen    = nestFreeScreen;
            xf86DrvMsg(pscrn->scrnIndex, X_INFO, "%s", "using default device\n");
        }
    }
    free(dev_sections);
    return found_screen;
}

/*****************************************************************************/
static const OptionInfoRec *
nestAvailableOptions(int chipid, int busid)
{
    LLOGLN(0, ("nestAvailableOptions:"));
    return 0;
}

#ifndef HW_SKIP_CONSOLE
#define HW_SKIP_CONSOLE 4
#endif

/*****************************************************************************/
static Bool
nestDriverFunc(ScrnInfoPtr pScrn, xorgDriverFuncOp op, pointer ptr)
{
    xorgHWFlags *flags;
    int rv;

    rv = FALSE;
    LLOGLN(0, ("nestDriverFunc: op %d", (int)op));
    if (op == GET_REQUIRED_HW_INTERFACES)
    {
        flags = (xorgHWFlags *) ptr;
        *flags = HW_SKIP_CONSOLE;
        rv = TRUE;
    }
    return rv;
}

/*****************************************************************************/
static void
nestIdentify(int flags)
{
    LLOGLN(0, ("nestIdentify:"));
    xf86PrintChipsets(XNEST_DRIVER_NAME, "driver for xnest", g_Chipsets);
}

/*****************************************************************************/
_X_EXPORT DriverRec g_DriverRec =
{
    XNEST_VERSION,
    g_xnest_driver_name,
    nestIdentify,
    nestProbe,
    nestAvailableOptions,
    0,
    0,
    nestDriverFunc
};

/*****************************************************************************/
static pointer
xnestdevSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
    LLOGLN(0, ("xnestdevSetup:"));
    if (!g_setup_done)
    {
        g_setup_done = 1;
        xf86AddDriver(&g_DriverRec, module, HaveDriverFuncs);
        return (pointer)1;
    }
    else
    {
        if (errmaj != 0)
        {
            *errmaj = LDR_ONCEONLY;
        }
        return 0;
    }
}

/*****************************************************************************/
static void
xnestdevTearDown(pointer Module)
{
    LLOGLN(0, ("xnestdevTearDown:"));
}

/* <drivername>ModuleData */
_X_EXPORT XF86ModuleData xnestdevModuleData =
{
    &g_VersRec,
    xnestdevSetup,
    xnestdevTearDown
};
