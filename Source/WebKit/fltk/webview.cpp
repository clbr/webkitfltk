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

#include "config.h"

#include "webview.h"
#include "webviewpriv.h"

#include <cairo-xlib.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <unistd.h>

#include <MainFrame.h>
#include <FrameLoadRequest.h>
#include <FrameView.h>
#include <InspectorController.h>
#include <Settings.h>
#include <WidgetBackingStoreCairo.h>
#include <wtf/CurrentTime.h>

using namespace WTF;
using namespace WebCore;

webview::webview(int x, int y, int w, int h): Fl_Widget(x, y, w, h) {

	priv = new privatewebview;
	priv->gc = NULL;
	priv->cairo = NULL;
	priv->w = w;
	priv->h = h;

	Fl_Widget *wid = this;
	while (wid->parent())
		wid = wid->parent();
	priv->window = wid->as_window();

	fl_open_display();
	priv->depth = fl_visual->depth;

	Page::PageClients clients;
	clients.chromeClient = new FlChromeClient(this);
	#if ENABLE(CONTEXT_MENUS)
	clients.contextMenuClient = new FlContextMenuClient(this);
	#endif
	#if ENABLE(DRAG_SUPPORT)
	clients.dragClient = new FlDragClient(this);
	#endif
	clients.editorClient = new FlEditorClient(this);
	clients.inspectorClient = new FlInspectorClient(this);
	clients.loaderClientForMainFrame = new FlFrameLoaderClient(this, (Frame*) 1);
	clients.progressTrackerClient = new FlProgressTrackerClient(this);

	priv->page = new Page(clients);
	priv->page->addLayoutMilestones(DidFirstVisuallyNonEmptyLayout);

	((FlFrameLoaderClient *) clients.loaderClientForMainFrame)->
			setFrame(&priv->page->mainFrame());

	//WebCore::provideGeolocationTo(priv->page, new FlGeoClient(this));
	//WebCore::provideUserMediaTo(priv->page, new FlMediaClient(this));

	priv->page->setGroupName("org.webkit.fltk.WebKitFLTK");

	// inspector exposing

	// viewportattributes?

	// backfwdlist?

	// window features?

	MainFrame *f = &priv->page->mainFrame();
	f->init();
	priv->event = &f->eventHandler();

	// Default settings
	Settings &set = priv->page->mainFrame().settings();
	set.setLoadsImagesAutomatically(true);
	set.setShrinksStandaloneImagesToFit(true);
	set.setScriptEnabled(true);
	set.setDNSPrefetchingEnabled(true);
	set.setDefaultMinDOMTimerInterval(0.016);
	set.setMinDOMTimerInterval(0.016);
	set.setHiddenPageDOMTimerAlignmentInterval(10);
	set.setHiddenPageDOMTimerThrottlingEnabled(true);
	set.setMinimumFontSize(8);
	set.setDefaultFontSize(16);

	// Cairo
	resize();

	clock_gettime(CLOCK_MONOTONIC, &priv->lastdraw);
}

webview::~webview() {
	delete priv->page;
	delete priv;
}

void webview::draw() {
	if (!priv->cairo)
		return;
	ASSERT(isMainThread());

	// Don't draw at over 60 fps. Save power and penguins.
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);

	unsigned usecs = (now.tv_sec - priv->lastdraw.tv_sec) * 1000 * 1000;
	usecs += (now.tv_nsec - priv->lastdraw.tv_nsec) / 1000;
	if (usecs < 16600)
		usleep(16600 - usecs);

	int cx, cy, cw, ch;
	fl_clip_box(x(), y(), w(), h(), cx, cy, cw, ch);
	if (!cw) return;

	drawWeb(); // for now here

	const int tgtx = cx, tgty = cy;

	// If the widget is offset somewhere, copy the right parts
	cx -= x();
	cy -= y();

	XCopyArea(fl_display, priv->cairopix, fl_window, fl_gc, cx, cy, cw, ch,
			tgtx, tgty);

	priv->lastdraw = now;
}

void webview::drawWeb() {

	Frame *f = &priv->page->mainFrame();
	if (!f->contentRenderer() || !f->view() || !priv->cairo)
		return;

	f->view()->updateLayoutAndStyleIfNeededRecursive();

	priv->gc->applyDeviceScaleFactor(f->page()->deviceScaleFactor());
	f->view()->paint(priv->gc, IntRect(0, 0, w(), h()));
	priv->page->inspectorController().drawHighlight(*priv->gc);
}

