/*
 * Copyright (C) 2007 Holger Hans Peter Freyther
 * Copyright (C) 2007, 2008 Alp Toker <alp@atoker.com>
 * Copyright (C) 2008 Collabora Ltd.
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

#ifndef webkitwebview_h
#define webkitwebview_h

#include <JavaScriptCore/JSBase.h>
#include <webkit/webkitdefines.h>
#include <webkit/webkitwebbackforwardlist.h>
#include <webkit/webkitwebframe.h>
#include <webkit/webkitwebhistoryitem.h>
#include <webkit/webkitwebsettings.h>

/**
 * WebKitNavigationResponse:
 * @WEBKIT_NAVIGATION_RESPONSE_ACCEPT: Instruct WebKit to allow the navigation.
 * @WEBKIT_NAVIGATION_RESPONSE_IGNORE: Instruct WebKit to ignore the navigation.
 * @WEBKIT_NAVIGATION_RESPONSE_DOWNLOAD: Instruct WebKit to start a download of the destination instead.
 *
 * Enum values used to denote the various responses to a navigation policy decision.
 **/
typedef enum {
	WEBKIT_NAVIGATION_RESPONSE_ACCEPT,
	WEBKIT_NAVIGATION_RESPONSE_IGNORE,
	WEBKIT_NAVIGATION_RESPONSE_DOWNLOAD
} WebKitNavigationResponse;

/**
 * WebKitWebViewTargetInfo:
 * @WEBKIT_WEB_VIEW_TARGET_INFO_HTML: Rich markup data
 * @WEBKIT_WEB_VIEW_TARGET_INFO_TEXT: Text data
 * @WEBKIT_WEB_VIEW_TARGET_INFO_IMAGE: Image data
 * @WEBKIT_WEB_VIEW_TARGET_INFO_URI_LIST: URI list data
 * @WEBKIT_WEB_VIEW_TARGET_INFO_NETSCAPE_URL: A single URL in the Netscape protocol
 *
 * Enum values used to denote the info value of various selection types. These can be used
 * to interpret the data WebKitGTK+ publishes via FLTKClipboard and drag-and-drop.
 **/
typedef enum {
	WEBKIT_WEB_VIEW_TARGET_INFO_HTML,
	WEBKIT_WEB_VIEW_TARGET_INFO_TEXT,
	WEBKIT_WEB_VIEW_TARGET_INFO_IMAGE,
	WEBKIT_WEB_VIEW_TARGET_INFO_URI_LIST,
	WEBKIT_WEB_VIEW_TARGET_INFO_NETSCAPE_URL
} WebKitWebViewTargetInfo;

/**
 * WebKitWebViewViewMode:
 * @WEBKIT_WEB_VIEW_VIEW_MODE_WINDOWED: Windowed view mode
 * @WEBKIT_WEB_VIEW_VIEW_MODE_FLOATING: Floating view mode
 * @WEBKIT_WEB_VIEW_VIEW_MODE_FULLSCREEN: Fullscreen view mode
 * @WEBKIT_WEB_VIEW_VIEW_MODE_MAXIMIZED: Maximized view mode
 * @WEBKIT_WEB_VIEW_VIEW_MODE_MINIMIZED: Minimized view mode
 *
 * Enum values used to denote the various types of view modes. See the
 * #WebKitWebView:view-mode property.
 **/
typedef enum {
	WEBKIT_WEB_VIEW_VIEW_MODE_WINDOWED,
	WEBKIT_WEB_VIEW_VIEW_MODE_FLOATING,
	WEBKIT_WEB_VIEW_VIEW_MODE_FULLSCREEN,
	WEBKIT_WEB_VIEW_VIEW_MODE_MAXIMIZED,
	WEBKIT_WEB_VIEW_VIEW_MODE_MINIMIZED
} WebKitWebViewViewMode;

typedef enum {
	WEBKIT_SELECTION_AFFINITY_UPSTREAM,
	WEBKIT_SELECTION_AFFINITY_DOWNSTREAM,
} WebKitSelectionAffinity;

typedef enum {
	WEBKIT_INSERT_ACTION_TYPED,
	WEBKIT_INSERT_ACTION_PASTED,
	WEBKIT_INSERT_ACTION_DROPPED,
} WebKitInsertAction;

class WebKitWebView {
public:
	WebKitWebView(WebKitWebFrame * web_frame);
	WebKitWebView();
	~WebKitWebView();

	bool web_view_ready();

	bool close_web_view();

	WebKitNavigationResponse navigation_requested(WebKitWebFrame *frame,
							WebKitNetworkRequest *request);
	void window_object_cleared(WebKitWebFrame * frame,
				       JSGlobalContextRef context,
				       JSObjectRef window_object);
	char *choose_file(WebKitWebFrame * frame,
			      const char *old_file);
	bool script_alert(WebKitWebFrame * frame,
			      const char *alert_message);
	bool script_confirm(WebKitWebFrame * frame,
				const char *confirm_message,
				bool * did_confirm);
	bool script_prompt(WebKitWebFrame * frame,
			       const char *message, const char *default_value,
			       char **value);
	bool console_message(const char *message,
				 unsigned line_number, const char *source_id);
	bool move_cursor(FLTKMovementStep step, int count);

	void set_scroll_adjustments(FLTKAdjustment * hadjustment,
					FLTKAdjustment * vadjustment);

	void undo();
	void redo();
	bool should_allow_editing_action();
	bool entering_fullscreen();
	bool leaving_fullscreen();
	bool run_file_chooser(WebKitFileChooserRequest * request);

