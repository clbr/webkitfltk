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
#include <FL/Fl_RGB_Image.H>
#include <vector>

#include "webview.h"

extern "C" {

#define WK_FLTK_MAJOR 0
#define WK_FLTK_MINOR 0
#define WK_FLTK_PATCH 0

// Init. Call this before show()ing anything.
void webkitInit();

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

// Callback that some downloads were updated
void wk_set_download_refresh_func(void (*func)());

// Callback for when a new download has been started
void wk_set_new_download_func(void (*func)());

// Page requests a popup to this address
void wk_set_popup_func(webview *(*func)(const char*));

// Please open this address in a background tab
void wk_set_bgtab_func(void (*func)(const char*));

// Drop RAM caches
void wk_drop_caches();

// Set streaming program and args, default none
void wk_set_streaming_prog(const char *);

// Cleanup on exit. Calls drop_caches.
void wk_exit();

// Returns a malloced, url-encoded string
char *wk_urlencode(const char *in);

// Favicons
void wk_set_favicon_dir(const char *dir, const std::vector<const char*> *preloads = NULL);
Fl_RGB_Image *wk_get_favicon(const char *url, const unsigned targetsize = 16);

// Cache
void wk_set_cache_dir(const char *dir);
void wk_set_cache_max(const unsigned bytes);

// Per-site settings
void wk_set_persite_settings_func(void (*func)(const char*));

// Spoofing functions
// Use this for per-page useragents.
void wk_set_useragent_func(const char * (*func)(const char *));
// Return the number of seconds east of GMT
void wk_set_tz_func(int (*func)());
// Return the Accept: header for this page
void wk_set_accept_func(const char *(*func)(const char*));
// Return the Accept-language: header for this page
void wk_set_language_func(const char *(*func)(const char *));

} // extern C

#endif
