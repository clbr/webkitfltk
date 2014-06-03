/*
 *	Copyright (C) 2007, 2008 Holger Hans Peter Freyther
 *	Copyright (C) 2007, 2008, 2009 Christian Dywan <christian@imendio.com>
 *	Copyright (C) 2007 Xan Lopez <xan@gnome.org>
 *	Copyright (C) 2007, 2008 Alp Toker <alp@atoker.com>
 *	Copyright (C) 2008 Jan Alonzo <jmalonzo@unpluggable.com>
 *	Copyright (C) 2008 Gustavo Noronha Silva <gns@gnome.org>
 *	Copyright (C) 2008 Nuanti Ltd.
 *	Copyright (C) 2008, 2009, 2010 Collabora Ltd.
 *	Copyright (C) 2009, 2010, 2012 Igalia S.L.
 *	Copyright (C) 2009 Movial Creative Technologies Inc.
 *	Copyright (C) 2009 Bobby Powers
 *	Copyright (C) 2010 Joone Hur <joone@kldp.org>
 *	Copyright (C) 2012 Igalia S.L.
 *	Copyright (C) 2013 Apple Inc. All rights reserved.
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
#include "webkitwebview.h"

#include "AXObjectCache.h"
#include "ArchiveResource.h"
#include "BackForwardController.h"
#include "BackForwardList.h"
#include "BatteryClientFLTK.h"
#include "CairoUtilities.h"
#include "Chrome.h"
#include "ChromeClientFLTK.h"
#include "ClipboardUtilitiesFLTK.h"
#include "ContextMenu.h"
#include "ContextMenuClientFLTK.h"
#include "ContextMenuController.h"
#include "Cursor.h"
#include "DatabaseManager.h"
#include "Document.h"
#include "DocumentLoader.h"
#include "DragActions.h"
#include "DragClientFLTK.h"
#include "DragController.h"
#include "DragData.h"
#include "DragSession.h"
#include "DumpRenderTreeSupportFLTK.h"
#include "Editor.h"
#include "EditorClientFLTK.h"
#include "EventHandler.h"
#include "FloatQuad.h"
#include "FocusController.h"
#include "FrameLoader.h"
#include "FrameLoaderClientFLTK.h"
#include "FrameLoaderTypes.h"
#include "FrameView.h"
#include "GUniquePtrFLTK.h"
#include "GeolocationClientFLTK.h"
#include "GeolocationController.h"
#include "GraphicsContext.h"
#include "FLTKUtilities.h"
#include "HTMLNames.h"
#include "HitTestRequest.h"
#include "HitTestResult.h"
#include "InspectorClientFLTK.h"
#include "MainFrame.h"
#include "MemoryCache.h"
#include "MouseEventWithHitTestResults.h"
#include "NotImplemented.h"
#include "PageCache.h"
#include "Pasteboard.h"
#include "PasteboardHelper.h"
#include "PlatformKeyboardEvent.h"
#include "PlatformWheelEvent.h"
#include "ProgressTracker.h"
#include "ProgressTrackerClientFLTK.h"
#include "RenderView.h"
#include "ResourceHandle.h"
#include "RuntimeEnabledFeatures.h"
#include "ScriptController.h"
#include "Settings.h"
#include "WebKitDOMDocumentPrivate.h"
#include "webkitdownload.h"
#include "webkitfavicondatabase.h"
#include "webkitgeolocationpolicydecision.h"
#include "webkitglobalsprivate.h"
#include "webkithittestresultprivate.h"
#include "webkiticondatabase.h"
#include "webkitnetworkrequest.h"
#include "webkitnetworkresponse.h"
#include "webkitviewportattributes.h"
#include "webkitwebbackforwardlist.h"
#include "webkitwebframeprivate.h"
#include "webkitwebhistoryitem.h"
#include "webkitwebhistoryitemprivate.h"
#include "webkitwebinspector.h"
#include "webkitwebpolicydecision.h"
#include "webkitwebresource.h"
#include "webkitwebplugindatabaseprivate.h"
#include "webkitwebwindowfeatures.h"
#include "webkitwebviewprivate.h"
#include <bindings/ScriptValue.h>
#include <gdk/gdkkeysyms.h>
#include <wtf/text/CString.h>

#if ENABLE(DEVICE_ORIENTATION)
#include "DeviceMotionClientFLTK.h"
#include "DeviceOrientationClientFLTK.h"
#endif

/**
 * SECTION:webkitwebview
 * @short_description: The central class of the WebKitGTK+ API
 * @see_also: #WebKitWebSettings, #WebKitWebFrame
 *
 * #WebKitWebView is the central class of the WebKitGTK+ API. It is a
 * #Fl_Widget implementing the scrolling interface which means you can
 * embed in a #FLTKScrolledWindow. It is responsible for managing the
 * drawing of the content, forwarding of events. You can load any URI
 * into the #WebKitWebView or any kind of data string. With #WebKitWebSettings
 * you can control various aspects of the rendering and loading of the content.
 * Each #WebKitWebView has exactly one #WebKitWebFrame as main frame. A
 * #WebKitWebFrame can have n children.
 *
 * <programlisting>
 * /<!-- -->* Create the widgets *<!-- -->/
 * Fl_Widget *main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
 * Fl_Widget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
 * Fl_Widget *web_view = webkit_web_view_new ();
 *
 * /<!-- -->* Place the WebKitWebView in the FLTKScrolledWindow *<!-- -->/
 * gtk_container_add (GTK_CONTAINER (scrolled_window), web_view);
 * gtk_container_add (GTK_CONTAINER (main_window), scrolled_window);
 *
 * /<!-- -->* Open a webpage *<!-- -->/
 * webkit_web_view_load_uri (WEBKIT_WEB_VIEW (web_view), "http://www.gnome.org");
 *
 * /<!-- -->* Show the result *<!-- -->/
 * gtk_window_set_default_size (GTK_WINDOW (main_window), 800, 600);
 * gtk_widget_show_all (main_window);
 * </programlisting>
 */

using namespace WebKit;
using namespace WebCore;

static void webkit_web_view_settings_notify(WebKitWebSettings* webSettings, GParamSpec* pspec, WebKitWebView* webView);
static void webkit_web_view_set_window_features(WebKitWebView* webView, WebKitWebWindowFeatures* webWindowFeatures);
static void webkitWebViewDirectionChanged(WebKitWebView*, FLTKTextDirection previousDirection, gpointer);

#if ENABLE(CONTEXT_MENUS)
static void PopupMenuPositionFunc(FLTKMenu* menu, int *x, int *y, bool *pushIn, gpointer userData)
{
	WebKitWebView* view = WEBKIT_WEB_VIEW(userData);
	WebKitWebViewPrivate* priv = view->priv;
	GdkScreen* screen = gtk_widget_get_screen(GTK_WIDGET(view));
	FLTKRequisition menuSize;

	gtk_widget_size_request(GTK_WIDGET(menu), &menuSize);

	*x = priv->lastPopupXPosition;
	if ((*x + menuSize.width) >= gdk_screen_get_width(screen))
	  *x -= menuSize.width;

	*y = priv->lastPopupYPosition;
	if ((*y + menuSize.height) >= gdk_screen_get_height(screen))
	  *y -= menuSize.height;

	*pushIn = FALSE;
}
#endif

static Node* getFocusedNode(Frame* frame)
{
	if (Document* doc = frame->document())
		return doc->focusedElement();
	return 0;
}

#if ENABLE(CONTEXT_MENUS)
static void contextMenuItemActivated(FLTKMenuItem* item, ContextMenuController* controller)
{
	ContextMenuItem contextItem(item);
	controller->contextMenuItemSelected(&contextItem);
}

static void contextMenuConnectActivate(FLTKMenuItem* item, ContextMenuController* controller)
{
	if (GTK_IS_SEPARATOR_MENU_ITEM(item))
		return;

	if (Fl_Widget* menu = gtk_menu_item_get_submenu(item)) {
		gtk_container_foreach(GTK_CONTAINER(menu), (FLTKCallback)contextMenuConnectActivate, controller);
		return;
	}

}

static MouseEventWithHitTestResults prepareMouseEventForFrame(Frame* frame, const PlatformMouseEvent& event)
{
	HitTestRequest request(HitTestRequest::Active| HitTestRequest::DisallowShadowContent);
	IntPoint point = frame->view()->windowToContents(event.position());
	return frame->document()->prepareMouseEvent(request, point, event);
}

// Check enable-default-context-menu setting for compatibility.
static bool defaultContextMenuEnabled(WebKitWebView* webView)
{
	bool enableDefaultContextMenu;
	g_object_get(webkit_web_view_get_settings(webView), "enable-default-context-menu", &enableDefaultContextMenu, NULL);
	return enableDefaultContextMenu;
}

static bool webkit_web_view_forward_context_menu_event(WebKitWebView* webView, const PlatformMouseEvent& event, bool triggeredWithKeyboard)
{
	Page* page = core(webView);
	page->contextMenuController().clearContextMenu();
	Frame* focusedFrame;
	Frame* mainFrame = &page->mainFrame();
	bool mousePressEventResult = FALSE;
	GRefPtr<WebKitHitTestResult> hitTestResult;

	if (!mainFrame->view())
		return FALSE;

	mainFrame->view()->setCursor(pointerCursor());
	if (page->subframeCount()) {
		MouseEventWithHitTestResults mev = prepareMouseEventForFrame(mainFrame, event);
		Frame* targetFrame = EventHandler::subframeForHitTestResult(mev);
		if (!targetFrame)
			targetFrame = mainFrame;

		focusedFrame = &page->focusController().focusedOrMainFrame();
		if (targetFrame != focusedFrame) {
			page->focusController().setFocusedFrame(targetFrame);
			focusedFrame = targetFrame;
		}
		if (focusedFrame == mainFrame)
			hitTestResult = adoptGRef(kit(mev.hitTestResult()));
	} else
		focusedFrame = mainFrame;

	if (focusedFrame->view() && focusedFrame->eventHandler().handleMousePressEvent(event))
		mousePressEventResult = TRUE;

	bool handledEvent = focusedFrame->eventHandler().sendContextMenuEvent(event);
	if (!handledEvent)
		return FALSE;

	// If coreMenu is NULL, this means WebCore decided to not create
	// the default context menu; this may happen when the page is
	// handling the right-click for reasons other than the context menu.
	ContextMenu* coreMenu = page->contextMenuController().contextMenu();
	if (!coreMenu)
		return mousePressEventResult;

	FLTKMenu* defaultMenu = coreMenu->platformDescription();
	ASSERT(defaultMenu);

	// We connect the "activate" signal here rather than in ContextMenuFLTK to avoid
	// a layering violation. ContextMenuFLTK should not know about the ContextMenuController.
	gtk_container_foreach(GTK_CONTAINER(defaultMenu), reinterpret_cast<FLTKCallback>(contextMenuConnectActivate), &page->contextMenuController());

	if (!hitTestResult) {
		MouseEventWithHitTestResults mev = prepareMouseEventForFrame(focusedFrame, event);
		hitTestResult = adoptGRef(kit(mev.hitTestResult()));
	}

	bool handled;
	if (handled)
		return TRUE;

	// Return now if default context menu is disabled by enable-default-context-menu setting.
	// Check enable-default-context-menu setting for compatibility.
	if (!defaultContextMenuEnabled(webView))
		return FALSE;

	// Emit populate-popup signal for compatibility.

	// If the context menu is now empty, don't show it.
	GUniquePtr<GList> items(gtk_container_get_children(GTK_CONTAINER(defaultMenu)));
	if (!items)
		return FALSE;

	WebKitWebViewPrivate* priv = webView->priv;
	priv->currentMenu = defaultMenu;
	priv->lastPopupXPosition = event.globalPosition().x();
	priv->lastPopupYPosition = event.globalPosition().y();

	gtk_menu_popup(defaultMenu, 0, 0, &PopupMenuPositionFunc, webView, event.button() + 1, gtk_get_current_event_time());
	return TRUE;
}

static const int gContextMenuMargin = 1;
static IntPoint getLocationForKeyboardGeneratedContextMenu(Frame* frame)
{
	FrameSelection& selection = frame->selection();
	if (!selection.selection().isNonOrphanedCaretOrRange()
		|| (selection.selection().isCaret() && !selection.selection().isContentEditable())) {
		if (Node* focusedNode = getFocusedNode(frame))
			return focusedNode->pixelSnappedBoundingBox().location();

		// There was no selection and no focused node, so just put the context
		// menu into the corner of the view, offset slightly.
		return IntPoint(gContextMenuMargin, gContextMenuMargin);
	}

	// selection->selection().firstRange can return 0 here, but if that was the case
	// selection->selection().isNonOrphanedCaretOrRange() would have returned false
	// above, so we do not have to check it.
	IntRect firstRect = frame->editor().firstRectForRange(selection.selection().firstRange().get());
	return IntPoint(firstRect.x(), firstRect.maxY());
}

static bool webkit_web_view_popup_menu_handler(Fl_Widget* widget)
{
	Frame& frame = core(WEBKIT_WEB_VIEW(widget))->focusController().focusedOrMainFrame();
	IntPoint location = getLocationForKeyboardGeneratedContextMenu(&frame);

	FrameView* view = frame.view();
	if (!view)
		return FALSE;

	// Never let the context menu touch the very edge of the view.
	location = view->contentsToWindow(location);
	location.expandedTo(IntPoint(gContextMenuMargin, gContextMenuMargin));
	location.shrunkTo(IntPoint(view->width() - gContextMenuMargin, view->height() - gContextMenuMargin));

	IntPoint globalPoint(convertWidgetPointToScreenPoint(widget, location));
	PlatformMouseEvent event(location, globalPoint, RightButton, PlatformEvent::MousePressed, 0, false, false, false, false, gtk_get_current_event_time());
	return webkit_web_view_forward_context_menu_event(WEBKIT_WEB_VIEW(widget), event, true);
}
#endif // ENABLE(CONTEXT_MENUS)

static void setHorizontalAdjustment(WebKitWebView* webView, FLTKAdjustment* adjustment)
{
	// This may be called after the page has been destroyed, in which case we do nothing.
	Page* page = core(webView);
	if (page)
		static_cast<WebKit::ChromeClient&>(page->chrome().client()).adjustmentWatcher()->setHorizontalAdjustment(adjustment);
}

static void setVerticalAdjustment(WebKitWebView* webView, FLTKAdjustment* adjustment)
{
	// This may be called after the page has been destroyed, in which case we do nothing.
	Page* page = core(webView);
	if (page)
		static_cast<WebKit::ChromeClient&>(page->chrome().client()).adjustmentWatcher()->setVerticalAdjustment(adjustment);
}

