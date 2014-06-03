/*
 * Copyright (C) 2010 Joone Hur <joone@kldp.org>
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

#ifndef webkitviewportattributes_h
#define webkitviewportattributes_h

#include <webkit/webkitdefines.h>

class WebKitWebView;

class WebKitViewportAttributes {
public:
	WebKitViewportAttributes();
	void recompute();

	WebKitWebView *webView;
	int deviceWidth;
	int deviceHeight;
	int availableWidth;
	int availableHeight;
	int desktopWidth;
	int deviceDPI;

	int width;
	int height;
	float initialScaleFactor;
	float minimumScaleFactor;
	float maximumScaleFactor;
	float devicePixelRatio;
	bool userScalable;
	bool isValid;
};

#endif
