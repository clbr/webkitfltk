/*
 * Copyright (C) 2008 Jan Michael C. Alonzo
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
#include "webkitwebbackforwardlist.h"

#include "BackForwardController.h"
#include "BackForwardList.h"
#include "HistoryItem.h"
#include "Page.h"
#include "PageGroup.h"
#include "webkitglobalsprivate.h"
#include "webkitwebbackforwardlistprivate.h"
#include "webkitwebhistoryitem.h"
#include "webkitwebhistoryitemprivate.h"
#include "webkitwebview.h"
#include "webkitwebviewprivate.h"

/**
 * SECTION:webkitwebbackforwardlist
 * @short_description: The history of a #WebKitWebView
 * @see_also: #WebKitWebView, #WebKitWebHistoryItem
 *
 */

using namespace WebKit;

WebKitWebBackForwardList::WebKitWebBackForwardList(WebKitWebView* inview) {
	webkitInit();
	backForwardList = NULL;

	if (inview) {
		backForwardList =
			static_cast<WebCore::BackForwardList*>(core(inview)->backForward().client());
		backForwardList->setEnabled(TRUE);
	}
}

WebKitWebBackForwardList::~WebKitWebBackForwardList() {
}


/**
 * WebKitWebBackForwardList::go_forward:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 *
 * Steps forward in the back forward list
 */
void WebKitWebBackForwardList::go_forward()
{
	if (backForwardList->enabled())
		backForwardList->goForward();
}

/**
 * WebKitWebBackForwardList::go_back:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 *
 * Steps backward in the back forward list
 */
void WebKitWebBackForwardList::go_back()
{
	if (backForwardList->enabled())
		backForwardList->goBack();
}

/**
 * WebKitWebBackForwardList::contains_item:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 * @history_item: (type WebKit.WebHistoryItem) (transfer none): the #WebKitWebHistoryItem to check
 *
 * Checks if @web_history_item is in the back forward list
 *
 * Return value: %TRUE if @web_history_item is in the back forward list, %FALSE if it doesn't
 */
bool WebKitWebBackForwardList::contains_item(WebKitWebHistoryItem* webHistoryItem)
{
	WebCore::HistoryItem* historyItem = core(webHistoryItem);

	return (backForwardList->enabled() ? backForwardList->containsItem(historyItem) : FALSE);
}

/**
 * WebKitWebBackForwardList::go_to_item:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 * @history_item: (type WebKit.WebHistoryItem) (transfer none): the #WebKitWebHistoryItem to go to
 *
 * Go to the specified @web_history_item in the back forward list
 */
void WebKitWebBackForwardList::go_to_item(WebKitWebHistoryItem* webHistoryItem)
{
	WebCore::HistoryItem* historyItem = core(webHistoryItem);

	if (backForwardList->enabled() && historyItem)
		backForwardList->goToItem(historyItem);
}

/**
 * WebKitWebBackForwardList::get_forward_list_with_limit:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 * @limit: the number of items to retrieve
 *
 * Returns a list of items that succeed the current item, limited by @limit
 *
 * Return value: (element-type WebKit.WebHistoryItem) (transfer container): a #GList of items succeeding the current item, limited by @limit
 */
vector<WebKitWebHistoryItem*> WebKitWebBackForwardList::get_forward_list_with_limit(int limit)
{
	if (!backForwardList || !backForwardList->enabled())
		return NULL;

	WebCore::HistoryItemVector items;
	vector<WebKitWebHistoryItem*> forwardItems;

	backForwardList->forwardListWithLimit(limit, items);

	const unsigned max = items.size();
	for (unsigned i = 0; i < max; i++) {
		WebKitWebHistoryItem* webHistoryItem = kit(items[i]);
		forwardItems.push_back(webHistoryItem);
	}

	return forwardItems;
}

/**
 * WebKitWebBackForwardList::get_back_list_with_limit:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 * @limit: the number of items to retrieve
 *
 * Returns a list of items that precede the current item, limited by @limit
 *
 * Return value: (element-type WebKit.WebHistoryItem) (transfer container): a #GList of items preceding the current item, limited by @limit
 */
vector<WebKitWebHistoryItem*> WebKitWebBackForwardList::get_back_list_with_limit(int limit)
{
	if (!backForwardList || !backForwardList->enabled())
		return NULL;

	WebCore::HistoryItemVector items;
	vector<WebKitWebHistoryItem*> forwardItems;

	backForwardList->backListWithLimit(limit, items);

	const unsigned max = items.size();
	for (unsigned i = 0; i < max; i++) {
		WebKitWebHistoryItem* webHistoryItem = kit(items[i]);
		forwardItems.push_back(webHistoryItem);
	}

	return forwardItems;
}

/**
 * WebKitWebBackForwardList::get_back_item:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 *
 * Returns the item that precedes the current item
 *
 * Return value: (type WebKit.WebHistoryItem) (transfer none): the #WebKitWebHistoryItem preceding the current item
 */
WebKitWebHistoryItem* WebKitWebBackForwardList::get_back_item()
{
	if (!backForwardList || !backForwardList->enabled())
		return NULL;

	WebCore::HistoryItem* historyItem = backForwardList->backItem();

	return (historyItem ? kit(historyItem) : NULL);
}