	const char *get_title();
	const char *get_uri();
	void set_maintains_back_forward_list(bool flag);
	WebKitWebBackForwardList *get_back_forward_list();

	bool go_to_back_forward_item(WebKitWebHistoryItem * item);

	bool can_go_back();
	bool can_go_back_or_forward(int steps);
	bool can_go_forward();

	void go_back();
	void go_back_or_forward(int steps);
	void go_forward();

	void stop_loading();
	void reload();
	void reload_bypass_cache();

	void load_uri(const char *uri);
	void load_string(const char *content,
			const char *mime_type,
			const char *encoding, const char *base_uri);
	void load_request(WebKitNetworkRequest * request);

	bool can_cut_clipboard();
	bool can_copy_clipboard();
	bool can_paste_clipboard();
	void cut_clipboard();
	void copy_clipboard();
	void paste_clipboard();
	void delete_selection();
	bool has_selection();
	void select_all();
	bool get_editable();

	bool search_text(const char *text,
			    bool case_sensitive, bool forward, bool wrap);

	unsigned mark_text_matches(const char *string,
				  bool case_sensitive, unsigned limit);
	void set_highlight_text_matches(bool highlight);

	void unmark_text_matches();

	WebKitWebFrame *get_main_frame();
	WebKitWebFrame *get_focused_frame();

	void execute_script(const char *script);

	void set_editable(bool flag);

	FLTKTargetList *get_copy_target_list();
	FLTKTargetList *get_paste_target_list();

	void set_settings(WebKitWebSettings * settings);
	WebKitWebSettings *get_settings();
	WebKitWebInspector *get_inspector();
	WebKitWebWindowFeatures *get_window_features();

	bool can_show_mime_type(const char *mime_type);

	bool get_transparent();
	void set_transparent(bool flag);

	float get_zoom_level();
	void set_zoom_level(float zoom_level);

	void zoom_in();
	void zoom_out();
	bool get_full_content_zoom();
	void set_full_content_zoom(bool full_content_zoom);

	const char *get_encoding();
	void set_custom_encoding(const char *encoding);
	const char *get_custom_encoding();

	void set_view_mode(WebKitWebViewViewMode mode);

	WebKitWebViewViewMode get_view_mode();

	void move_cursor(FLTKMovementStep step, int count);

	WebKitLoadStatus get_load_status();
	double get_progress();

	bool can_undo();
	bool can_redo();

	void set_view_source_mode(bool view_source_mode);

	bool get_view_source_mode();

	WebKitHitTestResult *get_hit_test_result(GdkEventButton * event);

	const char *get_icon_uri();

	GdkPixbuf *try_get_favicon_pixbuf(unsigned width,
					     unsigned height);

	WebKitDOMDocument *get_dom_document();

	WebKitViewportAttributes *get_viewport_attributes();

	cairo_surface_t *get_snapshot();

	void notify_ready();
	void request_download(WebKitNetworkRequest *,
				      const WebCore::ResourceResponse & =
				      WebCore::ResourceResponse(),
				      WebCore::ResourceHandle * = 0);

	void add_resource(const char *,
				  WebKitWebResource *);
	void add_main_resource(const char *,
				       WebKitWebResource *);
	void webkitWebViewRemoveSubresource(const char *);
	WebKitWebResource *get_resource(char *);
	WebKitWebResource *get_main_resource();
	void clear_resources();
	GList *get_subresources();

	void set_tooltip_text(const char *);
	FLTKMenu *get_context_menu();

	void run_file_chooser_request(WebKitFileChooserRequest *);

#if ENABLE(ICONDATABASE)
	void register_for_icon_notification(bool shouldRegister);
	void icon_loaded(const char *frameURI, WebKitWebView *);
#endif
private:
	WebCore::Page * corePage;
	bool hasNativeWindow;
	OwnPtr <WebCore::WidgetBackingStore> backingStore;
	WebKitWebSettings *webSettings;
	WebKitWebInspector *webInspector;
	WebKitViewportAttributes *viewportAttributes;
	WebKitWebWindowFeatures *webWindowFeatures;

	WebKitWebFrame *mainFrame;
	WebKitWebBackForwardList *backForwardList;

	FLTKMenu *currentMenu;
	int lastPopupXPosition;
	int lastPopupYPosition;

	HashSet <Fl_Widget *> children;

	bool transparent;
	bool needsResizeOnMap;

	bool zoomFullContent;
	WebKitLoadStatus loadStatus;
	CString encoding;
	CString customEncoding;

	CString iconURI;

	bool disposing;

	// These are hosted here because the DataSource object is
	// created too late in the frame loading process.
	GRefPtr < WebKitWebResource > mainResource;
	CString mainResourceIdentifier;
	GRefPtr < GHashTable > subResources;
	CString tooltipText;
	WebCore::IntRect tooltipArea;

	bool selfScrolling;

#if ENABLE(ICONDATABASE)
	unsigned long iconLoadedHandler;
#endif

#if ENABLE(MEDIA_STREAM)
	OwnPtr <WebKit::UserMediaClientFLTK> userMediaClient;
#endif

#if ENABLE(GEOLOCATION)
	OwnPtr <WebCore::GeolocationClientMock> geolocationClientMock;
#endif

#if ENABLE(NAVIGATOR_CONTENT_UTILS)
	OwnPtr <WebKit::NavigatorContentUtilsClient> navigatorContentUtilsClient;
#endif
};

#endif
