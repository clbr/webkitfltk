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

#ifndef inspectorclient_h
#define inspectorclient_h

#include <InspectorClient.h>
#include "webview.h"

class FlInspectorClient: public WebCore::InspectorClient {
public:
	FlInspectorClient(webview *);

	void inspectorDestroyed() override;
	WebCore::InspectorFrontendChannel*
		openInspectorFrontend(WebCore::InspectorController*) override;
	void closeInspectorFrontend() override;
	void bringFrontendToFront() override;
	void highlight() override;
	void hideHighlight() override;

private:
	webview *view;
};

#endif
