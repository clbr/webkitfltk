/*
 *	Copyright (C) 2008 Nuanti Ltd.
 *	Copyright (C) 2009 Gustavo Noronha Silva <gns@gnome.org>
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

#include "config.h"

#if ENABLE(CONTEXT_MENUS)

#include "ContextMenuClientFLTK.h"

#include "ContextMenu.h"
#include "ContextMenuController.h"
#include "HitTestResult.h"
#include "URL.h"
#include "LocalizedStrings.h"
#include "NotImplemented.h"
#include "Page.h"
#include "webkitwebviewprivate.h"
#include <wtf/text/CString.h>

using namespace WebCore;

namespace WebKit {

ContextMenuClient::ContextMenuClient(WebKitWebView *webView)
	: m_webView(webView)
{
}

void ContextMenuClient::contextMenuDestroyed()
{
	delete this;
}

PlatformMenuDescription ContextMenuClient::getCustomMenuFromDefaultItems(ContextMenu* menu)
{
/*
	FLTKMenu* gtkmenu = menu->releasePlatformDescription();

	WebKitWebView* webView = m_webView;
	HitTestResult result = core(webView)->contextMenuController().hitTestResult();

	if (result.isContentEditable()) {
		FLTKWidget* imContextMenu = inputMethodsMenuItem(webView);
		if (!imContextMenu)
			return gtkmenu;

		// Place the im context menu item right before the unicode menu item
		// if it's present.
		int unicodeMenuItemPosition = getUnicodeMenuItemPosition(gtkmenu);
		if (unicodeMenuItemPosition == -1) {
			FLTKWidget* separator = gtk_separator_menu_item_new();
			gtk_menu_shell_append(GTK_MENU_SHELL(gtkmenu), separator);
			gtk_widget_show(separator);
		}

		gtk_menu_shell_insert(GTK_MENU_SHELL(gtkmenu), imContextMenu, unicodeMenuItemPosition);
		gtk_widget_show(imContextMenu);
	}

	return gtkmenu;*/
}

void ContextMenuClient::contextMenuItemSelected(ContextMenuItem*, const ContextMenu*)
{
	notImplemented();
}

void ContextMenuClient::downloadURL(const URL& url)
{
	WebKitNetworkRequest* networkRequest = webkit_network_request_new(url.string().utf8().data());

	webkit_web_view_request_download(m_webView, networkRequest);
//	  g_object_unref(networkRequest);
}

void ContextMenuClient::copyImageToClipboard(const HitTestResult&)
{
	notImplemented();
}

void ContextMenuClient::searchWithGoogle(const Frame*)
{
	notImplemented();
}

void ContextMenuClient::lookUpInDictionary(Frame*)
{
	notImplemented();
}

void ContextMenuClient::speak(const String&)
{
	notImplemented();
}

void ContextMenuClient::stopSpeaking()
{
	notImplemented();
}

bool ContextMenuClient::isSpeaking()
{
	notImplemented();
	return false;
}

}

#endif // ENABLE(CONTEXT_MENUS)

