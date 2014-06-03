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

#ifndef webkitwebresource_h
#define webkitwebresource_h

#include <webkit/webkitdefines.h>

namespace WebCore {
	class ArchiveResource;
}

class WebKitWebResource {
public:
	WebKitWebResource();
	WebKitWebResource(PassRefPtr<ArchiveResource> resource);
	WebKitWebResource(const char *data, ssize_t size,
				const char *uri,
				const char *mime_type,
				const char *encoding,
				const char *frame_name);
	~WebKitWebResource();

	String *get_data();
	const char *get_uri();
	const char *get_mime_type();
	const char *get_encoding();
	const char *get_frame_name();

	WebCore::ArchiveResource * resource;
private:

	char *uri;
	char *mimeType;
	char *textEncoding;
	char *frameName;

	String *data;
};

#endif				/* webkitwebresource_h */