static bool webkit_web_view_expose_event(Fl_Widget* widget, GdkEventExpose* event)
{
	int rectCount;
	GUniqueOutPtr<GdkRectangle> rects;
	gdk_region_get_rectangles(event->region, &rects.outPtr(), &rectCount);

	RefPtr<cairo_t> cr = adoptRef(gdk_cairo_create(event->window));

	WebKitWebViewPrivate* priv = WEBKIT_WEB_VIEW(widget)->priv;
#if USE(TEXTURE_MAPPER)
	GdkRectangle clipRect;
	gdk_region_get_clipbox(event->region, &clipRect);
	if (priv->acceleratedCompositingContext->renderLayersToWindow(cr.get(), clipRect)) {
		GTK_WIDGET_CLASS(webkit_web_view_parent_class)->expose_event(widget, event);
		return FALSE;
	}
#endif

	for (int i = 0; i < rectCount; i++) {
		copyRectFromCairoSurfaceToContext(WEBKIT_WEB_VIEW(widget)->priv->backingStore->cairoSurface(),
										  cr.get(), IntSize(), IntRect(rects.get()[i]));
	}

	// Chaining up to the parent forces child widgets to be drawn.
	GTK_WIDGET_CLASS(webkit_web_view_parent_class)->expose_event(widget, event);
	return FALSE;
}

static bool webkit_web_view_key_press_event(Fl_Widget* widget, GdkEventKey* event)
{
/*	if (WEBKIT_WEB_VIEW(widget)->priv->imFilter.filterKeyEvent(event))
		return TRUE;
	return GTK_WIDGET_CLASS(webkit_web_view_parent_class)->key_press_event(widget, 	event);*/
}

static bool webkit_web_view_key_release_event(Fl_Widget* widget, GdkEventKey* event)
{
/*	if (WEBKIT_WEB_VIEW(widget)->priv->imFilter.filterKeyEvent(event))
		return TRUE;
	return GTK_WIDGET_CLASS(webkit_web_view_parent_class)->key_release_event(widget, event);*/
}

static bool webkit_web_view_button_press_event(Fl_Widget* widget, GdkEventButton* event)
{
	WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
	WebKitWebViewPrivate* priv = webView->priv;

	// FIXME: need to keep track of subframe focus for key events
	gtk_widget_grab_focus(widget);

	if (!priv->clickCounter.shouldProcessButtonEvent(event))
		return TRUE;

	PlatformMouseEvent platformEvent(event);
	int count = priv->clickCounter.clickCountForGdkButtonEvent(widget, event);
	platformEvent.setClickCount(count);

#if ENABLE(CONTEXT_MENUS)
	if (event->button == 3)
		return webkit_web_view_forward_context_menu_event(webView, PlatformMouseEvent(event), false);
#endif

	Frame& frame = core(webView)->mainFrame();
	if (!frame.view())
		return FALSE;

//	priv->imFilter.notifyMouseButtonPress();
	bool result = frame.eventHandler().handleMousePressEvent(platformEvent);

	return result;
}

static bool webkit_web_view_button_release_event(Fl_Widget* widget, GdkEventButton* event)
{
	WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);

	Frame& mainFrame = core(webView)->mainFrame();
	if (mainFrame.view())
		mainFrame.eventHandler().handleMouseReleaseEvent(PlatformMouseEvent(event));

	/* We always return FALSE here because WebKit can, for the same click, decide
	 * to not handle press-event but handle release-event, which can totally confuse
	 * some GTK+ containers when there are no other events in between. This way we
	 * guarantee that this case never happens, and that if press-event goes through
	 * release-event also goes through.
	 */

	return FALSE;
}

static bool webkit_web_view_motion_event(Fl_Widget* widget, GdkEventMotion* event)
{
	WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);

	Frame& frame = core(webView)->mainFrame();
	if (!frame.view())
		return FALSE;

	return frame.eventHandler().mouseMoved(PlatformMouseEvent(event));
}

static bool webkit_web_view_scroll_event(Fl_Widget* widget, GdkEventScroll* event)
{
	WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);

	Frame& frame = core(webView)->mainFrame();
	if (!frame.view())
		return FALSE;

	PlatformWheelEvent wheelEvent(event);
	return frame.eventHandler().handleWheelEvent(wheelEvent);
}

static void webkit_web_view_size_request(Fl_Widget* widget, GtkRequisition* requisition)
{
	WebKitWebView* web_view = WEBKIT_WEB_VIEW(widget);
	Frame* coreFrame = core(webkit_web_view_get_main_frame(web_view));
	if (!coreFrame)
		return;

	FrameView* view = coreFrame->view();
	if (!view)
		return;

	requisition->width = view->contentsWidth();
	requisition->height = view->contentsHeight();
}

static void updateChildAllocationFromPendingAllocation(Fl_Widget* child, void*)
{
	if (!gtk_widget_get_visible(child))
		return;

	FLTKAllocation* allocation = static_cast<GtkAllocation*>(g_object_get_data(G_OBJECT(child), "delayed-allocation"));
	if (!allocation)
		return;

	g_object_set_data(G_OBJECT(child), "delayed-allocation", 0);
	gtk_widget_size_allocate(child, allocation);
	*allocation = IntRect();
}

static void resizeWebViewFromAllocation(WebKitWebView* webView, FLTKAllocation* allocation, bool sizeChanged)
{
	Page* page = core(webView);
	IntSize oldSize;
	FrameView* frameView = page->mainFrame().view();
	if (sizeChanged && frameView) {
		oldSize = frameView->size();
		frameView->resize(allocation->width, allocation->height);
	}

	gtk_container_forall(GTK_CONTAINER(webView), updateChildAllocationFromPendingAllocation, 0);

	if (!sizeChanged)
		return;

	WebKit::ChromeClient& chromeClient = static_cast<WebKit::ChromeClient&>(page->chrome().client());
	chromeClient.widgetSizeChanged(oldSize, IntSize(allocation->width, allocation->height));
	chromeClient.adjustmentWatcher()->updateAdjustmentsFromScrollbars();
}

static void webkit_web_view_size_allocate(Fl_Widget* widget, GtkAllocation* allocation)
{
	FLTKAllocation oldAllocation;
	gtk_widget_get_allocation(widget, &oldAllocation);
	bool sizeChanged = allocation->width != oldAllocation.width || allocation->height != oldAllocation.height;

	GTK_WIDGET_CLASS(webkit_web_view_parent_class)->size_allocate(widget, allocation);

	WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
	if (sizeChanged && !gtk_widget_get_mapped(widget)) {
		webView->priv->needsResizeOnMap = true;
		return;
	}
	resizeWebViewFromAllocation(webView, allocation, sizeChanged);
}

static void webkitWebViewMap(Fl_Widget* widget)
{
	GTK_WIDGET_CLASS(webkit_web_view_parent_class)->map(widget);

	WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
	if (!webView->priv->needsResizeOnMap)
		return;

	FLTKAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);
	resizeWebViewFromAllocation(webView, &allocation, true);
	webView->priv->needsResizeOnMap = false;
}

static void webkit_web_view_grab_focus(Fl_Widget* widget)
{

	if (gtk_widget_is_sensitive(widget)) {
		WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
		FocusController& focusController = core(webView)->focusController();

		focusController.setActive(true);

		if (focusController.focusedFrame())
			focusController.setFocused(true);
		else
			focusController.setFocusedFrame(&core(webView)->mainFrame());
	}

	return GTK_WIDGET_CLASS(webkit_web_view_parent_class)->grab_focus(widget);
}

static bool webkit_web_view_focus_in_event(Fl_Widget* widget, GdkEventFocus* event)
{
	// TODO: Improve focus handling as suggested in
	// http://bugs.webkit.org/show_bug.cgi?id=16910
	Fl_Widget* toplevel = gtk_widget_get_toplevel(widget);
	if (!widgetIsOnscreenToplevelWindow(toplevel) || !gtk_window_has_toplevel_focus(GTK_WINDOW(toplevel)))
		return GTK_WIDGET_CLASS(webkit_web_view_parent_class)->focus_in_event(widget, event);

	WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
	FocusController& focusController = core(webView)->focusController();

	focusController.setActive(true);
	if (focusController.focusedFrame())
		focusController.setFocused(true);
	else
		focusController.setFocusedFrame(&core(webView)->mainFrame());

//	if (focusController.focusedFrame()->editor().canEdit())
//		webView->priv->imFilter.notifyFocusedIn();
	return GTK_WIDGET_CLASS(webkit_web_view_parent_class)->focus_in_event(widget, event);
}

static bool webkit_web_view_focus_out_event(Fl_Widget* widget, GdkEventFocus* event)
{
	WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);

	// We may hit this code while destroying the widget, and we might
	// no longer have a page, then.
	if (Page* page = core(webView)) {
		page->focusController().setActive(false);
		page->focusController().setFocused(false);
	}

//	webView->priv->imFilter.notifyFocusedOut();
	return GTK_WIDGET_CLASS(webkit_web_view_parent_class)->focus_out_event(widget, event);
}

static void webkit_web_view_realize(Fl_Widget* widget)
{
	gtk_widget_set_realized(widget, TRUE);

	FLTKAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);

	GdkWindowAttr attributes;
	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.x = allocation.x;
	attributes.y = allocation.y;
	attributes.width = allocation.width;
	attributes.height = allocation.height;
	attributes.wclass = GDK_INPUT_OUTPUT;
	attributes.visual = gtk_widget_get_visual(widget);
	attributes.colormap = gtk_widget_get_colormap(widget);
	attributes.event_mask = GDK_VISIBILITY_NOTIFY_MASK
							| GDK_EXPOSURE_MASK
							| GDK_BUTTON_PRESS_MASK
							| GDK_BUTTON_RELEASE_MASK
							| GDK_SCROLL_MASK
#if GTK_CHECK_VERSION(3, 3, 18)
							| GDK_SMOOTH_SCROLL_MASK
#endif
							| GDK_POINTER_MOTION_MASK
							| GDK_KEY_PRESS_MASK
							| GDK_KEY_RELEASE_MASK
							| GDK_BUTTON_MOTION_MASK
							| GDK_BUTTON1_MOTION_MASK
							| GDK_BUTTON2_MOTION_MASK
							| GDK_BUTTON3_MOTION_MASK;

	int attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;
	attributes_mask |= GDK_WA_COLORMAP;
	GdkWindow* window = gdk_window_new(gtk_widget_get_parent_window(widget), &attributes, attributes_mask);

	gtk_widget_set_window(widget, window);
	gdk_window_set_user_data(window, widget);

	gtk_widget_style_attach(widget);
	gtk_style_set_background(gtk_widget_get_style(widget), window, GTK_STATE_NORMAL);
}

static void webkit_web_view_set_scroll_adjustments(WebKitWebView* webView, FLTKAdjustment* horizontalAdjustment, GtkAdjustment* verticalAdjustment)
{
	setHorizontalAdjustment(webView, horizontalAdjustment);
	setVerticalAdjustment(webView, verticalAdjustment);
}

static void webkit_web_view_container_add(FLTKContainer* container, GtkWidget* widget)
{
	WebKitWebView* webView = WEBKIT_WEB_VIEW(container);
	WebKitWebViewPrivate* priv = webView->priv;

	priv->children.add(widget);
	gtk_widget_set_parent(widget, GTK_WIDGET(container));
}

static void webkit_web_view_container_remove(FLTKContainer* container, GtkWidget* widget)
{
	WebKitWebView* webView = WEBKIT_WEB_VIEW(container);
	WebKitWebViewPrivate* priv = webView->priv;

	if (priv->children.contains(widget)) {
		gtk_widget_unparent(widget);
		priv->children.remove(widget);
	}
}

static void webkit_web_view_container_forall(FLTKContainer* container, bool, GtkCallback callback, gpointer callbackData)
{
	WebKitWebView* webView = WEBKIT_WEB_VIEW(container);
	WebKitWebViewPrivate* priv = webView->priv;

	HashSet<Fl_Widget*> children = priv->children;
	HashSet<Fl_Widget*>::const_iterator end = children.end();
	for (HashSet<Fl_Widget*>::const_iterator current = children.begin(); current != end; ++current)
		(*callback)(*current, callbackData);
}

static WebKitWebView* webkit_web_view_real_create_web_view(WebKitWebView*, WebKitWebFrame*)
{
	return 0;
}

static bool webkit_web_view_real_web_view_ready(WebKitWebView*)
{
	return FALSE;
}

static bool webkit_web_view_real_close_web_view(WebKitWebView*)
{
	return FALSE;
}

static WebKitNavigationResponse webkit_web_view_real_navigation_requested(WebKitWebView*, WebKitWebFrame*, WebKitNetworkRequest*)
{
	return WEBKIT_NAVIGATION_RESPONSE_ACCEPT;
}

static void webkit_web_view_real_window_object_cleared(WebKitWebView*, WebKitWebFrame*, JSGlobalContextRef context, JSObjectRef window_object)
{
	notImplemented();
}

static char* webkit_web_view_real_choose_file(WebKitWebView*, WebKitWebFrame*, const char* old_name)
{
	notImplemented();
	return strdup(old_name);
}

typedef enum {
	WEBKIT_SCRIPT_DIALOG_ALERT,
	WEBKIT_SCRIPT_DIALOG_CONFIRM,
	WEBKIT_SCRIPT_DIALOG_PROMPT
 } WebKitScriptDialogType;

static bool webkit_web_view_script_dialog(WebKitWebView* webView, WebKitWebFrame* frame, const char* message, WebKitScriptDialogType type, const char* defaultValue, char** value)
{
	FLTKMessageType messageType;
	FLTKButtonsType buttons;
	int defaultResponse;
	Fl_Widget* window;
	Fl_Widget* dialog;
	Fl_Widget* entry = 0;
	bool didConfirm = FALSE;

	switch (type) {
	case WEBKIT_SCRIPT_DIALOG_ALERT:
		messageType = GTK_MESSAGE_WARNING;
		buttons = GTK_BUTTONS_CLOSE;
		defaultResponse = GTK_RESPONSE_CLOSE;
		break;
	case WEBKIT_SCRIPT_DIALOG_CONFIRM:
		messageType = GTK_MESSAGE_QUESTION;
		buttons = GTK_BUTTONS_OK_CANCEL;
		defaultResponse = GTK_RESPONSE_OK;
		break;
	case WEBKIT_SCRIPT_DIALOG_PROMPT:
		messageType = GTK_MESSAGE_QUESTION;
		buttons = GTK_BUTTONS_OK_CANCEL;
		defaultResponse = GTK_RESPONSE_OK;
		break;
	default:
		g_warning("Unknown value for WebKitScriptDialogType.");
		return FALSE;
	}

	window = gtk_widget_get_toplevel(GTK_WIDGET(webView));
	dialog = gtk_message_dialog_new(widgetIsOnscreenToplevelWindow(window) ? GTK_WINDOW(window) : 0,
									GTK_DIALOG_DESTROY_WITH_PARENT,
									messageType,
									buttons,
									"%s",
									message);
	char* title = g_strconcat("JavaScript - ", webkit_web_frame_get_uri(frame), NULL);
	gtk_window_set_title(GTK_WINDOW(dialog), title);
	free(title);

	if (type == WEBKIT_SCRIPT_DIALOG_PROMPT) {
		entry = gtk_entry_new();
		gtk_entry_set_text(GTK_ENTRY(entry), defaultValue);
		gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry);
		gtk_entry_set_activates_default(GTK_ENTRY(entry), TRUE);
		gtk_widget_show(entry);
	}

	gtk_dialog_set_default_response(GTK_DIALOG(dialog), defaultResponse);
	int response = gtk_dialog_run(GTK_DIALOG(dialog));

	switch (response) {
	case GTK_RESPONSE_OK:
		didConfirm = TRUE;
		if (entry)
			*value = strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
		break;
	case GTK_RESPONSE_CANCEL:
		didConfirm = FALSE;
		break;

	}
	gtk_widget_destroy(GTK_WIDGET(dialog));
	return didConfirm;
}

