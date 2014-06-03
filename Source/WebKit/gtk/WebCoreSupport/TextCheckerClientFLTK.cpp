/*
 *	Copyright (C) 2007 Alp Toker <alp@atoker.com>
 *	Copyright (C) 2008 Nuanti Ltd.
 *	Copyright (C) 2009 Diego Escalante Urrelo <diegoe@gnome.org>
 *	Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
 *	Copyright (C) 2009, 2010, 2011 Igalia S.L.
 *	Copyright (C) 2010, Martin Robinson <mrobinson@webkit.org>
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
#include "TextCheckerClientFLTK.h"

#include "NotImplemented.h"
#include <wtf/text/CString.h>

using namespace WebCore;

namespace WebKit {

TextCheckerClientFLTK::TextCheckerClientFLTK(WebKitSpellChecker* spellChecker)
	: m_spellChecker(spellChecker)
{
}

TextCheckerClientFLTK::~TextCheckerClientFLTK()
{
}

bool TextCheckerClientFLTK::shouldEraseMarkersAfterChangeSelection(TextCheckingType) const
{
	return true;
}

void TextCheckerClientFLTK::ignoreWordInSpellDocument(const String& text)
{
}

void TextCheckerClientFLTK::learnWord(const String& text)
{
}

void TextCheckerClientFLTK::checkSpellingOfString(const UChar* text, int length, int* misspellingLocation, int* misspellingLength)
{
}

String TextCheckerClientFLTK::getAutoCorrectSuggestionForMisspelledWord(const String& inputWord)
{
}

void TextCheckerClientFLTK::checkGrammarOfString(const UChar*, int, Vector<GrammarDetail>&, int*, int*)
{
	notImplemented();
}

void TextCheckerClientFLTK::getGuessesForWord(const String& word, const String& context, WTF::Vector<String>& guesses)
{
}

void TextCheckerClientFLTK::updateSpellCheckingLanguage(const char* spellCheckingLanguages)
{
}
}
