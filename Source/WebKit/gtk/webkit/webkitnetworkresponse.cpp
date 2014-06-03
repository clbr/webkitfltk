/*
 * Copyright (C) 2007, 2008 Holger Hans Peter Freyther
 * Copyright (C) 2009 Gustavo Noronha Silva
 * Copyright (C) 2009 Collabora Ltd.
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
#include "webkitnetworkresponse.h"

#include "ResourceResponse.h"
#include "webkitglobalsprivate.h"
#include "webkitnetworkresponseprivate.h"
#include <wtf/text/CString.h>

using namespace WebKit;

namespace WebKit {

WebCore::ResourceResponse core(WebKitNetworkResponse* response)
{
/*	SoupMessage* soupMessage = webkit_network_response_get_message(response);
	if (soupMessage)
		return WebCore::ResourceResponse(soupMessage);
*/
	return WebCore::ResourceResponse();
}

WebKitNetworkResponse* kitNew(const WebCore::ResourceResponse& resourceResponse)
{
/*
	GRefPtr<SoupMessage> soupMessage(adoptGRef(resourceResponse.toSoupMessage()));
	if (soupMessage)
		return WEBKIT_NETWORK_RESPONSE(g_object_new(WEBKIT_TYPE_NETWORK_RESPONSE, "message", soupMessage.get(), NULL));

	return WEBKIT_NETWORK_RESPONSE(g_object_new(WEBKIT_TYPE_NETWORK_RESPONSE, "uri", resourceResponse.url().string().utf8().data(), NULL));
	*/
}

} // ns webkit

WebKitNetworkResponse::WebKitNetworkResponse(const char *uri) {
	webkitInit();
	suggestedFilename = NULL;
	this->uri = strdup(uri);
}

WebKitNetworkResponse::set_uri(const char *uri) {
	if (this->uri)
		free(this->uri);
	this->uri = strdup(uri);

/*
	if (!priv->message)
		return;

	SoupURI* soupURI = soup_uri_new(uri);
	g_return_if_fail(soupURI);

	soup_message_set_uri(priv->message, soupURI);
	soup_uri_free(soupURI);*/
}

WebKitNetworkResponse::get_uri() {
	if (uri)
		return uri;
/*
	SoupURI* soupURI = soup_message_get_uri(priv->message);
	uri = soup_uri_to_string(soupURI, FALSE);
	return uri;*/
}

const char *WebKitNetworkResponse::get_suggested_filename() {

	if (suggestedFilename)
		return suggestedFilename;

	WebCore::ResourceResponse coreResponse = core(response);
	suggestedFilename = strdup(coreResponse.suggestedFilename().utf8().data());
	suggestedFilename;
}

WebKitNetworkResponse::~WebKitNetworkResponse() {
	free((void*)uri);
	free((void*)suggestedFilename);
}
