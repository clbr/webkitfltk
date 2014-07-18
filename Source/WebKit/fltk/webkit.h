/*
WebkitFLTK
Copyright (C) 2014 Lauri Kasanen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef webkit_h
#define webkit_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>

#include "webview.h"

extern "C" {

#define WK_FLTK_MAJOR 0
#define WK_FLTK_MINOR 0
#define WK_FLTK_PATCH 0

// Init. Call this before show()ing anything.
void webkitInit();

// Use this for per-page useragents.
void wk_set_useragent_func(const char * (*func)(const char *));

// Content blocking. Return 0 for ok, 1 for block.
void wk_set_urlblock_func(int (*func)(const char *));

// Where to open files for uploading?
void wk_set_uploaddir_func(const char * (*func)());

// Where to open files for downloading?
void wk_set_downloaddir_func(const char * (*func)());

// SSL control - return 1 if this cert is ok, 0 to abort
void wk_set_ssl_func(int (*func)(const char *, const char *));

// Inform the browser of which tab needs a scary SSL warning
void wk_set_ssl_err_func(void (*func)(webview *, const char *));

// Scrolling speed
void wk_set_wheel_speed(const int in);

// about:// pages. Return a malloced array (will be freed), NULL if no such page.
void wk_set_aboutpage_func(const char * (*func)(const char *));

// Callback for finished downloads
void wk_set_download_func(void (*func)(const char *url, const char *file));

// Drop RAM caches
void wk_drop_caches();

// Returns a malloced, url-encoded string
char *wk_urlencode(const char *in);

} // extern C

#endif
