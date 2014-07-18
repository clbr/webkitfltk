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

#ifndef editorclient_h
#define editorclient_h

#include <EditorClient.h>
#include <wtf/Deque.h>
#include "webview.h"

class FlEditorClient: public WebCore::EditorClient {
public:
	FlEditorClient(webview *);

	void pageDestroyed() override;

	bool shouldDeleteRange(WebCore::Range*) override;
	bool smartInsertDeleteEnabled() override;
	bool isSelectTrailingWhitespaceEnabled() override;
	bool isContinuousSpellCheckingEnabled() override;
	void toggleContinuousSpellChecking() override;
	bool isGrammarCheckingEnabled() override;
	void toggleGrammarChecking() override;
	int spellCheckerDocumentTag() override;

	bool shouldBeginEditing(WebCore::Range*) override;
	bool shouldEndEditing(WebCore::Range*) override;
	bool shouldInsertNode(WebCore::Node*, WebCore::Range*,
			WebCore::EditorInsertAction) override;
	bool shouldInsertText(const WTF::String&, WebCore::Range*,
			WebCore::EditorInsertAction) override;
	bool shouldChangeSelectedRange(WebCore::Range* fromRange,
			WebCore::Range* toRange, WebCore::EAffinity,
			bool stillSelecting) override;

	bool shouldApplyStyle(WebCore::StyleProperties*, WebCore::Range*) override;
	bool shouldMoveRangeAfterDelete(WebCore::Range*, WebCore::Range*) override;

	void didBeginEditing() override;
	void respondToChangedContents() override;
	void respondToChangedSelection(WebCore::Frame*) override;
	void didEndEditing() override;
	void willWriteSelectionToPasteboard(WebCore::Range*) override;
	void didWriteSelectionToPasteboard() override;
	void getClientPasteboardDataForRange(WebCore::Range*,
			Vector<WTF::String>& pasteboardTypes,
			Vector<RefPtr<WebCore::SharedBuffer>>& pasteboardData) override;

	void registerUndoStep(PassRefPtr<WebCore::UndoStep>) override;
	void registerRedoStep(PassRefPtr<WebCore::UndoStep>) override;
	void clearUndoRedoOperations() override;

	bool canCopyCut(WebCore::Frame*, bool defaultValue) const override;
	bool canPaste(WebCore::Frame*, bool defaultValue) const override;
	bool canUndo() const override;
	bool canRedo() const override;

	void undo() override;
	void redo() override;

	void handleKeyboardEvent(WebCore::KeyboardEvent*) override;
	void handleInputMethodKeydown(WebCore::KeyboardEvent*) override;

	void textFieldDidBeginEditing(WebCore::Element*) override;
	void textFieldDidEndEditing(WebCore::Element*) override;
	void textDidChangeInTextField(WebCore::Element*) override;
	bool doTextFieldCommandFromEvent(WebCore::Element*, WebCore::KeyboardEvent*) override;
	void textWillBeDeletedInTextField(WebCore::Element*) override;
	void textDidChangeInTextArea(WebCore::Element*) override;

	WebCore::TextCheckerClient* textChecker() override;

	void updateSpellingUIWithGrammarString(const WTF::String&,
			const WebCore::GrammarDetail& detail) override;
	void updateSpellingUIWithMisspelledWord(const WTF::String&) override;
	void showSpellingUI(bool show) override;
	bool spellingUIIsShowing() override;
	void willSetInputMethodState() override;
	void setInputMethodState(bool enabled) override;

	// Support for global selections, used on platforms like the X Window System that treat
	// selection as a type of clipboard.
	bool supportsGlobalSelection() { return true; }

private:
	webview *view;

	WTF::Deque<WTF::RefPtr<WebCore::UndoStep> > undoStack;
	WTF::Deque<WTF::RefPtr<WebCore::UndoStep> > redoStack;
	bool m_isInRedo;
};

#endif
