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

#ifndef chromeclient_h
#define chromeclient_h

#include <ChromeClient.h>
#include "webview.h"

class FlChromeClient: public WebCore::ChromeClient {
public:
	FlChromeClient(webview *);

	void chromeDestroyed() override;
	void setWindowRect(const WebCore::FloatRect&) override;
	WebCore::FloatRect windowRect() override;
	WebCore::FloatRect pageRect() override;
	void focus() override;
	void unfocus() override;
	bool canTakeFocus(WebCore::FocusDirection) override;
	void takeFocus(WebCore::FocusDirection) override;
	void focusedElementChanged(WebCore::Element*) override;
	void focusedFrameChanged(WebCore::Frame*) override;
	WebCore::Page* createWindow(WebCore::Frame*,
			const WebCore::FrameLoadRequest&,
			const WebCore::WindowFeatures&,
			const WebCore::NavigationAction&) override;
	void show() override;
	bool canRunModal() override;
	void runModal() override;
	void setToolbarsVisible(bool) override;
	bool toolbarsVisible() override;
	void setStatusbarVisible(bool) override;
	bool statusbarVisible() override;
	void setScrollbarsVisible(bool) override;
	bool scrollbarsVisible() override;
	void setMenubarVisible(bool) override;
	bool menubarVisible() override;
	void setResizable(bool) override;
	void addMessageToConsole(JSC::MessageSource, JSC::MessageLevel,
			const WTF::String&, unsigned int, unsigned int,
			const WTF::String&) override;
	bool canRunBeforeUnloadConfirmPanel() override;
	bool runBeforeUnloadConfirmPanel(const WTF::String&, WebCore::Frame*) override;
	void closeWindowSoon() override;
	void runJavaScriptAlert(WebCore::Frame*, const WTF::String&) override;
	bool runJavaScriptConfirm(WebCore::Frame*, const WTF::String&) override;
	bool runJavaScriptPrompt(WebCore::Frame*, const WTF::String&,
			const WTF::String&, WTF::String&) override;
	void setStatusbarText(const WTF::String&) override;
	bool shouldInterruptJavaScript() override;
	WebCore::KeyboardUIMode keyboardUIMode() override;
	WebCore::IntRect windowResizerRect() const override;
	void invalidateRootView(const WebCore::IntRect&) override;
	void invalidateContentsAndRootView(const WebCore::IntRect&) override;
	void invalidateContentsForSlowScroll(const WebCore::IntRect&) override;
	void scroll(const WebCore::IntSize&, const WebCore::IntRect&,
			const WebCore::IntRect&) override;
	WebCore::IntPoint screenToRootView(const WebCore::IntPoint&) const override;
	WebCore::IntRect rootViewToScreen(const WebCore::IntRect&) const override;
	Fl_Widget* platformPageClient() const override;
	void scrollbarsModeDidChange() const override;
	void setCursor(const WebCore::Cursor&) override;
	void setCursorHiddenUntilMouseMoves(bool) override;
	void contentsSizeChanged(WebCore::Frame*, const WebCore::IntSize&) const override;
	void mouseDidMoveOverElement(const WebCore::HitTestResult&, unsigned int) override;
	void setToolTip(const WTF::String&, WebCore::TextDirection) override;
	void print(WebCore::Frame*) override;
	void reachedMaxAppCacheSize(int64_t spaceNeeded) override;
	void reachedApplicationCacheOriginQuota(WebCore::SecurityOrigin*,
			int64_t) override;
	void runOpenPanel(WebCore::Frame*, WTF::PassRefPtr<WebCore::FileChooser>) override;
	void loadIconForFiles(const WTF::Vector<WTF::String>&, WebCore::FileIconLoader*) override;
	void attachRootGraphicsLayer(WebCore::Frame*, WebCore::GraphicsLayer*) override;
	void setNeedsOneShotDrawingSynchronization() override;
	void scheduleCompositingLayerFlush() override;
	bool selectItemWritingDirectionIsNatural() override;
	bool selectItemAlignmentFollowsMenuWritingDirection() override;
	bool hasOpenedPopup() const override;
	WTF::PassRefPtr<WebCore::PopupMenu> createPopupMenu(WebCore::PopupMenuClient*)
			const override;
	WTF::PassRefPtr<WebCore::SearchPopupMenu> createSearchPopupMenu(WebCore::PopupMenuClient*)
			const override;
	void numWheelEventHandlersChanged(unsigned) override;

private:
	webview *view;
};

#endif
