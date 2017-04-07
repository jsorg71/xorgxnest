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

the rest

*/

#if defined(HAVE_CONFIG_H)
#include "config_ac.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

/* this should be before all X11 .h files */
#include <xorg-server.h>
#include <xorgVersion.h>

/* all driver need this */
#include <xf86.h>
#include <xf86_OSproc.h>

#include "nestMisc.h"

/******************************************************************************/
int
nestBitsPerPixel(int depth)
{
    if (depth == 1)
    {
        return 1;
    }
    else if (depth <= 8)
    {
        return 8;
    }
    else if (depth <= 16)
    {
        return 16;
    }
    else
    {
        return 32;
    }
}

/*****************************************************************************/
void
g_sleep(int msecs)
{
    usleep(msecs * 1000);
}

/*****************************************************************************/
void
g_sprintf(char *dest, const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    vsprintf(dest, format, ap);
    va_end(ap);
}

/*****************************************************************************/
void
g_memcpy(void *d_ptr, const void *s_ptr, int size)
{
    memcpy(d_ptr, s_ptr, size);
}

/*****************************************************************************/
void
g_memset(void *d_ptr, const unsigned char chr, int size)
{
    memset(d_ptr, chr, size);
}

/*****************************************************************************/
/* returns boolean */
int
g_create_dir(const char *dirname)
{
    return mkdir(dirname, (mode_t) - 1) == 0;
}

/*****************************************************************************/
/* returns boolean, non zero if the directory exists */
int
g_directory_exist(const char *dirname)
{
    struct stat st;

    if (stat(dirname, &st) == 0)
    {
        return S_ISDIR(st.st_mode);
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
/* returns error */
int
g_chmod_hex(const char *filename, int flags)
{
    int fl;

    fl = 0;
    fl |= (flags & 0x4000) ? S_ISUID : 0;
    fl |= (flags & 0x2000) ? S_ISGID : 0;
    fl |= (flags & 0x1000) ? S_ISVTX : 0;
    fl |= (flags & 0x0400) ? S_IRUSR : 0;
    fl |= (flags & 0x0200) ? S_IWUSR : 0;
    fl |= (flags & 0x0100) ? S_IXUSR : 0;
    fl |= (flags & 0x0040) ? S_IRGRP : 0;
    fl |= (flags & 0x0020) ? S_IWGRP : 0;
    fl |= (flags & 0x0010) ? S_IXGRP : 0;
    fl |= (flags & 0x0004) ? S_IROTH : 0;
    fl |= (flags & 0x0002) ? S_IWOTH : 0;
    fl |= (flags & 0x0001) ? S_IXOTH : 0;
    return chmod(filename, fl);
}

/*****************************************************************************/
/* produce a hex dump */
void
g_hexdump(void *p, long len)
{
    unsigned char *line;
    int i;
    int thisline;
    int offset;

    offset = 0;
    line = (unsigned char *) p;

    while (offset < (int) len)
    {
        ErrorF("%04x ", offset);
        thisline = len - offset;

        if (thisline > 16)
        {
            thisline = 16;
        }

        for (i = 0; i < thisline; i++)
        {
            ErrorF("%02x ", line[i]);
        }

        for (; i < 16; i++)
        {
            ErrorF("   ");
        }

        for (i = 0; i < thisline; i++)
        {
            ErrorF("%c", (line[i] >= 0x20 && line[i] < 0x7f) ? line[i] : '.');
        }

        ErrorF("\n");
        offset += thisline;
        line += thisline;
    }
}
