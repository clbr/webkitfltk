/*
 * Copyright (C) 2011 Christian Dywan <christian@lanedo.com>
 * Copyright (C) 2012 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "webkitfavicondatabase.h"

#include "FileSystem.h"
#include "IconDatabase.h"
#include "IconDatabaseClient.h"
#include "Image.h"
#include "IntSize.h"
#include "webkitglobals.h"
#include "webkitglobalsprivate.h"
#include "webkitsecurityoriginprivate.h"
#include "webkitwebframe.h"
#include <wtf/MainThread.h>
#include <wtf/text/CString.h>

/**
 * SECTION:webkitfavicondatabase
 * @short_description: A WebKit favicon database
 * @Title: WebKitFaviconDatabase
 *
 * #WebKitFaviconDatabase provides access to the icons associated with
 * web sites.
 *
 * WebKit will automatically look for available icons in link elements
 * on opened pages as well as an existing favicon.ico and load the
 * images found into a memory cache if possible. That cache is frozen
 * to an on-disk database for persistence.
 *
 * The database is disabled by default. In order for icons to be
 * stored and accessed, you will need to set an icon database path
 * using webkit_favicon_database_set_path(). Disable the database
 * again passing %NULL to the previous call.
 *
 * If WebKitWebSettings::enable-private-browsing is %TRUE new icons
 * won't be added to the on-disk database and no existing icons will
 * be deleted from it. Nevertheless, WebKit will still store them in
 * the in-memory cache during the current execution.
 *
 * Since: 1.8
 */

using namespace WebKit;
using namespace WebCore;

class PendingIconRequest;

static void webkitFaviconDatabaseProcessPendingIconsForURI(WebKitFaviconDatabase*, const String& pageURI);
static void webkitFaviconDatabaseImportFinished(WebKitFaviconDatabase*);
static void webkitFaviconDatabaseGetIconPixbufCancelled(GCancellable*, PendingIconRequest*);
static void webkitFaviconDatabaseClose();

class IconDatabaseClientFLTK : public IconDatabaseClient {
public:
	// IconDatabaseClient interface
	virtual void didRemoveAllIcons() override { };

	// Called when an icon is requested while the initial import is
	// going on.
	virtual void didImportIconURLForPageURL(const String& URL) override { };

	// Called whenever a retained icon is read from database.
	virtual void didImportIconDataForPageURL(const String& URL) override
	{
		WebKitFaviconDatabase* db = webkit_get_favicon_database();
		// We need to emit this here because webkitFaviconDatabaseDispatchDidReceiveIcon()
		// is only called for icons that have just being downloaded, and this is called
		// when icon data is imported from the database.
		//g_signal_emit_by_name(database, "icon-loaded", URL.utf8().data());

		webkitFaviconDatabaseProcessPendingIconsForURI(database, URL);
	}

	virtual void didChangeIconForPageURL(const String& URL) override
	{
		// Called when the the favicon for a particular URL changes.
		// It does not mean that the new icon data is available yet.
	}

	virtual void didFinishURLImport() override
	{
		webkitFaviconDatabaseImportFinished(webkit_get_favicon_database());

		// Now that everything is imported enable pruning of old
		// icons. No icon will be removed during the import process
		// because we disable cleanups before opening the database.
		IconDatabase::allowDatabaseCleanup();
	}
};

class PendingIconRequest {
public:
	PendingIconRequest(const String& pageURL, GSimpleAsyncResult* result, GCancellable* cancellable, IntSize iconSize)
		: m_pageURL(pageURL)
		, m_asyncResult(result)
		, m_cancellable(cancellable)
		, m_cancelledId(0)
		, m_iconSize(iconSize)
	{
		if (cancellable) {
//			m_cancelledId = g_cancellable_connect(cancellable, G_CALLBACK(webkitFaviconDatabaseGetIconPixbufCancelled), this, 0);
//			g_object_set_data_full(G_OBJECT(result), "cancellable", g_object_ref(cancellable), static_cast<GDestroyNotify>(g_object_unref));
		}
	}

