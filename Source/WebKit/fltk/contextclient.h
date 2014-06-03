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

#ifndef contextclient_h
#define contextclient_h

#include <ContextMenuClient.h>
#include "webview.h"

class FlContextMenuClient: public WebCore::ContextMenuClient {
public:
	FlContextMenuClient(webview *);

	void contextMenuDestroyed() override;
	WebCore::PlatformMenuDescription
		getCustomMenuFromDefaultItems(WebCore::ContextMenu*) override;
	void contextMenuItemSelected(WebCore::ContextMenuItem*, const WebCore::ContextMenu*)
		override;
	void downloadURL(const WebCore::URL&) override;
	void searchWithGoogle(const WebCore::Frame*) override;
	void lookUpInDictionary(WebCore::Frame*) override;
	bool isSpeaking() override;
	void speak(const WTF::String&) override;
	void stopSpeaking() override;

private:
	webview *view;
};

#endif
