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

#ifndef __NESTRANDR_H
#define __NESTRANDR_H

#include <xorg-server.h>
#include <xorgVersion.h>
#include <xf86.h>

extern _X_EXPORT Bool
nestRRRegisterSize(ScreenPtr pScreen, int width, int height);
extern _X_EXPORT Bool
nestRRGetInfo(ScreenPtr pScreen, Rotation* pRotations);
extern _X_EXPORT Bool
nestRRSetConfig(ScreenPtr pScreen, Rotation rotateKind, int rate,
                RRScreenSizePtr pSize);
extern _X_EXPORT Bool
nestRRScreenSetSize(ScreenPtr pScreen, CARD16 width, CARD16 height,
                    CARD32 mmWidth, CARD32 mmHeight);
extern _X_EXPORT Bool
nestRRCrtcSet(ScreenPtr pScreen, RRCrtcPtr crtc, RRModePtr mode,
              int x, int y, Rotation rotation, int numOutputs,
              RROutputPtr* outputs);
extern _X_EXPORT Bool
nestRRCrtcSetGamma(ScreenPtr pScreen, RRCrtcPtr crtc);
extern _X_EXPORT Bool
nestRRCrtcGetGamma(ScreenPtr pScreen, RRCrtcPtr crtc);
extern _X_EXPORT Bool
nestRROutputSetProperty(ScreenPtr pScreen, RROutputPtr output, Atom property,
                        RRPropertyValuePtr value);
extern _X_EXPORT Bool
nestRROutputValidateMode(ScreenPtr pScreen, RROutputPtr output,
                         RRModePtr mode);
extern _X_EXPORT void
nestRRModeDestroy(ScreenPtr pScreen, RRModePtr mode);
extern _X_EXPORT Bool
nestRROutputGetProperty(ScreenPtr pScreen, RROutputPtr output, Atom property);
extern _X_EXPORT Bool
nestRRGetPanning(ScreenPtr pScrn, RRCrtcPtr crtc, BoxPtr totalArea,
                 BoxPtr trackingArea, INT16* border);
extern _X_EXPORT Bool
nestRRSetPanning(ScreenPtr pScrn, RRCrtcPtr crtc, BoxPtr totalArea,
                 BoxPtr trackingArea, INT16* border);

#endif