void webview::load(const char *url) {

	MainFrame *f = &priv->page->mainFrame();

	// Local path?
	String orig;
	String str = orig = String::fromUTF8(url);
	if (url[0] == '/') {
		str = "file://" + orig;
	} else if (url[0] == '~') {
		const char *home = getenv("HOME");
		if (!home)
			home = "/tmp";
		str = "file://";
		str.append(home);
		str.append(url + 1);
	}

	// No protocol?
	if (!str.contains("://")) {
		str = "http://";
		str.append(orig);
	}

	FrameLoadRequest req(f, ResourceRequest(URL(URL(), str)));

	f->loader().load(req);
}

void webview::loadString(const char * const str, const char * const mime,
				const char * const enc, const char * const baseurl) {
	MainFrame *f = &priv->page->mainFrame();

	URL base = baseurl ? URL(URL(), String::fromUTF8(baseurl)) : blankURL();
	ResourceRequest req(base);

	RefPtr<SharedBuffer> sharedBuffer = SharedBuffer::create(str, strlen(str));
	SubstituteData substituteData(sharedBuffer.release(),
		mime ? String::fromUTF8(mime) : String::fromUTF8("text/html"),
		enc ? String::fromUTF8(enc) : String::fromUTF8("UTF-8"),
		URL(URL(), String::fromUTF8("")),
		URL(URL(), String::fromUTF8("")));

	f->loader().load(FrameLoadRequest(f, req, substituteData));
}

void webview::resize() {
	ASSERT(isMainThread());

	bool old = false;

	if (priv->cairo) {
		cairo_destroy(priv->cairo);
		old = true;

		priv->w = w();
		priv->h = h();
	}

	if (old)
		XFreePixmap(fl_display, priv->cairopix);
	priv->cairopix = XCreatePixmap(fl_display, DefaultRootWindow(fl_display),
					priv->w, priv->h, priv->depth);

	cairo_surface_t *surf = cairo_xlib_surface_create(fl_display, priv->cairopix,
								fl_visual->visual,
								priv->w, priv->h);
	priv->cairo = cairo_create(surf);
	priv->cairosurf = surf;
	cairo_surface_destroy(surf);

	if (priv->gc)
		delete priv->gc;
	priv->gc = new GraphicsContext(priv->cairo);

	if (old)
		priv->page->mainFrame().view()->resize(priv->w, priv->h);
}

int webview::handle(int e) {

	switch (e) {
		case FL_PUSH:
		case FL_RELEASE:
		case FL_MOVE:
			{
			const IntPoint pos(Fl::event_x() - x(), Fl::event_y() - y());
			const IntPoint gpos(Fl::event_x_root(), Fl::event_y_root());
			MouseButton btn = NoButton;
			PlatformEvent::Type type;
			unsigned clicks = 0;

			if (e == FL_PUSH || e == FL_RELEASE) {
				switch (Fl::event_button()) {
					case FL_LEFT_MOUSE:
						btn = LeftButton;
					break;
					case FL_MIDDLE_MOUSE:
						btn = MiddleButton;
					break;
					case FL_RIGHT_MOUSE:
						btn = RightButton;
					break;
				}
				clicks = Fl::event_clicks() ? 2 : 1;
			}

			if (e == FL_PUSH)
				type = PlatformEvent::MousePressed;
			else if (e == FL_RELEASE)
				type = PlatformEvent::MouseReleased;
			else
				type = PlatformEvent::MouseMoved;


			PlatformMouseEvent pev(pos, gpos, btn, type, clicks,
						Fl::event_shift(), Fl::event_ctrl(),
						Fl::event_alt(),
						Fl::event_command(),
						currentTime());

			if (e == FL_PUSH)
				priv->event->handleMousePressEvent(pev);
			else if (e == FL_RELEASE)
				priv->event->handleMouseReleaseEvent(pev);
			else
				priv->event->mouseMoved(pev);

			return 1;
			}
		break;
		case FL_MOUSEWHEEL:
			{
			const IntPoint pos(Fl::event_x() - x(), Fl::event_y() - y());
			const IntPoint gpos(Fl::event_x_root(), Fl::event_y_root());

			const int scrollspeed = 100;

			PlatformWheelEvent pev(pos, gpos,
						Fl::event_dx() * scrollspeed,
						-Fl::event_dy() * scrollspeed,
						1, 1, ScrollByPixelWheelEvent,
						Fl::event_shift(), Fl::event_ctrl(),
						Fl::event_alt(),
						Fl::event_command());

			priv->event->handleWheelEvent(pev);

			return 1;
			}
		break;
		case FL_ENTER:
		case FL_LEAVE:
			return 1;
		default:
			return Fl_Widget::handle(e);
	}
}

void webview::show() {
	priv->page->setIsVisible(true);
	Fl_Widget::show();
}

void webview::hide() {
	priv->page->setIsVisible(false);
	Fl_Widget::hide();
}
