/*
 * Copyright (C) 2008 Gustavo Noronha Silva
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

#ifndef webkitwebinspector_h
#define webkitwebinspector_h

#include <webkit/webkitdefines.h>

namespace WebCore {
	class Page;
}

class WebKitWebInspector {
public:
	WebKitWebInspector();
	~WebKitWebInspector();

	WebKitWebView *get_web_view();

	const char *get_inspected_uri();

	void inspect_node(WebKitDOMNode * node);
	void inspect_coordinates(double x, double y);

	void show();
	void close();

	void set_inspector_client(WebCore::Page *);

	void set_web_view(WebKitWebView *);

	void set_inspected_uri(const char *);

	void execute_script(long callId, const char *script);
private:
	WebCore::Page* page;
	WebKitWebView* inspector_view;
	char* inspected_uri;
};

#endif				/* webkitwebinspector_h */
