/*
 * Copyright (C) 2009, 2010 Igalia S.L.
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
#include "DragClientFLTK.h"

#if ENABLE(DRAG_SUPPORT)

#include "Document.h"
#include "DragController.h"
#include "Element.h"
#include "Frame.h"
#include "NotImplemented.h"
#include "Pasteboard.h"
#include "RenderObject.h"
#include "webkitwebframeprivate.h"
#include "webkitwebviewprivate.h"
#include "webkitwebview.h"

using namespace WebCore;

namespace WebKit {

DragClient::DragClient(WebKitWebView* webView)
	: m_webView(webView)
	, m_startPos(0, 0)
{
}

DragClient::~DragClient()
{
}

void DragClient::willPerformDragDestinationAction(DragDestinationAction, DragData&)
{
}

void DragClient::willPerformDragSourceAction(DragSourceAction, const IntPoint& startPos, Clipboard&)
{
	m_startPos = startPos;
}

DragDestinationAction DragClient::actionMaskForDrag(DragData&)
{
	notImplemented();
	return DragDestinationActionAny;
}

DragSourceAction DragClient::dragSourceActionMaskForPoint(const IntPoint&)
{
	notImplemented();
	return DragSourceActionAny;
}

void DragClient::startDrag(DragImageRef image, const IntPoint& dragImageOrigin, const IntPoint& eventPos, Clipboard& clipboard, Frame& frame, bool linkDrag)
{
	notImplemented();
}

void DragClient::dragControllerDestroyed()
{
	delete this;
}
}

#endif // ENABLE(DRAG_SUPPORT)
