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

to deal with regions changing in xorg versions

*/

#ifndef __NESTREG_H
#define __NESTREG_H

#include <xorg-server.h>
#include <xorgVersion.h>
#include <xf86.h>

extern _X_EXPORT Bool
nestRegionCopy(RegionPtr dst, RegionPtr src);
extern _X_EXPORT void
nestRegionTranslate(RegionPtr pReg, int x, int y);
extern _X_EXPORT Bool
nestRegionNotEmpty(RegionPtr pReg);
extern _X_EXPORT Bool
nestRegionIntersect(RegionPtr newReg, RegionPtr reg1, RegionPtr reg2);
extern _X_EXPORT int
nestRegionContainsRect(RegionPtr region, BoxPtr prect);
extern _X_EXPORT void
nestRegionInit(RegionPtr pReg, BoxPtr rect, int size);
extern _X_EXPORT void
nestRegionUninit(RegionPtr pReg);
extern _X_EXPORT RegionPtr
nestRegionFromRects(int nrects, xRectanglePtr prect, int ctype);
extern _X_EXPORT void
nestRegionDestroy(RegionPtr pReg);
extern _X_EXPORT RegionPtr
nestRegionCreate(BoxPtr rect, int size);
extern _X_EXPORT Bool
nestRegionUnion(RegionPtr newReg, RegionPtr reg1, RegionPtr reg2);
extern _X_EXPORT Bool
nestRegionSubtract(RegionPtr newReg, RegionPtr reg1, RegionPtr reg2);
extern _X_EXPORT Bool
nestRegionInverse(RegionPtr newReg, RegionPtr reg1, BoxPtr invRect);
extern _X_EXPORT BoxPtr
nestRegionExtents(RegionPtr pReg);
extern _X_EXPORT void
nestRegionReset(RegionPtr pReg, BoxPtr pBox);
extern _X_EXPORT Bool
nestRegionBreak(RegionPtr pReg);
extern _X_EXPORT void
nestRegionUnionRect(RegionPtr pReg, BoxPtr prect);
extern _X_EXPORT int
nestRegionPixelCount(RegionPtr pReg);

#endif
