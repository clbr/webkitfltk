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
#include "chromeclient.h"
#include "webviewpriv.h"

#include <wtf/text/CString.h>
#include <NotImplemented.h>

using namespace WTF;
using namespace WebCore;

FlChromeClient::FlChromeClient(webview *inview) {
	view = inview;
}

void FlChromeClient::chromeDestroyed() {
	notImplemented();
}

void FlChromeClient::setWindowRect(const FloatRect&) {
	notImplemented();
}

FloatRect FlChromeClient::windowRect() {
	return FloatRect(0, 0, view->w(), view->h());
}

FloatRect FlChromeClient::pageRect() {
	return windowRect();
}

void FlChromeClient::focus() {
	notImplemented();
}

void FlChromeClient::unfocus() {
	notImplemented();
}

bool FlChromeClient::canTakeFocus(FocusDirection) {
	notImplemented();
	return true;
}

void FlChromeClient::takeFocus(FocusDirection) {
	notImplemented();
}

void FlChromeClient::focusedElementChanged(Element*) {
	notImplemented();
}

void FlChromeClient::focusedFrameChanged(Frame*) {
	notImplemented();
}

Page* FlChromeClient::createWindow(Frame*,
		const FrameLoadRequest&, const WindowFeatures&,
		const NavigationAction&) {

	notImplemented();
	return NULL;
}

void FlChromeClient::show() {
	notImplemented();
}

bool FlChromeClient::canRunModal() {
	notImplemented();
	return false;
}

void FlChromeClient::runModal() {
	notImplemented();
}

void FlChromeClient::setToolbarsVisible(bool) {
	notImplemented();
}

bool FlChromeClient::toolbarsVisible() {
	notImplemented();
	return true;
}

void FlChromeClient::setStatusbarVisible(bool) {
	notImplemented();
}

bool FlChromeClient::statusbarVisible() {
	notImplemented();
	return true;
}

void FlChromeClient::setScrollbarsVisible(bool) {
	notImplemented();
}

bool FlChromeClient::scrollbarsVisible() {
	notImplemented();
	return true;
}

void FlChromeClient::setMenubarVisible(bool) {
	notImplemented();
}

bool FlChromeClient::menubarVisible() {
	notImplemented();
	return true;
}

void FlChromeClient::setResizable(bool) {
	notImplemented();
}

void FlChromeClient::addMessageToConsole(JSC::MessageSource, JSC::MessageLevel,
		const String&, unsigned, unsigned,
		const String&) {
	notImplemented();
}

bool FlChromeClient::canRunBeforeUnloadConfirmPanel() {
	return false;
}

bool FlChromeClient::runBeforeUnloadConfirmPanel(const String&, Frame*) {
	notImplemented();
}

void FlChromeClient::closeWindowSoon() {
	notImplemented();
}

void FlChromeClient::runJavaScriptAlert(Frame*, const String&) {
	notImplemented();
}

bool FlChromeClient::runJavaScriptConfirm(Frame*, const String&) {
	notImplemented();
}

bool FlChromeClient::runJavaScriptPrompt(Frame*, const String&,
		const String&, String&) {
	notImplemented();
}

void FlChromeClient::setStatusbarText(const String &s) {
	printf("Statusbar '%s'\n", s.utf8().data());
	notImplemented();
}

bool FlChromeClient::shouldInterruptJavaScript() {
	notImplemented();
	return false;
}

KeyboardUIMode FlChromeClient::keyboardUIMode() {
	return KeyboardAccessTabsToLinks;
}

IntRect FlChromeClient::windowResizerRect() const {
	notImplemented();
	return IntRect();
}

void FlChromeClient::invalidateRootView(const IntRect &rect) {
	view->redraw();
}

void FlChromeClient::invalidateContentsAndRootView(const IntRect &rect) {
	invalidateRootView(rect);
}

void FlChromeClient::invalidateContentsForSlowScroll(const IntRect &rect) {
	invalidateRootView(rect);
}

void FlChromeClient::scroll(const IntSize&, const IntRect&,
		const IntRect&) {
	notImplemented();
}

IntPoint FlChromeClient::screenToRootView(const IntPoint &p) const {
	notImplemented();
	return p;
}

IntRect FlChromeClient::rootViewToScreen(const IntRect &r) const {
	notImplemented();
	return r;
}

Fl_Widget* FlChromeClient::platformPageClient() const {
	return view;
}

void FlChromeClient::scrollbarsModeDidChange() const {
	notImplemented();
}

void FlChromeClient::setCursor(const Cursor&) {
	notImplemented();
}

void FlChromeClient::setCursorHiddenUntilMouseMoves(bool) {
	notImplemented();
}

void FlChromeClient::contentsSizeChanged(Frame*, const IntSize&) const {
	notImplemented();
}

void FlChromeClient::mouseDidMoveOverElement(const HitTestResult&, unsigned int) {
	notImplemented();
}

void FlChromeClient::setToolTip(const String&, TextDirection) {
	notImplemented();
}

void FlChromeClient::print(Frame*) {
	notImplemented();
}

void FlChromeClient::reachedMaxAppCacheSize(int64_t spaceNeeded) {
	notImplemented();
}

void FlChromeClient::reachedApplicationCacheOriginQuota(SecurityOrigin*,
		int64_t) {
	notImplemented();
}

void FlChromeClient::runOpenPanel(Frame*, PassRefPtr<FileChooser>) {
	notImplemented();
}

void FlChromeClient::loadIconForFiles(const Vector<String>&, FileIconLoader*) {
	notImplemented();
}

void FlChromeClient::attachRootGraphicsLayer(Frame*, GraphicsLayer*) {
	notImplemented();
}

void FlChromeClient::setNeedsOneShotDrawingSynchronization() {
	notImplemented();
}

void FlChromeClient::scheduleCompositingLayerFlush() {
	notImplemented();
}

bool FlChromeClient::selectItemWritingDirectionIsNatural() {
	return false;
}

bool FlChromeClient::selectItemAlignmentFollowsMenuWritingDirection() {
	return true;
}

bool FlChromeClient::hasOpenedPopup() const {
	notImplemented();
	return false;
}

PassRefPtr<PopupMenu> FlChromeClient::createPopupMenu(PopupMenuClient*) const {
	notImplemented();
}

PassRefPtr<SearchPopupMenu>
	FlChromeClient::createSearchPopupMenu(PopupMenuClient*) const {
	notImplemented();
}

void FlChromeClient::numWheelEventHandlersChanged(unsigned) {
	notImplemented();
}

