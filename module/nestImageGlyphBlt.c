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
#include "nestImageGlyphBlt.h"

#define LOG_LEVEL 1
#define LLOGLN(_level, _args) \
    do { if (_level < LOG_LEVEL) { ErrorF _args ; ErrorF("\n"); } } while (0)

/******************************************************************************/
static void
nestImageGlyphBltOrg(DrawablePtr pDrawable, GCPtr pGC,
                     int x, int y, unsigned int nglyph,
                     CharInfoPtr *ppci, pointer pglyphBase)
{
    GC_OP_VARS;

    GC_OP_PROLOGUE(pGC);
    pGC->ops->ImageGlyphBlt(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase);
    GC_OP_EPILOGUE(pGC);
}

/******************************************************************************/
void
nestImageGlyphBlt(DrawablePtr pDrawable, GCPtr pGC,
                  int x, int y, unsigned int nglyph,
                  CharInfoPtr *ppci, pointer pglyphBase)
{
    LLOGLN(0, ("nestImageGlyphBlt:"));
    /* do original call */
    nestImageGlyphBltOrg(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase);
}
