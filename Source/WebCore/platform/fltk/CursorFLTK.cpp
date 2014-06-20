/*
 * Copyright (C) 2006 Dirk Mueller <mueller@kde.org>
 * Copyright (C) 2006 George Staikos <staikos@kde.org>
 * Copyright (C) 2006 Charles Samuels <charles@kde.org>
 * Copyright (C) 2008 Holger Hans Peter Freyther
 * Copyright (C) 2008 Kenneth Rohde Christiansen
 * Copyright (C) 2009-2010 ProFUSION embedded systems
 * Copyright (C) 2009-2010 Samsung Electronics
 *
 * All rights reserved.
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
#include "Cursor.h"

#include <FL/Enumerations.H>
#include <wtf/Assertions.h>

namespace WebCore {

Cursor::Cursor(const Cursor& other)
    : m_type(other.m_type)
    , m_image(other.m_image)
    , m_hotSpot(other.m_hotSpot)
#if ENABLE(MOUSE_CURSOR_SCALE)
    , m_imageScaleFactor(other.m_imageScaleFactor)
#endif
    , m_platformCursor(other.m_platformCursor)
{
}

Cursor::~Cursor()
{
}

Cursor& Cursor::operator=(const Cursor& other)
{
    m_type = other.m_type;
    m_image = other.m_image;
    m_hotSpot = other.m_hotSpot;
#if ENABLE(MOUSE_CURSOR_SCALE)
    m_imageScaleFactor = other.m_imageScaleFactor;
#endif
    m_platformCursor = other.m_platformCursor;

    return *this;
}

static Fl_Cursor cursormap(const Cursor::Type type)
{
	switch (type) {
		case Cursor::Pointer:
		break;
		case Cursor::Cross:
			return FL_CURSOR_CROSS;
		break;
		case Cursor::Hand:
			return FL_CURSOR_HAND;
		break;
		case Cursor::IBeam:
			return FL_CURSOR_INSERT;
		break;
		case Cursor::Wait:
			return FL_CURSOR_WAIT;
		break;
		case Cursor::Help:
			return FL_CURSOR_HELP;
		break;
		case Cursor::EastResize:
			return FL_CURSOR_E;
		break;
		case Cursor::NorthResize:
			return FL_CURSOR_N;
		break;
		case Cursor::NorthEastResize:
			return FL_CURSOR_NE;
		break;
		case Cursor::NorthWestResize:
			return FL_CURSOR_NW;
		break;
		case Cursor::SouthResize:
			return FL_CURSOR_S;
		break;
		case Cursor::SouthEastResize:
			return FL_CURSOR_SE;
		break;
		case Cursor::SouthWestResize:
			return FL_CURSOR_SW;
		break;
		case Cursor::WestResize:
			return FL_CURSOR_W;
		break;
		case Cursor::NorthSouthResize:
			return FL_CURSOR_NS;
		break;
		case Cursor::EastWestResize:
			return FL_CURSOR_WE;
		break;
		case Cursor::NorthEastSouthWestResize:
			return FL_CURSOR_NESW;
		break;
		case Cursor::NorthWestSouthEastResize:
			return FL_CURSOR_NWSE;
		break;
		case Cursor::ColumnResize:
			return FL_CURSOR_WE;
		break;
		case Cursor::RowResize:
			return FL_CURSOR_NS;
		break;
		case Cursor::MiddlePanning:
		break;
		case Cursor::EastPanning:
		break;
		case Cursor::NorthPanning:
		break;
		case Cursor::NorthEastPanning:
		break;
		case Cursor::NorthWestPanning:
		break;
		case Cursor::SouthPanning:
		break;
		case Cursor::SouthEastPanning:
		break;
		case Cursor::SouthWestPanning:
		break;
		case Cursor::WestPanning:
		break;
		case Cursor::Move:
			return FL_CURSOR_MOVE;
		break;
		case Cursor::VerticalText:
		break;
		case Cursor::Cell:
		break;
		case Cursor::ContextMenu:
		break;
		case Cursor::Alias:
		break;
		case Cursor::Progress:
		break;
		case Cursor::NoDrop:
		break;
		case Cursor::Copy:
		break;
		case Cursor::None:
		break;
		case Cursor::NotAllowed:
		break;
		case Cursor::ZoomIn:
		break;
		case Cursor::ZoomOut:
		break;
		case Cursor::Grab:
		break;
		case Cursor::Grabbing:
		break;
		case Cursor::Custom:
			return FL_CURSOR_DEFAULT;
	}

	return FL_CURSOR_DEFAULT;
}

void Cursor::ensurePlatformCursor() const
{
    if (m_platformCursor)
        return;
    m_platformCursor = cursormap(m_type);
}

}
