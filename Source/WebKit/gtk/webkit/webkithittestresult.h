/*
 * Copyright (C) 2009 Collabora Ltd.
 * Copyright (C) 2009 Igalia S.L.
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

#ifndef webkithittestresult_h
#define webkithittestresult_h

#include <webkit/webkitdefines.h>

/**
 * WebKitHitTestResultContext:
 * @WEBKIT_HIT_TEST_RESULT_CONTEXT_DOCUMENT: anywhere in the document.
 * @WEBKIT_HIT_TEST_RESULT_CONTEXT_LINK: a hyperlink element.
 * @WEBKIT_HIT_TEST_RESULT_CONTEXT_IMAGE: an image element.
 * @WEBKIT_HIT_TEST_RESULT_CONTEXT_MEDIA: a video or audio element.
 * @WEBKIT_HIT_TEST_RESULT_CONTEXT_SELECTION: the area is selected by
 * the user.
 * @WEBKIT_HIT_TEST_RESULT_CONTEXT_EDITABLE: the area is
 * editable by the user.
 */
typedef enum {
	WEBKIT_HIT_TEST_RESULT_CONTEXT_DOCUMENT = 1 << 1,
	WEBKIT_HIT_TEST_RESULT_CONTEXT_LINK = 1 << 2,
	WEBKIT_HIT_TEST_RESULT_CONTEXT_IMAGE = 1 << 3,
	WEBKIT_HIT_TEST_RESULT_CONTEXT_MEDIA = 1 << 4,
	WEBKIT_HIT_TEST_RESULT_CONTEXT_SELECTION = 1 << 5,
	WEBKIT_HIT_TEST_RESULT_CONTEXT_EDITABLE = 1 << 6,
} WebKitHitTestResultContext;

namespace WebCore {
	class Node;
}

class WebKitHitTestResult {
public:
	WebKitHitTestResult();
	WebKitHitTestResult(char*, char*, char*, unsigned, WebCore::Node*, int x, int y);
	~WebKitHitTestResult();

	unsigned context;
	char* linkURI;
	char* imageURI;
	char* mediaURI;
	WebCore::Node *innerNode;
	int x;
	int y;
};

#endif
