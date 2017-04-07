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

#ifndef __NESTCURSOR_H
#define __NESTCURSOR_H

#include <xorg-server.h>
#include <xorgVersion.h>
#include <xf86.h>

extern _X_EXPORT Bool
nestSpriteRealizeCursor(DeviceIntPtr pDev, ScreenPtr pScr, CursorPtr pCurs);
extern _X_EXPORT Bool
nestSpriteUnrealizeCursor(DeviceIntPtr pDev, ScreenPtr pScr, CursorPtr pCurs);
extern _X_EXPORT void
nestSpriteSetCursor(DeviceIntPtr pDev, ScreenPtr pScr, CursorPtr pCurs,
                    int x, int y);
extern _X_EXPORT void
nestSpriteMoveCursor(DeviceIntPtr pDev, ScreenPtr pScr, int x, int y);
extern _X_EXPORT Bool
nestSpriteDeviceCursorInitialize(DeviceIntPtr pDev, ScreenPtr pScr);
extern _X_EXPORT void
nestSpriteDeviceCursorCleanup(DeviceIntPtr pDev, ScreenPtr pScr);

#endif
