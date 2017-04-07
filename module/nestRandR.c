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

RandR draw calls

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
#include "nestDraw.h"
#include "nestReg.h"
#include "nestMisc.h"
#include "nestRandR.h"

static int g_panning = 0;

/******************************************************************************/
#define LOG_LEVEL 1
#define LLOGLN(_level, _args) \
    do { if (_level < LOG_LEVEL) { ErrorF _args ; ErrorF("\n"); } } while (0)

/******************************************************************************/
Bool
nestRRRegisterSize(ScreenPtr pScreen, int width, int height)
{
    int mmwidth;
    int mmheight;
    RRScreenSizePtr pSize;
    ScrnInfoPtr pScrn;

    LLOGLN(0, ("nestRRRegisterSize: width %d height %d", width, height));
    pScrn = xf86Screens[pScreen->myNum];
    mmwidth = PixelToMM(width, pScrn->xDpi);
    mmheight = PixelToMM(height, pScrn->yDpi);
    pSize = RRRegisterSize(pScreen, width, height, mmwidth, mmheight);
    /* Tell RandR what the current config is */
    RRSetCurrentConfig(pScreen, RR_Rotate_0, 0, pSize);
    return TRUE;
}

/******************************************************************************/
Bool
nestRRSetConfig(ScreenPtr pScreen, Rotation rotateKind, int rate,
                RRScreenSizePtr pSize)
{
    LLOGLN(0, ("nestRRSetConfig:"));
    return TRUE;
}

/******************************************************************************/
Bool
nestRRGetInfo(ScreenPtr pScreen, Rotation *pRotations)
{
    LLOGLN(0, ("nestRRGetInfo:"));
    *pRotations = RR_Rotate_0;
    return TRUE;
}

/******************************************************************************/
Bool
nestRRScreenSetSize(ScreenPtr pScreen, CARD16 width, CARD16 height,
                   CARD32 mmWidth, CARD32 mmHeight)
{
    WindowPtr root;
    PixmapPtr screenPixmap;
    BoxRec box;
    nestPtr dev;

    LLOGLN(0, ("nestRRScreenSetSize: width %d height %d mmWidth %d mmHeight %d",
           width, height, (int)mmWidth, (int)mmHeight));
    dev = nestGetDevFromScreen(pScreen);
    root = nestGetRootWindowPtr(pScreen);
    if ((width < 1) || (height < 1))
    {
        LLOGLN(10, ("  error width %d height %d", width, height));
        return FALSE;
    }
    dev->width = width;
    dev->height = height;
    dev->paddedWidthInBytes = PixmapBytePad(dev->width, dev->depth);
    dev->sizeInBytes = dev->paddedWidthInBytes * dev->height;
    pScreen->width = width;
    pScreen->height = height;
    pScreen->mmWidth = mmWidth;
    pScreen->mmHeight = mmHeight;
    screenPixmap = pScreen->GetScreenPixmap(pScreen);
    free(dev->pfbMemory_alloc);
    dev->pfbMemory_alloc = g_new0(char, dev->sizeInBytes + 16);
    dev->pfbMemory = (char *) NESTALIGN(dev->pfbMemory_alloc, 16);
    if (screenPixmap != 0)
    {
        pScreen->ModifyPixmapHeader(screenPixmap, width, height,
                                    -1, -1,
                                    dev->paddedWidthInBytes,
                                    dev->pfbMemory);
    }
    box.x1 = 0;
    box.y1 = 0;
    box.x2 = width;
    box.y2 = height;
    nestRegionInit(&root->winSize, &box, 1);
    nestRegionInit(&root->borderSize, &box, 1);
    nestRegionReset(&root->borderClip, &box);
    nestRegionBreak(&root->clipList);
    root->drawable.width = width;
    root->drawable.height = height;
    ResizeChildrenWinSize(root, 0, 0, 0, 0);
    RRGetInfo(pScreen, 1);
    LLOGLN(0, ("  screen resized to %dx%d", pScreen->width, pScreen->height));
    RRScreenSizeNotify(pScreen);
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 13, 0, 0, 0)
    xf86EnableDisableFBAccess(pScreen->myNum, FALSE);
    xf86EnableDisableFBAccess(pScreen->myNum, TRUE);
