/*
 * Copyright (C) 2008, 2009 Jan Michael C. Alonzo
 * Copyright (C) 2009 Igalia S.L.
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
#include "webkitwebhistoryitem.h"

#include "HistoryItem.h"
#include "URL.h"
#include "webkitglobalsprivate.h"
#include "webkitwebhistoryitemprivate.h"
#include <wtf/text/CString.h>
#include <wtf/text/WTFString.h>

/**
 * SECTION:webkitwebhistoryitem
 * @short_description: One item of the #WebKitWebBackForwardList and or global history
 * @see_also: #WebKitWebBackForwardList
 *
 * A history item consists out of a title and a uri. It can be part of the
 * #WebKitWebBackForwardList and the global history. The global history is used
 * for coloring the links of visited sites.  #WebKitWebHistoryItem's constructed with
 * #WebKitWebHistoryItem::new and #WebKitWebHistoryItem::new_with_data are
 * automatically added to the global history.
 *
 * <informalexample><programlisting>
 * /<!-- -->* Inject a visited page into the global history *<!-- -->/
 * WebKitWebHistoryItem::new_with_data("http://www.gnome.org/", "GNOME: The Free Software Desktop Project");
 * WebKitWebHistoryItem::new_with_data("http://www.webkit.org/", "The WebKit Open Source Project");
 * </programlisting></informalexample>
 *
 */

using namespace WebKit;

map<WebCore::HistoryItem*, WebKitWebHistoryItem*> globalHistory;

WebKitWebHistoryItem::WebKitWebHistoryItem() {
	webkitInit();

	RefPtr<WebCore::HistoryItem> item = WebCore::HistoryItem::create();
	historyItem = item.release().leakRef();
	webkit_history_item_add(historyItem, this);
}

WebKitWebHistoryItem::WebKitWebHistoryItem(const char* uri, const char* title) {
	webkitInit();

	WebCore::URL historyUri(WebCore::URL(), uri);
	WTF::String historyTitle = WTF::String::fromUTF8(title);
	RefPtr<WebCore::HistoryItem> item = WebCore::HistoryItem::create(historyUri, historyTitle);
	historyItem = item.release().leakRef();
	webkit_history_item_add(webHistoryItem, historyItem);
}

WebKitWebHistoryItem::~WebKitWebHistoryItem() {

}

void webkit_history_item_add(WebKitWebHistoryItem* webHistoryItem, WebCore::HistoryItem* historyItem)
{
	globalHistory->insert(historyItem, webHistoryItem);
}

/* Helper function to create a new WebHistoryItem instance when needed */
WebKitWebHistoryItem* WebKitWebHistoryItem::new_with_core_item(PassRefPtr<WebCore::HistoryItem> historyItem)
{
	return kit(historyItem);
}

/**
 * WebKitWebHistoryItem::get_title:
 * @web_history_item: a #WebKitWebHistoryItem
 *
 * Returns: the page title of @web_history_item
 */
const char* WebKitWebHistoryItem::get_title()
{
	WebCore::HistoryItem* item = core(webHistoryItem);

	WebKitWebHistoryItemPrivate* priv = webHistoryItem->priv;
	title = item->title().utf8();

	return title.data();
}

/**
 * WebKitWebHistoryItem::get_alternate_title:
 * @web_history_item: a #WebKitWebHistoryItem
 *
 * Returns the alternate title of @web_history_item
 *
 * Return value: the alternate title of @web_history_item
 */
const char* WebKitWebHistoryItem::get_alternate_title()
{
	WebCore::HistoryItem* item = core(webHistoryItem);

	WebKitWebHistoryItemPrivate* priv = webHistoryItem->priv;
	alternateTitle = item->alternateTitle().utf8();

	return alternateTitle.data();
}

/**
 * WebKitWebHistoryItem::set_alternate_title:
 * @web_history_item: a #WebKitWebHistoryItem
 * @title: the alternate title for @this history item
 *
 * Sets an alternate title for @web_history_item
 */
