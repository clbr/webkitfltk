/*
 * Copyright (C) 2008 Gustavo Noronha Silva <gns@gnome.org>
 * Copyright (C) 2008 Holger Hans Peter Freyther
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
#include "webkitwebwindowfeatures.h"

#include "WindowFeatures.h"
#include "webkitglobalsprivate.h"
#include "webkitwebwindowfeaturesprivate.h"

/**
 * SECTION:webkitwebwindowfeatures
 * @short_description: Window properties of a #WebKitWebView
 * @see_also: #WebKitWebView::web-view-ready
 *
 * The content of a #WebKitWebView can request to change certain
 * properties of a #WebKitWebView. This can include the x, y position
 * of the window, the width and height but also if a toolbar,
 * scrollbar, statusbar, locationbar should be visible to the user,
 * the request to show the #WebKitWebView fullscreen.
 *
 * In the normal case one will use #webkit_web_view_get_window_features
 * to get the #WebKitWebWindowFeatures and then monitor the property
 * changes. Be aware that the #WebKitWebWindowFeatures might change
 * before #WebKitWebView::web-view-ready signal is emitted.
 * To be safe listen to the notify::window-features signal of the #WebKitWebView
 * and reconnect the signals whenever the #WebKitWebWindowFeatures of
 * a #WebKitWebView changes.
 */

WebKitWebWindowFeatures::WebKitWebWindowFeatures() {
	webkitInit();
}

WebKitWebWindowFeatures::~WebKitWebWindowFeatures() {

}

namespace WebKit {

WebKitWebWindowFeatures* kitNew(const WebCore::WindowFeatures& features)
{
	return new WebKitWebWindowFeatures;
}

}
