/*
 * Copyright (C) 2007, 2008 Holger Hans Peter Freyther
 * Copyright (C) 2009 Gustavo Noronha Silva
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
#include "webkitnetworkrequest.h"

#include "ResourceRequest.h"
#include "webkitglobalsprivate.h"
#include <wtf/text/CString.h>

/**
 * SECTION:webkitnetworkrequest
 * @short_description: The target of a navigation request
 * @see_also: #WebKitWebView::navigation-policy-decision-requested
 *
 * This class represents the network related aspects of a navigation
 * request. It is used whenever WebKit wants to provide information
 * about a request that will be sent, or has been sent. Inside it you
 * can find the URI of the request, and, for valid URIs, a
 * #SoupMessage object, which provides access to further information
 * such as headers.
 *
 */

WebKitNetworkRequest::WebKitNetworkRequest(const char *in) {
	uri = strdup(in);
	webkitInit();
}

WebKitNetworkRequest::~WebKitNetworkRequest() {
	free(uri);
}

WebKitNetworkRequest::set_uri(const char *in) {

	free(uri);
	uri = strdup(in);
/*
	if (!priv->message)
		return;

	SoupURI* soupURI = soup_uri_new(uri);
	g_return_if_fail(soupURI);

	soup_message_set_uri(priv->message, soupURI);
	soup_uri_free(soupURI);*/
}

const char* WebKitNetworkRequest::get_uri()
{
	if (uri)
		return uri;
/*
	SoupURI* soupURI = soup_message_get_uri(priv->message);
	priv->uri = soup_uri_to_string(soupURI, FALSE);
	return priv->uri;*/
}

namespace WebKit {

WebKitNetworkRequest* kitNew(const WebCore::ResourceRequest& resourceRequest)
{
/*
	GRefPtr<SoupMessage> soupMessage(adoptGRef(resourceRequest.toSoupMessage()));
	if (soupMessage)
		return WEBKIT_NETWORK_REQUEST(g_object_new(WEBKIT_TYPE_NETWORK_REQUEST, "message", soupMessage.get(), NULL));
*/
	return new WebKitNetworkRequest(resourceRequest.url().string().utf8().data());
}

WebCore::ResourceRequest core(WebKitNetworkRequest* request)
{
/*
	SoupMessage* soupMessage = webkit_network_request_get_message(request);
	if (soupMessage)
		return WebCore::ResourceRequest(soupMessage);
*/
	WebCore::URL url = WebCore::URL(WebCore::URL(),
				String::fromUTF8(request->get_uri()));
	return WebCore::ResourceRequest(url);
}

}
