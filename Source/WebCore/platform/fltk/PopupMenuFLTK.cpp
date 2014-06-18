/*
 * Copyright (C) 2014 Lauri Kasanen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"

#include "PopupMenuFLTK.h"
#include <wtf/text/CString.h>

#include <FL/Fl_Menu_Item.H>

namespace WebCore {

static Vector<Fl_Menu_Item> items;

PopupMenuFLTK::PopupMenuFLTK(PopupMenuClient *c, const unsigned x, const unsigned y) {
	m_client = c;
	m_x = x;
	m_y = y;
}

PopupMenuFLTK::~PopupMenuFLTK() {
}

void PopupMenuFLTK::show(const IntRect &rect, FrameView *view, int index) {

	ASSERT(m_client);

	const unsigned max = m_client->listSize();
	if (!max) {
		m_client->popupDidHide();
		return;
	}

	unsigned i;
	items.clear();
	for (i = 0; i < max; i++) {
		if (!m_client->itemIsSeparator(i)) {
			Fl_Menu_Item it = {strdup(m_client->itemText(i).utf8().data()),
					0, 0, (void *) (unsigned long) i,
					0,
					FL_NORMAL_LABEL, FL_HELVETICA,
					FL_NORMAL_SIZE, FL_FOREGROUND_COLOR };
			items.append(it);
		}
	}
	Fl_Menu_Item end;
	memset(&end, 0, sizeof(Fl_Menu_Item));
	items.append(end);

	// Offset by the webview coords
	unsigned x = rect.x() + m_x;
	unsigned y = rect.y() + m_y;

	const Fl_Menu_Item *ptr = items[0].pulldown(x, y,
							rect.width(), rect.height());
	if (ptr)
		m_client->valueChanged((unsigned long) ptr->user_data_);

	m_client->popupDidHide();
}

void PopupMenuFLTK::hide() {

}

void PopupMenuFLTK::updateFromElement() {
	m_client->setTextFromItem(m_client->selectedIndex());
}

void PopupMenuFLTK::disconnectClient() {
	m_client = NULL;
}

}
