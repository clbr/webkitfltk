/*
 * Copyright (C) 2008 Collabora Ltd.
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
#include "webkitwebnavigationaction.h"

#include "FrameLoaderTypes.h"
#include "webkitglobalsprivate.h"
#include <string.h>
#include <wtf/Assertions.h>

/**
 * SECTION:webkitwebnavigationaction
 * @short_description: Object used to report details of navigation actions
 *
 * #WebKitWebNavigationAction is used in signals to provide details about
 * what led the navigation to happen. This includes, for instance, if the user
 * clicked a link to start that navigation, and what mouse button was used.
 */

WebKitWebNavigationAction::WebKitWebNavigationAction() {
	originalUri = targetFrame = NULL;
}

WebKitWebNavigationAction::~WebKitWebNavigationAction() {
	free(originalUri);
	free(targetFrame);
}

/**
 * WebKitWebNavigationAction::get_reason:
 * @navigationAction: a #WebKitWebNavigationAction
 *
 * Returns the reason why WebKit is requesting a navigation.
 *
 * Return value: a #WebKitWebNavigationReason
 *
 * Since: 1.0.3
 */
WebKitWebNavigationReason WebKitWebNavigationAction::get_reason()
{
	return reason;
}

/**
 * WebKitWebNavigationAction::set_reason:
 * @navigationAction: a #WebKitWebNavigationAction
 * @reason: a #WebKitWebNavigationReason
 *
 * Sets the reason why WebKit is requesting a navigation.
 *
 * Since: 1.0.3
 */
void WebKitWebNavigationAction::set_reason(WebKitWebNavigationReason reason)
{
	if (this->reason == reason)
		return;

	this->reason = reason;
}

/**
 * WebKitWebNavigationAction::get_original_uri:
 * @navigationAction: a #WebKitWebNavigationAction
 *
 * Returns the URI that was originally requested. This may differ from the
 * navigation target, for instance because of a redirect.
 *
 * Return value: the originally requested URI
 *
 * Since: 1.0.3
 */
const char* WebKitWebNavigationAction::get_original_uri()
{
	return originalUri;
}

/**
 * WebKitWebNavigationAction::set_original_uri:
 * @navigationAction: a #WebKitWebNavigationAction
 * @originalUri: a URI
 *
 * Sets the URI that was originally requested. This may differ from the
 * navigation target, for instance because of a redirect.
 *
 * Since: 1.0.3
 */
void WebKitWebNavigationAction::set_original_uri(const char* originalUri)
{
	if (this->originalUri &&
		(!strcmp(this->originalUri, originalUri)))
		return;

	free(this->originalUri);
	this->originalUri = strdup(originalUri);
}

/**
 * WebKitWebNavigationAction::get_button:
 * @navigationAction: a #WebKitWebNavigationAction
 *
 * The GTK+ identifier for the mouse button used to click. Notice that GTK+ button values
 * are 1, 2 and 3 for left, middle and right buttons, so they are DOM button values +1. If the action was not
 * initiated by a mouse click the value will be -1.
 *
 * Return value: the mouse button used to click
 *
 * Since: 1.0.3
 */
int WebKitWebNavigationAction::get_button()
{
	return button;
}

/**
 * WebKitWebNavigationAction::get_modifier_state:
 * @navigationAction: a #WebKitWebNavigationAction
 *
 * Returns a bitmask with the the state of the modifier keys.
 *
 * Return value: a bitmask with the state of the modifier keys
 *
 * Since: 1.0.3
 */
int WebKitWebNavigationAction::get_modifier_state()
{
	return modifier_state;
}

/**
 * WebKitWebNavigationAction::get_target_frame:
 * @navigationAction: a #WebKitWebNavigationAction
 *
 * Returns the target frame of the action.
 *
 * Return value: the target frame of the action or NULL
 * if there is no target.
 *
 * Since: 1.1.13
 */
const char* WebKitWebNavigationAction::get_target_frame()
{
	return targetFrame;
}

void WebKitWebNavigationAction::set_target_frame(const char* targetFrame)
{
	if (!strcmp(this->targetFrame, targetFrame))
		return;

	free(this->targetFrame);
	this->targetFrame = strdup(targetFrame);
}

namespace WebKit {

WebKitWebNavigationReason kit(WebCore::NavigationType type)
{
	return (WebKitWebNavigationReason)type;
}

WebCore::NavigationType core(WebKitWebNavigationReason type)
{
	return static_cast<WebCore::NavigationType>(type);
}

}