#else
    xf86EnableDisableFBAccess(xf86Screens[pScreen->myNum], FALSE);
    xf86EnableDisableFBAccess(xf86Screens[pScreen->myNum], TRUE);
#endif
    return TRUE;
}

/******************************************************************************/
Bool
nestRRCrtcSet(ScreenPtr pScreen, RRCrtcPtr crtc, RRModePtr mode,
              int x, int y, Rotation rotation, int numOutputs,
              RROutputPtr *outputs)
{
    LLOGLN(0, ("nestRRCrtcSet:"));
    return TRUE;
}

/******************************************************************************/
Bool
nestRRCrtcSetGamma(ScreenPtr pScreen, RRCrtcPtr crtc)
{
    LLOGLN(0, ("nestRRCrtcSetGamma:"));
    return TRUE;
}

/******************************************************************************/
Bool
nestRRCrtcGetGamma(ScreenPtr pScreen, RRCrtcPtr crtc)
{
    LLOGLN(0, ("nestRRCrtcGetGamma: %p %p %p %p", crtc, crtc->gammaRed,
           crtc->gammaBlue, crtc->gammaGreen));
    crtc->gammaSize = 1;
    if (crtc->gammaRed == NULL)
    {
        crtc->gammaRed = g_new0(CARD16, 16);
    }
    if (crtc->gammaBlue == NULL)
    {
        crtc->gammaBlue = g_new0(CARD16, 16);
    }
    if (crtc->gammaGreen == NULL)
    {
        crtc->gammaGreen = g_new0(CARD16, 16);
    }
    return TRUE;
}

/******************************************************************************/
Bool
nestRROutputSetProperty(ScreenPtr pScreen, RROutputPtr output, Atom property,
                       RRPropertyValuePtr value)
{
    LLOGLN(0, ("nestRROutputSetProperty:"));
    return TRUE;
}

/******************************************************************************/
Bool
nestRROutputValidateMode(ScreenPtr pScreen, RROutputPtr output,
                         RRModePtr mode)
{
    LLOGLN(0, ("nestRROutputValidateMode:"));
    return TRUE;
}

/******************************************************************************/
void
nestRRModeDestroy(ScreenPtr pScreen, RRModePtr mode)
{
    LLOGLN(0, ("nestRRModeDestroy:"));
}

/******************************************************************************/
Bool
nestRROutputGetProperty(ScreenPtr pScreen, RROutputPtr output, Atom property)
{
    LLOGLN(0, ("nestRROutputGetProperty:"));
    return TRUE;
}

/******************************************************************************/
Bool
nestRRGetPanning(ScreenPtr pScreen, RRCrtcPtr crtc, BoxPtr totalArea,
                 BoxPtr trackingArea, INT16 *border)
{
    nestPtr dev;
    BoxRec totalAreaRect;
    BoxRec trackingAreaRect;

    LLOGLN(10, ("nestRRGetPanning: totalArea %p trackingArea %p border %p",
                totalArea, trackingArea, border));

    if (!g_panning)
    {
        return FALSE;
    }

    dev = nestGetDevFromScreen(pScreen);

    totalAreaRect.x1 = 0;
    totalAreaRect.y1 = 0;
    totalAreaRect.x2 = dev->width;
    totalAreaRect.y2 = dev->height;

    trackingAreaRect.x1 = 0;
    trackingAreaRect.y1 = 0;
    trackingAreaRect.x2 = dev->width;
    trackingAreaRect.y2 = dev->height;

    if (totalArea != 0)
    {
        *totalArea = totalAreaRect;
    }

    if (trackingArea != 0)
    {
        *trackingArea = trackingAreaRect;
    }

    if (border != 0)
    {
        border[0] = 0;
        border[1] = 0;
        border[2] = 0;
        border[3] = 0;
    }
    return TRUE;
}

/******************************************************************************/
Bool
nestRRSetPanning(ScreenPtr pScreen, RRCrtcPtr crtc, BoxPtr totalArea,
                BoxPtr trackingArea, INT16 *border)
{
    LLOGLN(0, ("nestRRSetPanning:"));
    return TRUE;
}
