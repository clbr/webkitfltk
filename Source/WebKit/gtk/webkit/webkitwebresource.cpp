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
#include "webkitwebresource.h"

#include "ArchiveResource.h"
#include "URL.h"
#include "SharedBuffer.h"
#include "webkitglobalsprivate.h"
#include "webkitnetworkresponse.h"
#include <wtf/Assertions.h>
#include <wtf/text/CString.h>
#include <wtf/text/WTFString.h>

/**
 * SECTION:webkitwebresource
 * @short_description: Represents a downloaded URI.
 * @see_also: #WebKitWebDataSource
 *
 * A web resource encapsulates the data of the download as well as the URI,
 * MIME type and frame name of the resource.
 */

using namespace WebCore;

WebKitWebResource::~WebKitWebResource()
{
	free(uri);
	free(mimeType);
	free(textEncoding);
	free(frameName);
}

WebKitWebResource::WebKitWebResource() {
	uri = mimeType = textEncoding = frameName = NULL;
}

// internal use only
WebKitWebResource::WebKitWebResource(PassRefPtr<ArchiveResource> resource)
{
	this->resource = resource.leakRef();
}

/**
 * WebKitWebResource::new:
 * @data: the data to initialize the #WebKitWebResource
 * @size: the length of @data
 * @uri: the URI of the #WebKitWebResource
 * @mime_type: the MIME type of the #WebKitWebResource
 * @encoding: the text encoding name of the #WebKitWebResource
 * @frame_name: the frame name of the #WebKitWebResource
 *
 * Returns a new #WebKitWebResource. The @encoding can be %NULL. The
 * @frame_name argument can be used if the resource represents contents of an
 * entire HTML frame, otherwise pass %NULL.
 *
 * Return value: a new #WebKitWebResource
 *
 * Since: 1.1.14
 */
WebKitWebResource::WebKitWebResource(const char* data,
					ssize_t size,
					const char* uri,
					const char* mimeType,
					const char* encoding,
					const char* frameName)
{
	ASSERT(data && uri && mimeType);

	if (size < 0)
		size = strlen(data);

	RefPtr<SharedBuffer> buffer = SharedBuffer::create(data, size);
	resource = ArchiveResource::create(buffer, URL(URL(), String::fromUTF8(uri)), String::fromUTF8(mimeType), String::fromUTF8(encoding), String::fromUTF8(frameName));
}

/**
 * WebKitWebResource::get_data:
 * @web_resource: a #WebKitWebResource
 *
 * Returns the data of the @webResource.
 *
 * Return value: (transfer none): a #GString containing the character
 * data of the @webResource.  The string is owned by WebKit and should
 * not be freed or destroyed.
 *
 * Since: 1.1.14
 */
String* WebKitWebResource::get_data()
{
	if (!data)
		data = new String(resource->data()->data(), resource->data()->len());

	return data;
}

/**
 * WebKitWebResource::get_uri:
 * @web_resource: a #WebKitWebResource
 *
 * Return value: the URI of the resource
 *
 * Since: 1.1.14
 */
const char* WebKitWebResource::get_uri()
{
	// We may have an URI without having a resource assigned to us (e.g., if the
	// FrameLoaderClient only had a ResourceRequest when we got created
	if (uri)
		return uri;

	if (!resource)
		return NULL;

	uri = strdup(resource->url().string().utf8().data());

	return uri;
}

/**
 * WebKitWebResource::get_mime_type:
 * @web_resource: a #WebKitWebResource
 *
 * Return value: the MIME type of the resource
 *
 * Since: 1.1.14
 */
const char* WebKitWebResource::get_mime_type()
{
	if (!resource)
		return NULL;

	if (!mimeType)
		mimeType = strdup(resource->mimeType().utf8().data());

	return mimeType;
}

/**
 * WebKitWebResource::get_encoding:
 * @web_resource: a #WebKitWebResource
 *
 * Return value: the encoding name of the resource
 *
 * Since: 1.1.14
 */
const char* WebKitWebResource::get_encoding()
{
	if (!resource)
		return NULL;

	if (!textEncoding)
		textEncoding = strdup(resource->textEncoding().utf8().data());

	return textEncoding;
}

/**
 * WebKitWebResource::get_frame_name:
 * @web_resource: a #WebKitWebResource
 *
 * Return value: the frame name of the resource.
 *
 * Since: 1.1.14
 */
const char* WebKitWebResource::get_frame_name()
{
	if (!resource)
		return NULL;

	if (!frameName)
		frameName = strdup(resource->frameName().utf8().data());

	return frameName;
}

