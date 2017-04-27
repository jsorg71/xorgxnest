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

#include <X11/X.h>
#include <X11/Xlib.h>

#include "nestXClient.h"

void
ErrorF(const char * f, ...);

#define LOG_LEVEL 1
#define LLOGLN(_level, _args) \
    do { if (_level < LOG_LEVEL) { ErrorF _args ; ErrorF("\n"); } } while (0)

/******************************************************************************/
nestXClientPtr
nestXClientCreate(const char *diplay_name)
{
    nestXClientPtr client;

    client = (nestXClientPtr) calloc(1, sizeof(nestXClientRec));
    if (client == NULL)
    {
        return NULL;
    }
    client->display = (void *) XOpenDisplay(diplay_name);
    if (client->display == NULL)
    {
        LLOGLN(0, ("nestXClientCreate: connected to %s failed",
               XDisplayName(diplay_name)));
        free(client);
        return NULL;
    }
    else
    {
        LLOGLN(0, ("nestXClientCreate: connected to %s ok",
               XDisplayName(diplay_name)));
    }
    client->screen = DefaultScreen(client->display);
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
    XCloseDisplay(client->display);
    free(client);
    return 0;
}