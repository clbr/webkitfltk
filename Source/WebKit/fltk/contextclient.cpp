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
#include "contextclient.h"
#include "webviewpriv.h"

#include <ContextMenu.h>
#include <NotImplemented.h>

using namespace WebCore;

FlContextMenuClient::FlContextMenuClient(webview *inview) {
	view = inview;
}

void FlContextMenuClient::contextMenuDestroyed() {
	delete this;
}

void FlContextMenuClient::contextMenuItemSelected(ContextMenuItem*, const ContextMenu*) {
	notImplemented();
}

void FlContextMenuClient::downloadURL(const URL &url) {
	view->download(url.string().utf8().data(),
			url.lastPathComponent().utf8().data());
}

void FlContextMenuClient::searchWithGoogle(const Frame*) {
	notImplemented();
}

void FlContextMenuClient::lookUpInDictionary(Frame*) {
	notImplemented();
}

bool FlContextMenuClient::isSpeaking() {
	notImplemented();
	return false;
}

void FlContextMenuClient::speak(const WTF::String&) {
	notImplemented();
}

void FlContextMenuClient::stopSpeaking() {
	notImplemented();
}

PassOwnPtr<ContextMenu> FlContextMenuClient::customizeMenu(PassOwnPtr<ContextMenu> menu) {

	OwnPtr<ContextMenu> m = menu;
	Vector<ContextMenuItem> newitems;

	unsigned i;
	for (i = 0; i < m->items().size(); i++) {
		const ContextMenuItem &cur = m->items()[i];

		switch (cur.action()) {
			// Remove some unsupported menu items
			case ContextMenuItemTagSpellingMenu:
			case ContextMenuItemTagSearchWeb:
			case ContextMenuItemTagStartSpeaking:
			case ContextMenuItemTagStopSpeaking:
			break;
			default:
				newitems.append(cur);
			break;
		}
	}

	m->setItems(newitems);
	return m.release();
}
