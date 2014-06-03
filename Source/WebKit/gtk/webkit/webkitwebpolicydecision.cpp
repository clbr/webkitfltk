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
#include "webkitwebpolicydecision.h"

#include "FrameLoaderClient.h"
#include "FrameLoaderTypes.h"
#include "webkitwebframeprivate.h"

using namespace WebKit;
using namespace WebCore;

/**
 * SECTION:webkitwebpolicydecision
 * @short_description: Liason between WebKit and the application regarding asynchronous policy decisions
 *
 * #WebKitWebPolicyDecision objects are given to the application on
 * signal emissions that deal with policy decisions, such as if a new
 * window should be opened, or if a given navigation should be
 * allowed. The application uses it to tell the engine what to do.
 */

WebKitWebPolicyDecision::WebKitWebPolicyDecision(WebKitWebFrame* frame,
						FramePolicyFunction function)
{
	frame = frame;
	framePolicyFunction = function;
	isCancelled = FALSE;
}

WebKitWebPolicyDecision::~WebKitWebPolicyDecision() {

}

/**
 * WebKitWebPolicyDecision::use:
 * @decision: a #WebKitWebPolicyDecision
 *
 * Will send the USE decision to the policy implementer.
 *
 * Since: 1.0.3
 */
void WebKitWebPolicyDecision::use()
{
	if (!isCancelled)
		framePolicyFunction(WebCore::PolicyUse);
}

/**
 * WebKitWebPolicyDecision::ignore:
 * @decision: a #WebKitWebPolicyDecision
 *
 * Will send the IGNORE decision to the policy implementer.
 *
 * Since: 1.0.3
 */
void WebKitWebPolicyDecision::ignore()
{
	if (!isCancelled)
		framePolicyFunction(WebCore::PolicyIgnore);
}

/**
 * WebKitWebPolicyDecision::download:
 * @decision: a #WebKitWebPolicyDecision
 *
 * Will send the DOWNLOAD decision to the policy implementer.
 *
 * Since: 1.0.3
 */
void WebKitWebPolicyDecision::download()
{
	if (!isCancelled)
		framePolicyFunction(WebCore::PolicyDownload);
}

void WebKitWebPolicyDecision::cancel()
{
	isCancelled = TRUE;
}
