/*
 * Copyright (C) 2007, 2008 Holger Hans Peter Freyther
 * Copyright (C) 2007 Alp Toker <alp@atoker.com>
 * Copyright (C) 2007 Apple Inc.
 * Copyright (C) 2008 Christian Dywan <christian@imendio.com>
 * Copyright (C) 2008 Collabora Ltd.
 * Copyright (C) 2008 Nuanti Ltd.
 * Copyright (C) 2009 Jan Alonzo <jmalonzo@gmail.com>
 * Copyright (C) 2009 Gustavo Noronha Silva <gns@gnome.org>
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
#include "webkitwebframe.h"

#include "AXObjectCache.h"
#include "AnimationController.h"
#include "DOMObjectCache.h"
#include "DocumentFragment.h"
#include "DocumentLoader.h"
#include "DocumentLoaderFLTK.h"
#include "Editor.h"
#include "FrameLoadRequest.h"
#include "FrameLoader.h"
#include "FrameLoaderClientFLTK.h"
#include "FrameSelection.h"
#include "FrameTree.h"
#include "FrameView.h"
#include "GCController.h"
#include "GraphicsContext.h"
#include "FLTKUtilities.h"
#include "HTMLFrameOwnerElement.h"
#include "JSDOMBinding.h"
#include "JSDOMWindow.h"
#include "JSElement.h"
#include "MainFrame.h"
#include "PlatformContextCairo.h"
#include "PrintContext.h"
#include "RenderListItem.h"
#include "RenderTreeAsText.h"
#include "RenderView.h"
#include "ScriptController.h"
#include "SubstituteData.h"
#include "TextIterator.h"
#include "WebKitAccessibleWrapperAtk.h"
#include "WebKitDOMDocumentPrivate.h"
#include "WebKitDOMRangePrivate.h"
#include "markup.h"
#include "webkitglobalsprivate.h"
#include "webkitnetworkresponse.h"
#include "webkitnetworkrequestprivate.h"
#include "webkitnetworkresponseprivate.h"
#include "webkitsecurityoriginprivate.h"
#include "webkitwebframeprivate.h"
#include "webkitwebresource.h"
#include "webkitwebview.h"
#include "webkitwebviewprivate.h"
#include <JavaScriptCore/APICast.h>
#include <atk/atk.h>
#include <wtf/text/CString.h>

#if ENABLE(SVG)
#include "SVGSMILElement.h"
#endif

/**
 * SECTION:webkitwebframe
 * @short_description: The content of a #WebKitWebView
 *
 * A #WebKitWebView contains a main #WebKitWebFrame. A #WebKitWebFrame
 * contains the content of one URI. The URI and name of the frame can
 * be retrieved, the load status and progress can be observed using the
 * signals and can be controlled using the methods of the #WebKitWebFrame.
 * A #WebKitWebFrame can have any number of children and one child can
 * be found by using #WebKitWebFrame::find_frame.
 *
 */

using namespace WebKit;
using namespace WebCore;
using namespace std;

// Called from the FrameLoaderClient when it is destroyed. Normally
// the unref in the FrameLoaderClient is destroying this object as
// well but due reference counting a user might have added a reference...
void WebKitWebFrame::core_frame_gone()
{
	if (coreFrame)
		DOMObjectCache::clearByFrame(coreFrame);
	coreFrame = 0;
}

WebKitWebFrame::WebKitWebFrame(WebKitWebView* webView) {
	name = title = uri = NULL;
	webkitInit();

	this->webView = webView;
	coreFrame = &viewcorePage->mainFrame();
	static_cast<WebKit::FrameLoaderClient*>(&viewcorePage->mainFrame().loader().client())->setWebFrame(frame);
	coreFrame->init();

	origin = 0;
}

WebKitWebFrame::~WebKitWebFrame() {
	if (coreFrame) {
		DOMObjectCache::clearByFrame(coreFrame);
		coreFrame->loader().cancelAndClear();
		coreFrame = 0;
	}

	free(name);
	free(title);
	free(uri);
}

static WebKitWebDataSource* WebKitWebFrame::get_data_source_from_core_loader(WebCore::DocumentLoader* loader)
{
	return loader ? static_cast<WebKit::DocumentLoader*>(loader)->dataSource() : 0;
}

/**
 * WebKitWebFrame::get_title:
 * @frame: a #WebKitWebFrame
 *
 * Returns the @frame's document title
 *
 * Return value: the title of @frame
 */
const char* WebKitWebFrame::get_title()
{
	return title;
}

