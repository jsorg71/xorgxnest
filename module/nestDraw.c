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

misc draw calls

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
#include <dixfontstr.h>

#include "nest.h"
#include "nestDraw.h"
#include "nestMain.h"

#define LOG_LEVEL 1
#define LLOGLN(_level, _args) \
    do { if (_level < LOG_LEVEL) { ErrorF _args ; ErrorF("\n"); } } while (0)

#if !defined(XORG_VERSION_CURRENT)
#warning XORG_VERSION_CURRENT not defined
#endif

/*****************************************************************************/
void
nestCopyWindow(WindowPtr pWin, DDXPointRec ptOldOrg, RegionPtr pOldRegion)
{
}

#if XNEST_CLOSESCR == 1 /* before v1.13 */

/*****************************************************************************/
Bool
nestCloseScreen(int index, ScreenPtr pScreen)
{
    LLOGLN(0, ("nestCloseScreen:"));
    xorgxnestDownDown(pScreen);
    return TRUE;
}

#else

/*****************************************************************************/
Bool
nestCloseScreen(ScreenPtr pScreen)
{
    LLOGLN(0, ("nestCloseScreen:"));
    xorgxnestDownDown(pScreen);
    return TRUE;
}

#endif

/******************************************************************************/
WindowPtr
nestGetRootWindowPtr(ScreenPtr pScreen)
{
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 9, 0, 0, 0)
    return WindowTable[pScreen->myNum]; /* in globals.c */
#else
    return pScreen->root;
#endif
}

/******************************************************************************/
nestPtr
nestGetDevFromScreen(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn;
    nestPtr dev;

    if (pScreen == NULL)
    {
        pScrn = xf86Screens[0];
    }
    else
    {
        pScrn = xf86Screens[pScreen->myNum];
    }
    dev = XNESTPTR(pScrn);
    return dev;
}