static bool webkit_web_view_real_script_alert(WebKitWebView* webView, WebKitWebFrame* frame, const char* message)
{
	webkit_web_view_script_dialog(webView, frame, message, WEBKIT_SCRIPT_DIALOG_ALERT, 0, 0);
	return TRUE;
}

static bool webkit_web_view_real_script_confirm(WebKitWebView* webView, WebKitWebFrame* frame, const char* message, bool* didConfirm)
{
	*didConfirm = webkit_web_view_script_dialog(webView, frame, message, WEBKIT_SCRIPT_DIALOG_CONFIRM, 0, 0);
	return TRUE;
}

static bool webkit_web_view_real_script_prompt(WebKitWebView* webView, WebKitWebFrame* frame, const char* message, const char* defaultValue, char** value)
{
	if (!webkit_web_view_script_dialog(webView, frame, message, WEBKIT_SCRIPT_DIALOG_PROMPT, defaultValue, value))
		*value = NULL;
	return TRUE;
}

static bool webkit_web_view_real_console_message(WebKitWebView* webView, const char* message, unsigned int line, const char* sourceId)
{
	g_message("console message: %s @%d: %s\n", sourceId, line, message);
	return TRUE;
}

static void webkit_web_view_real_select_all(WebKitWebView* webView)
{
	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	frame.editor().command("SelectAll").execute();
}

static void webkit_web_view_real_cut_clipboard(WebKitWebView* webView)
{
	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	frame.editor().command("Cut").execute();
}

static void webkit_web_view_real_copy_clipboard(WebKitWebView* webView)
{
	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	frame.editor().command("Copy").execute();
}

static void webkit_web_view_real_undo(WebKitWebView* webView)
{
	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	frame.editor().command("Undo").execute();
}

static void webkit_web_view_real_redo(WebKitWebView* webView)
{
	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	frame.editor().command("Redo").execute();
}

static bool webkit_web_view_real_move_cursor (WebKitWebView* webView, FLTKMovementStep step, int count)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW (webView), FALSE);
	g_return_val_if_fail(step == GTK_MOVEMENT_VISUAL_POSITIONS ||
						 step == GTK_MOVEMENT_DISPLAY_LINES ||
						 step == GTK_MOVEMENT_PAGES ||
						 step == GTK_MOVEMENT_BUFFER_ENDS, FALSE);
	g_return_val_if_fail(count == 1 || count == -1, FALSE);

	ScrollDirection direction;
	ScrollGranularity granularity;

	switch (step) {
	case GTK_MOVEMENT_DISPLAY_LINES:
		granularity = ScrollByLine;
		if (count == 1)
			direction = ScrollDown;
		else
			direction = ScrollUp;
		break;
	case GTK_MOVEMENT_VISUAL_POSITIONS:
		granularity = ScrollByLine;
		if (count == 1)
			direction = ScrollRight;
		else
			direction = ScrollLeft;
		break;
	case GTK_MOVEMENT_PAGES:
		granularity = ScrollByPage;
		if (count == 1)
			direction = ScrollDown;
		else
			direction = ScrollUp;
		break;
	case GTK_MOVEMENT_BUFFER_ENDS:
		granularity = ScrollByDocument;
		if (count == 1)
			direction = ScrollDown;
		else
			direction = ScrollUp;
		break;
	default:
		g_assert_not_reached();
		return false;
	}

	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	if (!frame.eventHandler().scrollOverflow(direction, granularity))
		frame.view()->scroll(direction, granularity);

	return true;
}

static void webkit_web_view_real_paste_clipboard(WebKitWebView* webView)
{
	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	frame.editor().command("Paste").execute();
}

static bool webkit_web_view_real_should_allow_editing_action(WebKitWebView*)
{
	return TRUE;
}

static bool webkit_web_view_real_entering_fullscreen(WebKitWebView* webView)
{
	return FALSE;
}

static bool webkit_web_view_real_leaving_fullscreen(WebKitWebView* webView)
{
	return FALSE;
}

static void fileChooserDialogResponseCallback(FLTKDialog* dialog, int responseID, WebKitFileChooserRequest* request)
{
	GRefPtr<WebKitFileChooserRequest> adoptedRequest = adoptGRef(request);
	if (responseID == GTK_RESPONSE_ACCEPT) {
		GUniquePtr<GSList> filesList(gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(dialog)));
		GRefPtr<GPtrArray> filesArray = adoptGRef(g_ptr_array_new());
		for (GSList* file = filesList.get(); file; file = g_slist_next(file))
			g_ptr_array_add(filesArray.get(), file->data);
		g_ptr_array_add(filesArray.get(), 0);
		webkit_file_chooser_request_select_files(adoptedRequest.get(), reinterpret_cast<const char* const*>(filesArray->pdata));
	}

	gtk_widget_destroy(GTK_WIDGET(dialog));
}

static bool webkitWebViewRealRunFileChooser(WebKitWebView* webView, WebKitFileChooserRequest* request)
{
	Fl_Widget* toplevel = gtk_widget_get_toplevel(GTK_WIDGET(webView));
	if (!widgetIsOnscreenToplevelWindow(toplevel))
		toplevel = 0;

	bool allowsMultipleSelection = webkit_file_chooser_request_get_select_multiple(request);
	Fl_Widget* dialog = gtk_file_chooser_dialog_new(allowsMultipleSelection ? _("Select Files") : _("Select File"),
													toplevel ? GTK_WINDOW(toplevel) : 0,
													GTK_FILE_CHOOSER_ACTION_OPEN,
													GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
													GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
													NULL);

	if (FLTKFileFilter* filter = webkit_file_chooser_request_get_mime_types_filter(request))
		gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);
	gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), allowsMultipleSelection);

	if (const char* const* selectedFiles = webkit_file_chooser_request_get_selected_files(request))
		gtk_file_chooser_select_filename(GTK_FILE_CHOOSER(dialog), selectedFiles[0]);

	gtk_widget_show(dialog);

	return TRUE;
}

static void webkit_web_view_dispose(GObject* object)
{
	WebKitWebView* webView = WEBKIT_WEB_VIEW(object);
	WebKitWebViewPrivate* priv = webView->priv;
	WebCore::Page* corePagePtr = priv->corePage;

	priv->disposing = TRUE;

	// Make sure FLTKAdjustmentWatcher won't be reacting to adjustment changes after the
	// WebView is destroyed.
	setHorizontalAdjustment(webView, 0);
	setVerticalAdjustment(webView, 0);

	// These smart pointers are cleared manually, because some cleanup operations are
	// very sensitive to their value. We may crash if these are done in the wrong order.
	priv->backForwardList.clear();

	if (priv->corePage) {
		webkit_web_view_stop_loading(WEBKIT_WEB_VIEW(object));
		core(priv->mainFrame)->loader().detachFromParent();
		priv->corePage = 0;
	}

	if (priv->webSettings) {
		priv->webSettings.clear();
	}

	if (priv->currentMenu) {
		gtk_widget_destroy(GTK_WIDGET(priv->currentMenu));
		priv->currentMenu = 0;
	}

	priv->webInspector.clear();
	priv->viewportAttributes.clear();
	priv->webWindowFeatures.clear();
	priv->mainResource.clear();
	priv->subResources.clear();

	G_OBJECT_CLASS(webkit_web_view_parent_class)->dispose(object);

	// We need to run the parent's dispose before destroying the Page
	// pointer. Otherwise we're triggering the deletion of
	// InspectorFrontendClient before it can clean up itself.
	delete corePagePtr;
}

static void webkit_web_view_finalize(GObject* object)
{
	// We need to manually call the destructor here, since this object's memory is managed
	// by GLib. This calls all C++ members' destructors and prevents memory leaks.
	WEBKIT_WEB_VIEW(object)->priv->~WebKitWebViewPrivate();
	G_OBJECT_CLASS(webkit_web_view_parent_class)->finalize(object);
}

static bool webkit_signal_accumulator_object_handled(GSignalInvocationHint* ihint, GValue* returnAccu, const GValue* handlerReturn, gpointer dummy)
{
	gpointer newWebView = g_value_get_object(handlerReturn);
	g_value_set_object(returnAccu, newWebView);

	// Continue if we don't have a newWebView
	return !newWebView;
}

static bool webkit_navigation_request_handled(GSignalInvocationHint* ihint, GValue* returnAccu, const GValue* handlerReturn, gpointer dummy)
{
	WebKitNavigationResponse navigationResponse = (WebKitNavigationResponse)g_value_get_enum(handlerReturn);
	g_value_set_enum(returnAccu, navigationResponse);

	if (navigationResponse != WEBKIT_NAVIGATION_RESPONSE_ACCEPT)
		return FALSE;

	return TRUE;
}

#if HAVE(ACCESSIBILITY)
static AtkObject* webkit_web_view_get_accessible(Fl_Widget* widget)
{
	WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
	if (!core(webView))
		return 0;

	if (!AXObjectCache::accessibilityEnabled())
		AXObjectCache::enableAccessibility();

	Frame& coreFrame = core(webView)->mainFrame();

	Document* doc = coreFrame.document();
	if (!doc)
		return 0;

	AccessibilityObject* rootAccessible = doc->axObjectCache()->rootObject();
	if (!rootAccessible)
		return 0;

	AtkObject* axRoot = rootAccessible->wrapper();
	if (!axRoot || !ATK_IS_OBJECT(axRoot))
		return 0;

	// Ensure the parent is set to make top-down and bottom-up navigation work.
	Fl_Widget* parentWidget = gtk_widget_get_parent(widget);
	AtkObject* axParent = parentWidget ? gtk_widget_get_accessible(parentWidget) : 0;
	if (axParent)
		atk_object_set_parent(axRoot, axParent);

	return axRoot;
}
#endif

static double screenDPI(GdkScreen* screen)
{
	// gdk_screen_get_resolution() returns -1 when no DPI is set.
	double dpi = gdk_screen_get_resolution(screen);
	if (dpi != -1)
		return dpi;

	static const double kMillimetresPerInch = 25.4;
	double diagonalSizeInPixels = hypot(gdk_screen_get_width(screen), gdk_screen_get_height(screen));
	double diagonalSizeInInches = hypot(gdk_screen_get_width_mm(screen), gdk_screen_get_height_mm(screen)) / kMillimetresPerInch;
	return diagonalSizeInPixels / diagonalSizeInInches;
}

static double webViewGetDPI(WebKitWebView* webView)
{
	if (webView->priv->webSettings->priv->enforce96DPI)
		return 96;

	static const double defaultDPI = 96;
	GdkScreen* screen = gtk_widget_has_screen(GTK_WIDGET(webView)) ? gtk_widget_get_screen(GTK_WIDGET(webView)) : gdk_screen_get_default();
	return screen ? screenDPI(screen) : defaultDPI;
}

static inline int webViewConvertFontSizeToPixels(WebKitWebView* webView, double fontSize)
{
	return fontSize / 72.0 * webViewGetDPI(webView);
}

static void webkit_web_view_screen_changed(Fl_Widget* widget, GdkScreen* previousScreen)
{
	WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
	WebKitWebViewPrivate* priv = webView->priv;

	if (priv->disposing)
		return;

	WebKitWebSettings* webSettings = priv->webSettings.get();
	Settings& settings = core(webView)->settings();
	unsigned defaultFontSize, defaultMonospaceFontSize, minimumFontSize, minimumLogicalFontSize;

	g_object_get(webSettings,
				 "default-font-size", &defaultFontSize,
				 "default-monospace-font-size", &defaultMonospaceFontSize,
				 "minimum-font-size", &minimumFontSize,
				 "minimum-logical-font-size", &minimumLogicalFontSize,
				 NULL);

	settings.setDefaultFontSize(webViewConvertFontSizeToPixels(webView, defaultFontSize));
	settings.setDefaultFixedFontSize(webViewConvertFontSizeToPixels(webView, defaultMonospaceFontSize));
	settings.setMinimumFontSize(webViewConvertFontSizeToPixels(webView, minimumFontSize));
	settings.setMinimumLogicalFontSize(webViewConvertFontSizeToPixels(webView, minimumLogicalFontSize));
}

#if ENABLE(DRAG_SUPPORT)
static void webkit_web_view_drag_end(Fl_Widget* widget, GdkDragContext* context)
{
	notImplemented();
}

static void webkit_web_view_drag_data_get(Fl_Widget* widget, GdkDragContext* context, GtkSelectionData* selectionData, unsigned info, unsigned)
{
	notImplemented();
}

static void dragExitedCallback(Fl_Widget* widget, DragData& dragData, bool dropHappened)
{
	// Don't call dragExited if we have just received a drag-drop signal. This
	// happens in the case of a successful drop onto the view.
	if (!dropHappened)
		core(WEBKIT_WEB_VIEW(widget))->dragController().dragExited(dragData);
	core(WEBKIT_WEB_VIEW(widget))->dragController().dragEnded();
}

static void webkit_web_view_drag_leave(Fl_Widget* widget, GdkDragContext* context, unsigned time)
{
	notImplemented();
}

static bool webkit_web_view_drag_motion(Fl_Widget* widget, GdkDragContext* context, int x, int y, unsigned time)
{
	notImplemented();
}

static void webkit_web_view_drag_data_received(Fl_Widget* widget, GdkDragContext* context, int x, int y, GtkSelectionData* selectionData, unsigned info, unsigned time)
{
	notImplemented();
}

