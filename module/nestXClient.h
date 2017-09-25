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

#ifndef __NESTXCLIENT_H
#define __NESTXCLIENT_H

struct _nestXClientRec; /* defined in nestXClient.c */
typedef struct _nestXClientRec nestXClientRec;
typedef nestXClientRec *nestXClientPtr;

extern _X_EXPORT nestXClientPtr
nestXClientCreate(const char *diplay_name);
extern _X_EXPORT int
nestXClientDestroy(nestXClientPtr client);
extern _X_EXPORT int
nestXClientSetupScreen(nestPtr dev);

#endif
