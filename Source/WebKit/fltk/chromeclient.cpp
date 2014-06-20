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

#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <FL/Fl_File_Chooser.H>

#include <Document.h>
#include <FileChooser.h>
#include <Frame.h>
#include <NotImplemented.h>
#include <PopupMenuFLTK.h>

using namespace WTF;
using namespace WebCore;

extern const char * (*uploaddirfunc)();

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

bool FlChromeClient::runBeforeUnloadConfirmPanel(const String &s, Frame *f) {
	return runJavaScriptConfirm(f, s);
}

void FlChromeClient::closeWindowSoon() {
	notImplemented();
}

void FlChromeClient::runJavaScriptAlert(Frame *f, const String &s) {
	fl_message_title("Javascript alert");
	fl_alert("<%s>\n\n%s", f->document()->baseURI().string().utf8().data(),
			s.utf8().data());
}

bool FlChromeClient::runJavaScriptConfirm(Frame *f, const String &s) {
	fl_message_title("Javascript confirm");
	return fl_choice("<%s>\n\n%s", fl_cancel, fl_ok, NULL,
			f->document()->baseURI().string().utf8().data(),
			s.utf8().data());
}

bool FlChromeClient::runJavaScriptPrompt(Frame *f, const String &s,
		const String &def, String &out) {

	fl_message_title("Javascript prompt");
	const char *res = fl_input("<%s>\n\n%s", def.utf8().data(),
				f->document()->baseURI().string().utf8().data(),
				s.utf8().data());
	if (!res)
		return false;
	out = res;
	return true;
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

void FlChromeClient::setCursor(const WebCore::Cursor &cursor) {
	fl_cursor((Fl_Cursor) cursor.platformCursor());
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

void FlChromeClient::runOpenPanel(Frame *f, PassRefPtr<FileChooser> chooser) {
	bool multi = false;

	if (chooser->settings().allowsMultipleFiles)
		multi = true;

	Fl_File_Chooser c(uploaddirfunc ? uploaddirfunc() : "/tmp", NULL,
				multi ? Fl_File_Chooser::MULTI : Fl_File_Chooser::SINGLE,
				"Select file");
	c.show();
	while (c.shown())
		Fl::wait();
	if (!c.value())
		return;

	if (multi) {
		Vector<String> filenames;
		unsigned i;
		const unsigned max = c.count();
		for (i = 1; i <= max; i++) {
			filenames.append(c.value(i));
		}

		chooser->chooseFiles(filenames);
	} else {
		chooser->chooseFile(c.value());
	}
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

PassRefPtr<PopupMenu> FlChromeClient::createPopupMenu(PopupMenuClient *c) const {
	return adoptRef(new PopupMenuFLTK(c, view->x(), view->y()));
}

PassRefPtr<SearchPopupMenu>
	FlChromeClient::createSearchPopupMenu(PopupMenuClient*) const {
	notImplemented();
}

void FlChromeClient::numWheelEventHandlersChanged(unsigned) {
	notImplemented();
}

