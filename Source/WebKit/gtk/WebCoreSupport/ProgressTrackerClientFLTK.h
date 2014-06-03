/*
 *	Copyright (C) 2014 Igalia S.L.
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation; either
 *	version 2 of the License, or (at your option) any later version.
 *
 *	This library is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *	Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public
 *	License along with this library; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef ProgressTrackerClientFLTK_h
#define ProgressTrackerClientFLTK_h

#include "ProgressTrackerClient.h"

class WebKitWebView;

namespace WebKit {

class ProgressTrackerClient final : public WebCore::ProgressTrackerClient {
public:
	explicit ProgressTrackerClient(WebKitWebView*);

private:
	virtual void progressTrackerDestroyed() override;

	virtual void progressStarted(WebCore::Frame&) override;
	virtual void progressEstimateChanged(WebCore::Frame&) override;
	virtual void progressFinished(WebCore::Frame&) override;

	WebKitWebView* m_webView;
};

} // namespace WebKit

#endif // ProgressTrackerClientFLTK_h