/**
 * WebKitWebBackForwardList::get_current_item:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 *
 * Returns the current item.
 *
 * Returns a NULL value if the back forward list is empty
 *
 * Return value: (type WebKit.WebHistoryItem) (transfer none): a #WebKitWebHistoryItem
 */
WebKitWebHistoryItem* WebKitWebBackForwardList::get_current_item()
{
	if (!backForwardList || !backForwardList->enabled())
		return NULL;

	WebCore::HistoryItem* historyItem = backForwardList->currentItem();

	return (historyItem ? kit(historyItem) : NULL);
}

/**
 * WebKitWebBackForwardList::get_forward_item:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 *
 * Returns the item that succeeds the current item.
 *
 * Returns a NULL value if there nothing that succeeds the current item
 *
 * Return value: (type WebKit.WebHistoryItem) (transfer none): a #WebKitWebHistoryItem
 */
WebKitWebHistoryItem* WebKitWebBackForwardList::get_forward_item()
{
	if (!backForwardList || !backForwardList->enabled())
		return NULL;

	WebCore::HistoryItem* historyItem = backForwardList->forwardItem();

	return (historyItem ? kit(historyItem) : NULL);
}

/**
 * WebKitWebBackForwardList::get_nth_item:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 * @index: the index of the item
 *
 * Returns the item at a given index relative to the current item.
 *
 * Return value: (type WebKit.WebHistoryItem) (transfer none): the #WebKitWebHistoryItem located at the specified index relative to the current item
 */
WebKitWebHistoryItem* WebKitWebBackForwardList::get_nth_item(int index)
{
	if (!backForwardList)
		return NULL;

	WebCore::HistoryItem* historyItem = backForwardList->itemAtIndex(index);

	return (historyItem ? kit(historyItem) : NULL);
}

/**
 * WebKitWebBackForwardList::get_back_length:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 *
 * Returns the number of items that preced the current item.
 *
 * Return value: a #int corresponding to the number of items preceding the current item
 */
int WebKitWebBackForwardList::get_back_length()
{
	if (!backForwardList || !backForwardList->enabled())
		return 0;

	return backForwardList->backListCount();
}

/**
 * WebKitWebBackForwardList::get_forward_length:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 *
 * Returns the number of items that succeed the current item.
 *
 * Return value: a #int corresponding to the nuber of items succeeding the current item
 */
int WebKitWebBackForwardList::get_forward_length()
{
	if (!backForwardList || !backForwardList->enabled())
		return 0;

	return backForwardList->forwardListCount();
}

/**
 * WebKitWebBackForwardList::get_limit:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 *
 * Returns the maximum limit of the back forward list.
 *
 * Return value: a #int indicating the number of #WebKitWebHistoryItem the back forward list can hold
 */
int WebKitWebBackForwardList::get_limit()
{
	if (!backForwardList || !backForwardList->enabled())
		return 0;

	return backForwardList->capacity();
}

/**
 * WebKitWebBackForwardList::set_limit:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 * @limit: the limit to set the back forward list to
 *
 * Sets the maximum limit of the back forward list. If the back forward list
 * exceeds its capacity, items will be removed everytime a new item has been
 * added.
 */
void WebKitWebBackForwardList::set_limit(int limit)
{
	if (backForwardList)
		backForwardList->setCapacity(limit);
}

/**
 * WebKitWebBackForwardList::add_item:
 * @web_back_forward_list: a #WebKitWebBackForwardList
 * @history_item: (type WebKit.WebHistoryItem) (transfer none): the #WebKitWebHistoryItem to add
 *
 * Adds the item to the #WebKitWebBackForwardList.
 *
 * The @webBackForwardList will add a reference to the @webHistoryItem, so you
 * don't need to keep a reference once you've added it to the list.
 *
 * Since: 1.1.1
 */
void WebKitWebBackForwardList::add_item(WebKitWebHistoryItem *webHistoryItem)
{
	WebCore::HistoryItem* historyItem = core(webHistoryItem);

	backForwardList->addItem(historyItem);
}

/**
 * WebKitWebBackForwardList::clear:
 * @web_back_forward_list: the #WebKitWebBackForwardList to be cleared
 *
 * Clears the @webBackForwardList by removing all its elements. Note that not even
 * the current page is kept in list when cleared so you would have to add it later.
 * This method also clears the list of visited links which means that all links will
 * appear unvisited.
 *
 * Since: 1.3.1
 **/
void WebKitWebBackForwardList::clear()
{
	if (!backForwardList)
		return;

	WebCore::Page* page = backForwardList->page();
	if (page && page->groupPtr())
		page->groupPtr()->removeVisitedLinks();

	if (!backForwardList->enabled() || !backForwardList->entries().size())
		return;

	// Clear the current list by setting capacity to 0
	int capacity = backForwardList->capacity();
	backForwardList->setCapacity(0);
	backForwardList->setCapacity(capacity);
}

WebCore::BackForwardList* WebKit::core(WebKitWebBackForwardList* webBackForwardList)
{
	return webBackForwardList->priv ? webBackForwardList->priv->backForwardList : 0;
}
