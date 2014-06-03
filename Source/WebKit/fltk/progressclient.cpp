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
#include "progressclient.h"
#include "webviewpriv.h"

#include "NotImplemented.h"

using namespace WebCore;

FlProgressTrackerClient::FlProgressTrackerClient(webview *inview) {
	view = inview;
}

void FlProgressTrackerClient::progressStarted(Frame&) {
	notImplemented();
}

void FlProgressTrackerClient::progressEstimateChanged(Frame&) {
	notImplemented();
}

void FlProgressTrackerClient::progressFinished(Frame&) {
	notImplemented();
}

