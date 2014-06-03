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

#include "config.h"
#include "inspectorclient.h"
#include "webviewpriv.h"

#include "NotImplemented.h"

using namespace WebCore;

FlInspectorClient::FlInspectorClient(webview *inview) {
	view = inview;
}

void FlInspectorClient::inspectorDestroyed() {
	notImplemented();
}

InspectorFrontendChannel*
	FlInspectorClient::openInspectorFrontend(InspectorController*) {
	notImplemented();
}

void FlInspectorClient::closeInspectorFrontend() {
	notImplemented();
}

void FlInspectorClient::bringFrontendToFront() {
	notImplemented();
}

void FlInspectorClient::highlight() {
	notImplemented();
}

void FlInspectorClient::hideHighlight() {
	notImplemented();
}

