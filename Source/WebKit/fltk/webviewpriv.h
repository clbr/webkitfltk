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

#ifndef webviewpriv_h
#define webviewpriv_h

#include "chromeclient.h"
#include "contextclient.h"
#include "download.h"
#include "dragclient.h"
#include "editorclient.h"
#include "inspectorclient.h"
#include "frameclient.h"
#include "progressclient.h"

#include <EventHandler.h>
#include <GraphicsContext.h>
#include <Page.h>
#include <wtf/text/CString.h>

#include <time.h>
#include <vector>

typedef unsigned long Pixmap;

class privatewebview {
public:
	WebCore::Page *page;
	WebCore::EventHandler *event;

	cairo_t *cairo;
	cairo_surface_t *cairosurf;
	WebCore::GraphicsContext *gc;
	Pixmap cairopix;

	Fl_Window *window;
	unsigned depth;
	unsigned w, h;

	struct timespec lastdraw;

	bool editing;

	const char *statusbartext;
	const char *title;
	const char *url;
	bool hoveringlink;

	std::vector<download *> downloads;

	// Callbacks
	void (*titleChanged)();
	void (*loadStateChanged)(webview *);
};

#endif