static bool webkit_web_view_drag_drop(Fl_Widget* widget, GdkDragContext* context, int x, int y, unsigned time)
{
	notImplemented();
}
#endif // ENABLE(DRAG_SUPPORT)

static bool webkit_web_view_query_tooltip(Fl_Widget *widget, int x, int y, bool keyboard_mode, GtkTooltip *tooltip)
{
	WebKitWebViewPrivate* priv = WEBKIT_WEB_VIEW(widget)->priv;

	if (keyboard_mode) {
		WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);

		// Get the title of the current focused element.
		Frame& coreFrame = core(webView)->focusController().focusedOrMainFrame();

		Node* node = getFocusedNode(&coreFrame);
		if (!node)
			return FALSE;

		for (Node* titleNode = node; titleNode; titleNode = titleNode->parentNode()) {
			if (titleNode->isElementNode()) {
				String title = toElement(titleNode)->title();
				if (!title.isEmpty()) {
					if (FrameView* view = coreFrame.view()) {
						GdkRectangle area = view->contentsToWindow(node->pixelSnappedBoundingBox());
						gtk_tooltip_set_tip_area(tooltip, &area);
					}
					gtk_tooltip_set_text(tooltip, title.utf8().data());

					return TRUE;
				}
			}
		}

		return FALSE;
	}

	if (priv->tooltipText.length() > 0) {
		if (!keyboard_mode) {
			if (!priv->tooltipArea.isEmpty()) {
				GdkRectangle area = priv->tooltipArea;
				gtk_tooltip_set_tip_area(tooltip, &area);
			} else
				gtk_tooltip_set_tip_area(tooltip, 0);
		}
		gtk_tooltip_set_text(tooltip, priv->tooltipText.data());
		return TRUE;
	}

	return FALSE;
}

static bool webkit_web_view_show_help(Fl_Widget* widget, GtkWidgetHelpType help_type)
{
	if (help_type == GTK_WIDGET_HELP_TOOLTIP)
		gtk_widget_set_has_tooltip(widget, TRUE);

	return GTK_WIDGET_CLASS(webkit_web_view_parent_class)->show_help(widget, help_type);
}

static void webkit_web_view_class_init(WebKitWebViewClass* webViewClass)
{
	webkitInit();
	webViewClass->create_web_view = webkit_web_view_real_create_web_view;
	webViewClass->web_view_ready = webkit_web_view_real_web_view_ready;
	webViewClass->close_web_view = webkit_web_view_real_close_web_view;
	webViewClass->navigation_requested = webkit_web_view_real_navigation_requested;
	webViewClass->window_object_cleared = webkit_web_view_real_window_object_cleared;
	webViewClass->choose_file = webkit_web_view_real_choose_file;
	webViewClass->script_alert = webkit_web_view_real_script_alert;
	webViewClass->script_confirm = webkit_web_view_real_script_confirm;
	webViewClass->script_prompt = webkit_web_view_real_script_prompt;
	webViewClass->console_message = webkit_web_view_real_console_message;
	webViewClass->select_all = webkit_web_view_real_select_all;
	webViewClass->cut_clipboard = webkit_web_view_real_cut_clipboard;
	webViewClass->copy_clipboard = webkit_web_view_real_copy_clipboard;
	webViewClass->paste_clipboard = webkit_web_view_real_paste_clipboard;
	webViewClass->undo = webkit_web_view_real_undo;
	webViewClass->redo = webkit_web_view_real_redo;
	webViewClass->move_cursor = webkit_web_view_real_move_cursor;
	webViewClass->should_allow_editing_action = webkit_web_view_real_should_allow_editing_action;
	webViewClass->entering_fullscreen = webkit_web_view_real_entering_fullscreen;
	webViewClass->leaving_fullscreen = webkit_web_view_real_leaving_fullscreen;
	webViewClass->run_file_chooser = webkitWebViewRealRunFileChooser;

	Fl_WidgetClass* widgetClass = GTK_WIDGET_CLASS(webViewClass);
	widgetClass->realize = webkit_web_view_realize;
	widgetClass->expose_event = webkit_web_view_expose_event;
	widgetClass->key_press_event = webkit_web_view_key_press_event;
	widgetClass->key_release_event = webkit_web_view_key_release_event;
	widgetClass->button_press_event = webkit_web_view_button_press_event;
	widgetClass->button_release_event = webkit_web_view_button_release_event;
	widgetClass->motion_notify_event = webkit_web_view_motion_event;
	widgetClass->scroll_event = webkit_web_view_scroll_event;
	widgetClass->size_allocate = webkit_web_view_size_allocate;
	widgetClass->size_request = webkit_web_view_size_request;
#if ENABLE(CONTEXT_MENUS)
	widgetClass->popup_menu = webkit_web_view_popup_menu_handler;
#else
	widgetClass->popup_menu = NULL;
#endif
	widgetClass->grab_focus = webkit_web_view_grab_focus;
	widgetClass->focus_in_event = webkit_web_view_focus_in_event;
	widgetClass->focus_out_event = webkit_web_view_focus_out_event;
#if HAVE(ACCESSIBILITY)
	widgetClass->get_accessible = webkit_web_view_get_accessible;
#endif
	widgetClass->screen_changed = webkit_web_view_screen_changed;
#if ENABLE(DRAG_SUPPORT)
	widgetClass->drag_end = webkit_web_view_drag_end;
	widgetClass->drag_data_get = webkit_web_view_drag_data_get;
	widgetClass->drag_motion = webkit_web_view_drag_motion;
	widgetClass->drag_leave = webkit_web_view_drag_leave;
	widgetClass->drag_drop = webkit_web_view_drag_drop;
	widgetClass->drag_data_received = webkit_web_view_drag_data_received;
#else
	widgetClass->drag_end = NULL;
	widgetClass->drag_data_get = NULL;
	widgetClass->drag_motion = NULL;
	widgetClass->drag_leave = NULL;
	widgetClass->drag_drop = NULL;
	widgetClass->drag_data_received = NULL;
#endif
	widgetClass->query_tooltip = webkit_web_view_query_tooltip;
	widgetClass->show_help = webkit_web_view_show_help;
	widgetClass->map = webkitWebViewMap;

	FLTKContainerClass* containerClass = GTK_CONTAINER_CLASS(webViewClass);
	containerClass->add = webkit_web_view_container_add;
	containerClass->remove = webkit_web_view_container_remove;
	containerClass->forall = webkit_web_view_container_forall;

	/*
	 * make us scrollable (e.g. addable to a FLTKScrolledWindow)
	 */
}

static void updateAcceleratedCompositingSetting(Settings& settings, bool value)
{
	settings.setAcceleratedCompositingEnabled(value);
}

static void webkit_web_view_update_settings(WebKitWebView* webView)
{
	WebKitWebSettingsPrivate* settingsPrivate = webView->priv->webSettings->priv;
	Settings& coreSettings = core(webView)->settings();

	coreSettings.setDefaultTextEncodingName(settingsPrivate->defaultEncoding.data());
	coreSettings.setCursiveFontFamily(settingsPrivate->cursiveFontFamily.data());
	coreSettings.setStandardFontFamily(settingsPrivate->defaultFontFamily.data());
	coreSettings.setFantasyFontFamily(settingsPrivate->fantasyFontFamily.data());
	coreSettings.setFixedFontFamily(settingsPrivate->monospaceFontFamily.data());
	coreSettings.setSansSerifFontFamily(settingsPrivate->sansSerifFontFamily.data());
	coreSettings.setSerifFontFamily(settingsPrivate->serifFontFamily.data());
	coreSettings.setLoadsImagesAutomatically(settingsPrivate->autoLoadImages);
	coreSettings.setShrinksStandaloneImagesToFit(settingsPrivate->autoShrinkImages);
	coreSettings.setShouldRespectImageOrientation(settingsPrivate->respectImageOrientation);
	coreSettings.setShouldPrintBackgrounds(settingsPrivate->printBackgrounds);
	coreSettings.setScriptEnabled(settingsPrivate->enableScripts);
	coreSettings.setPluginsEnabled(settingsPrivate->enablePlugins);
	coreSettings.setTextAreasAreResizable(settingsPrivate->resizableTextAreas);
	coreSettings.setUserStyleSheetLocation(URL(URL(), settingsPrivate->userStylesheetURI.data()));
	coreSettings.setDeveloperExtrasEnabled(settingsPrivate->enableDeveloperExtras);
	coreSettings.setPrivateBrowsingEnabled(settingsPrivate->enablePrivateBrowsing);
	coreSettings.setCaretBrowsingEnabled(settingsPrivate->enableCaretBrowsing);
	coreSettings.setLocalStorageEnabled(settingsPrivate->enableHTML5LocalStorage);
	coreSettings.setLocalStorageDatabasePath(settingsPrivate->html5LocalStorageDatabasePath.data());
	coreSettings.setXSSAuditorEnabled(settingsPrivate->enableXSSAuditor);
	coreSettings.setSpatialNavigationEnabled(settingsPrivate->enableSpatialNavigation);
	coreSettings.setFrameFlatteningEnabled(settingsPrivate->enableFrameFlattening);
	coreSettings.setJavaScriptCanOpenWindowsAutomatically(settingsPrivate->javascriptCanOpenWindowsAutomatically);
	coreSettings.setJavaScriptCanAccessClipboard(settingsPrivate->javascriptCanAccessClipboard);
	coreSettings.setOfflineWebApplicationCacheEnabled(settingsPrivate->enableOfflineWebApplicationCache);
	coreSettings.setEditingBehaviorType(static_cast<WebCore::EditingBehaviorType>(settingsPrivate->editingBehavior));
	coreSettings.setAllowUniversalAccessFromFileURLs(settingsPrivate->enableUniversalAccessFromFileURIs);
	coreSettings.setAllowFileAccessFromFileURLs(settingsPrivate->enableFileAccessFromFileURIs);
	coreSettings.setDOMPasteAllowed(settingsPrivate->enableDOMPaste);
	coreSettings.setNeedsSiteSpecificQuirks(settingsPrivate->enableSiteSpecificQuirks);
	coreSettings.setUsesPageCache(settingsPrivate->enablePageCache);
	coreSettings.setJavaEnabled(settingsPrivate->enableJavaApplet);
	coreSettings.setHyperlinkAuditingEnabled(settingsPrivate->enableHyperlinkAuditing);
	coreSettings.setDNSPrefetchingEnabled(settingsPrivate->enableDNSPrefetching);
	coreSettings.setMediaPlaybackRequiresUserGesture(settingsPrivate->mediaPlaybackRequiresUserGesture);
	coreSettings.setMediaPlaybackAllowsInline(settingsPrivate->mediaPlaybackAllowsInline);
	coreSettings.setAllowDisplayOfInsecureContent(settingsPrivate->enableDisplayOfInsecureContent);
	coreSettings.setAllowRunningOfInsecureContent(settingsPrivate->enableRunningOfInsecureContent);

#if ENABLE(SQL_DATABASE)
	DatabaseManager::manager().setIsAvailable(settingsPrivate->enableHTML5Database);
#endif

#if ENABLE(FULLSCREEN_API)
	coreSettings.setFullScreenEnabled(settingsPrivate->enableFullscreen);
#endif

#if ENABLE(MEDIA_STREAM)
	WebCore::RuntimeEnabledFeatures::sharedFeatures().setMediaStreamEnabled(settingsPrivate->enableMediaStream);
#endif

#if ENABLE(SMOOTH_SCROLLING)
	coreSettings.setScrollAnimatorEnabled(settingsPrivate->enableSmoothScrolling);
#endif

#if ENABLE(CSS_REGIONS)
	WebCore::RuntimeEnabledFeatures::sharedFeatures().setCSSRegionsEnabled(true);
#endif

#if ENABLE(MEDIA_SOURCE)
	coreSettings.setMediaSourceEnabled(settingsPrivate->enableMediaSource);
#endif

	// Use mock scrollbars if in DumpRenderTree mode (i.e. testing layout tests).
	coreSettings.setMockScrollbarsEnabled(DumpRenderTreeSupportFLTK::dumpRenderTreeModeEnabled());

	if (Page* page = core(webView))
		page->setTabKeyCyclesThroughElements(settingsPrivate->tabKeyCyclesThroughElements);

	webkit_web_view_screen_changed(GTK_WIDGET(webView), NULL);
}

