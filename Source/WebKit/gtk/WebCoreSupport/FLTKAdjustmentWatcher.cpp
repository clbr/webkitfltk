/*
 *	Copyright (C) 2011 Igalia S.L.
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
#include "FLTKAdjustmentWatcher.h"

#include "FrameView.h"
#include "MainFrame.h"
#include "Page.h"
#include "Scrollbar.h"
#include "webkitwebviewprivate.h"

using namespace WebCore;

namespace WebKit {

FLTKAdjustmentWatcher::FLTKAdjustmentWatcher(WebKitWebView* webView)
	: m_webView(webView)
	, m_scrollbarsDisabled(false)
	, m_handlingFLTKAdjustmentChange(false)
	, m_updateAdjustmentCallbackId(0)
{
}

FLTKAdjustmentWatcher::~FLTKAdjustmentWatcher()
{
}

static void updateAdjustmentFromScrollbar(FLTKAdjustment* adjustment, Scrollbar* scrollbar)
{
	if (!adjustment)
		return;
	if (!scrollbar) {
		return;
	}
	notImplemented();
}

void FLTKAdjustmentWatcher::updateAdjustmentsFromScrollbars()
{
	if (m_scrollbarsDisabled)
		return;
	if (m_handlingFLTKAdjustmentChange)
		return;
	if (!core(m_webView))
		return;

	FrameView* frameView = core(m_webView)->mainFrame().view();
	updateAdjustmentFromScrollbar(m_horizontalAdjustment.get(), frameView->horizontalScrollbar());
	updateAdjustmentFromScrollbar(m_verticalAdjustment.get(), frameView->verticalScrollbar());
	if (m_updateAdjustmentCallbackId) {
		g_source_remove(m_updateAdjustmentCallbackId);
		m_updateAdjustmentCallbackId = 0;
	}
}

bool FLTKAdjustmentWatcher::updateAdjustmentCallback(FLTKAdjustmentWatcher* watcher)
{
	watcher->m_updateAdjustmentCallbackId = 0;
	watcher->updateAdjustmentsFromScrollbars();
	return FALSE;
}

void FLTKAdjustmentWatcher::updateAdjustmentsFromScrollbarsLater() const
{
	// We've already scheduled an update. No need to schedule another.
	if (m_updateAdjustmentCallbackId || m_scrollbarsDisabled)
		return;

	// The fact that this method was called means that we need to update the scrollbars, but at the
	// time of invocation they are not updated to reflect the scroll yet. We set a short timeout
	// here, which means that they will be updated as soon as WebKit returns to the main loop.
}

static void adjustmentValueChangedCallback(FLTKAdjustment* adjustment, FLTKAdjustmentWatcher* watcher)
{
	watcher->adjustmentValueChanged(adjustment);
}

static void setAdjustment(FLTKAdjustmentWatcher* watcher, GRefPtr<FLTKAdjustment>& adjustmentMember, GtkAdjustment* newAdjustment)
{
	notImplemented();
}

void FLTKAdjustmentWatcher::setHorizontalAdjustment(FLTKAdjustment* newAdjustment)
{
	setAdjustment(this, m_horizontalAdjustment, newAdjustment);
}

void FLTKAdjustmentWatcher::setVerticalAdjustment(FLTKAdjustment* newAdjustment)
{
	setAdjustment(this, m_verticalAdjustment, newAdjustment);
}

void FLTKAdjustmentWatcher::adjustmentValueChanged(FLTKAdjustment* adjustment)
{
	FrameView* frameView = core(m_webView)->mainFrame().view();
	Scrollbar* scrollbar = (adjustment == m_horizontalAdjustment.get()) ?
		frameView->horizontalScrollbar() : frameView->verticalScrollbar();
	if (!scrollbar)
		return;

	int newValue = static_cast<int>(gtk_adjustment_get_value(adjustment));
	if (newValue != scrollbar->value()) {
		m_handlingFLTKAdjustmentChange = true;
		frameView->scrollToOffsetWithoutAnimation(scrollbar->orientation(), newValue);
		m_handlingFLTKAdjustmentChange = false;
	}
}

void FLTKAdjustmentWatcher::disableAllScrollbars()
{
	updateAdjustmentFromScrollbar(m_horizontalAdjustment.get(), 0);
	updateAdjustmentFromScrollbar(m_verticalAdjustment.get(), 0);
	m_scrollbarsDisabled = true;
}

void FLTKAdjustmentWatcher::enableAllScrollbars()
{
	m_scrollbarsDisabled = false;
	updateAdjustmentsFromScrollbars();
}

} // namespace WebKit

