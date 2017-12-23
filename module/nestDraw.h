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

#ifndef __NESTDRAW_H
#define __NESTDRAW_H

#include <xorg-server.h>
#include <xorgVersion.h>
#include <xf86.h>

#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 13, 0, 0, 0)
/* 1.1, 1.2, 1.3, 1.4 1.5, 1.6, 1.7, 1.8, 1.9, 1.10, 1.11, 1.12 */
#define XNEST_CLOSESCR 1
#else
/* 1.13 */
#define XNEST_CLOSESCR 2
#endif

/* true if drawable is window or pixmap is screen */
#define XNEST_DRAWABLE_IS_VISIBLE(_dev, _drw) \
( \
    ( \
        ((_drw)->type == DRAWABLE_WINDOW) && \
        (((WindowPtr)(_drw))->viewable) && \
        ( \
            (_drw)->pScreen->GetScreenPixmap((_drw)->pScreen) == \
            (_drw)->pScreen->GetWindowPixmap((WindowPtr)(_drw)) \
        ) \
    ) || \
    ( \
        ((_drw)->type == DRAWABLE_PIXMAP) && \
        (((PixmapPtr)(_drw))->devPrivate.ptr == (_dev)->pfbMemory) \
    ) \
)

extern GCOps g_nestGCOps; /* in nestGC.c */

extern _X_EXPORT void
nestCopyWindow(WindowPtr pWin, DDXPointRec ptOldOrg, RegionPtr pOldRegion);
#if XNEST_CLOSESCR == 1
extern _X_EXPORT Bool
nestCloseScreen(int index, ScreenPtr pScreen);
#else
extern _X_EXPORT Bool
nestCloseScreen(ScreenPtr pScreen);
#endif
extern _X_EXPORT WindowPtr
nestGetRootWindowPtr(ScreenPtr pScreen);
extern _X_EXPORT nestPtr
nestGetDevFromScreen(ScreenPtr pScreen);

#endif