/**
 * WebKitWebFrame::get_uri:
 * @frame: a #WebKitWebFrame
 *
 * Returns the current URI of the contents displayed by the @frame
 *
 * Return value: the URI of @frame
 */
const char* WebKitWebFrame::get_uri()
{
	return uri;
}

/**
 * WebKitWebFrame::get_web_view:
 * @frame: a #WebKitWebFrame
 *
 * Returns the #WebKitWebView that manages this #WebKitWebFrame.
 *
 * The #WebKitWebView returned manages the entire hierarchy of #WebKitWebFrame
 * objects that contains @frame.
 *
 * Return value: (transfer none): the #WebKitWebView that manages @frame
 */
WebKitWebView* WebKitWebFrame::get_web_view()
{
	return webView;
}

/**
 * WebKitWebFrame::get_name:
 * @frame: a #WebKitWebFrame
 *
 * Returns the @frame's name
 *
 * Return value: the name of @frame. This method will return NULL if
 * the #WebKitWebFrame is invalid or an empty string if it is not backed
 * by a live WebCore frame.
 */
const char* WebKitWebFrame::get_name()
{
	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return "";

	CString frameName = coreFrame->tree().uniqueName().string().utf8();
	if (!strcmp(frameName.data(), name))
		return name;

	free(name);
	name = strdup(frameName.data());
	return name;
}

/**
 * WebKitWebFrame::get_parent:
 * @frame: a #WebKitWebFrame
 *
 * Returns the @frame's parent frame, or %NULL if it has none.
 *
 * Return value: (transfer none): the parent #WebKitWebFrame or %NULL in case there is none
 */
WebKitWebFrame* WebKitWebFrame::get_parent()
{
	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return 0;

	return kit(coreFrame->tree().parent());
}

/**
 * WebKitWebFrame::load_uri:
 * @frame: a #WebKitWebFrame
 * @uri: an URI string
 *
 * Requests loading of the specified URI string.
 *
 * Since: 1.1.1
 */
void WebKitWebFrame::load_uri(const char* uri)
{
	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return;

	coreFrame->loader().load(FrameLoadRequest(coreFrame, ResourceRequest(URL(URL(), String::fromUTF8(uri)))));
}

static void load_data(const char* content, const char* mimeType, const char* encoding, const char* baseURL, const char* unreachableURL)
{
	Frame* coreFrame = core(frame);
	ASSERT(coreFrame);

	URL baseKURL = baseURL ? URL(URL(), String::fromUTF8(baseURL)) : blankURL();

	ResourceRequest request(baseKURL);

	RefPtr<SharedBuffer> sharedBuffer = SharedBuffer::create(content, strlen(content));
	SubstituteData substituteData(sharedBuffer.release(),
				  mimeType ? String::fromUTF8(mimeType) : String::fromUTF8("text/html"),
				  encoding ? String::fromUTF8(encoding) : String::fromUTF8("UTF-8"),
				  URL(URL(), String::fromUTF8(unreachableURL)),
				  URL(URL(), String::fromUTF8(unreachableURL)));

	coreFrame->loader().load(FrameLoadRequest(coreFrame, request, substituteData));
}

/**
 * WebKitWebFrame::load_string:
 * @frame: a #WebKitWebFrame
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
 *
 * Since: 1.1.1
 */
void WebKitWebFrame::load_string(const char* content, const char* contentMimeType, const char* contentEncoding, const char* baseUri)
{
	load_data(frame, content, contentMimeType, contentEncoding, baseUri, 0);
}

/**
 * WebKitWebFrame::load_alternate_string:
 * @frame: a #WebKitWebFrame
 * @content: the alternate content to display as the main page of the @frame
 * @base_url: the base URI for relative locations
 * @unreachable_url: the URL for the alternate page content
 *
 * Request loading of an alternate content for a URL that is unreachable.
 * Using this method will preserve the back-forward list. The URI passed in
 * @base_url has to be an absolute URI.
 *
 * Since: 1.1.6
 */
void WebKitWebFrame::load_alternate_string(const char* content, const char* baseURL, const char* unreachableURL)
{
	load_data(frame, content, 0, 0, baseURL, unreachableURL);
}

/**
 * WebKitWebFrame::load_request:
 * @frame: a #WebKitWebFrame
 * @request: a #WebKitNetworkRequest
 *
 * Connects to a given URI by initiating an asynchronous client request.
 *
 * Creates a provisional data source that will transition to a committed data
 * source once any data has been received. Use WebKitWebFrame::stop_loading() to
 * stop the load. This function is typically invoked on the main frame.
 */
void WebKitWebFrame::load_request(WebKitNetworkRequest* request)
{
	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return;

	coreFrame->loader().load(FrameLoadRequest(coreFrame->document()->securityOrigin(), core(request)));
}

