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
#include "editorclient.h"
#include "webviewpriv.h"

#include <Frame.h>
#include <FrameSelection.h>
#include "NotImplemented.h"
#include <Settings.h>

#include <FL/Fl.H>

using namespace WebCore;
using namespace WTF;

FlEditorClient::FlEditorClient(webview *inview) {
	view = inview;
}

void FlEditorClient::pageDestroyed() {
	notImplemented();
}

bool FlEditorClient::shouldDeleteRange(Range*) {
	notImplemented();
	return true;
}

bool FlEditorClient::smartInsertDeleteEnabled() {
	Page *page = view->priv->page;
	if (!page)
		return false;
	return page->settings().smartInsertDeleteEnabled();
}

bool FlEditorClient::isSelectTrailingWhitespaceEnabled() {
	Page *page = view->priv->page;
	if (!page)
		return false;
	return page->settings().selectTrailingWhitespaceEnabled();
}

bool FlEditorClient::isContinuousSpellCheckingEnabled() {
	notImplemented();
	return false;
}

void FlEditorClient::toggleContinuousSpellChecking() {
	notImplemented();
}

bool FlEditorClient::isGrammarCheckingEnabled() {
	notImplemented();
	return false;
}

void FlEditorClient::toggleGrammarChecking() {
	notImplemented();
}

int FlEditorClient::spellCheckerDocumentTag() {
	notImplemented();
	return 0;
}

bool FlEditorClient::shouldBeginEditing(Range*) {
	notImplemented();
	return true;
}

bool FlEditorClient::shouldEndEditing(Range*) {
	notImplemented();
	return true;
}

bool FlEditorClient::shouldInsertNode(Node*, Range*,
		EditorInsertAction) {
	notImplemented();
	return true;
}

bool FlEditorClient::shouldInsertText(const String&, Range*,
		EditorInsertAction) {
	notImplemented();
	return true;
}

bool FlEditorClient::shouldChangeSelectedRange(Range* fromRange,
		Range* toRange, EAffinity,
		bool stillSelecting) {
	notImplemented();
	return true;
}

bool FlEditorClient::shouldApplyStyle(StyleProperties*, Range*) {
	notImplemented();
	return true;
}

bool FlEditorClient::shouldMoveRangeAfterDelete(Range*, Range*) {
	notImplemented();
	return true;
}

void FlEditorClient::didBeginEditing() {
	view->priv->editing = true;
}

void FlEditorClient::respondToChangedContents() {
	notImplemented();
}

void FlEditorClient::respondToChangedSelection(Frame *f) {
	if (!f || !f->selection().isRange())
		return;
	String str = f->selection().toNormalizedRange()->text();
	Fl::copy(str.utf8().data(), str.length());
}

void FlEditorClient::didEndEditing() {
	view->priv->editing = false;
}

void FlEditorClient::willWriteSelectionToPasteboard(Range*) {
	notImplemented();
}

void FlEditorClient::didWriteSelectionToPasteboard() {
	notImplemented();
}

void FlEditorClient::getClientPasteboardDataForRange(Range*,
		Vector<String>& pasteboardTypes,
		Vector<RefPtr<SharedBuffer>>& pasteboardData) {
	notImplemented();
}

void FlEditorClient::registerUndoStep(PassRefPtr<UndoStep>) {
	notImplemented();
}

void FlEditorClient::registerRedoStep(PassRefPtr<UndoStep>) {
	notImplemented();
}

void FlEditorClient::clearUndoRedoOperations() {
	notImplemented();
}

bool FlEditorClient::canCopyCut(Frame*, bool defaultValue) const {
	notImplemented();
	return defaultValue;
}

bool FlEditorClient::canPaste(Frame*, bool defaultValue) const {
	notImplemented();
	return defaultValue;
}

bool FlEditorClient::canUndo() const {
	notImplemented();
	return false;
}

bool FlEditorClient::canRedo() const {
	notImplemented();
	return false;
}

void FlEditorClient::undo() {
	notImplemented();
}

void FlEditorClient::redo() {
	notImplemented();
}

void FlEditorClient::handleKeyboardEvent(KeyboardEvent*) {
	notImplemented();
}

void FlEditorClient::handleInputMethodKeydown(KeyboardEvent*) {
	notImplemented();
}

void FlEditorClient::textFieldDidBeginEditing(Element*) {
	notImplemented();
}

void FlEditorClient::textFieldDidEndEditing(Element*) {
	notImplemented();
}

void FlEditorClient::textDidChangeInTextField(Element*) {
	notImplemented();
}

bool FlEditorClient::doTextFieldCommandFromEvent(Element*, KeyboardEvent*) {
	notImplemented();
	return false;
}

void FlEditorClient::textWillBeDeletedInTextField(Element*) {
	notImplemented();
}

void FlEditorClient::textDidChangeInTextArea(Element*) {
	notImplemented();
}

TextCheckerClient* FlEditorClient::textChecker() {
	notImplemented();
	return NULL;
}

void FlEditorClient::updateSpellingUIWithGrammarString(const String&,
		const GrammarDetail& detail) {
	notImplemented();
}

void FlEditorClient::updateSpellingUIWithMisspelledWord(const String&) {
	notImplemented();
}

void FlEditorClient::showSpellingUI(bool show) {
	notImplemented();
}

bool FlEditorClient::spellingUIIsShowing() {
	notImplemented();
	return false;
}

void FlEditorClient::willSetInputMethodState() {
	notImplemented();
}

void FlEditorClient::setInputMethodState(bool enabled) {
	notImplemented();
}