static void webkit_web_view_settings_notify(WebKitWebSettings* webSettings, GParamSpec* pspec, WebKitWebView* webView)
{
	Settings& settings = core(webView)->settings();

	const char* name = g_intern_string(pspec->name);
	GValue value = { 0, { { 0 } } };
	g_value_init(&value, pspec->value_type);
	g_object_get_property(G_OBJECT(webSettings), name, &value);

	if (name == g_intern_string("default-encoding"))
		settings.setDefaultTextEncodingName(g_value_get_string(&value));
	else if (name == g_intern_string("cursive-font-family"))
		settings.setCursiveFontFamily(g_value_get_string(&value));
	else if (name == g_intern_string("default-font-family"))
		settings.setStandardFontFamily(g_value_get_string(&value));
	else if (name == g_intern_string("fantasy-font-family"))
		settings.setFantasyFontFamily(g_value_get_string(&value));
	else if (name == g_intern_string("monospace-font-family"))
		settings.setFixedFontFamily(g_value_get_string(&value));
	else if (name == g_intern_string("sans-serif-font-family"))
		settings.setSansSerifFontFamily(g_value_get_string(&value));
	else if (name == g_intern_string("serif-font-family"))
		settings.setSerifFontFamily(g_value_get_string(&value));
	else if (name == g_intern_string("default-font-size"))
		settings.setDefaultFontSize(webViewConvertFontSizeToPixels(webView, g_value_get_int(&value)));
	else if (name == g_intern_string("default-monospace-font-size"))
		settings.setDefaultFixedFontSize(webViewConvertFontSizeToPixels(webView, g_value_get_int(&value)));
	else if (name == g_intern_string("minimum-font-size"))
		settings.setMinimumFontSize(webViewConvertFontSizeToPixels(webView, g_value_get_int(&value)));
	else if (name == g_intern_string("minimum-logical-font-size"))
		settings.setMinimumLogicalFontSize(webViewConvertFontSizeToPixels(webView, g_value_get_int(&value)));
	else if (name == g_intern_string("enforce-96-dpi"))
		webkit_web_view_screen_changed(GTK_WIDGET(webView), NULL);
	else if (name == g_intern_string("auto-load-images"))
		settings.setLoadsImagesAutomatically(g_value_get_boolean(&value));
	else if (name == g_intern_string("auto-shrink-images"))
		settings.setShrinksStandaloneImagesToFit(g_value_get_boolean(&value));
	else if (name == g_intern_string("respect-image-orientation"))
		settings.setShouldRespectImageOrientation(g_value_get_boolean(&value));
	else if (name == g_intern_string("print-backgrounds"))
		settings.setShouldPrintBackgrounds(g_value_get_boolean(&value));
	else if (name == g_intern_string("enable-scripts"))
		settings.setScriptEnabled(g_value_get_boolean(&value));
	else if (name == g_intern_string("enable-plugins"))
		settings.setPluginsEnabled(g_value_get_boolean(&value));
	else if (name == g_intern_string("enable-dns-prefetching"))
		settings.setDNSPrefetchingEnabled(g_value_get_boolean(&value));
	else if (name == g_intern_string("resizable-text-areas"))
		settings.setTextAreasAreResizable(g_value_get_boolean(&value));
	else if (name == g_intern_string("user-stylesheet-uri"))
		settings.setUserStyleSheetLocation(URL(URL(), g_value_get_string(&value)));
	else if (name == g_intern_string("enable-developer-extras"))
		settings.setDeveloperExtrasEnabled(g_value_get_boolean(&value));
	else if (name == g_intern_string("enable-private-browsing"))
		settings.setPrivateBrowsingEnabled(g_value_get_boolean(&value));
	else if (name == g_intern_string("enable-caret-browsing"))
		settings.setCaretBrowsingEnabled(g_value_get_boolean(&value));
#if ENABLE(SQL_DATABASE)
	else if (name == g_intern_string("enable-html5-database")) {
		DatabaseManager::manager().setIsAvailable(g_value_get_boolean(&value));
	}
#endif
	else if (name == g_intern_string("enable-html5-local-storage"))
		settings.setLocalStorageEnabled(g_value_get_boolean(&value));
	else if (name == g_intern_string("html5-local-storage-database-path"))
		settings.setLocalStorageDatabasePath(g_value_get_string(&value));
	else if (name == g_intern_string("enable-xss-auditor"))
		settings.setXSSAuditorEnabled(g_value_get_boolean(&value));
	else if (name == g_intern_string("enable-spatial-navigation"))
		settings.setSpatialNavigationEnabled(g_value_get_boolean(&value));
	else if (name == g_intern_string("enable-frame-flattening"))
		settings.setFrameFlatteningEnabled(g_value_get_boolean(&value));
	else if (name == g_intern_string("javascript-can-open-windows-automatically"))
		settings.setJavaScriptCanOpenWindowsAutomatically(g_value_get_boolean(&value));
	else if (name == g_intern_string("javascript-can-access-clipboard"))
		settings.setJavaScriptCanAccessClipboard(g_value_get_boolean(&value));
	else if (name == g_intern_string("enable-offline-web-application-cache"))
		settings.setOfflineWebApplicationCacheEnabled(g_value_get_boolean(&value));
	else if (name == g_intern_string("editing-behavior"))
		settings.setEditingBehaviorType(static_cast<WebCore::EditingBehaviorType>(g_value_get_enum(&value)));
	else if (name == g_intern_string("enable-universal-access-from-file-uris"))
		settings.setAllowUniversalAccessFromFileURLs(g_value_get_boolean(&value));
	else if (name == g_intern_string("enable-file-access-from-file-uris"))
		settings.setAllowFileAccessFromFileURLs(g_value_get_boolean(&value));
	else if (name == g_intern_string("enable-dom-paste"))
		settings.setDOMPasteAllowed(g_value_get_boolean(&value));
	else if (name == g_intern_string("tab-key-cycles-through-elements")) {
		Page* page = core(webView);
		if (page)
			page->setTabKeyCyclesThroughElements(g_value_get_boolean(&value));
	} else if (name == g_intern_string("enable-site-specific-quirks"))
		settings.setNeedsSiteSpecificQuirks(g_value_get_boolean(&value));
	else if (name == g_intern_string("enable-page-cache"))
		settings.setUsesPageCache(g_value_get_boolean(&value));
	else if (name == g_intern_string("enable-java-applet"))
		settings.setJavaEnabled(g_value_get_boolean(&value));
	else if (name == g_intern_string("enable-hyperlink-auditing"))
		settings.setHyperlinkAuditingEnabled(g_value_get_boolean(&value));
	else if (name == g_intern_string("media-playback-requires-user-gesture"))
		settings.setMediaPlaybackRequiresUserGesture(g_value_get_boolean(&value));
	else if (name == g_intern_string("media-playback-allows-inline"))
		settings.setMediaPlaybackAllowsInline(g_value_get_boolean(&value));

#if ENABLE(SMOOTH_SCROLLING)
	else if (name == g_intern_string("enable-smooth-scrolling"))
		settings.setScrollAnimatorEnabled(g_value_get_boolean(&value));
#endif

#if ENABLE(MEDIA_SOURCE)
	else if (name == g_intern_string("enable-mediasource"))
		settings.setMediaSourceEnabled(g_value_get_boolean(&value));
#endif

	else if (!g_object_class_find_property(G_OBJECT_GET_CLASS(webSettings), name))
		g_warning("Unexpected setting '%s'", name);
	g_value_unset(&value);
}

static void webkit_web_view_init(WebKitWebView* webView)
{
	WebKitWebViewPrivate* priv = G_TYPE_INSTANCE_GET_PRIVATE(webView, WEBKIT_TYPE_WEB_VIEW, WebKitWebViewPrivate);
	webView->priv = priv;
	// This is the placement new syntax: http://www.parashift.com/c++-faq-lite/dtors.html#faq-11.10
	// It allows us to call a constructor on manually allocated locations in memory. We must use it
	// in this case, because GLib manages the memory for the private data section, but we wish it
	// to contain C++ object members. The use of placement new calls the constructor on all C++ data
	// members, which ensures they are initialized properly.
	new (priv) WebKitWebViewPrivate();

//	priv->imFilter.setWebView(webView);

	Page::PageClients pageClients;
	pageClients.chromeClient = new WebKit::ChromeClient(webView);
#if ENABLE(CONTEXT_MENUS)
	pageClients.contextMenuClient = new WebKit::ContextMenuClient(webView);
#endif
	pageClients.editorClient = new WebKit::EditorClient(webView);
#if ENABLE(DRAG_SUPPORT)
	pageClients.dragClient = new WebKit::DragClient(webView);
#endif
	pageClients.inspectorClient = new WebKit::InspectorClient(webView);
	pageClients.loaderClientForMainFrame = new WebKit::FrameLoaderClient;
	pageClients.progressTrackerClient = new WebKit::ProgressTrackerClient(webView);

	priv->corePage = new Page(pageClients);

	priv->corePage->addLayoutMilestones(DidFirstVisuallyNonEmptyLayout);

#if ENABLE(GEOLOCATION)
	if (DumpRenderTreeSupportFLTK::dumpRenderTreeModeEnabled()) {
		priv->geolocationClientMock = adoptPtr(new GeolocationClientMock);
		WebCore::provideGeolocationTo(priv->corePage, priv->geolocationClientMock.get());
		priv->geolocationClientMock.get()->setController(GeolocationController::from(priv->corePage));
	} else
		WebCore::provideGeolocationTo(priv->corePage, new WebKit::GeolocationClient(webView));
#endif
#if ENABLE(DEVICE_ORIENTATION)
	WebCore::provideDeviceMotionTo(priv->corePage, new DeviceMotionClientFLTK);
	WebCore::provideDeviceOrientationTo(priv->corePage, new DeviceOrientationClientFLTK);
#endif

#if ENABLE(MEDIA_STREAM)
	priv->userMediaClient = adoptPtr(new UserMediaClientFLTK);
	WebCore::provideUserMediaTo(priv->corePage, priv->userMediaClient.get());
#endif

#if ENABLE(NAVIGATOR_CONTENT_UTILS)
	priv->navigatorContentUtilsClient = WebKit::NavigatorContentUtilsClient::create();
	WebCore::provideNavigatorContentUtilsTo(priv->corePage, priv->navigatorContentUtilsClient.get());
#endif

#if ENABLE(BATTERY_STATUS)
	WebCore::provideBatteryTo(priv->corePage, new BatteryClientFLTK);
#endif

	if (DumpRenderTreeSupportFLTK::dumpRenderTreeModeEnabled()) {
		// Set some testing-specific settings
		priv->corePage->settings().setInteractiveFormValidationEnabled(true);
		priv->corePage->settings().setValidationMessageTimerMagnification(-1);
	}

	// Pages within a same session need to be linked together otherwise some functionalities such
	// as visited link coloration (across pages) and changing popup window location will not work.
	// To keep the default behavior simple (and because no PageGroup API exist in WebKitGTK at the
	// time of writing this comment), we simply set all the pages to the same group.
	priv->corePage->setGroupName(webkitPageGroupName());

	// We also add a simple wrapper class to provide the public
	// interface for the Web Inspector.
	priv->webInspector = adoptGRef(WEBKIT_WEB_INSPECTOR(g_object_new(WEBKIT_TYPE_WEB_INSPECTOR, NULL)));
	webkit_web_inspector_set_inspector_client(priv->webInspector.get(), priv->corePage);

	// And our ViewportAttributes friend.
	priv->viewportAttributes = adoptGRef(WEBKIT_VIEWPORT_ATTRIBUTES(g_object_new(WEBKIT_TYPE_VIEWPORT_ATTRIBUTES, NULL)));
	priv->viewportAttributes->priv->webView = webView;

	gtk_widget_set_can_focus(GTK_WIDGET(webView), TRUE);

	priv->mainFrame = WEBKIT_WEB_FRAME(webkit_web_frame_new(webView));
	priv->lastPopupXPosition = priv->lastPopupYPosition = -1;

	priv->backForwardList = adoptGRef(webkit_web_back_forward_list_new_with_web_view(webView));

	priv->zoomFullContent = FALSE;

	priv->webSettings = adoptGRef(webkit_web_settings_new());
	webkit_web_view_update_settings(webView);

	priv->webWindowFeatures = adoptGRef(webkit_web_window_features_new());

	priv->subResources = adoptGRef(g_hash_table_new_full(g_str_hash, g_str_equal, free, g_object_unref));

	priv->selfScrolling = false;

}

Fl_Widget* webkit_web_view_new(void)
{
	WebKitWebView* webView = WEBKIT_WEB_VIEW(g_object_new(WEBKIT_TYPE_WEB_VIEW, NULL));

	return GTK_WIDGET(webView);
}

void webkitWebViewRunFileChooserRequest(WebKitWebView* webView, WebKitFileChooserRequest* request)
{
	bool returnValue;
}

// for internal use only
void webkit_web_view_notify_ready(WebKitWebView* webView)
{

	bool isHandled = FALSE;
}

void webkit_web_view_request_download(WebKitWebView* webView, WebKitNetworkRequest* request, const ResourceResponse& response, ResourceHandle* handle)
{

	DownloadClient* download;

	if (handle)
		download = webkit_download_new_with_handle(request, handle, response);
	else
		download = webkit_download_new(request);

	bool handled;

	if (!handled) {
		webkit_download_cancel(download);
		g_object_unref(download);
		return;
	}

	/* Start the download now if it has a destination URI, otherwise it
		may be handled asynchronously by the application. */
	if (webkit_download_get_destination_uri(download))
		webkit_download_start(download);
}

/**
 * webkit_web_view_set_settings:
 * @web_view: a #WebKitWebView
 * @settings: (transfer none): the #WebKitWebSettings to be set
 *
 * Replaces the #WebKitWebSettings instance that is currently attached
 * to @web_view with @settings. The reference held by the @web_view on
 * the old #WebKitWebSettings instance is dropped, and the reference
 * count of @settings is inscreased.
 *
 * The settings are automatically applied to @web_view.
 */
void webkit_web_view_set_settings(WebKitWebView* webView, WebKitWebSettings* webSettings)
{

	WebKitWebViewPrivate* priv = webView->priv;

	priv->webSettings = webSettings;
	webkit_web_view_update_settings(webView);
	g_object_notify(G_OBJECT(webView), "settings");
}

/**
 * webkit_web_view_get_settings:
 * @web_view: a #WebKitWebView
 *
 * Obtains the #WebKitWebSettings associated with the
 * #WebKitWebView. The #WebKitWebView always has an associated
 * instance of #WebKitWebSettings. The reference that is returned by
 * this call is owned by the #WebKitWebView. You may need to increase
 * its reference count if you intend to keep it alive for longer than
 * the #WebKitWebView.
 *
 * Return value: (transfer none): the #WebKitWebSettings instance
 */
WebKitWebSettings* webkit_web_view_get_settings(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);
	return webView->priv->webSettings.get();
}

/**
 * webkit_web_view_get_inspector:
 * @web_view: a #WebKitWebView
 *
 * Obtains the #WebKitWebInspector associated with the
 * #WebKitWebView. Every #WebKitWebView object has a
 * #WebKitWebInspector object attached to it as soon as it is created,
 * so this function will only return NULL if the argument is not a
 * valid #WebKitWebView.
 *
 * Return value: (transfer none): the #WebKitWebInspector instance.
 *
 * Since: 1.0.3
 */
WebKitWebInspector* webkit_web_view_get_inspector(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);
	return webView->priv->webInspector.get();
}

/**
 * webkit_web_view_get_viewport_attributes:
 * @web_view: a #WebKitWebView
 *
 * Obtains the #WebKitViewportAttributes associated with the
 * #WebKitWebView. Every #WebKitWebView object has a
 * #WebKitViewportAttributes object attached to it as soon as it is
 * created, so this function will only return NULL if the argument is
 * not a valid #WebKitWebView. Do note however that the viewport
 * attributes object only contains valid information when the current
 * page has a viewport meta tag. You can check whether the data should
 * be used by checking the #WebKitViewportAttributes:valid property.
 *
 * Return value: (transfer none): the #WebKitViewportAttributes instance.
 *
 * Since: 1.3.8
 */
WebKitViewportAttributes* webkit_web_view_get_viewport_attributes(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);
	return webView->priv->viewportAttributes.get();
}

// internal
static void webkit_web_view_set_window_features(WebKitWebView* webView, WebKitWebWindowFeatures* webWindowFeatures)
{
	if (!webWindowFeatures)
	  return;
	if (webkit_web_window_features_equal(webView->priv->webWindowFeatures.get(), webWindowFeatures))
	  return;
	webView->priv->webWindowFeatures = webWindowFeatures;
}

/**
 * webkit_web_view_get_window_features:
 * @web_view: a #WebKitWebView
 *
 * Returns: (transfer none): the instance of #WebKitWebWindowFeatures held by the given
 * #WebKitWebView.
 *
 * Since: 1.0.3
 */
WebKitWebWindowFeatures* webkit_web_view_get_window_features(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);
	return webView->priv->webWindowFeatures.get();
}

/**
 * webkit_web_view_get_title:
 * @web_view: a #WebKitWebView
 *
 * Returns the @web_view's document title
 *
 * Since: 1.1.4
 *
 * Return value: the title of @web_view
 */
