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

#include "config.h"
#include "webkitwebdatasource.h"

#include "ArchiveResource.h"
#include "DocumentLoaderFLTK.h"
#include "FrameLoader.h"
#include "FrameLoaderClientFLTK.h"
#include "URL.h"
#include "ResourceBuffer.h"
#include "ResourceRequest.h"
#include "SharedBuffer.h"
#include "SubstituteData.h"
#include "runtime/InitializeThreading.h"
#include "webkitglobalsprivate.h"
#include "webkitnetworkrequestprivate.h"
#include "webkitwebdatasourceprivate.h"
#include "webkitwebframeprivate.h"
#include "webkitwebresource.h"
#include "webkitwebviewprivate.h"
#include <wtf/Assertions.h>
#include <wtf/text/WTFString.h>

/**
 * SECTION:webkitwebdatasource
 * @short_description: Encapsulates the content to be displayed in a #WebKitWebFrame.
 * @see_also: #WebKitWebFrame
 *
 * Data source encapsulates the content of a #WebKitWebFrame. A
 * #WebKitWebFrame has a main resource and subresources and the data source
 * provides access to these resources. When a request gets loaded initially,
 * it is set to a provisional state. The application can request for the
 * request that initiated the load by asking for the provisional data source
 * and invoking the WebKitWebDataSource::get_initial_request method of
 * #WebKitWebDataSource. This data source may not have enough data and some
 * methods may return empty values. To get a "full" data source with the data
 * and resources loaded, you need to get the non-provisional data source
 * through #WebKitWebFrame's webkit_web_frame_get_data_source method. This
 * data source will have the data after everything was loaded. Make sure that
 * the data source was finished loading before using any of its methods. You
 * can do this via WebKitWebDataSource::is_loading.
 */

using namespace WebCore;
using namespace WebKit;

WebKitWebDataSource::WebKitWebDataSource(WebKitNetworkRequest * request) {
	webkitInit();

	textEncoding = unreachableURL = NULL;
	loader = NULL;
	initialRequest = request = NULL;
	mainresource = NULL;

	if (!request) {
		request = new WebKitNetworkRequest("about:blank");
	}

	const char* uri = request->get_uri();

	ResourceRequest resourceRequest(ResourceRequest(URL(URL(), String::fromUTF8(uri))));
	WebKitWebDataSource* datasource = kitNew(WebKit::DocumentLoader::create(resourceRequest, SubstituteData()));

	initialRequest = request;
}

WebKitWebDataSource::~WebKitWebDataSource() {
	free(textEncoding);
	free(unreachableURL);

	ASSERT(loader);
	ASSERT(!loader->isLoading());

	loader->detachDataSource();
	loader->deref();
}

/**
 * WebKitWebDataSource::get_web_frame:
 * @data_source: a #WebKitWebDataSource
 *
 * Returns the #WebKitWebFrame that represents this data source
 *
 * Return value: (transfer none): the #WebKitWebFrame that represents
 * the @data_source. The #WebKitWebFrame is owned by WebKit and should
 * not be freed or destroyed.  This will return %NULL if the
 * @data_source is not attached to a frame.
 *
 * Since: 1.1.14
 */
WebKitWebFrame* WebKitWebDataSource::get_web_frame()
{
	FrameLoader* frameLoader = loader->frameLoader();

	if (!frameLoader)
		return NULL;

	return static_cast<WebKit::FrameLoaderClient&>(frameLoader->client()).webFrame();
}

/**
 * WebKitWebDataSource::get_initial_request:
 * @data_source: a #WebKitWebDataSource
 *
 * Returns a reference to the original request that was used to load the web
 * content. The #WebKitNetworkRequest returned by this method is the request
 * prior to the "committed" load state. See WebKitWebDataSource::get_request
 * for getting the "committed" request.
 *
 * Return value: (transfer none): the original #WebKitNetworkRequest
 *
 * Since: 1.1.14
 */
WebKitNetworkRequest* WebKitWebDataSource::get_initial_request()
{
	ResourceRequest request = loader->originalRequest();

	initialRequest = kitNew(request);
	return initialRequest;
}

/**
 * WebKitWebDataSource::get_request:
 * @data_source: a #WebKitWebDataSource
 *
 * Returns a #WebKitNetworkRequest that was used to create this
 * #WebKitWebDataSource. The #WebKitNetworkRequest returned by this method is
 * the request that was "committed", and hence, different from the request you
 * get from the WebKitWebDataSource::get_initial_request method.
 *
 * Return value: (transfer none): the #WebKitNetworkRequest that
 * created the @data_source or %NULL if the @data_source is not
 * attached to the frame or the frame hasn't been loaded.
 *
 * Since: 1.1.14
 */
