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

#include "kbd.h"
#include "webview.h"
#include "webviewpriv.h"

#include <cairo-xlib.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Item.H>
#include <unistd.h>

#include <ContextMenuController.h>
#include <FocusController.h>
#include <FrameLoadRequest.h>
#include <FrameView.h>
#include <InspectorController.h>
#include <MainFrame.h>
#include <PlatformKeyboardEvent.h>
#include <Settings.h>
#include <WidgetBackingStoreCairo.h>
#include <WindowsKeyboardCodes.h>
#include <wtf/CurrentTime.h>

using namespace WTF;
using namespace WebCore;

extern int wheelspeed;

webview::webview(int x, int y, int w, int h): Fl_Widget(x, y, w, h) {

	priv = new privatewebview;
	priv->gc = NULL;
	priv->cairo = NULL;
	priv->w = w;
	priv->h = h;
	priv->editing = false;

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

static void keyscroll(Frame &f, const unsigned key, const bool shift) {

	ScrollDirection dir;
	ScrollGranularity gran;

	switch (key) {
		case VK_SPACE:
		case VK_NEXT:
		case VK_PRIOR:
			gran = ScrollByPage;
			if (shift || key == VK_PRIOR)
				dir = ScrollUp;
			else
				dir = ScrollDown;
		break;
		case VK_HOME:
			gran = ScrollByDocument;
			dir = ScrollUp;
		break;
		case VK_END:
			gran = ScrollByDocument;
			dir = ScrollDown;
		break;
		case VK_LEFT:
			gran = ScrollByLine;
			dir = ScrollLeft;
		break;
		case VK_RIGHT:
			gran = ScrollByLine;
			dir = ScrollRight;
		break;
		case VK_UP:
			gran = ScrollByLine;
			dir = ScrollUp;
		break;
		case VK_DOWN:
			gran = ScrollByLine;
			dir = ScrollDown;
		break;
		default:
			return;
	}

	if (f.eventHandler().scrollOverflow(dir, gran))
		return;
	f.view()->scroll(dir, gran);
}

int webview::handle(const int e) {

	switch (e) {
		case FL_PUSH:
		case FL_RELEASE:
		case FL_MOVE:
		case FL_DRAG:
			{
			const IntPoint pos(Fl::event_x() - x(), Fl::event_y() - y());
			const IntPoint gpos(Fl::event_x_root(), Fl::event_y_root());
			MouseButton btn = NoButton;
			PlatformEvent::Type type;
			unsigned clicks = 0;

			if (e == FL_PUSH || e == FL_RELEASE || e == FL_DRAG) {
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

			if (e == FL_PUSH) {
				priv->event->handleMousePressEvent(pev);
				if (btn == RightButton)
					handlecontextmenu(&pev);
			} else if (e == FL_RELEASE) {
				priv->event->handleMouseReleaseEvent(pev);
			} else {
				priv->event->mouseMoved(pev);
			}

			return 1;
			}
		break;
		case FL_MOUSEWHEEL:
			{
			const IntPoint pos(Fl::event_x() - x(), Fl::event_y() - y());
			const IntPoint gpos(Fl::event_x_root(), Fl::event_y_root());

			PlatformWheelEvent pev(pos, gpos,
						Fl::event_dx() * wheelspeed,
						-Fl::event_dy() * wheelspeed,
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
		case FL_FOCUS:
		case FL_UNFOCUS:
			return 1;
		case FL_KEYDOWN:
		case FL_KEYUP:
			{
			PlatformEvent::Type type = PlatformEvent::KeyDown;
			if (e == FL_KEYUP)
				type = PlatformEvent::KeyUp;

			String text(Fl::event_text(), Fl::event_length());

			unsigned modifiers = 0;
			if (Fl::event_shift())
				modifiers |= PlatformEvent::ShiftKey;
			if (Fl::event_alt())
				modifiers |= PlatformEvent::AltKey;
			if (Fl::event_ctrl())
				modifiers |= PlatformEvent::CtrlKey;
			if (Fl::event_command())
				modifiers |= PlatformEvent::MetaKey;

			bool iskeypad = false;
			const unsigned key = Fl::event_key();
			if (key >= FL_KP && key <= FL_KP_Last)
				iskeypad = true;

			String keyid = keyidfor(Fl::event_key());
			unsigned win = winkeyfor(Fl::event_key());

			PlatformKeyboardEvent pev(type, text, text,
						keyid, win, 0, 0,
						false, iskeypad, false,
						(PlatformEvent::Modifiers) modifiers,
						currentTime());

			bool ret = priv->event->keyEvent(pev);

			if (e == FL_KEYDOWN && !ret)
				keyscroll(priv->page->mainFrame(), win, Fl::event_shift());

			if (priv->editing)
				return 1;
			else
				return 0;
			}
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

static void additem(Vector<Fl_Menu_Item> &items, const ContextMenuItem &cur) {

	const ContextMenuItemType type = cur.type();
	//const ContextMenuAction action = cur.action();
	const char * const title = strdup(cur.title().utf8().data());
	const bool enabled = cur.enabled();
	const bool checked = cur.checked();

	Fl_Menu_Item it = {
		title, 0, 0, (void *) &cur,
		0,
		FL_NORMAL_LABEL, FL_HELVETICA,
		FL_NORMAL_SIZE, FL_FOREGROUND_COLOR
	};

	if (!enabled)
		it.flags |= FL_MENU_INACTIVE;
	if (type == CheckableActionType) {
		it.flags |= FL_MENU_TOGGLE;
		if (checked)
			it.flags |= FL_MENU_VALUE;
	} else if (type == SeparatorType) {
		items[items.size() - 1].flags |= FL_MENU_DIVIDER;
		return;
	} else if (type == SubmenuType) {
		it.flags |= FL_SUBMENU;
	}

	items.append(it);
}

void webview::handlecontextmenu(void *ptr) {
	const PlatformMouseEvent * const pev = (PlatformMouseEvent *) ptr;

	Frame * const focused = &priv->page->focusController().focusedOrMainFrame();
	focused->eventHandler().sendContextMenuEvent(*pev);

	ContextMenu *m = priv->page->contextMenuController().contextMenu();
	if (!m)
		return;

	Vector<Fl_Menu_Item> items;
	const unsigned max = m->items().size();
	items.reserveCapacity(max);

	unsigned i;
	for (i = 0; i < max; i++) {
		const ContextMenuItem &cur = m->items()[i];

		additem(items, cur);

		// Just one level of recursion for now.
		if (cur.type() == SubmenuType) {
			unsigned k;
			unsigned kmax = cur.subMenuItems().size();
			for (k = 0; k < kmax; k++) {
				const ContextMenuItem &kcur = cur.subMenuItems()[k];

				additem(items, kcur);

				ASSERT(kcur.type() != SubmenuType);
			}
			Fl_Menu_Item end;
			memset(&end, 0, sizeof(Fl_Menu_Item));
			items.append(end);
		}
	}
	Fl_Menu_Item end;
	memset(&end, 0, sizeof(Fl_Menu_Item));
	items.append(end);

	// Show
	const Fl_Menu_Item *res = items[0].popup(Fl::event_x(), Fl::event_y());

	const unsigned newmax = items.size();
	for (i = 0; i < newmax; i++) {
		free((char *) items[i].text);
	}

	if (res) {
		ContextMenuItem sel(*(ContextMenuItem *) res->user_data_);
		if (sel.type() == SubmenuType) return;
		priv->page->contextMenuController().contextMenuItemSelected(&sel);
	}
}