const char* webkit_web_view_get_title(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), NULL);

	WebKitWebViewPrivate* priv = webView->priv;
	return priv->mainFrame->priv->title;
}

/**
 * webkit_web_view_get_uri:
 * @web_view: a #WebKitWebView
 *
 * Returns the current URI of the contents displayed by the @web_view
 *
 * Since: 1.1.4
 *
 * Return value: the URI of @web_view
 */
const char* webkit_web_view_get_uri(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), NULL);

	WebKitWebViewPrivate* priv = webView->priv;
	return priv->mainFrame->priv->uri;
}

/**
 * webkit_web_view_set_maintains_back_forward_list:
 * @web_view: a #WebKitWebView
 * @flag: to tell the view to maintain a back or forward list
 *
 * Set the view to maintain a back or forward list of history items.
 */
void webkit_web_view_set_maintains_back_forward_list(WebKitWebView* webView, bool flag)
{

	static_cast<BackForwardList*>(core(webView)->backForward().client())->setEnabled(flag);
}

/**
 * webkit_web_view_get_back_forward_list:
 * @web_view: a #WebKitWebView
 *
 * Obtains the #WebKitWebBackForwardList associated with the given #WebKitWebView. The
 * #WebKitWebBackForwardList is owned by the #WebKitWebView.
 *
 * Return value: (transfer none): the #WebKitWebBackForwardList
 */
WebKitWebBackForwardList* webkit_web_view_get_back_forward_list(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);
	if (!core(webView) || !static_cast<BackForwardList*>(core(webView)->backForward().client())->enabled())
		return 0;
	return webView->priv->backForwardList.get();
}

/**
 * webkit_web_view_go_to_back_forward_item:
 * @web_view: a #WebKitWebView
 * @item: a #WebKitWebHistoryItem*
 *
 * Go to the specified #WebKitWebHistoryItem
 *
 * Return value: %TRUE if loading of item is successful, %FALSE if not
 */
bool webkit_web_view_go_to_back_forward_item(WebKitWebView* webView, WebKitWebHistoryItem* item)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);
	g_return_val_if_fail(WEBKIT_IS_WEB_HISTORY_ITEM(item), FALSE);

	WebKitWebBackForwardList* backForwardList = webkit_web_view_get_back_forward_list(webView);
	if (!webkit_web_back_forward_list_contains_item(backForwardList, item))
		return FALSE;

	core(webView)->goToItem(core(item), FrameLoadTypeIndexedBackForward);
	return TRUE;
}

/**
 * webkit_web_view_go_back:
 * @web_view: a #WebKitWebView
 *
 * Loads the previous history item.
 */
void webkit_web_view_go_back(WebKitWebView* webView)
{

	core(webView)->backForward().goBack();
}

/**
 * webkit_web_view_go_back_or_forward:
 * @web_view: a #WebKitWebView
 * @steps: the number of steps
 *
 * Loads the history item that is the number of @steps away from the current
 * item. Negative values represent steps backward while positive values
 * represent steps forward.
 */
void webkit_web_view_go_back_or_forward(WebKitWebView* webView, int steps)
{

	core(webView)->backForward().goBackOrForward(steps);
}

/**
 * webkit_web_view_go_forward:
 * @web_view: a #WebKitWebView
 *
 * Loads the next history item.
 */
void webkit_web_view_go_forward(WebKitWebView* webView)
{

	core(webView)->backForward().goForward();
}

/**
 * webkit_web_view_can_go_back:
 * @web_view: a #WebKitWebView
 *
 * Determines whether #web_view has a previous history item.
 *
 * Return value: %TRUE if able to move back, %FALSE otherwise
 */
bool webkit_web_view_can_go_back(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

	if (!core(webView) || !core(webView)->backForward().canGoBackOrForward(-1))
		return FALSE;

	return TRUE;
}

/**
 * webkit_web_view_can_go_back_or_forward:
 * @web_view: a #WebKitWebView
 * @steps: the number of steps
 *
 * Determines whether #web_view has a history item of @steps. Negative values
 * represent steps backward while positive values represent steps forward.
 *
 * Return value: %TRUE if able to move back or forward the given number of
 * steps, %FALSE otherwise
 */
bool webkit_web_view_can_go_back_or_forward(WebKitWebView* webView, int steps)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

	return core(webView)->backForward().canGoBackOrForward(steps);
}

/**
 * webkit_web_view_can_go_forward:
 * @web_view: a #WebKitWebView
 *
 * Determines whether #web_view has a next history item.
 *
 * Return value: %TRUE if able to move forward, %FALSE otherwise
 */
bool webkit_web_view_can_go_forward(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

	Page* page = core(webView);

	if (!page)
		return FALSE;

	if (!page->backForward().forwardItem())
		return FALSE;

	return TRUE;
}

/**
 * webkit_web_view_open:
 * @web_view: a #WebKitWebView
 * @uri: an URI
 *
 * Requests loading of the specified URI string.
 *
 * Deprecated: 1.1.1: Use webkit_web_view_load_uri() instead.
  */
void webkit_web_view_open(WebKitWebView* webView, const char* uri)
{

	// We used to support local paths, unlike the newer
	// function webkit_web_view_load_uri
	if (g_path_is_absolute(uri)) {
		char* fileUri = g_filename_to_uri(uri, NULL, NULL);
		webkit_web_view_load_uri(webView, fileUri);
		free(fileUri);
	}
	else
		webkit_web_view_load_uri(webView, uri);
}

void webkit_web_view_reload(WebKitWebView* webView)
{

	core(webView)->mainFrame().loader().reload();
}

/**
 * webkit_web_view_reload_bypass_cache:
 * @web_view: a #WebKitWebView
 *
 * Reloads the @web_view without using any cached data.
 *
 * Since: 1.0.3
 */
void webkit_web_view_reload_bypass_cache(WebKitWebView* webView)
{

	core(webView)->mainFrame().loader().reload(true);
}

/**
 * webkit_web_view_load_uri:
 * @web_view: a #WebKitWebView
 * @uri: an URI string
 *
 * Requests loading of the specified URI string.
 *
 * Since: 1.1.1
 */
void webkit_web_view_load_uri(WebKitWebView* webView, const char* uri)
{

	WebKitWebFrame* frame = webView->priv->mainFrame;
	webkit_web_frame_load_uri(frame, uri);
}

/**
  * webkit_web_view_load_string:
  * @web_view: a #WebKitWebView
  * @content: an URI string
  * @mime_type: the MIME type, or %NULL
  * @encoding: the encoding, or %NULL
  * @base_uri: the base URI for relative locations
  *
  * Requests loading of the given @content with the specified @mime_type,
  * @encoding and @base_uri.
  *
  * If @mime_type is %NULL, "text/html" is assumed.
  *
  * If @encoding is %NULL, "UTF-8" is assumed.
  */
void webkit_web_view_load_string(WebKitWebView* webView, const char* content, const char* mimeType, const char* encoding, const char* baseUri)
{

	WebKitWebFrame* frame = webView->priv->mainFrame;
	webkit_web_frame_load_string(frame, content, mimeType, encoding, baseUri);
}
/**
 * webkit_web_view_load_html_string:
 * @web_view: a #WebKitWebView
 * @content: an URI string
 * @base_uri: the base URI for relative locations
 *
 * Requests loading of the given @content with the specified @base_uri.
 *
 * Deprecated: 1.1.1: Use webkit_web_view_load_string() instead.
 */
void webkit_web_view_load_html_string(WebKitWebView* webView, const char* content, const char* baseUri)
{

	webkit_web_view_load_string(webView, content, NULL, NULL, baseUri);
}

/**
 * webkit_web_view_load_request:
 * @web_view: a #WebKitWebView
 * @request: a #WebKitNetworkRequest
 *
 * Requests loading of the specified asynchronous client request.
 *
 * Creates a provisional data source that will transition to a committed data
 * source once any data has been received. Use webkit_web_view_stop_loading() to
 * stop the load.
 *
 * Since: 1.1.1
 */
void webkit_web_view_load_request(WebKitWebView* webView, WebKitNetworkRequest* request)
{

	WebKitWebFrame* frame = webView->priv->mainFrame;
	webkit_web_frame_load_request(frame, request);
}

/**
 * webkit_web_view_stop_loading:
 * @web_view: a #WebKitWebView
 *
 * Stops any ongoing load in the @web_view.
 **/
void webkit_web_view_stop_loading(WebKitWebView* webView)
{

	core(webView)->mainFrame().loader().stopForUserCancel();
}

/**
 * webkit_web_view_search_text:
 * @web_view: a #WebKitWebView
 * @text: a string to look for
 * @forward: whether to find forward or not
 * @case_sensitive: whether to respect the case of text
 * @wrap: whether to continue looking at the beginning after reaching the end
 *
 * Looks for a specified string inside #web_view.
 *
 * Return value: %TRUE on success or %FALSE on failure
 */
bool webkit_web_view_search_text(WebKitWebView* webView, const char* string, bool caseSensitive, bool forward, bool shouldWrap)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);
	g_return_val_if_fail(string, FALSE);

	FindOptions options = (caseSensitive ? 0 : CaseInsensitive) | (forward ? 0 : Backwards) | (shouldWrap ? WrapAround : 0);
	return core(webView)->findString(String::fromUTF8(string), options);
}

/**
 * webkit_web_view_mark_text_matches:
 * @web_view: a #WebKitWebView
 * @string: a string to look for
 * @case_sensitive: whether to respect the case of text
 * @limit: the maximum number of strings to look for or 0 for all
 *
 * Attempts to highlight all occurances of #string inside #web_view.
 *
 * Return value: the number of strings highlighted
 */
unsigned webkit_web_view_mark_text_matches(WebKitWebView* webView, const char* string, bool caseSensitive, unsigned limit)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);
	g_return_val_if_fail(string, 0);

	TextCaseSensitivity caseSensitivity = caseSensitive ? TextCaseSensitive : TextCaseInsensitive;

	return core(webView)->markAllMatchesForText(String::fromUTF8(string), caseSensitivity, false, limit);
}

/**
 * webkit_web_view_set_highlight_text_matches:
 * @web_view: a #WebKitWebView
 * @highlight: whether to highlight text matches
 *
 * Highlights text matches previously marked by webkit_web_view_mark_text_matches.
 */
void webkit_web_view_set_highlight_text_matches(WebKitWebView* webView, bool shouldHighlight)
{

	Frame* frame = &core(webView)->mainFrame();
	do {
		frame->editor().setMarkedTextMatchesAreHighlighted(shouldHighlight);
		frame = frame->tree().traverseNextWithWrap(false);
	} while (frame);
}

/**
 * webkit_web_view_unmark_text_matches:
 * @web_view: a #WebKitWebView
 *
 * Removes highlighting previously set by webkit_web_view_mark_text_matches.
 */
void webkit_web_view_unmark_text_matches(WebKitWebView* webView)
{

	return core(webView)->unmarkAllTextMatches();
}

/**
 * webkit_web_view_get_main_frame:
 * @web_view: a #WebKitWebView
 *
 * Returns the main frame for the @web_view.
 *
 * Return value: (transfer none): the main #WebKitWebFrame for @web_view
 */
WebKitWebFrame* webkit_web_view_get_main_frame(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), NULL);

	return webView->priv->mainFrame;
}

/**
 * webkit_web_view_get_focused_frame:
 * @web_view: a #WebKitWebView
 *
 * Returns the frame that has focus or an active text selection.
 *
 * Return value: (transfer none): The focused #WebKitWebFrame or %NULL if no frame is focused
 */
WebKitWebFrame* webkit_web_view_get_focused_frame(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), NULL);

	Frame* focusedFrame = core(webView)->focusController().focusedFrame();
	return kit(focusedFrame);
}

void webkit_web_view_execute_script(WebKitWebView* webView, const char* script)
{

	core(webView)->mainFrame().script().executeScript(String::fromUTF8(script), true);
}

/**
 * webkit_web_view_can_cut_clipboard:
 * @web_view: a #WebKitWebView
 *
 * Determines whether or not it is currently possible to cut to the clipboard.
 *
 * Return value: %TRUE if a selection can be cut, %FALSE if not
 */
bool webkit_web_view_can_cut_clipboard(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	return frame.editor().canCut() || frame.editor().canDHTMLCut();
}

/**
 * webkit_web_view_can_copy_clipboard:
 * @web_view: a #WebKitWebView
 *
 * Determines whether or not it is currently possible to copy to the clipboard.
 *
 * Return value: %TRUE if a selection can be copied, %FALSE if not
 */
bool webkit_web_view_can_copy_clipboard(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	return frame.editor().canCopy() || frame.editor().canDHTMLCopy();
}

/**
 * webkit_web_view_can_paste_clipboard:
 * @web_view: a #WebKitWebView
 *
 * Determines whether or not it is currently possible to paste from the clipboard.
 *
 * Return value: %TRUE if a selection can be pasted, %FALSE if not
 */
bool webkit_web_view_can_paste_clipboard(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	return frame.editor().canPaste() || frame.editor().canDHTMLPaste();
}

/**
 * webkit_web_view_cut_clipboard:
 * @web_view: a #WebKitWebView
 *
 * Cuts the current selection inside the @web_view to the clipboard.
 */
void webkit_web_view_cut_clipboard(WebKitWebView* webView)
{

	if (webkit_web_view_can_cut_clipboard(webView))
}

/**
 * webkit_web_view_copy_clipboard:
 * @web_view: a #WebKitWebView
 *
 * Copies the current selection inside the @web_view to the clipboard.
 */
void webkit_web_view_copy_clipboard(WebKitWebView* webView)
{

	if (webkit_web_view_can_copy_clipboard(webView))
}

/**
 * webkit_web_view_paste_clipboard:
 * @web_view: a #WebKitWebView
 *
 * Pastes the current contents of the clipboard to the @web_view.
 */
void webkit_web_view_paste_clipboard(WebKitWebView* webView)
{

	if (webkit_web_view_can_paste_clipboard(webView))
}

/**
 * webkit_web_view_delete_selection:
 * @web_view: a #WebKitWebView
 *
 * Deletes the current selection inside the @web_view.
 */
void webkit_web_view_delete_selection(WebKitWebView* webView)
{

	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	frame.editor().performDelete();
}

/**
 * webkit_web_view_has_selection:
 * @web_view: a #WebKitWebView
 *
 * Determines whether text was selected.
 *
 * Return value: %TRUE if there is selected text, %FALSE if not
 */
bool webkit_web_view_has_selection(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

	return !core(webView)->selection().isNone();
}

/**
 * webkit_web_view_select_all:
 * @web_view: a #WebKitWebView
 *
 * Attempts to select everything inside the @web_view.
 */
