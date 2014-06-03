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

#include <FL/fl_draw.H>
#include <MainFrame.h>
#include <FrameLoadRequest.h>
#include <FrameView.h>
#include <InspectorController.h>
#include <Settings.h>
#include <WidgetBackingStoreCairo.h>

using namespace WTF;
using namespace WebCore;

webview::webview(int x, int y, int w, int h): Fl_Widget(x, y, w, h) {

	priv = new privatewebview;
	priv->cairodata = NULL;

	Fl_Widget *wid = this;
	while (wid->parent())
		wid = wid->parent();
	priv->window = wid->as_window();

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

	// Cairo
	cairo_surface_t *surf = cairo_image_surface_create(CAIRO_FORMAT_RGB24, w, h);
	priv->cairo = cairo_create(surf);
	priv->cairosurf = surf;
	cairo_surface_destroy(surf);
	priv->gc = new GraphicsContext(priv->cairo);
	priv->cairodata = (unsigned char *) calloc(w * h * 3, 1);
}

webview::~webview() {
	delete priv->page;
	delete priv;
}

void webview::draw() {
	if (!priv->cairodata)
		return;

	ASSERT(isMainThread());

	drawWeb(); // for now here

	fl_draw_image(priv->cairodata, 0, 0, w(), h(), 3);
}

void webview::drawWeb() {

	Frame *f = &priv->page->mainFrame();
	if (!f->contentRenderer() || !f->view() || !priv->cairodata)
		return;

	f->view()->updateLayoutAndStyleIfNeededRecursive();

	priv->gc->applyDeviceScaleFactor(f->page()->deviceScaleFactor());
	f->view()->paint(priv->gc, IntRect(0, 0, w(), h()));
	priv->page->inspectorController().drawHighlight(*priv->gc);

	// Convert to FLTK's RGB format. Sucks, but whatcha gonna do.
	unsigned char * const d = cairo_image_surface_get_data(priv->cairosurf);
	unsigned p;
	const unsigned max = w() * h();
	for (p = 0; p < max; p++) {
		priv->cairodata[p*3] = d[p*4 + 2];
		priv->cairodata[p*3 + 1] = d[p*4 + 1];
		priv->cairodata[p*3 + 2] = d[p*4 + 0];
	}
}

void webview::load(const char *url) {

	MainFrame *f = &priv->page->mainFrame();

	FrameLoadRequest req(f, ResourceRequest(URL(URL(), String::fromUTF8(url))));

	f->loader().load(req);
}

void webview::resize() {
	cairo_destroy(priv->cairo);

	cairo_surface_t *surf = cairo_image_surface_create(CAIRO_FORMAT_RGB24, w(), h());
	priv->cairo = cairo_create(surf);
	priv->cairosurf = surf;
	cairo_surface_destroy(surf);

	delete priv->gc;
	priv->gc = new GraphicsContext(priv->cairo);

	free(priv->cairodata);
	priv->cairodata = (unsigned char *) calloc(w() * h() * 3, 1);
}
