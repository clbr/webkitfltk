/*
 * Copyright (C) 2008 Gustavo Noronha Silva <gns@gnome.org>
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

#ifndef webkitwebwindowfeatures_h
#define webkitwebwindowfeatures_h

#include <webkit/webkitdefines.h>

class WebKitWebWindowFeatures {
public:
	WebKitWebWindowFeatures();
	~WebKitWebWindowFeatures();

	bool operator==(WebKitWebWindowFeatures &other) const {
		return (x == other.x &&
			y == other.y &&
			width == other.width &&
			height == other.height &&
			toolbar_visible == other.toolbar_visible &&
			statusbar_visible == other.statusbar_visible &&
			scrollbar_visible == other.scrollbar_visible &&
			menubar_visible == other.menubar_visible &&
			locationbar_visible == other.locationbar_visible &&
			fullscreen == other.fullscreen)
	}
private:
	int x;
	int y;
	int width;
	int height;

	bool toolbar_visible;
	bool statusbar_visible;
	bool scrollbar_visible;
	bool menubar_visible;
	bool locationbar_visible;

	bool fullscreen;
};

#endif				/* webkitwebwindowfeatures_h */