void webkit_web_view_select_all(WebKitWebView* webView)
{

}

/**
 * webkit_web_view_get_editable:
 * @web_view: a #WebKitWebView
 *
 * Returns whether the user is allowed to edit the document.
 *
 * Returns %TRUE if @web_view allows the user to edit the HTML document, %FALSE if
 * it doesn't. You can change @web_view's document programmatically regardless of
 * this setting.
 *
 * Return value: a #bool indicating the editable state
 */
bool webkit_web_view_get_editable(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

	return core(webView)->isEditable();
}

/**
 * webkit_web_view_set_editable:
 * @web_view: a #WebKitWebView
 * @flag: a #bool indicating the editable state
 *
 * Sets whether @web_view allows the user to edit its HTML document.
 *
 * If @flag is %TRUE, @web_view allows the user to edit the document. If @flag is
 * %FALSE, an element in @web_view's document can only be edited if the
 * CONTENTEDITABLE attribute has been set on the element or one of its parent
 * elements. You can change @web_view's document programmatically regardless of
 * this setting. By default a #WebKitWebView is not editable.

 * Normally, an HTML document is not editable unless the elements within the
 * document are editable. This function provides a low-level way to make the
 * contents of a #WebKitWebView editable without altering the document or DOM
 * structure.
 */
void webkit_web_view_set_editable(WebKitWebView* webView, bool flag)
{

	flag = flag != FALSE;
	if (flag == webkit_web_view_get_editable(webView))
		return;

	core(webView)->setEditable(flag);

	if (flag) {
		core(webView)->mainFrame().editor().applyEditingStyleToBodyElement();
		// TODO: If the WebKitWebView is made editable and the selection is empty, set it to something.
		//if (!webkit_web_view_get_selected_dom_range(webView))
		//	  mainFrame->setSelectionFromNone();
	}
	g_object_notify(G_OBJECT(webView), "editable");
}

/**
 * webkit_web_view_get_copy_target_list:
 * @web_view: a #WebKitWebView
 *
 * This function returns the list of targets this #WebKitWebView can
 * provide for clipboard copying and as DND source. The targets in the list are
 * added with values from the #WebKitWebViewTargetInfo enum,
 * using gtk_target_list_add() and
 * gtk_target_list_add_text_targets().
 *
 * Return value: the #FLTKTargetList
 **/
FLTKTargetList* webkit_web_view_get_copy_target_list(WebKitWebView* webView)
{
	return PasteboardHelper::defaultPasteboardHelper()->targetList();
}

/**
 * webkit_web_view_get_paste_target_list:
 * @web_view: a #WebKitWebView
 *
 * This function returns the list of targets this #WebKitWebView can
 * provide for clipboard pasting and as DND destination. The targets in the list are
 * added with values from the #WebKitWebViewTargetInfo enum,
 * using gtk_target_list_add() and
 * gtk_target_list_add_text_targets().
 *
 * Return value: the #FLTKTargetList
 **/
FLTKTargetList* webkit_web_view_get_paste_target_list(WebKitWebView* webView)
{
	return PasteboardHelper::defaultPasteboardHelper()->targetList();
}

/**
 * webkit_web_view_can_show_mime_type:
 * @web_view: a #WebKitWebView
 * @mime_type: a MIME type
 *
 * This functions returns whether or not a MIME type can be displayed using this view.
 *
 * Return value: a #bool indicating if the MIME type can be displayed
 *
 * Since: 1.0.3
 **/

bool webkit_web_view_can_show_mime_type(WebKitWebView* webView, const char* mimeType)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

	Frame* frame = core(webkit_web_view_get_main_frame(webView));
	return frame->loader().client().canShowMIMEType(String::fromUTF8(mimeType));
}

/**
 * webkit_web_view_get_transparent:
 * @web_view: a #WebKitWebView
 *
 * Returns whether the #WebKitWebView has a transparent background.
 *
 * Return value: %FALSE when the #WebKitWebView draws a solid background
 * (the default), otherwise %TRUE.
 */
bool webkit_web_view_get_transparent(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

	WebKitWebViewPrivate* priv = webView->priv;
	return priv->transparent;
}

/**
 * webkit_web_view_set_transparent:
 * @web_view: a #WebKitWebView
 * @flag: whether or not @web_view should be transparent
 *
 * Sets whether the #WebKitWebView has a transparent background.
 *
 * Pass %FALSE to have the #WebKitWebView draw a solid background
 * (the default), otherwise %TRUE.
 */
void webkit_web_view_set_transparent(WebKitWebView* webView, bool flag)
{

	WebKitWebViewPrivate* priv = webView->priv;
	priv->transparent = flag;

	// TODO: This needs to be made persistent or it could become a problem when
	// the main frame is replaced.
	core(webView)->mainFrame().view()->setTransparent(flag);
	g_object_notify(G_OBJECT(webView), "transparent");
}

/**
 * webkit_web_view_get_zoom_level:
 * @web_view: a #WebKitWebView
 *
 * Returns the zoom level of @web_view, i.e. the factor by which elements in
 * the page are scaled with respect to their original size.
 * If the "full-content-zoom" property is set to %FALSE (the default)
 * the zoom level changes the text size, or if %TRUE, scales all
 * elements in the page.
 *
 * Return value: the zoom level of @web_view
 *
 * Since: 1.0.1
 */
gfloat webkit_web_view_get_zoom_level(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 1.0f);

	Frame& frame = core(webView)->mainFrame();
	WebKitWebViewPrivate* priv = webView->priv;
	return priv->zoomFullContent ? frame.pageZoomFactor() : frame.textZoomFactor();
}

static void webkit_web_view_apply_zoom_level(WebKitWebView* webView, gfloat zoomLevel)
{
	Frame& frame = core(webView)->mainFrame();

	WebKitWebViewPrivate* priv = webView->priv;
	if (priv->zoomFullContent)
		frame.setPageZoomFactor(zoomLevel);
	else
		frame.setTextZoomFactor(zoomLevel);
}

/**
 * webkit_web_view_set_zoom_level:
 * @web_view: a #WebKitWebView
 * @zoom_level: the new zoom level
 *
 * Sets the zoom level of @web_view, i.e. the factor by which elements in
 * the page are scaled with respect to their original size.
 * If the "full-content-zoom" property is set to %FALSE (the default)
 * the zoom level changes the text size, or if %TRUE, scales all
 * elements in the page.
 *
 * Since: 1.0.1
 */
void webkit_web_view_set_zoom_level(WebKitWebView* webView, gfloat zoomLevel)
{

	webkit_web_view_apply_zoom_level(webView, zoomLevel);
	g_object_notify(G_OBJECT(webView), "zoom-level");
}

/**
 * webkit_web_view_zoom_in:
 * @web_view: a #WebKitWebView
 *
 * Increases the zoom level of @web_view. The current zoom
 * level is incremented by the value of the "zoom-step"
 * property of the #WebKitWebSettings associated with @web_view.
 *
 * Since: 1.0.1
 */
void webkit_web_view_zoom_in(WebKitWebView* webView)
{

	WebKitWebViewPrivate* priv = webView->priv;
	gfloat zoomMultiplierRatio;
	g_object_get(priv->webSettings.get(), "zoom-step", &zoomMultiplierRatio, NULL);

	webkit_web_view_set_zoom_level(webView, webkit_web_view_get_zoom_level(webView) + zoomMultiplierRatio);
}

/**
 * webkit_web_view_zoom_out:
 * @web_view: a #WebKitWebView
 *
 * Decreases the zoom level of @web_view. The current zoom
 * level is decremented by the value of the "zoom-step"
 * property of the #WebKitWebSettings associated with @web_view.
 *
 * Since: 1.0.1
 */
void webkit_web_view_zoom_out(WebKitWebView* webView)
{

	WebKitWebViewPrivate* priv = webView->priv;
	gfloat zoomMultiplierRatio;
	g_object_get(priv->webSettings.get(), "zoom-step", &zoomMultiplierRatio, NULL);

	webkit_web_view_set_zoom_level(webView, webkit_web_view_get_zoom_level(webView) - zoomMultiplierRatio);
}

/**
 * webkit_web_view_get_full_content_zoom:
 * @web_view: a #WebKitWebView
 *
 * Returns whether the zoom level affects only text or all elements.
 *
 * Return value: %FALSE if only text should be scaled (the default),
 * %TRUE if the full content of the view should be scaled.
 *
 * Since: 1.0.1
 */
bool webkit_web_view_get_full_content_zoom(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

	WebKitWebViewPrivate* priv = webView->priv;
	return priv->zoomFullContent;
}

/**
 * webkit_web_view_set_full_content_zoom:
 * @web_view: a #WebKitWebView
 * @full_content_zoom: %FALSE if only text should be scaled (the default),
 * %TRUE if the full content of the view should be scaled.
 *
 * Sets whether the zoom level affects only text or all elements.
 *
 * Since: 1.0.1
 */
void webkit_web_view_set_full_content_zoom(WebKitWebView* webView, bool zoomFullContent)
{

	WebKitWebViewPrivate* priv = webView->priv;
	if (priv->zoomFullContent == zoomFullContent)
	  return;

	Frame& frame = core(webView)->mainFrame();

	gfloat zoomLevel = priv->zoomFullContent ? frame.pageZoomFactor() : frame.textZoomFactor();

	priv->zoomFullContent = zoomFullContent;
	if (priv->zoomFullContent)
		frame.setPageAndTextZoomFactors(zoomLevel, 1);
	else
		frame.setPageAndTextZoomFactors(1, zoomLevel);

	g_object_notify(G_OBJECT(webView), "full-content-zoom");
}

/**
 * webkit_web_view_get_load_status:
 * @web_view: a #WebKitWebView
 *
 * Determines the current status of the load.
 *
 * Returns: a #WebKitLoadStatus specifying the status of the current load
 *
 * Since: 1.1.7
 */
WebKitLoadStatus webkit_web_view_get_load_status(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), WEBKIT_LOAD_FINISHED);

	WebKitWebViewPrivate* priv = webView->priv;
	return priv->loadStatus;
}

/**
 * webkit_web_view_get_progress:
 * @web_view: a #WebKitWebView
 *
 * Determines the current progress of the load.
 *
 * Returns: a double representing the status of the current load. This value
 *	will be in the range [0, 1].
 *
 * Since: 1.1.7
 */
double webkit_web_view_get_progress(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 1.0);

	return core(webView)->progress().estimatedProgress();
}

/**
 * webkit_web_view_get_encoding:
 * @web_view: a #WebKitWebView
 *
 * Returns the default encoding of the #WebKitWebView.
 *
 * Return value: the default encoding
 *
 * Since: 1.1.1
 */
const char* webkit_web_view_get_encoding(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), NULL);
	String encoding = core(webView)->mainFrame().document()->encoding();
	if (encoding.isEmpty())
		return 0;
	webView->priv->encoding = encoding.utf8();
	return webView->priv->encoding.data();
}

/**
 * webkit_web_view_set_custom_encoding:
 * @web_view: a #WebKitWebView
 * @encoding: the new encoding, or %NULL to restore the default encoding
 *
 * Sets the current #WebKitWebView encoding, without modifying the default one,
 * and reloads the page.
 *
 * Since: 1.1.1
 */
void webkit_web_view_set_custom_encoding(WebKitWebView* webView, const char* encoding)
{

	core(webView)->mainFrame().loader().reloadWithOverrideEncoding(String::fromUTF8(encoding));
}

/**
 * webkit_web_view_get_custom_encoding:
 * @web_view: a #WebKitWebView
 *
 * Returns the current encoding of the #WebKitWebView, not the default-encoding
 * of WebKitWebSettings.
 *
 * Return value: a string containing the current custom encoding for @web_view, or %NULL if there's none set.
 *
 * Since: 1.1.1
 */
const char* webkit_web_view_get_custom_encoding(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), NULL);
	String overrideEncoding = core(webView)->mainFrame().loader().documentLoader()->overrideEncoding();
	if (overrideEncoding.isEmpty())
		return 0;
	webView->priv->customEncoding = overrideEncoding.utf8();
	return webView->priv->customEncoding.data();
}

/**
 * webkit_web_view_set_view_mode:
 * @web_view: the #WebKitWebView that will have its view mode set
 * @mode: the %WebKitWebViewViewMode to be set
 *
 * Sets the view-mode property of the #WebKitWebView. Check the
 * property's documentation for more information.
 *
 * Since: 1.3.4
 */
void webkit_web_view_set_view_mode(WebKitWebView* webView, WebKitWebViewViewMode mode)
{

	Page* page = core(webView);

	switch (mode) {
	case WEBKIT_WEB_VIEW_VIEW_MODE_FLOATING:
		page->setViewMode(Page::ViewModeFloating);
		break;
	case WEBKIT_WEB_VIEW_VIEW_MODE_FULLSCREEN:
		page->setViewMode(Page::ViewModeFullscreen);
		break;
	case WEBKIT_WEB_VIEW_VIEW_MODE_MAXIMIZED:
		page->setViewMode(Page::ViewModeMaximized);
		break;
	case WEBKIT_WEB_VIEW_VIEW_MODE_MINIMIZED:
		page->setViewMode(Page::ViewModeMinimized);
		break;
	default:
		page->setViewMode(Page::ViewModeWindowed);
		break;
	}
}

/**
 * webkit_web_view_get_view_mode:
 * @web_view: the #WebKitWebView to obtain the view mode from
 *
 * Gets the value of the view-mode property of the
 * #WebKitWebView. Check the property's documentation for more
 * information.
 *
 * Return value: the %WebKitWebViewViewMode currently set for the
 * #WebKitWebView.
 *
 * Since: 1.3.4
 */
WebKitWebViewViewMode webkit_web_view_get_view_mode(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), WEBKIT_WEB_VIEW_VIEW_MODE_WINDOWED);

	Page* page = core(webView);
	Page::ViewMode mode = page->viewMode();

	if (mode == Page::ViewModeFloating)
		return WEBKIT_WEB_VIEW_VIEW_MODE_FLOATING;

	if (mode == Page::ViewModeFullscreen)
		return WEBKIT_WEB_VIEW_VIEW_MODE_FULLSCREEN;

	if (mode == Page::ViewModeMaximized)
		return WEBKIT_WEB_VIEW_VIEW_MODE_MAXIMIZED;

	if (mode == Page::ViewModeMinimized)
		return WEBKIT_WEB_VIEW_VIEW_MODE_MINIMIZED;

	return WEBKIT_WEB_VIEW_VIEW_MODE_WINDOWED;
}