void WebKitWebHistoryItem::set_alternate_title(const char* title)
{
	WebCore::HistoryItem* item = core(webHistoryItem);

	item->setAlternateTitle(WTF::String::fromUTF8(title));
}

/**
 * WebKitWebHistoryItem::get_uri:
 * @web_history_item: a #WebKitWebHistoryItem
 *
 * Returns the URI of @this
 *
 * Return value: the URI of @web_history_item
 */
const char* WebKitWebHistoryItem::get_uri()
{
	WebCore::HistoryItem* item = core(WEBKIT_WEB_HISTORY_ITEM(webHistoryItem));

	WebKitWebHistoryItemPrivate* priv = webHistoryItem->priv;
	uri = item->urlString().utf8();

	return uri.data();
}

/**
 * WebKitWebHistoryItem::get_original_uri:
 * @web_history_item: a #WebKitWebHistoryItem
 *
 * Returns the original URI of @web_history_item.
 *
 * Return value: the original URI of @web_history_item
 */
const char* WebKitWebHistoryItem::get_original_uri()
{
	WebCore::HistoryItem* item = core(WEBKIT_WEB_HISTORY_ITEM(webHistoryItem));

	WebKitWebHistoryItemPrivate* priv = webHistoryItem->priv;
	originalUri = item->originalURLString().utf8();

	return webHistoryItem->originalUri.data();
}

/**
 * WebKitWebHistoryItem::get_last_visisted_time :
 * @web_history_item: a #WebKitWebHistoryItem
 *
 * Returns the last time @web_history_item was visited
 *
 * Return value: the time in seconds this @web_history_item was last visited
 */
double WebKitWebHistoryItem::get_last_visited_time()
{
	WebCore::HistoryItem* item = core(WEBKIT_WEB_HISTORY_ITEM(webHistoryItem));

	// FIXME: HistoryItem in WebCore doesn't implement visit stats anymore.
	// https://bugs.webkit.org/show_bug.cgi?id=127673.
	return 0;
}

/* private methods */

char* WebKitWebHistoryItem::get_target(WebKitWebHistoryItem* webHistoryItem)
{
	WebCore::HistoryItem* item = core(webHistoryItem);

	WTF::CString t = item->target().utf8();
	return strdup(t.data());
}

bool WebKitWebHistoryItem::is_target_item(WebKitWebHistoryItem* webHistoryItem)
{
	WebCore::HistoryItem* item = core(webHistoryItem);

	return item->isTargetItem();
}

GList* WebKitWebHistoryItem::get_children(WebKitWebHistoryItem* webHistoryItem)
{
	WebCore::HistoryItem* item = core(webHistoryItem);

	const WebCore::HistoryItemVector& children = item->children();
	if (!children.size())
		return NULL;

	unsigned size = children.size();
	GList* kids = NULL;
	for (unsigned i = 0; i < size; ++i)
		kids = g_list_prepend(kids, kit(children[i].get()));

	return g_list_reverse(kids);
}

WebCore::HistoryItem* WebKit::core(WebKitWebHistoryItem* webHistoryItem)
{
	return webHistoryItem->historyItem;
}

WebKitWebHistoryItem* WebKit::kit(PassRefPtr<WebCore::HistoryItem> historyItem)
{
	RefPtr<WebCore::HistoryItem> item = historyItem;
	GHashTable* table = webkit_history_items();
	WebKitWebHistoryItem* webHistoryItem = (WebKitWebHistoryItem*) g_hash_table_lookup(table, item.get());

	if (!webHistoryItem) {
		webHistoryItem = WEBKIT_WEB_HISTORY_ITEM(g_object_new(WEBKIT_TYPE_WEB_HISTORY_ITEM, NULL));
		WebKitWebHistoryItemPrivate* priv = webHistoryItem->priv;

		historyItem = item.release().leakRef();
		webkit_history_item_add(webHistoryItem, historyItem);
	}

	return webHistoryItem;
}

