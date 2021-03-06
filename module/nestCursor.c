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

cursor

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
#include <cursor.h>
#include <cursorstr.h>

#include <X11/Xarch.h>

#include "nest.h"
#include "nestDraw.h"
#include "nestCursor.h"

/******************************************************************************/
#define LOG_LEVEL 1
#define LLOGLN(_level, _args) \
    do { if (_level < LOG_LEVEL) { ErrorF _args ; ErrorF("\n"); } } while (0)

/******************************************************************************/
Bool
nestSpriteRealizeCursor(DeviceIntPtr pDev, ScreenPtr pScr, CursorPtr pCurs)
{
    LLOGLN(10, ("nestSpriteRealizeCursor:"));
    return TRUE;
}

/******************************************************************************/
Bool
nestSpriteUnrealizeCursor(DeviceIntPtr pDev, ScreenPtr pScr, CursorPtr pCurs)
{
    LLOGLN(10, ("nestSpriteUnrealizeCursor:"));
    return TRUE;
}

/******************************************************************************/
void
nestSpriteSetCursor(DeviceIntPtr pDev, ScreenPtr pScr, CursorPtr pCurs,
                    int x, int y)
{
    LLOGLN(10, ("nestSpriteSetCursor:"));
}

/******************************************************************************/
void
nestSpriteMoveCursor(DeviceIntPtr pDev, ScreenPtr pScr, int x, int y)
{
    LLOGLN(10, ("nestSpriteMoveCursor:"));
}

/******************************************************************************/
Bool
nestSpriteDeviceCursorInitialize(DeviceIntPtr pDev, ScreenPtr pScr)
{
    LLOGLN(10, ("nestSpriteDeviceCursorInitialize:"));
    return TRUE;
}

/******************************************************************************/
void
nestSpriteDeviceCursorCleanup(DeviceIntPtr pDev, ScreenPtr pScr)
{
    LLOGLN(10, ("nestSpriteDeviceCursorCleanup:"));
}
