/*
 * Copyright (C) 2008 Jan Michael C. Alonzo
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

#ifndef webkitwebhistoryitem_h
#define webkitwebhistoryitem_h

#include <webkit/webkitdefines.h>

class WebKitWebHistoryItem {
public:
	WebKitWebHistoryItem();
	WebKitWebHistoryItem(const char *uri, const char *title);
	~WebKitWebHistoryItem();

	const char *get_title();
	const char *get_alternate_title();

	void set_alternate_title(const char *title);

	const char *get_uri();
	const char *get_original_uri();

	double get_last_visited_time();
private:
	WebCore::HistoryItem* historyItem;

	WTF::CString title;
	WTF::CString alternateTitle;
	WTF::CString uri;
	WTF::CString originalUri;
};

namespace WebCore {
	class HistoryItem;
};

extern map<WebCore::HistoryItem*, WebKitWebHistoryItem*> globalHistory;

#endif				/* webkitwebhistoryitem_h */