/**
 * WebKitWebFrame::stop_loading:
 * @frame: a #WebKitWebFrame
 *
 * Stops any pending loads on @frame's data source, and those of its children.
 */
void WebKitWebFrame::stop_loading()
{
	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return;

	coreFrame->loader().stopAllLoaders();
}

/**
 * WebKitWebFrame::reload:
 * @frame: a #WebKitWebFrame
 *
 * Reloads the initial request.
 */
void WebKitWebFrame::reload()
{
	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return;

	coreFrame->loader().reload();
}

/**
 * WebKitWebFrame::find_frame:
 * @frame: a #WebKitWebFrame
 * @name: the name of the frame to be found
 *
 * For pre-defined names, returns @frame if @name is "_self" or "_current",
 * returns @frame's parent frame if @name is "_parent", and returns the main
 * frame if @name is "_top". Also returns @frame if it is the main frame and
 * @name is either "_parent" or "_top". For other names, this function returns
 * the first frame that matches @name. This function searches @frame and its
 * descendents first, then @frame's parent and its children moving up the
 * hierarchy until a match is found. If no match is found in @frame's
 * hierarchy, this function will search for a matching frame in other main
 * frame hierarchies. Returns %NULL if no match is found.
 *
 * Return value: (transfer none): the found #WebKitWebFrame or %NULL in case none is found
 */
WebKitWebFrame* WebKitWebFrame::find_frame(const char* name)
{
	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return 0;

	String nameString = String::fromUTF8(name);
	return kit(coreFrame->tree().find(AtomicString(nameString)));
}

/**
 * WebKitWebFrame::get_global_context:
 * @frame: a #WebKitWebFrame
 *
 * Gets the global JavaScript execution context. Use this function to bridge
 * between the WebKit and JavaScriptCore APIs.
 *
 * Return value: (transfer none): the global JavaScript context #JSGlobalContextRef
 */
JSGlobalContextRef WebKitWebFrame::get_global_context()
{
	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return 0;

	return toGlobalRef(coreFrame->script().globalObject(mainThreadNormalWorld())->globalExec());
}

/**
 * WebKitWebFrame::get_data_source:
 * @frame: a #WebKitWebFrame
 *
 * Returns the committed data source.
 *
 * Return value: (transfer none): the committed #WebKitWebDataSource.
 *
 * Since: 1.1.14
 */
WebKitWebDataSource* WebKitWebFrame::get_data_source()
{
	Frame* coreFrame = core(frame);
	return get_data_source_from_core_loader(coreFrame->loader().documentLoader());
}

/**
 * WebKitWebFrame::get_provisional_data_source:
 * @frame: a #WebKitWebFrame
 *
 * You use the WebKitWebFrame::load_request method to initiate a request that
 * creates a provisional data source. The provisional data source will
 * transition to a committed data source once any data has been received. Use
 * WebKitWebFrame::get_data_source to get the committed data source.
 *
 * Return value: (transfer none): the provisional #WebKitWebDataSource or %NULL if a load
 * request is not in progress.
 *
 * Since: 1.1.14
 */
WebKitWebDataSource* WebKitWebFrame::get_provisional_data_source()
{
	Frame* coreFrame = core(frame);
	return get_data_source_from_core_loader(coreFrame->loader().provisionalDocumentLoader());
}

/**
 * WebKitWebFrame::print:
 * @frame: a #WebKitWebFrame
 *
 * Prints the given #WebKitWebFrame, by presenting a print dialog to the
 * user. If you need more control over the printing process, see
 * WebKitWebFrame::print_full().
 *
 * Since: 1.1.5
 */
void WebKitWebFrame::print()
{
}

char* WebKitWebFrame::get_response_mime_type()
{
	Frame* coreFrame = core(frame);
	WebCore::DocumentLoader* docLoader = coreFrame->loader().documentLoader();
	String mimeType = docLoader->responseMIMEType();
	return strdup(mimeType.utf8().data());
}

/**
 * WebKitWebFrame::get_load_status:
 * @frame: a #WebKitWebView
 *
 * Determines the current status of the load.
 *
 * Returns: The #WebKitLoadStatus specifying the status of the current load.
 *
 * Since: 1.1.7
 */
WebKitLoadStatus WebKitWebFrame::get_load_status()
{
	return loadStatus;
}

