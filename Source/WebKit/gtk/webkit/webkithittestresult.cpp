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

#include "config.h"
#include "webkithittestresult.h"

#include "Frame.h"
#include "FrameView.h"
#include "HitTestResult.h"
#include "URL.h"
#include "webkitglobals.h"
#include "webkitglobalsprivate.h"
#include <wtf/text/CString.h>

/**
 * SECTION:webkithittestresult
 * @short_description: The target of a mouse event
 *
 * This class holds context information about the coordinates
 * specified by a GDK event.
 */

WebKitHitTestResult::WebKitHitTestResult() {
	linkURI = imageURI = mediaURI = NULL;
	innerNode = NULL;
	context = 0;
	x = y = 0;

	webkitInit();
}

WebKitHitTestResult::WebKitHitTestResult(char* l, char* i, char* m,
					unsigned c, WebCore::Node* n, int x, int y) {
	WebKitHitTestResult();

	linkURI = l;
	imageURI = i;
	mediaURI = m;
	context = c;
	innerNode = n;
	this->x = x;
	this->y = y;
}

WebKitHitTestResult::~WebKitHitTestResult() {
	free(linkURI);
	free(imageURI);
	free(mediaURI);
}

namespace WebKit {

WebKitHitTestResult* kit(const WebCore::HitTestResult& result)
{
	unsigned context = WEBKIT_HIT_TEST_RESULT_CONTEXT_DOCUMENT;
	char *linkURI;
	char *imageURI;
	char *mediaURI;
	WebCore::Node* node = 0;
	WebCore::Frame* innerNodeFrame;
	WebCore::IntPoint point;

	if (!result.absoluteLinkURL().isEmpty()) {
		context |= WEBKIT_HIT_TEST_RESULT_CONTEXT_LINK;
		linkURI = strdup(result.absoluteLinkURL().string().utf8().data());
	}

	if (!result.absoluteImageURL().isEmpty()) {
		context |= WEBKIT_HIT_TEST_RESULT_CONTEXT_IMAGE;
		imageURI = strdup(result.absoluteImageURL().string().utf8().data());
	}

	if (!result.absoluteMediaURL().isEmpty()) {
		context |= WEBKIT_HIT_TEST_RESULT_CONTEXT_MEDIA;
		mediaURI = strdup(result.absoluteMediaURL().string().utf8().data());
	}

	if (result.isSelected())
		context |= WEBKIT_HIT_TEST_RESULT_CONTEXT_SELECTION;

	if (result.isContentEditable())
		context |= WEBKIT_HIT_TEST_RESULT_CONTEXT_EDITABLE;

	if (result.innerNonSharedNode())
		node = result.innerNonSharedNode();

	innerNodeFrame = result.innerNodeFrame();
	if (innerNodeFrame && innerNodeFrame->view()) {
		// Convert document coords to widget coords.
		point = innerNodeFrame->view()->contentsToWindow(result.roundedPointInInnerNodeFrame());
	} else {
		// FIXME: Main frame coords is not the same as window coords,
		// but we do not have pointer to  mainframe view here.
		point = result.roundedPointInMainFrame();
	}

	return new WebKitHitTestResult(linkURI, imageURI, mediaURI, context, node,
					point.x(), point.y());
}

}
