/*
 * Copyright (C) 2008 Christian Dywan <christian@imendio.com>
 * Copyright (C) 2008 Nuanti Ltd.
 * Copyright (C) 2008 Collabora Ltd.
 * Copyright (C) 2008 Holger Hans Peter Freyther
 * Copyright (C) 2009 Jan Michael Alonzo
 * Copyright (C) 2009 Movial Creative Technologies Inc.
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
#include "webkitwebsettings.h"

#include "EditingBehavior.h"
#include "FileSystem.h"
#include "URL.h"
#include "PluginDatabase.h"
#include "UserAgentFLTK.h"
#include "webkitglobalsprivate.h"
#include "webkitversion.h"
#include <wtf/text/CString.h>
#include <wtf/text/StringConcatenate.h>

/**
 * SECTION:webkitwebsettings
 * @short_description: Control the behaviour of a #WebKitWebView
 *
 * #WebKitWebSettings can be applied to a #WebKitWebView to control text encoding,
 * color, font sizes, printing mode, script support, loading of images and various other things.
 * After creation, a #WebKitWebSettings object contains default settings.
 */

using namespace WebCore;

WebKitWebSettings::WebKitWebSettings() {
	webkitInit();
}

WebKitWebSettings::~WebKitWebSettings() {

}

/**
 * WebKitWebSettings::get_user_agent:
 * @web_settings: a #WebKitWebSettings
 *
 * Returns: the User-Agent string currently used by the web view(s) associated
 * with the @web_settings.
 *
 * Since: 1.1.11
 */
const char* WebKitWebSettings::get_user_agent()
{
	return userAgent.data();
}

/*
 * Private usage only.
 * webkitWebSettingsUserAgentForURI:
 * @web_settings: the #WebKitWebSettings object to query
 * @uri: the URI we want to know the User-Agent for
 *
 * Some web sites have been using User-Agent parsing heavily to decide
 * the kind of content that is sent to the browser. When
 * WebKitWebSettings:enable-site-specific-quirks is enabled WebKitGTK+
 * will use its knowledge of sites doing bad things and lie to them by
 * sending either the default User-Agent, i.e. not using the one
 * specified by the browser in WebKitWebSettings:user-agent, or the
 * Safari one (including lying about the underlying operating system).
 *
 * This function allows the browser to figure out what User-Agent is
 * going to be sent to a particular URI.
 *
 * Please note that if WebKitWebSettings:use-site-specific-quirks is
 * turned off calling this function is the same as calling
 * WebKitWebSettings::get_user_agent(), except you have to free the
 * result.
 *
 * Returns: (transfer full): a newly allocated string containing the
 * User-Agent that will be sent for the given URI.
 */
char* WebKitWebSettings::UserAgentForURI(const char* uri)
{
	if (webSettings->priv->enableSiteSpecificQuirks) {
		String userAgentString = userAgentForURL(WebCore::URL(WebCore::URL(), String::fromUTF8(uri)));
		if (!userAgentString.isEmpty())
			return strdup(userAgentString.utf8().data());
	}

	return strdup(WebKitWebSettings::get_user_agent(webSettings));
}

namespace WebKit {

WebCore::EditingBehaviorType core(WebKitEditingBehavior type)
{
	return static_cast<WebCore::EditingBehaviorType>(type);
}

}
