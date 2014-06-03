/*
 * Copyright (C) 2008 Christian Dywan <christian@imendio.com>
 * Copyright (C) 2009 Jan Michael Alonzo <jmalonzo@gmail.com
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

#ifndef webkitwebsettings_h
#define webkitwebsettings_h

#include <webkit/webkitdefines.h>

/**
 * WebKitEditingBehavior:
 * @WEBKIT_EDITING_BEHAVIOR_MAC: Editing behavior mimicking OS X user interfaces.
 * @WEBKIT_EDITING_BEHAVIOR_WINDOWS: Editing behavior mimicking Windows user interfaces.
 * @WEBKIT_EDITING_BEHAVIOR_UNIX: Editing behavior mimicking free desktop user interfaces.
 *
 * Enum values used for determining the editing behavior of editable elements.
 *
 **/
typedef enum {
	WEBKIT_EDITING_BEHAVIOR_MAC,
	WEBKIT_EDITING_BEHAVIOR_WINDOWS,
	WEBKIT_EDITING_BEHAVIOR_UNIX
} WebKitEditingBehavior;

class WebKitWebSettings {
public:
	WebKitWebSettings();
	~WebKitWebSettings();

	const char *get_user_agent();
	const char *UserAgentForURI(const char *uri);

	CString defaultEncoding;
	CString cursiveFontFamily;
	CString defaultFontFamily;
	CString fantasyFontFamily;
	CString monospaceFontFamily;
	CString sansSerifFontFamily;
	CString serifFontFamily;
	unsigned defaultFontSize;
	unsigned defaultMonospaceFontSize;
	unsigned minimumFontSize;
	unsigned minimumLogicalFontSize;
	bool enforce96DPI;
	bool autoLoadImages;
	bool autoShrinkImages;
	bool respectImageOrientation;
	bool printBackgrounds;
	bool enableScripts;
	bool enablePlugins;
	bool resizableTextAreas;
	CString userStylesheetURI;
	float zoomStep;
	bool enableDeveloperExtras;
	bool enablePrivateBrowsing;
	bool enableSpellChecking;
	CString spellCheckingLanguages;
	bool enableCaretBrowsing;
	bool enableHTML5Database;
	bool enableHTML5LocalStorage;
	CString html5LocalStorageDatabasePath;
	bool enableXSSAuditor;
	bool enableSpatialNavigation;
	bool enableFrameFlattening;
	CString userAgent;
	bool javascriptCanOpenWindowsAutomatically;
	bool javascriptCanAccessClipboard;
	bool enableOfflineWebApplicationCache;
	WebKitEditingBehavior editingBehavior;
	bool enableUniversalAccessFromFileURIs;
	bool enableFileAccessFromFileURIs;
	bool enableDOMPaste;
	bool tabKeyCyclesThroughElements;
	bool enableDefaultContextMenu;
	bool enableSiteSpecificQuirks;
	bool enablePageCache;
	bool autoResizeWindow;
	bool enableJavaApplet;
	bool enableHyperlinkAuditing;
	bool enableFullscreen;
	bool enableDNSPrefetching;
	bool enableWebgl;
	bool enableMediaStream;
	bool enableWebAudio;
	bool enableAcceleratedCompositing;
	bool enableSmoothScrolling;
	bool mediaPlaybackRequiresUserGesture;
	bool mediaPlaybackAllowsInline;
	bool enableDisplayOfInsecureContent;
	bool enableRunningOfInsecureContent;
	bool enableMediaSource;
};

#endif