	~PendingIconRequest()
	{
//		if (m_cancelledId > 0)
//			g_cancellable_disconnect(m_cancellable.get(), m_cancelledId);
	}

	const String& pageURL() { return m_pageURL; }
//	GSimpleAsyncResult* asyncResult() { return m_asyncResult.get(); }
	const IntSize& iconSize() { return m_iconSize; }

	void asyncResultCancel()
	{
/*		ASSERT(m_asyncResult);
		g_simple_async_result_set_error(m_asyncResult.get(), G_IO_ERROR, G_IO_ERROR_CANCELLED, "%s", _("Operation was cancelled"));
		g_simple_async_result_complete(m_asyncResult.get());*/
	}

	void asyncResultCompleteInIdle(GdkPixbuf* icon)
	{
/*		ASSERT(m_asyncResult);
		g_simple_async_result_set_op_res_gpointer(m_asyncResult.get(), icon, 0);
		g_simple_async_result_complete_in_idle(m_asyncResult.get());*/
	}

	void asyncResultComplete(GdkPixbuf* icon)
	{
/*		ASSERT(m_asyncResult);
		g_simple_async_result_set_op_res_gpointer(m_asyncResult.get(), icon, 0);
		g_simple_async_result_complete(m_asyncResult.get());*/
	}

private:
	String m_pageURL;
//	GRefPtr<GSimpleAsyncResult> m_asyncResult;
//	GRefPtr<GCancellable> m_cancellable;
//	gulong m_cancelledId;
	IntSize m_iconSize;
};

static void webkitFaviconDatabaseClose()
{
	if (iconDatabase().isEnabled()) {
		iconDatabase().setEnabled(false);
		iconDatabase().close();
	}
}

/*
GdkPixbuf* webkit_favicon_database_try_get_favicon_pixbuf(WebKitFaviconDatabase* database, const char* pageURI, unsigned width, unsigned height)
{
	g_return_val_if_fail(pageURI, 0);
	g_return_val_if_fail((width && height) || (!width && !height), 0);

	const String& pageURL = String::fromUTF8(pageURI);
	const IntSize iconSize(width, height);
	ASSERT(isMainThread());

	// The exact size we pass is irrelevant to the iconDatabase code.
	// We must pass something greater than 0x0 to get a pixbuf.
	RefPtr<cairo_surface_t> surface = iconDatabase().synchronousNativeIconForPageURL(pageURL, !iconSize.isZero() ? iconSize : IntSize(1, 1));
	if (!surface)
		return 0;

	GRefPtr<GdkPixbuf> pixbuf = adoptGRef(cairoSurfaceToGdkPixbuf(surface.get()));
	if (!pixbuf)
		return 0;

	// A size of (0, 0) means the maximum available size.
	int pixbufWidth = gdk_pixbuf_get_width(pixbuf.get());
	int pixbufHeight = gdk_pixbuf_get_height(pixbuf.get());
	if (!iconSize.isZero() && (pixbufWidth != iconSize.width() || pixbufHeight != iconSize.height()))
		pixbuf = adoptGRef(gdk_pixbuf_scale_simple(pixbuf.get(), iconSize.width(), iconSize.height(), GDK_INTERP_BILINEAR));
	return pixbuf.leakRef();
}*/

