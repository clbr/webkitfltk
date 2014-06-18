/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 * Copyright (C) 2008 INdT - Instituto Nokia de Tecnologia
 * Copyright (C) 2009-2010 ProFUSION embedded systems
 * Copyright (C) 2009-2010 Samsung Electronics
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include "NotImplemented.h"
#include "ScrollbarThemeClient.h"
#include "ScrollbarThemeFLTK.h"

namespace WebCore {

ScrollbarTheme* ScrollbarTheme::nativeTheme()
{
    static ScrollbarThemeFLTK theme;
    return &theme;
}

void ScrollbarThemeFLTK::registerScrollbar(ScrollbarThemeClient *bar) {
	m_scrollbars.add(bar);
}

void ScrollbarThemeFLTK::unregisterScrollbar(ScrollbarThemeClient *bar) {
	m_scrollbars.remove(bar);
}

bool ScrollbarThemeFLTK::paint(ScrollbarThemeClient *bar,
				GraphicsContext *gc, const IntRect &damage) {
	// Happens in the widget.
	return false;
}

ScrollbarPart ScrollbarThemeFLTK::hitTest(ScrollbarThemeClient *bar,
		const IntPoint &rect) {
	// fixme
	return NoPart;
}

int ScrollbarThemeFLTK::thumbPosition(ScrollbarThemeClient *bar) {

	if (!bar->enabled())
		return 0;
	// fixme
	return 3;
}

int ScrollbarThemeFLTK::thumbLength(ScrollbarThemeClient *bar) {

	if (!bar->enabled())
		return 0;
	// fixme
	return 3;
}

int ScrollbarThemeFLTK::trackPosition(ScrollbarThemeClient *bar) {

	if (!bar->enabled())
		return 0;
	// fixme
	return 3;
}

int ScrollbarThemeFLTK::trackLength(ScrollbarThemeClient *bar) {

	if (!bar->enabled())
		return 0;
	// fixme
	return 3;
}

}
