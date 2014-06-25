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

#include <Editor.h>
#include <Frame.h>
#include <FrameSelection.h>
#include <KeyboardEvent.h>
#include <NotImplemented.h>
#include <PlatformKeyboardEvent.h>
#include <Settings.h>
#include <WindowsKeyboardCodes.h>

#include <FL/Fl.H>

using namespace WebCore;
using namespace WTF;

FlEditorClient::FlEditorClient(webview *inview) {
	view = inview;
}

void FlEditorClient::pageDestroyed() {
	delete this;
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

static const unsigned CtrlKey = 1 << 0;
static const unsigned AltKey = 1 << 1;
static const unsigned ShiftKey = 1 << 2;

struct KeyDownEntry {
	unsigned virtualKey;
	unsigned modifiers;
	const char* name;
};

struct KeyPressEntry {
	unsigned charCode;
	unsigned modifiers;
	const char* name;
};

static const KeyDownEntry keyDownEntries[] = {
    { VK_LEFT,   0,                  "MoveLeft"                                    },
    { VK_LEFT,   ShiftKey,           "MoveLeftAndModifySelection"                  },
    { VK_LEFT,   CtrlKey,            "MoveWordLeft"                                },
    { VK_LEFT,   CtrlKey | ShiftKey, "MoveWordLeftAndModifySelection"              },
    { VK_RIGHT,  0,                  "MoveRight"                                   },
    { VK_RIGHT,  ShiftKey,           "MoveRightAndModifySelection"                 },
    { VK_RIGHT,  CtrlKey,            "MoveWordRight"                               },
    { VK_RIGHT,  CtrlKey | ShiftKey, "MoveWordRightAndModifySelection"             },
    { VK_UP,     0,                  "MoveUp"                                      },
    { VK_UP,     ShiftKey,           "MoveUpAndModifySelection"                    },
    { VK_PRIOR,  ShiftKey,           "MovePageUpAndModifySelection"                },
    { VK_DOWN,   0,                  "MoveDown"                                    },
    { VK_DOWN,   ShiftKey,           "MoveDownAndModifySelection"                  },
    { VK_NEXT,   ShiftKey,           "MovePageDownAndModifySelection"              },
    { VK_PRIOR,  0,                  "MovePageUp"                                  },
    { VK_NEXT,   0,                  "MovePageDown"                                },
    { VK_HOME,   0,                  "MoveToBeginningOfLine"                       },
    { VK_HOME,   ShiftKey,           "MoveToBeginningOfLineAndModifySelection"     },
    { VK_HOME,   CtrlKey,            "MoveToBeginningOfDocument"                   },
    { VK_HOME,   CtrlKey | ShiftKey, "MoveToBeginningOfDocumentAndModifySelection" },

    { VK_END,    0,                  "MoveToEndOfLine"                             },
    { VK_END,    ShiftKey,           "MoveToEndOfLineAndModifySelection"           },
    { VK_END,    CtrlKey,            "MoveToEndOfDocument"                         },
    { VK_END,    CtrlKey | ShiftKey, "MoveToEndOfDocumentAndModifySelection"       },

    { VK_BACK,   0,                  "DeleteBackward"                              },
    { VK_BACK,   ShiftKey,           "DeleteBackward"                              },
    { VK_DELETE, 0,                  "DeleteForward"                               },
    { VK_BACK,   CtrlKey,            "DeleteWordBackward"                          },
    { VK_DELETE, CtrlKey,            "DeleteWordForward"                           },

    { 'B',       CtrlKey,            "ToggleBold"                                  },
    { 'I',       CtrlKey,            "ToggleItalic"                                },

    { VK_ESCAPE, 0,                  "Cancel"                                      },
    { VK_TAB,    0,                  "InsertTab"                                   },
    { VK_TAB,    ShiftKey,           "InsertBacktab"                               },
    { VK_RETURN, 0,                  "InsertNewline"                               },
    { VK_RETURN, CtrlKey,            "InsertNewline"                               },
    { VK_RETURN, AltKey,             "InsertNewline"                               },
    { VK_RETURN, AltKey | ShiftKey,  "InsertNewline"                               },

    // It's not quite clear whether clipboard shortcuts and Undo/Redo should be handled
    // in the application or in WebKit. We chose WebKit for now.
    { 'C',       CtrlKey,            "Copy"                                        },
    { 'V',       CtrlKey,            "Paste"                                       },
    { 'X',       CtrlKey,            "Cut"                                         },
    { 'A',       CtrlKey,            "SelectAll"                                   },
    { VK_INSERT, CtrlKey,            "Copy"                                        },
    { VK_DELETE, ShiftKey,           "Cut"                                         },
    { VK_INSERT, ShiftKey,           "Paste"                                       },
    { 'Z',       CtrlKey,            "Undo"                                        },
    { 'Z',       CtrlKey | ShiftKey, "Redo"                                        }
};

static const KeyPressEntry keyPressEntries[] = {
    { '\t',   0,                  "InsertTab"                                   },
    { '\t',   ShiftKey,           "InsertBacktab"                               },
    { '\r',   0,                  "InsertNewline"                               },
    { '\r',   CtrlKey,            "InsertNewline"                               },
    { '\r',   AltKey,             "InsertNewline"                               },
    { '\r',   AltKey | ShiftKey,  "InsertNewline"                               }
};

static const char* interpretKeyEvent(const KeyboardEvent* event)
{
	ASSERT(event->type() == eventNames().keydownEvent || event->type() == eventNames().keypressEvent);

	static HashMap<int, const char*>* keyDownCommandsMap = 0;
	static HashMap<int, const char*>* keyPressCommandsMap = 0;

	if (!keyDownCommandsMap) {
		keyDownCommandsMap = new HashMap<int, const char*>;
		keyPressCommandsMap = new HashMap<int, const char*>;

		for (size_t i = 0; i < WTF_ARRAY_LENGTH(keyDownEntries); ++i)
			keyDownCommandsMap->set(keyDownEntries[i].modifiers << 16 | keyDownEntries[i].virtualKey, keyDownEntries[i].name);

		for (size_t i = 0; i < WTF_ARRAY_LENGTH(keyPressEntries); ++i)
			keyPressCommandsMap->set(keyPressEntries[i].modifiers << 16 | keyPressEntries[i].charCode, keyPressEntries[i].name);
	}

	unsigned modifiers = 0;
	if (event->shiftKey())
		modifiers |= ShiftKey;
	if (event->altKey())
		modifiers |= AltKey;
	if (event->ctrlKey())
		modifiers |= CtrlKey;

	if (event->type() == eventNames().keydownEvent) {
		int mapKey = modifiers << 16 | event->keyCode();
		return mapKey ? keyDownCommandsMap->get(mapKey) : 0;
	}

	int mapKey = modifiers << 16 | event->charCode();
	return mapKey ? keyPressCommandsMap->get(mapKey) : 0;
}


void FlEditorClient::handleKeyboardEvent(KeyboardEvent *ev) {
	const PlatformKeyboardEvent * const pev = ev->keyEvent();
	if (!pev)
		return;

	Node* node = ev->target()->toNode();
	Frame* frame = node->document().frame();
	Editor::Command command = frame->editor().command(interpretKeyEvent(ev));

	if (pev->type() == PlatformEvent::RawKeyDown) {
		if (!command.isTextInsertion() && command.execute(ev))
			ev->setDefaultHandled();
		return;
	}
	if (!frame->editor().canEdit())
		return;

	command.execute(ev);

	if (pev->ctrlKey() || pev->altKey() || ev->charCode() < ' ')
		return;

	frame->editor().insertText(pev->text(), ev);
	ev->setDefaultHandled();
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
