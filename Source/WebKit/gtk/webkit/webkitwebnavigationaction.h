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

#ifndef webkitwebnavigationaction_h
#define webkitwebnavigationaction_h

#include <webkit/webkitdefines.h>

/**
 * WebKitWebNavigationReason:
 * @WEBKIT_WEB_NAVIGATION_REASON_LINK_CLICKED: The navigation was triggered by clicking a link.
 * @WEBKIT_WEB_NAVIGATION_REASON_FORM_SUBMITTED: The navigation was triggered by submitting a form.
 * @WEBKIT_WEB_NAVIGATION_REASON_BACK_FORWARD: The navigation was triggered by navigating forward or backward.
 * @WEBKIT_WEB_NAVIGATION_REASON_RELOAD: The navigation was triggered by reloading.
 * @WEBKIT_WEB_NAVIGATION_REASON_FORM_RESUBMITTED: The navigation was triggered by resubmitting a form.
 * @WEBKIT_WEB_NAVIGATION_REASON_OTHER: The navigation was triggered by some other action.
 *
 * Enum values used to denote the various navigation reasons.
 */
typedef enum {
	WEBKIT_WEB_NAVIGATION_REASON_LINK_CLICKED,
	WEBKIT_WEB_NAVIGATION_REASON_FORM_SUBMITTED,
	WEBKIT_WEB_NAVIGATION_REASON_BACK_FORWARD,
	WEBKIT_WEB_NAVIGATION_REASON_RELOAD,
	WEBKIT_WEB_NAVIGATION_REASON_FORM_RESUBMITTED,
	WEBKIT_WEB_NAVIGATION_REASON_OTHER,
} WebKitWebNavigationReason;

class WebKitWebNavigationAction {
public:
	WebKitWebNavigationAction();
	~WebKitWebNavigationAction();

	WebKitWebNavigationReason get_reason();
	void set_reason(WebKitWebNavigationReason reason);

	const char *get_original_uri();
	void set_original_uri(const char *originalUri);

	int get_button();
	int get_modifier_state();
	const char *get_target_frame();

	void set_target_frame(const char*);
private:
	WebKitWebNavigationReason reason;
	char* originalUri;
	int button;
	int modifier_state;
	char* targetFrame;
};

#endif
