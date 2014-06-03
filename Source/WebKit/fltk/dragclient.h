/*
WebkitFLTK
Copyright (C) 2014 Lauri Kasanen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef dragclient_h
#define dragclient_h

#include <DragClient.h>
#include "webview.h"

class FlDragClient: public WebCore::DragClient {
public:
	FlDragClient(webview *);

	void dragControllerDestroyed() override;
	void willPerformDragDestinationAction(WebCore::DragDestinationAction,
			WebCore::DragData&) override;
	void willPerformDragSourceAction(WebCore::DragSourceAction,
			const WebCore::IntPoint&, WebCore::DataTransfer&) override;
	WebCore::DragDestinationAction actionMaskForDrag(WebCore::DragData&) override;
	WebCore::DragSourceAction dragSourceActionMaskForPoint(const WebCore::IntPoint&) override;
	void startDrag(WebCore::DragImageRef, const WebCore::IntPoint&,
			const WebCore::IntPoint&, WebCore::DataTransfer&,
			WebCore::Frame&, bool) override;

private:
	webview *view;
};

#endif
