/*
 * Copyright (C) 2008 Jan Michael C. Alonzo <jmalonzo@unpluggable.com>
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

#ifndef webkitwebbackforwardlist_h
#define webkitwebbackforwardlist_h

#include <webkit/webkitdefines.h>
#include <webkit/webkitwebhistoryitem.h>

namespace WebCore {
	class BackForwardList;
}

class WebKitWebBackForwardList {
public:
	WebKitWebBackForwardList(WebKitWebView*);
	~WebKitWebBackForwardList();

	void go_forward();
	void go_back();

	bool contains_item(WebKitWebHistoryItem * history_item);

	void go_to_item(WebKitWebHistoryItem * history_item);

	vector<WebKitWebHistoryItem*> get_forward_list_with_limit(int limit);
	vector<WebKitWebHistoryItem*> get_back_list_with_limit(int limit);

	WebKitWebHistoryItem *get_back_item();
	WebKitWebHistoryItem *get_current_item();
	WebKitWebHistoryItem *get_forward_item();
	WebKitWebHistoryItem *get_nth_item(int index);

	int get_back_length();
	int get_forward_length();

	int get_limit();
	void set_limit(int limit);
	void add_item(WebKitWebHistoryItem * history_item);

	void clear();

private:
	WebCore::BackForwardList* backForwardList;
};

#endif				/* webkitwebbackforwardlist_h */