/*
static void webkitfavicondatabaseDeleteRequests(WebKitFaviconDatabase* database, PendingIconRequestVector* requests, const String& pageURL)
{
	database->priv->pendingIconRequests.remove(pageURL);
	delete requests;
}

static void getIconPixbufCancelled(void* userData)
{
	PendingIconRequest* request = static_cast<PendingIconRequest*>(userData);
	request->asyncResultCancel();

	const String& pageURL = request->pageURL();
	WebKitFaviconDatabase* database = webkit_get_favicon_database();
	PendingIconRequestVector* icons = database->priv->pendingIconRequests.get(pageURL);
	if (!icons)
		return;

	size_t itemIndex = icons->find(request);
	if (itemIndex != notFound)
		icons->remove(itemIndex);
	if (icons->isEmpty())
		webkitfavicondatabaseDeleteRequests(database, icons, pageURL);
}

static void webkitFaviconDatabaseProcessPendingIconsForURI(WebKitFaviconDatabase* database, const String& pageURL)
{
	PendingIconRequestVector* icons = database->priv->pendingIconRequests.get(pageURL);
	if (!icons)
		return;

	for (size_t i = 0; i < icons->size(); ++i) {
		PendingIconRequest* request = icons->at(i).get();
		if (request->asyncResult())
			request->asyncResultComplete(getIconPixbufSynchronously(database, pageURL, request->iconSize()));
	}
	webkitfavicondatabaseDeleteRequests(database, icons, pageURL);
}

static void webkitFaviconDatabaseImportFinished(WebKitFaviconDatabase* database)
{
	ASSERT(isMainThread());
	database->priv->importFinished = true;

	// Import is complete, process pending requests for pages that are not in the database,
	// since didImportIconDataForPageURL() will never be called for them.
	Vector<String> toDeleteURLs;
	PendingIconRequestMap::const_iterator end = database->priv->pendingIconRequests.end();
	for (PendingIconRequestMap::const_iterator iter = database->priv->pendingIconRequests.begin(); iter != end; ++iter) {
		String iconURL = iconDatabase().synchronousIconURLForPageURL(iter->key);
		if (!iconURL.isEmpty())
			continue;

		PendingIconRequestVector* icons = iter->value;
		for (size_t i = 0; i < icons->size(); ++i) {
			PendingIconRequest* request = icons->at(i).get();
			if (request->asyncResult())
				request->asyncResultComplete(0);
		}

		toDeleteURLs.append(iter->key);
	}

	for (size_t i = 0; i < toDeleteURLs.size(); ++i)
		webkitfavicondatabaseDeleteRequests(database, database->priv->pendingIconRequests.get(toDeleteURLs[i]), toDeleteURLs[i]);
}*/

WebKitFaviconDatabase::WebKitFaviconDatabase() {
	webkitInit();
}

WebKitFaviconDatabase::~WebKitFaviconDatabase() {
	webkitFaviconDatabaseClose();
}

const char *WebKitFaviconDatabase::get_path() {
	return path;
}

void WebKitFaviconDatabase::set_path(const char *in) {

	// Always try to close because the deprecated icondatabase is opened by default.
	webkitFaviconDatabaseClose();

	free(path);
	path = NULL;

	importFinished = false;
	if (!in || !in[0]) {
		iconDatabase().setEnabled(false);
		return;
	}

	iconDatabase().setClient(icondbClient);
	IconDatabase::delayDatabaseCleanup();
	iconDatabase().setEnabled(true);
	if (!iconDatabase().open(filenameToString(in), IconDatabase::defaultDatabaseFilename())) {
		IconDatabase::allowDatabaseCleanup();
		return;
	}

	path = strdup(in);
}

char *WebKitFaviconDatabase::get_favicon_uri(const char *page_uri) {

	if (!page_uri)
		return 0;
	ASSERT(isMainThread());

	String iconURI = iconDatabase().synchronousIconURLForPageURL(String::fromUTF8(page_uri));

	if (iconURI.isEmpty())
		return 0;

	return strdup(iconURI.utf8().data());
}

void WebKitFaviconDatabase::clear() {
	iconDatabase().removeAllIcons();
}

void WebKitFaviconDatabase::dispatchDidReceiveIcon(const char *frame) {

	// Retain the new icon.
	iconDatabase().retainIconForPageURL(String::fromUTF8(frameURI));
}
