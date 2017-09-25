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

#ifndef __NESTMISC_H
#define __NESTMISC_H

#include <xorg-server.h>
#include <xorgVersion.h>
#include <xf86.h>

extern _X_EXPORT int
nestBitsPerPixel(int depth);
extern _X_EXPORT void
g_sleep(int msecs);
extern _X_EXPORT void
g_sprintf(char *dest, const char *format, ...);
extern _X_EXPORT void
g_memcpy(void *d_ptr, const void *s_ptr, int size);
extern _X_EXPORT void
g_memset(void *d_ptr, const unsigned char chr, int size);
extern _X_EXPORT int
g_create_dir(const char *dirname);
extern _X_EXPORT int
g_directory_exist(const char *dirname);
extern _X_EXPORT int
g_chmod_hex(const char *filename, int flags);
extern _X_EXPORT void
g_hexdump(void *p, long len);

/* glib-style memory allocation macros */
#define g_new(struct_type, n_structs) \
    (struct_type *) malloc(sizeof(struct_type) * (n_structs))
#define g_new0(struct_type, n_structs) \
    (struct_type *) calloc((n_structs), sizeof(struct_type))

#endif
