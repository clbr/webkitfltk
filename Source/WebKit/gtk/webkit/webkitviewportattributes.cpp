/*
 * Copyright (C) 2010 Joone Hur <joone@kldp.org>
 * Copyright (C) 2010 Collabora Ltd.
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
#include "webkitviewportattributes.h"

#include "Chrome.h"
#include "Document.h"
#include "MainFrame.h"
#include "Page.h"
#include "webkitglobalsprivate.h"
#include "webkitwebviewprivate.h"

/**
 * SECTION:webkitviewportattributes
 * @short_description: Represents the viewport properties of a web page
 * @see_also: #WebKitWebView::viewport-attributes-recompute-requested, #WebKitWebView::viewport-attributes-changed
 *
 * #WebKitViewportAttributes offers the viewport properties to user agents to
 * control the viewport layout. It contains the viewport size, initial scale with limits,
 * and information about whether a user is able to scale the contents in the viewport.
 * This makes a web page fit the device screen.
 *
 * The #WebKitWebView::viewport-attributes-changed signal will be emitted with #WebKitViewportAttributes
 * when the viewport attributes are updated in the case of loading web pages contain
 * the viewport properties and calling webkit_viewport_attributes_recompute.
 *
 * If the device size, available size, desktop width, or device DPI needs to be changed due to
 * a consequence of an explicit browser request (caused by screen rotation, resizing, or similar reasons),
 * You should call #webkit_viewport_attributes_recompute to recompute the viewport properties and
 * override those values in the handler of #WebKitWebView::viewport-attributes-recompute-requested signal.
 *
 * For more information on the viewport properties, refer to the Safari reference library at
 * http://developer.apple.com/safari/library/documentation/appleapplications/reference/safarihtmlref/articles/metatags.html
 *
 */

using namespace WebKit;
using namespace WebCore;

WebKitViewportAttributes::WebKitViewportAttributes() {
	deviceWidth = 0;
	deviceHeight = 0;
	availableWidth = 0;
	availableHeight = 0;
	desktopWidth = 980; // This value works well for most web pages designed for desktop browsers.
	deviceDPI = 160; // It is the dpi of the original iPhone and Android devices.
	width = 0;
	height = 0;
	initialScaleFactor = -1;
	minimumScaleFactor = -1;
	maximumScaleFactor = -1;
	devicePixelRatio = -1;
	userScalable = TRUE;
	isValid = FALSE;
}

void WebKitViewportAttributes::recompute() {

	IntRect windowRect(webView->corePage->chrome().windowRect());
	deviceWidth = windowRect.width();
	deviceHeight = windowRect.height();

	IntRect rect(webView->corePage->chrome().pageRect());
	availableWidth = rect.width();
	availableHeight = rect.height();

	ViewportArguments arguments = webView->corePage->mainFrame().document()->viewportArguments();

	float devicePixelRatio = deviceDPI / ViewportArguments::deprecatedTargetDPI;
	ViewportAttributes attributes = computeViewportAttributes(arguments, desktopWidth, deviceWidth, deviceHeight, devicePixelRatio, IntSize(availableWidth, availableHeight));
	restrictMinimumScaleFactorToViewportSize(attributes, IntSize(availableWidth, availableHeight), devicePixelRatio);
	restrictScaleFactorToInitialScaleIfNotUserScalable(attributes);

	width = attributes.layoutSize.width();
	height = attributes.layoutSize.height();
	initialScaleFactor = attributes.initialScale;
	minimumScaleFactor = attributes.minimumScale;
	maximumScaleFactor = attributes.maximumScale;
	devicePixelRatio = devicePixelRatio;
	userScalable = static_cast<bool>(arguments.userZoom);
}