FLTKPolicyType WebKitWebFrame::get_horizontal_scrollbar_policy()
{
	Frame* coreFrame = core(frame);
	FrameView* view = coreFrame->view();
	if (!view)
		return GTK_POLICY_AUTOMATIC;

	ScrollbarMode hMode = view->horizontalScrollbarMode();

	if (hMode == ScrollbarAlwaysOn)
		return GTK_POLICY_ALWAYS;

	if (hMode == ScrollbarAlwaysOff)
		return GTK_POLICY_NEVER;

	return GTK_POLICY_AUTOMATIC;
}

FLTKPolicyType WebKitWebFrame::get_vertical_scrollbar_policy()
{
	Frame* coreFrame = core(frame);
	FrameView* view = coreFrame->view();
	if (!view)
		return GTK_POLICY_AUTOMATIC;

	ScrollbarMode vMode = view->verticalScrollbarMode();

	if (vMode == ScrollbarAlwaysOn)
		return GTK_POLICY_ALWAYS;

	if (vMode == ScrollbarAlwaysOff)
		return GTK_POLICY_NEVER;

	return GTK_POLICY_AUTOMATIC;
}

/**
 * WebKitWebFrame::get_security_origin:
 * @frame: a #WebKitWebFrame
 *
 * Returns the @frame's security origin.
 *
 * Return value: (transfer none): the security origin of @frame
 *
 * Since: 1.1.14
 */
WebKitSecurityOrigin* WebKitWebFrame::get_security_origin()
{
	if (!coreFrame || !coreFrame->document() || !coreFrame->document()->securityOrigin())
		return 0;

	if (origin && origin->coreOrigin.get() == coreFrame->document()->securityOrigin())
		return origin;

	origin = kit(coreFrame->document()->securityOrigin());
	return origin;
}

/**
 * WebKitWebFrame::get_network_response:
 * @frame: a #WebKitWebFrame
 *
 * Returns a #WebKitNetworkResponse object representing the response
 * that was given to the request for the given frame, or NULL if the
 * frame was not created by a load. You must unref the object when you
 * are done with it.
 *
 * Return value: (transfer full): a #WebKitNetworkResponse object
 *
 * Since: 1.1.18
 */
WebKitNetworkResponse* WebKitWebFrame::get_network_response()
{
	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return 0;

	WebCore::DocumentLoader* loader = coreFrame->loader().activeDocumentLoader();
	if (!loader)
		return 0;

	return kitNew(loader->response());
}

/**
 * WebKitWebFrame::replace_selection:
 * @frame: a #WebKitWeFrame
 * @text: the text to insert in place of the current selection
 *
 * Replaces the current selection in @frame, if any, with @text.
 *
 * Since: 1.5.1
 **/
void WebKitWebFrame::replace_selection(, const char* text)
{
	Frame* coreFrame = core(frame);
	bool selectReplacement = false;
	bool smartReplace = true;
	coreFrame->editor().replaceSelectionWithText(text, selectReplacement, smartReplace);
}

/**
 * WebKitWebFrame::get_range_for_word_around_caret:
 * @frame: a #WebKitWebFrame
 *
 * Returns a #WebKitDOMRange for the word where the caret is currently
 * positioned.
 *
 * Returns: (transfer none): a #WebKitDOMRange spanning the word where the caret
 * currently is positioned. If there is no caret %NULL will be
 * returned.
 *
 * Since: 1.5.1.
 **/
WebKitDOMRange* WebKitWebFrame::get_range_for_word_around_caret()
{
	Frame* coreFrame = core(frame);
	FrameSelection& selection = coreFrame->selection();
	if (selection.isNone() || selection.isRange())
		return 0;
	VisibleSelection visibleSelection(selection.selection().visibleStart());
	visibleSelection.expandUsingGranularity(WordGranularity);

	return kit(visibleSelection.firstRange().get());
}

/**
 * WebKitWebFrame::get_dom_document:
 * @frame: a #WebKitWebFrame
 *
 * Returns: (transfer none): the #WebKitDOMDocument currently loaded
 * in the @frame or %NULL if no document is loaded
 *
 * Since: 1.10
 **/
WebKitDOMDocument* WebKitWebFrame::get_dom_document()
{
	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return 0;

	Document* doc = coreFrame->document();
	if (!doc)
		return 0;

	return kit(doc);
}

namespace WebKit {

WebKitWebView* getViewFromFrame(WebKitWebFrame* frame)
{
	return frame->webView;
}

WebCore::Frame* core(WebKitWebFrame* frame)
{
	if (!frame)
		return 0;

	return frame->coreFrame;
}

WebKitWebFrame* kit(WebCore::Frame* coreFrame)
{
	if (!coreFrame)
		return 0;

	return static_cast<WebKit::FrameLoaderClient&>(coreFrame->loader().client()).webFrame();
}

}