/**
 * webkit_web_view_move_cursor:
 * @web_view: a #WebKitWebView
 * @step: a #FLTKMovementStep
 * @count: integer describing the direction of the movement. 1 for forward, -1 for backwards.
 *
 * Move the cursor in @view as described by @step and @count.
 *
 * Since: 1.1.4
 */
void webkit_web_view_move_cursor(WebKitWebView* webView, FLTKMovementStep step, int count)
{
					 step == GTK_MOVEMENT_DISPLAY_LINES ||
					 step == GTK_MOVEMENT_PAGES ||
					 step == GTK_MOVEMENT_BUFFER_ENDS);

	bool handled;
}

/**
 * webkit_web_view_can_undo:
 * @web_view: a #WebKitWebView
 *
 * Determines whether or not it is currently possible to undo the last
 * editing command in the view.
 *
 * Return value: %TRUE if a undo can be done, %FALSE if not
 *
 * Since: 1.1.14
 */
bool webkit_web_view_can_undo(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	return frame.editor().canUndo();
}

/**
 * webkit_web_view_undo:
 * @web_view: a #WebKitWebView
 *
 * Undoes the last editing command in the view, if possible.
 *
 * Since: 1.1.14
 */
void webkit_web_view_undo(WebKitWebView* webView)
{

	if (webkit_web_view_can_undo(webView))
}

/**
 * webkit_web_view_can_redo:
 * @web_view: a #WebKitWebView
 *
 * Determines whether or not it is currently possible to redo the last
 * editing command in the view.
 *
 * Return value: %TRUE if a redo can be done, %FALSE if not
 *
 * Since: 1.1.14
 */
bool webkit_web_view_can_redo(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	return frame.editor().canRedo();
}

/**
 * webkit_web_view_redo:
 * @web_view: a #WebKitWebView
 *
 * Redoes the last editing command in the view, if possible.
 *
 * Since: 1.1.14
 */
void webkit_web_view_redo(WebKitWebView* webView)
{

	if (webkit_web_view_can_redo(webView))
}


/**
 * webkit_web_view_set_view_source_mode:
 * @web_view: a #WebKitWebView
 * @view_source_mode: the mode to turn on or off view source mode
 *
 * Set whether the view should be in view source mode. Setting this mode to
 * %TRUE before loading a URI will display the source of the web page in a
 * nice and readable format.
 *
 * Since: 1.1.14
 */
void webkit_web_view_set_view_source_mode (WebKitWebView* webView, bool mode)
{

	core(webView)->mainFrame().setInViewSourceMode(mode);
}

/**
 * webkit_web_view_get_view_source_mode:
 * @web_view: a #WebKitWebView
 *
 * Return value: %TRUE if @web_view is in view source mode, %FALSE otherwise.
 *
 * Since: 1.1.14
 */
bool webkit_web_view_get_view_source_mode (WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);

	return core(webView)->mainFrame().inViewSourceMode();
}

// Internal subresource management
void webkit_web_view_add_main_resource(WebKitWebView* webView, const char* identifier, WebKitWebResource* webResource)
{
	WebKitWebViewPrivate* priv = webView->priv;

	priv->mainResource = adoptGRef(webResource);
	priv->mainResourceIdentifier = identifier;
}

void webkit_web_view_add_resource(WebKitWebView* webView, const char* identifier, WebKitWebResource* webResource)
{
	WebKitWebViewPrivate* priv = webView->priv;
	g_hash_table_insert(priv->subResources.get(), strdup(identifier), webResource);
}

void webkitWebViewRemoveSubresource(WebKitWebView* webView, const char* identifier)
{
	ASSERT(identifier);

	// Don't remove the main resource.
	const CString& mainResource = webView->priv->mainResourceIdentifier;
	if (!mainResource.isNull() && g_str_equal(identifier, mainResource.data()))
		return;
	g_hash_table_remove(webView->priv->subResources.get(), identifier);
}

WebKitWebResource* webkit_web_view_get_resource(WebKitWebView* webView, char* identifier)
{
	WebKitWebViewPrivate* priv = webView->priv;
	gpointer webResource = 0;
	bool resourceFound = g_hash_table_lookup_extended(priv->subResources.get(), identifier, NULL, &webResource);

	// The only resource we do not store in this hash table is the
	// main!  If we did not find a request, it probably means the load
	// has been interrupted while while a resource was still being
	// loaded.
	if (!resourceFound && !g_str_equal(identifier, priv->mainResourceIdentifier.data()))
		return 0;

	if (!webResource)
		return webkit_web_view_get_main_resource(webView);

	return WEBKIT_WEB_RESOURCE(webResource);
}

WebKitWebResource* webkit_web_view_get_main_resource(WebKitWebView* webView)
{
	return webView->priv->mainResource.get();
}

void webkit_web_view_clear_resources(WebKitWebView* webView)
{
	WebKitWebViewPrivate* priv = webView->priv;

	if (priv->subResources)
		g_hash_table_remove_all(priv->subResources.get());
}

static bool cleanupTemporarilyCachedSubresources(gpointer data)
{
	GList* subResources = static_cast<GList*>(data);
	g_list_foreach(subResources, reinterpret_cast<GFunc>(g_object_unref), NULL);
	g_list_free(subResources);
	return FALSE;
}

GList* webkit_web_view_get_subresources(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), NULL);
	GList* subResources = 0;
	Vector<PassRefPtr<ArchiveResource> > coreSubResources;

	core(webView)->mainFrame().loader().documentLoader()->getSubresources(coreSubResources);

	for (unsigned i = 0; i < coreSubResources.size(); i++) {
		WebKitWebResource* webResource = WEBKIT_WEB_RESOURCE(g_object_new(WEBKIT_TYPE_WEB_RESOURCE, NULL));
		webkit_web_resource_init_with_core_resource(webResource, coreSubResources[i]);
		subResources = g_list_append(subResources, webResource);
	}

	if (subResources)
		g_timeout_add(1, cleanupTemporarilyCachedSubresources, g_list_copy(subResources));

	return subResources;
}

/* From EventHandler.cpp */
static IntPoint documentPointForWindowPoint(Frame* frame, const IntPoint& windowPoint)
{
	FrameView* view = frame->view();
	// FIXME: Is it really OK to use the wrong coordinates here when view is 0?
	// Historically the code would just crash; this is clearly no worse than that.
	return view ? view->windowToContents(windowPoint) : windowPoint;
}

void webkit_web_view_set_tooltip_text(WebKitWebView* webView, const char* tooltip)
{
	WebKitWebViewPrivate* priv = webView->priv;
	if (tooltip && *tooltip != '\0') {
		priv->tooltipText = tooltip;
		gtk_widget_set_has_tooltip(GTK_WIDGET(webView), TRUE);
	} else {
		priv->tooltipText = "";
		gtk_widget_set_has_tooltip(GTK_WIDGET(webView), FALSE);
	}

	gtk_widget_trigger_tooltip_query(GTK_WIDGET(webView));
}

/**
 * webkit_web_view_get_hit_test_result:
 * @web_view: a #WebKitWebView
 * @event: a #GdkEventButton
 *
 * Does a 'hit test' in the coordinates specified by @event to figure
 * out context information about that position in the @web_view.
 *
 * Returns: (transfer full): a newly created #WebKitHitTestResult with the context of the
 * specified position.
 *
 * Since: 1.1.15
 **/
WebKitHitTestResult* webkit_web_view_get_hit_test_result(WebKitWebView* webView, GdkEventButton* event)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), NULL);
	g_return_val_if_fail(event, NULL);

	PlatformMouseEvent mouseEvent = PlatformMouseEvent(event);
	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	HitTestRequest request(HitTestRequest::Active | HitTestRequest::DisallowShadowContent);
	IntPoint documentPoint = documentPointForWindowPoint(&frame, mouseEvent.position());
	MouseEventWithHitTestResults mev = frame.document()->prepareMouseEvent(request, documentPoint, mouseEvent);

	return kit(mev.hitTestResult());
}

/**
 * webkit_web_view_get_icon_uri:
 * @web_view: the #WebKitWebView object
 *
 * Obtains the URI for the favicon for the given #WebKitWebView, or
 * %NULL if there is none.
 *
 * Return value: the URI for the favicon, or %NULL
 *
 * Since: 1.1.18
 */
const char* webkit_web_view_get_icon_uri(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);
	String iconURL = iconDatabase().synchronousIconURLForPageURL(core(webView)->mainFrame().document()->url().string());
	webView->priv->iconURI = iconURL.utf8();
	return webView->priv->iconURI.data();
}

/**
 * webkit_web_view_get_icon_pixbuf:
 * @web_view: the #WebKitWebView object
 *
 * Obtains a #GdkPixbuf of the favicon for the given #WebKitWebView, or
 * a default icon if there is no icon for the given page. Use
 * webkit_web_view_get_icon_uri() if you need to distinguish these cases.
 * Usually you want to connect to WebKitWebView::icon-loaded and call this
 * method in the callback.
 *
 * The pixbuf will have the largest size provided by the server and should
 * be resized before it is displayed.
 * See also webkit_icon_database_get_icon_pixbuf().
 *
 * Returns: (transfer full): a new reference to a #GdkPixbuf, or %NULL
 *
 * Since: 1.3.13
 *
 * Deprecated: 1.8: Use webkit_web_view_try_get_favicon_pixbuf() instead.
 */
GdkPixbuf* webkit_web_view_get_icon_pixbuf(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);

	const char* pageURI = webkit_web_view_get_uri(webView);
	WebKitIconDatabase* database = webkit_get_icon_database();
	return webkit_icon_database_get_icon_pixbuf(database, pageURI);
}

/**
 * webkit_web_view_try_get_favicon_pixbuf:
 * @web_view: the #WebKitWebView object
 * @width: the desired width for the icon
 * @height: the desired height for the icon
 *
 * Obtains a #GdkPixbuf of the favicon for the given
 * #WebKitWebView. This will return %NULL is there is no icon for the
 * current #WebKitWebView or if the icon is in the database but not
 * available at the moment of this call. Use
 * webkit_web_view_get_icon_uri() if you need to distinguish these
 * cases.  Usually you want to connect to WebKitWebView::icon-loaded
 * and call this method in the callback.
 *
 * See also webkit_favicon_database_try_get_favicon_pixbuf(). Contrary
 * to this function the icon database one returns the URL of the page
 * containing the icon.
 *
 * Returns: (transfer full): a new reference to a #GdkPixbuf, or %NULL
 *
 * Since: 1.8
 */
GdkPixbuf* webkit_web_view_try_get_favicon_pixbuf(WebKitWebView* webView, unsigned width, unsigned height)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);

	const char* pageURI = webkit_web_view_get_uri(webView);
	WebKitFaviconDatabase* database = webkit_get_favicon_database();
	return webkit_favicon_database_try_get_favicon_pixbuf(database, pageURI, width, height);
}

/**
 * webkit_web_view_get_dom_document:
 * @web_view: a #WebKitWebView
 *
 * Returns: (transfer none): the #WebKitDOMDocument currently loaded in
 * the main frame of the @web_view or %NULL if no document is loaded
 *
 * Since: 1.3.1
 **/
WebKitDOMDocument*
webkit_web_view_get_dom_document(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);

	return webkit_web_frame_get_dom_document(webView->priv->mainFrame);
}

FLTKMenu* webkit_web_view_get_context_menu(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);

#if ENABLE(CONTEXT_MENUS)
	ContextMenu* menu = core(webView)->contextMenuController().contextMenu();
	if (!menu)
		return 0;
	return menu->platformDescription();
#else
	return 0;
#endif
}

/**
 * webkit_web_view_get_snapshot:
 * @web_view: a #WebKitWebView
 *
 * Retrieves a snapshot with the visible contents of @webview.
 *
 * Returns: (transfer full): a @cairo_surface_t
 *
 * Since: 1.10
 **/
cairo_surface_t*
webkit_web_view_get_snapshot(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);

	Frame& frame = core(webView)->mainFrame();
	if (!frame.contentRenderer() || !frame.view())
		return 0;

	frame.view()->updateLayoutAndStyleIfNeededRecursive();
	FLTKAllocation allocation;
	gtk_widget_get_allocation(GTK_WIDGET(webView), &allocation);
	cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, allocation.width, allocation.height);
	RefPtr<cairo_t> cr = adoptRef(cairo_create(surface));
	GraphicsContext gc(cr.get());

	IntRect rect = allocation;
	gc.applyDeviceScaleFactor(frame.page()->deviceScaleFactor());
	gc.save();
	gc.clip(rect);
	if (webView->priv->transparent)
		gc.clearRect(rect);
	frame.view()->paint(&gc, rect);
	gc.restore();

	return surface;
}

#if ENABLE(ICONDATABASE)
void webkitWebViewIconLoaded(WebKitFaviconDatabase* database, const char* frameURI, WebKitWebView* webView)
{
	// Since we definitely have an icon the WebView doesn't need to
	// listen for notifications any longer.
	webkitWebViewRegisterForIconNotification(webView, false);

	// webkit_web_view_get_icon_uri() properly updates the "icon-uri" property.
	g_object_notify(G_OBJECT(webView), "icon-uri");
}

void webkitWebViewRegisterForIconNotification(WebKitWebView* webView, bool shouldRegister)
{
	WebKitFaviconDatabase* database = webkit_get_favicon_database();
	if (shouldRegister) {
																G_CALLBACK(webkitWebViewIconLoaded), webView);
	} else
}
#endif

void webkitWebViewDirectionChanged(WebKitWebView* webView, FLTKTextDirection previousDirection, gpointer)
{

	FLTKTextDirection direction = gtk_widget_get_direction(GTK_WIDGET(webView));

	Frame* focusedFrame = core(webView)->focusController().focusedFrame();
	if (!focusedFrame)
		return;

	Editor& editor = focusedFrame->editor();
	if (!editor.canEdit())
		return;

	switch (direction) {
	case GTK_TEXT_DIR_NONE:
		editor.setBaseWritingDirection(NaturalWritingDirection);
		break;
	case GTK_TEXT_DIR_LTR:
		editor.setBaseWritingDirection(LeftToRightWritingDirection);
		break;
	case GTK_TEXT_DIR_RTL:
		editor.setBaseWritingDirection(RightToLeftWritingDirection);
		break;
	default:
		g_assert_not_reached();
		return;
	}
}

namespace WebKit {

WebCore::Page* core(WebKitWebView* webView)
{
	if (!webView)
		return 0;

	WebKitWebViewPrivate* priv = webView->priv;
	return priv ? priv->corePage : 0;
}

WebKitWebView* kit(WebCore::Page* corePage)
{
	if (!corePage)
		return 0;

	WebCore::ChromeClient& chromeClient = corePage->chrome().client();
	if (chromeClient.isEmptyChromeClient())
		return 0;

	return static_cast<WebKit::ChromeClient&>(chromeClient).webView();
}

}
