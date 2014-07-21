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

#include <Frame.h>
#include <NotImplemented.h>
#include <ProgressTracker.h>

using namespace WebCore;

FlProgressTrackerClient::FlProgressTrackerClient(webview *inview) {
	view = inview;
}

void FlProgressTrackerClient::progressTrackerDestroyed() {
	delete this;
}

void FlProgressTrackerClient::progressStarted(Frame &f) {
	progressEstimateChanged(f);
}

void FlProgressTrackerClient::progressEstimateChanged(Frame &f) {
	// Ignore non-main frames.
	if (&f != (Frame*) &view->priv->page->mainFrame())
		return;

	const float progress = f.page()->progress().estimatedProgress();
	if (view->priv->progressChanged)
		view->priv->progressChanged(view, progress);
}

void FlProgressTrackerClient::progressFinished(Frame &f) {
}

