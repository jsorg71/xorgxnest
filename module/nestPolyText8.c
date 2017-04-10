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

#include "nest.h"
#include "nestDraw.h"
#include "nestReg.h"
#include "nestPolyText8.h"

#define LOG_LEVEL 1
#define LLOGLN(_level, _args) \
    do { if (_level < LOG_LEVEL) { ErrorF _args ; ErrorF("\n"); } } while (0)

/******************************************************************************/
static int
nestPolyText8Org(DrawablePtr pDrawable, GCPtr pGC,
                 int x, int y, int count, char *chars)
{
    GC_OP_VARS;
    int rv;

    GC_OP_PROLOGUE(pGC);
    rv = pGC->ops->PolyText8(pDrawable, pGC, x, y, count, chars);
    GC_OP_EPILOGUE(pGC);
    return rv;
}

/******************************************************************************/
int
nestPolyText8(DrawablePtr pDrawable, GCPtr pGC,
              int x, int y, int count, char *chars)
{
    int rv;

    LLOGLN(10, ("nestPolyText8:"));
    /* do original call */
    rv = nestPolyText8Org(pDrawable, pGC, x, y, count, chars);
    return rv;
}