/*
 * Copyright (C) 2007, 2008, 2009 Holger Hans Peter Freyther
 * Copyright (C) 2008 Jan Michael C. Alonzo
 * Copyright (C) 2008, 2010 Collabora Ltd.
 * Copyright (C) 2010 Igalia S.L.
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

#ifndef webkitwebviewprivate_h
#define webkitwebviewprivate_h

#include "AcceleratedCompositingContext.h"
#include "GeolocationClientMock.h"
#include "Page.h"
#include "ResourceHandle.h"
#include "ResourceResponse.h"
#include "WidgetBackingStore.h"
#include <webkit/webkitwebview.h>

#if ENABLE(MEDIA_STREAM)
#include "UserMediaClientFLTK.h"
#endif

#if ENABLE(NAVIGATOR_CONTENT_UTILS)
#include "NavigatorContentUtilsClientFLTK.h"
#endif

namespace WebKit {
	WebCore::Page * core(WebKitWebView *);
	WebKitWebView *kit(WebCore::Page *);
}

#endif