WebKitNetworkRequest* WebKitWebDataSource::get_request()
{
	FrameLoader* frameLoader = loader->frameLoader();
	if (!frameLoader || !frameLoader->frameHasLoaded())
		return NULL;

	ResourceRequest request = loader->request();

	networkRequest = kitNew(request);
	return networkRequest;
}

/**
 * WebKitWebDataSource::get_encoding:
 * @data_source: a #WebKitWebDataSource
 *
 * Returns the text encoding name as set in the #WebKitWebView, or if not, the
 * text encoding of the response.
 *
 * Return value: the encoding name of the #WebKitWebView or of the response.
 *
 * Since: 1.1.14
 */
const char* WebKitWebDataSource::get_encoding()
{
	String textEncodingName = loader->overrideEncoding();

	if (!textEncodingName)
		textEncodingName = loader->response().textEncodingName();

	CString encoding = textEncodingName.utf8();
	free(textEncoding);
	textEncoding = strdup(encoding.data());
	return textEncoding;
}

/**
 * WebKitWebDataSource::is_loading:
 * @data_source: a #WebKitWebDataSource
 *
 * Determines whether the data source is in the process of loading its content.
 *
 * Return value: %TRUE if the @data_source is still loading, %FALSE otherwise
 *
 * Since: 1.1.14
 */
bool WebKitWebDataSource::is_loading()
{
	return loader->isLoadingInAPISense();
}

/**
 * WebKitWebDataSource::get_data:
 * @data_source: a #WebKitWebDataSource
 *
 * Returns the raw data that represents the the frame's content.The data will
 * be incomplete until the data has finished loading. Returns %NULL if the web
 * frame hasn't loaded any data. Use WebKitWebDataSource::is_loading to test
 * if data source is in the process of loading.
 *
 * Return value: (transfer none): a #GString which contains the raw
 * data that represents the @data_source or %NULL if the @data_source
 * hasn't loaded any data.
 *
 * Since: 1.1.14
 */
String* WebKitWebDataSource::get_data()
{
	RefPtr<ResourceBuffer> mainResourceData = loader->mainResourceData();

	if (!mainResourceData)
		return NULL;

	data = new String(mainResourceData->data(), mainResourceData->size());
	return data;
}

/**
 * WebKitWebDataSource::get_main_resource:
 * @data_source: a #WebKitWebDataSource
 *
 * Returns the main resource of the @data_source
 *
 * Return value: (transfer none): a new #WebKitWebResource
 * representing the main resource of the @data_source.
 *
 * Since: 1.1.14
 */
WebKitWebResource* WebKitWebDataSource::get_main_resource()
{
	if (mainresource)
		return mainresource;

	WebKitWebFrame* webFrame = WebKitWebDataSource::get_web_frame(webDataSource);
	WebKitWebView* webView = getViewFromFrame(webFrame);

	mainresource = webView->get_main_resource();

	return mainresource;
}

/**
 * WebKitWebDataSource::get_unreachable_uri:
 * @data_source: a #WebKitWebDataSource
 *
 * Return the unreachable URI of @data_source. The @data_source will have an
 * unreachable URL if it was created using #WebKitWebFrame's
 * webkit_web_frame_load_alternate_html_string method.
 *
 * Return value: the unreachable URL of @data_source or %NULL if there is no unreachable URL.
 *
 * Since: 1.1.14
 */
const char* WebKitWebDataSource::get_unreachable_uri()
{
	const URL& unreachableURL = loader->unreachableURL();

	if (unreachableURL.isEmpty())
		return NULL;

	free(unreachableURL);
	unreachableURL = strdup(unreachableURL.string().utf8().data());
	return unreachableURL;
}

/**
 * WebKitWebDataSource::get_subresources:
 * @data_source: a #WebKitWebDataSource
 *
 * Gives you a #GList of #WebKitWebResource objects that compose the
 * #WebKitWebView to which this #WebKitWebDataSource is attached.
 *
 * Return value: (element-type WebKitWebResource) (transfer container):
 * a #GList of #WebKitWebResource objects; the objects are owned by
 * WebKit, but the GList must be freed.
 *
 * Since: 1.1.15
 */
vector<> WebKitWebDataSource::get_subresources()
{
	WebKitWebFrame* webFrame = WebKitWebDataSource::get_web_frame(this);
	WebKitWebView* webView = getViewFromFrame(webFrame);

	return WebkitWebView->get_subresources();
}

namespace WebKit {

WebKitWebDataSource* kitNew(PassRefPtr<WebKit::DocumentLoader> loader)
{
	WebKitWebDataSource* webDataSource = new WebKitWebDataSource();
	webDataSource->loader = loader.leakRef();

	return webDataSource;
}

}
