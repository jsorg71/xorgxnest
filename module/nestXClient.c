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

X11 client calls
This should be the one and only file that calls Xlib functions

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
#include <mi.h>

#include "nest.h"
#include "nestXClient.h"
#include "nestMisc.h"

#include "Xnest.h"

#define LOG_LEVEL 1
#define LLOGLN(_level, _args) \
    do { if (_level < LOG_LEVEL) { ErrorF _args ; ErrorF("\n"); } } while (0)

#define MAXDEPTH 32
#define MAXVISUALSPERDEPTH 256
#define UNDEFINED -1
#define MAXCMAPS 1
#define MINCMAPS 1

struct _nestXClientRec
{
    Display *display;
    int screen;
    Visual *visual;
    VisualID visualid;
    int xnestNumVisuals;
    XVisualInfo *xnestVisuals;
    int xnestDefaultVisualIndex;
    Pixel blackPixel;
    Pixel whitePixel;
};

/******************************************************************************/
nestXClientPtr
nestXClientCreate(const char *diplay_name)
{
    nestXClientPtr client;

    client = g_new0(nestXClientRec, 1);
    if (client == NULL)
    {
        return NULL;
    }
    client->display = XOpenDisplay(diplay_name);
    if (client->display == NULL)
    {
        LLOGLN(0, ("nestXClientCreate: connected to %s failed",
               XDisplayName(diplay_name)));
        free(client);
        return NULL;
    }
    LLOGLN(0, ("nestXClientCreate: connected to %s ok",
           XDisplayName(diplay_name)));
    return client;
}

/******************************************************************************/
int
nestXClientDestroy(nestXClientPtr client)
{
    if (client == NULL)
    {
        return 0;
    }
    XFree(client->xnestVisuals);
    XCloseDisplay(client->display);
    free(client);
    return 0;
}

/******************************************************************************/
static int
offset(unsigned long mask)
{
    int count;

    for (count = 0; (!(mask & 1)) && (count < 32); count++)
    {
        mask >>= 1;
    }
    return count;
}

/******************************************************************************/
int
nestXClientSetupScreen(nestPtr dev)
{
    nestXClientPtr client;
    XVisualInfo vi;
    XVisualInfo *cvi;
    long mask;
    VisualPtr visuals;
    VisualPtr svi;
    int numVisuals;
    int index;
    int jndex;
    int rootDepth;
    DepthPtr depths;
    int numDepths;
    int depthIndex;
    VisualID defaultVisual;

    client = dev->client;
    client->screen = XDefaultScreen(client->display);
    client->visual = XDefaultVisual(client->display, client->screen);
    client->visualid = XVisualIDFromVisual(client->visual);
    client->blackPixel = XBlackPixel(client->display, client->screen);
    client->whitePixel = XWhitePixel(client->display, client->screen);
    memset(&vi, 0, sizeof(vi));
    vi.screen = client->screen;
    mask = VisualScreenMask;
    client->xnestVisuals = XGetVisualInfo(client->display, mask, &vi,
                                          &(client->xnestNumVisuals));
    if (client->xnestVisuals == NULL)
    {
        LLOGLN(0, ("nestXClientSetupScreen: error client xnestVisuals is nil"));
        return 1;
    }
    LLOGLN(0, ("nestXClientSetupScreen: xnestNumVisuals %d "
           "sizeof(XVisualInfo) %zu sizeof(VisualID) %zu",
           client->xnestNumVisuals, sizeof(XVisualInfo), sizeof(VisualID)));
    depths = g_new0(DepthRec, MAXDEPTH);
    depths[0].depth = 1;
    depths[0].numVids = 0;
    depths[0].vids = g_new0(VisualID, MAXVISUALSPERDEPTH);
    numDepths = 1;
    visuals = g_new0(VisualRec, client->xnestNumVisuals);
    numVisuals = client->xnestNumVisuals;
    for (index = 0; index < client->xnestNumVisuals; index++)
    {
        svi = visuals + index;
        cvi = client->xnestVisuals + index;
        LLOGLN(0, ("nestXClientSetupScreen: bits_per_rgb %d depth %d screen %d",
               cvi->bits_per_rgb, cvi->depth, cvi->screen));
        svi->class = cvi->class;
        svi->bitsPerRGBValue = cvi->bits_per_rgb;
        svi->ColormapEntries = cvi->colormap_size;
        svi->nplanes = cvi->depth;
        svi->redMask = cvi->red_mask;
        svi->greenMask = cvi->green_mask;
        svi->blueMask = cvi->blue_mask;
        svi->offsetRed = offset(cvi->red_mask);
        svi->offsetGreen = offset(cvi->green_mask);
        svi->offsetBlue = offset(cvi->blue_mask);
        svi->vid = FakeClientID(0);
        depthIndex = UNDEFINED;
        for (jndex = 0; jndex < numDepths; jndex++)
        {
            if (depths[jndex].depth == cvi->depth)
            {
                depthIndex = jndex;
                break;
            }
        }
        if (depthIndex == UNDEFINED)
        {
            depthIndex = numDepths;
            depths[depthIndex].depth = cvi->depth;
            depths[depthIndex].numVids = 0;
            depths[depthIndex].vids = g_new0(VisualID, MAXVISUALSPERDEPTH);
            numDepths++;
        }
        if (depths[depthIndex].numVids >= MAXVISUALSPERDEPTH)
        {
            FatalError("Visual table overflow");
        }
        depths[depthIndex].vids[depths[depthIndex].numVids] =
                visuals[index].vid;
        depths[depthIndex].numVids++;
        if (cvi->visualid == client->visualid)
        {
            LLOGLN(0, ("nestXClientSetupScreen: found default visual at "
                   "index %d visualid %d", index, (int)(client->visualid)));
            client->xnestDefaultVisualIndex = index;
        }
    }
    LLOGLN(0, ("nestXClientSetupScreen: numVisuals %d numDepths %d",
           numVisuals, numDepths));
    defaultVisual = visuals[client->xnestDefaultVisualIndex].vid;
    rootDepth = visuals[client->xnestDefaultVisualIndex].nplanes;
    LLOGLN(0, ("nestXClientSetupScreen: defaultVisual %d rootDepth %d",
           defaultVisual, rootDepth));
    miScreenInit(dev->pScreen, NULL, dev->width, dev->height, 1, 1,
                 dev->width, rootDepth, numDepths, depths,
                 defaultVisual, /* root visual */
                 numVisuals, visuals);

    dev->pScreen->defColormap = (Colormap) FakeClientID(0);
    dev->pScreen->minInstalledCmaps = MINCMAPS;
    dev->pScreen->maxInstalledCmaps = MAXCMAPS;
    dev->pScreen->backingStoreSupport = NotUseful;
    dev->pScreen->saveUnderSupport = NotUseful;
    dev->pScreen->whitePixel = client->whitePixel;
    dev->pScreen->blackPixel = client->blackPixel;

    return 0;
}
