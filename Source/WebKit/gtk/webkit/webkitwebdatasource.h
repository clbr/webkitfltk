/*
 * Copyright (C) 2009 Jan Michael C. Alonzo
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

#ifndef webkitwebdatasource_h
#define webkitwebdatasource_h

#include <webkit/webkitdefines.h>
#include <webkit/webkitwebframe.h>
#include <webkit/webkitwebresource.h>
#include <webkit/webkitnetworkrequest.h>
#include <vector>

namespace WebKit {
	class DocumentLoader;
}

class WebKitWebDataSource {
public:
	WebKitWebDataSource(WebKitNetworkRequest * request = NULL);
	~WebKitWebDataSource();

	bool is_loading;

	WebKitWebFrame *get_web_frame();

	WebKitNetworkRequest *get_initial_request();
	WebKitNetworkRequest *get_request();

	const char *get_encoding();


	String *get_data();

	WebKitWebResource *get_main_resource();

	const char *get_unreachable_uri();

	std::vector<> get_subresources();

	WebKit::DocumentLoader* loader;
private:

	WebKitNetworkRequest* initialRequest;
	WebKitNetworkRequest* networkRequest;
	WebKitWebResource* mainresource;

	String* data;

	char* textEncoding;
	char* unreachableURL;
};

#endif				/* webkitwebdatasource_h */
