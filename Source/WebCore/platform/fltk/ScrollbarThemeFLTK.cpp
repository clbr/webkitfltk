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

#include "myscroll.h"
#include "NotImplemented.h"
#include "ScrollbarThemeClient.h"
#include "ScrollbarThemeFLTK.h"
#include <PlatformContextCairo.h>

#include <cairo-xlib.h>
#include <FL/fl_draw.H>
#include <FL/x.H>

static myscroll *flbar;

namespace WebCore {

ScrollbarTheme* ScrollbarTheme::nativeTheme()
{
    static ScrollbarThemeFLTK theme;
    return &theme;
}

ScrollbarThemeFLTK::ScrollbarThemeFLTK() {
	if (!flbar)
		flbar = new myscroll(0, 0, 10, 10);
}

bool ScrollbarThemeFLTK::hasThumb(ScrollbarThemeClient *bar) {
	return thumbLength(bar) > 0;
}

IntRect ScrollbarThemeFLTK::backButtonRect(ScrollbarThemeClient *bar,
		const ScrollbarPart part, const bool painting) {

	// Just single arrows
	if (part == BackButtonEndPart)
		return IntRect();

	const unsigned thick = scrollbarThickness();

	return IntRect(bar->x(), bar->y(), thick, thick);
}

IntRect ScrollbarThemeFLTK::forwardButtonRect(ScrollbarThemeClient *bar,
		const ScrollbarPart part, const bool painting) {

	// Just single arrows
	if (part == ForwardButtonStartPart)
		return IntRect();

	const unsigned thick = scrollbarThickness();

	if (bar->orientation() == HorizontalScrollbar)
		return IntRect(bar->x() + bar->width() - thick, bar->y(), thick, thick);
	else
		return IntRect(bar->x(), bar->y() + bar->height() - thick, thick, thick);
}

IntRect ScrollbarThemeFLTK::trackRect(ScrollbarThemeClient *bar, bool painting) {
	const unsigned thick = scrollbarThickness();

	if (bar->orientation() == HorizontalScrollbar) {
		return IntRect(bar->x() + thick, bar->y(), bar->width() - 2*thick, thick);
	}

	return IntRect(bar->x(), bar->y() + thick, thick, bar->height() - 2*thick);
}

bool ScrollbarThemeFLTK::paint(ScrollbarThemeClient *bar, GraphicsContext *gc,
				const IntRect& rect) {

	if (gc->paintingDisabled())
		return false;

	cairo_t* cairo = gc->platformContext()->cr();
	ASSERT(cairo);

	cairo_surface_t *surf = cairo_get_target(cairo);
	cairo_matrix_t mat;
	cairo_get_matrix(cairo, &mat);
	cairo_surface_flush(surf);

	ASSERT(CAIRO_SURFACE_TYPE_XLIB == cairo_surface_get_type(surf));
	Drawable d = cairo_xlib_surface_get_drawable(surf);

	const unsigned x0 = mat.x0;
	const unsigned y0 = mat.y0;

	flbar->resize(bar->x() + x0, bar->y() + y0, bar->width(), bar->height());
	float div;
	if (bar->orientation() == HorizontalScrollbar) {
		flbar->type(FL_HORIZONTAL);
		div = trackRect(bar).width();
	} else {
		flbar->type(FL_VERTICAL);
		div = trackRect(bar).height();
	}

	flbar->slider_size(thumbLength(bar) / div);

	flbar->button1 = flbar->button2 = false;
	if (bar->pressedPart() == BackButtonStartPart)
		flbar->button1 = true;
	else if (bar->pressedPart() == ForwardButtonEndPart)
		flbar->button2 = true;

	flbar->maximum(bar->maximum());
	flbar->value(bar->currentPos());

	flbar->set_active();
	if (!bar->enabled())
		flbar->clear_active();

	fl_begin_offscreen(d);
	fl_push_clip(rect.x() + x0, rect.y() + y0, rect.width(), rect.height());
	((Fl_Widget *) flbar)->draw();

	fl_pop_clip();
	fl_end_offscreen();
	cairo_surface_mark_dirty_rectangle(surf, rect.x() + x0, rect.y() + y0,
						rect.width(), rect.height());

	return true;
}

int ScrollbarThemeFLTK::scrollbarThickness(ScrollbarControlSize) {
	return 16;
}

}
