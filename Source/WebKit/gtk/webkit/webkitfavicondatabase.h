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

#ifndef webkitfavicondatabase_h
#define webkitfavicondatabase_h

#include <webkit/webkitdefines.h>

class IconDatabaseClientFLTK;
class PendingIconRequest;

typedef Vector<OwnPtr<PendingIconRequest> > PendingIconRequestVector;
typedef HashMap<String, PendingIconRequestVector*> PendingIconRequestMap;

class WebKitFaviconDatabase {
public:
	WebKitFaviconDatabase();
	~WebKitFaviconDatabase();

	const char *get_path();
	void set_path(const char *in);

	char *get_favicon_uri(const char *page_uri);
	// try_get_favicon_pixbuf(const char *page_uri, unsigned w, unsigned h);

	void clear();
	void dispatchDidReceiveIcon(const char *frame);
private:
	const char *path;
	IconDatabaseClientFLTK *icondbClient;
	PendingIconRequestMap pendingIconRequests;
	bool importFinished;
};

#endif				/* webkitfavicondatabase_h */
