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

nest module main

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
#include "nestInput.h"
#include "nestDraw.h"
#include "nestMain.h"

/******************************************************************************/
#define LOG_LEVEL 1
#define LLOGLN(_level, _args) \
    do { if (_level < LOG_LEVEL) { ErrorF _args ; ErrorF("\n"); } } while (0)

static Bool g_initialised = FALSE;

/*****************************************************************************/
static pointer
xorgxnestSetup(pointer Module, pointer Options,
               int *ErrorMajor, int *ErrorMinor)
{
    LLOGLN(0, ("xorgxnestSetup:"));
    if (!g_initialised)
    {
        g_initialised = TRUE;
    }
    nestInputInit();
    nestPrivateInit();
    return (pointer) 1;
}

/*****************************************************************************/
static void
xorgxnestTearDown(pointer Module)
{
    LLOGLN(0, ("xorgxnestTearDown:"));
}

/*****************************************************************************/
void
xorgxnestDownDown(ScreenPtr pScreen)
{
    LLOGLN(0, ("xorgxnestDownDown:"));
    if (g_initialised)
    {
        g_initialised = FALSE;
        LLOGLN(0, ("xorgxnestDownDown: 1"));
    }
}

static MODULESETUPPROTO(xorgxnestSetup);
static XF86ModuleVersionInfo NESTVersRec =
{
    XNEST_MODULE_NAME,
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

_X_EXPORT XF86ModuleData xorgxnestModuleData =
{
    &NESTVersRec,
    xorgxnestSetup,
    xorgxnestTearDown
};
