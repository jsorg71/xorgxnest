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

input

*/

#ifndef __NESTINPUT_H
#define __NESTINPUT_H

#include <xorg-server.h>
#include <xorgVersion.h>
#include <xf86.h>

typedef int (*nestInputEventProcPtr)(nestPtr dev, int msg,
                                     long param1, long param2,
                                     long param3, long param4);

extern _X_EXPORT int
nestRegisterInputCallback(int type, nestInputEventProcPtr proc);
extern _X_EXPORT int
nestUnregisterInputCallback(nestInputEventProcPtr proc);
extern _X_EXPORT int
nestInputKeyboardEvent(nestPtr dev, int msg,
                       long param1, long param2,
                       long param3, long param4);
extern _X_EXPORT int
nestInputMouseEvent(nestPtr dev, int msg,
                    long param1, long param2,
                    long param3, long param4);
extern _X_EXPORT int
nestInputInit(void);

#endif
