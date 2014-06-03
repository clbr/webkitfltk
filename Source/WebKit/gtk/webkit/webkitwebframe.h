/*
 * Copyright (C) 2007 Holger Hans Peter Freyther
 * Copyright (C) 2007 Alp Toker <alp@atoker.com>
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

#ifndef webkitwebframe_h
#define webkitwebframe_h

#include <JavaScriptCore/JSBase.h>
#include <webkit/webkitdefines.h>
#include <webkit/webkitnetworkrequest.h>
#include <webkit/webkitnetworkresponse.h>
#include <webkit/webkitwebdatasource.h>
#include <webkit/webkitsecurityorigin.h>

class WebKitWebView;

/**
 * WebKitLoadStatus:
 * @WEBKIT_LOAD_PROVISIONAL: No data has been received yet, empty
 * structures have been allocated to perform the load; the load may
 * still fail for transport issues such as not being able to resolve a
 * name, or connect to a port.
 * @WEBKIT_LOAD_COMMITTED: The first data chunk has arrived, meaning
 * that the necessary transport requirements are stabilished, and the
 * load is being performed.
 * @WEBKIT_LOAD_FIRST_VISUALLY_NON_EMPTY_LAYOUT: The first layout with
 * actual visible content happened; one or more layouts may have
 * happened before that caused nothing to be visible on the screen,
 * because the data available at the time was not significant enough.
 * @WEBKIT_LOAD_FINISHED: This state means that everything that was
 * required to display the page has been loaded.
 * @WEBKIT_LOAD_FAILED: This state means that some error occurred
 * during the page load that prevented it from being completed. You
 * can connect to the #WebKitWebView::load-error signal if you want to
 * know precisely what kind of error occurred.
 */
typedef enum {
	WEBKIT_LOAD_PROVISIONAL,
	WEBKIT_LOAD_COMMITTED,
	WEBKIT_LOAD_FINISHED,
	WEBKIT_LOAD_FIRST_VISUALLY_NON_EMPTY_LAYOUT,
	WEBKIT_LOAD_FAILED
} WebKitLoadStatus;

namespace WebCore {
	class Frame;
}

class WebKitWebFrame {
public:
	WebKitWebFrame();
	~WebKitWebFrame();

	WebKitWebView *get_web_view();

	const char *get_name();
	const char *get_title();
	const char *get_uri();

	WebKitWebFrame *get_parent();

	void load_uri(const char *uri);

	void load_string(const char *content, const char *mime_type,
			const char *encoding, const char *base_uri);

	void load_alternate_string(const char *content,
					const char *base_url,
					const char *unreachable_url);

	void load_request(WebKitNetworkRequest * request);

	void stop_loading();
	void reload();
	void print();

	WebKitWebFrame *find_frame(const char *name);

	JSGlobalContextRef get_global_context();

	WebKitLoadStatus get_load_status();

	FLTKPolicyType get_horizontal_scrollbar_policy();
	FLTKPolicyType get_vertical_scrollbar_policy();

	WebKitWebDataSource *get_data_source();
	WebKitWebDataSource *get_provisional_data_source();

	WebKitSecurityOrigin *get_security_origin();

	WebKitNetworkResponse *get_network_response();

	void replace_selection(const char *text);

	WebKitDOMRange *get_range_for_word_around_caret();
	WebKitDOMDocument *get_dom_document();

	char *get_response_mime_type();
	void core_frame_gone();
private:
	WebCore::Frame * coreFrame;
	WebKitWebView *webView;

	char *name;
	char *title;
	char *uri;
	WebKitLoadStatus loadStatus;
	WebKitSecurityOrigin *origin;
};

#endif
