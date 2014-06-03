/*
 * Copyright (C) 2008 Gustavo Noronha Silva
 * Copyright (C) 2008, 2009 Holger Hans Peter Freyther
 * Copyright (C) 2009 Collabora Ltd.
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
#include "webkitwebinspector.h"

#include "DumpRenderTreeSupportFLTK.h"
#include "FocusController.h"
#include "Frame.h"
#include "HitTestRequest.h"
#include "HitTestResult.h"
#include "InspectorClientFLTK.h"
#include "InspectorController.h"
#include "InspectorInstrumentation.h"
#include "IntPoint.h"
#include "Page.h"
#include "RenderLayer.h"
#include "RenderView.h"
#include "webkitglobalsprivate.h"

/**
 * SECTION:webkitwebinspector
 * @short_description: Access to the WebKit Inspector
 *
 * The WebKit Inspector is a graphical tool to inspect and change
 * the content of a #WebKitWebView. It also includes an interactive
 * JavaScriptDebugger. Using this class one can get a Fl_Widget which
 * can be embedded into an application to show the inspector.
 *
 * The inspector is available when the #WebKitWebSettings of the
 * #WebKitWebView has set the #WebKitWebSettings:enable-developer-extras
 * to true otherwise no inspector is available.
 */

using namespace WebKit;
using namespace WebCore;

WebKitWebInspector::WebKitWebInspector() {

}

WebKitWebInspector::~WebKitWebInspector() {
	free(inspected_uri);
}

// internal use only
void WebKitWebInspector::set_web_view(WebKitWebView *web_view)
{
	inspector_view = web_view;
}

/**
 * WebKitWebInspector::get_web_view:
 * @web_inspector: a #WebKitWebInspector
 *
 * Obtains the #WebKitWebView that is used to render the
 * inspector. The #WebKitWebView instance is created by the
 * application, by handling the #WebKitWebInspector::inspect-web-view signal. This means
 * that this method may return %NULL if the user hasn't inspected
 * anything.
 *
 * Returns: (transfer none): the #WebKitWebView instance that is used
 * to render the inspector or %NULL if it is not yet created.
 *
 * Since: 1.0.3
 **/
WebKitWebView* WebKitWebInspector::get_web_view()
{
	return inspector_view;
}

// internal use only
void WebKitWebInspector::set_inspected_uri(const char* inspected_uri)
{
	free(inspected_uri);
	inspected_uri = strdup(inspected_uri);
}

/**
 * WebKitWebInspector::get_inspected_uri:
 * @web_inspector: a #WebKitWebInspector
 *
 * Obtains the URI that is currently being inspected.
 *
 * Returns: a pointer to the URI as an internally allocated string; it
 * should not be freed, modified or stored.
 *
 * Since: 1.0.3
 **/
const char* WebKitWebInspector::get_inspected_uri()
{
	return inspected_uri;
}

void WebKitWebInspector::set_inspector_client(WebCore::Page* page)
{
	this->page = page;
}

/**
 * WebKitWebInspector::show:
 * @web_inspector: the #WebKitWebInspector that will be shown
 *
 * Causes the Web Inspector to be shown.
 *
 * Since: 1.1.17
 */
void WebKitWebInspector::show()
{
	Frame& frame = page->focusController().focusedOrMainFrame();
	FrameView* view = frame.view();

	if (!view)
		return;

	page->inspectorController().show();
}

/**
 * WebKitWebInspector::inspect_node:
 * @web_inspector: the #WebKitWebInspector that will do the inspection
 * @node: the #WebKitDOMNode to inspect
 *
 * Causes the Web Inspector to inspect the given node.
 *
 * Since: 1.3.7
 */
void WebKitWebInspector::inspect_node(WebKitDOMNode* node)
{
	webInspector->page->inspectorController().inspect(core(node));
}

/**
 * WebKitWebInspector::inspect_coordinates:
 * @web_inspector: the #WebKitWebInspector that will do the inspection
 * @x: the X coordinate of the node to be inspected
 * @y: the Y coordinate of the node to be inspected
 *
 * Causes the Web Inspector to inspect the node that is located at the
 * given coordinates of the widget. The coordinates should be relative
 * to the #WebKitWebView widget, not to the scrollable content, and
 * may be obtained from a #GdkEvent directly.
 *
 * This means @x, and @y being zero doesn't guarantee you will hit the
 * left-most top corner of the content, since the contents may have
 * been scrolled.
 *
 * Since: 1.1.17
 */
void WebKitWebInspector::inspect_coordinates(double x, double y)
{
	if (x < 0 || y < 0)
		return;

	Frame& frame = page->focusController().focusedOrMainFrame();
	FrameView* view = frame.view();

	if (!view)
		return;

	HitTestRequest request(HitTestRequest::ReadOnly | HitTestRequest::Active | HitTestRequest::DisallowShadowContent);
	IntPoint documentPoint = view->windowToContents(IntPoint(static_cast<int>(x), static_cast<int>(y)));
	HitTestResult result(documentPoint);

	frame.contentRenderer()->layer()->hitTest(request, result);
	page->inspectorController().inspect(result.innerNonSharedNode());
}

/**
 * WebKitWebInspector::close:
 * @web_inspector: the #WebKitWebInspector that will be closed
 *
 * Causes the Web Inspector to be closed.
 *
 * Since: 1.1.17
 */
void WebKitWebInspector::close()
{
	page->inspectorController().close();
}

void WebKitWebInspector::execute_script(long callId, const char* script)
{
	page->inspectorController().evaluateForTestInFrontend(callId, script);
}
