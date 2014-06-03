/*
 *	Copyright (C) Research In Motion Limited 2010. All rights reserved.
 *	Copyright (C) 2010 Joone Hur <joone@kldp.org>
 *	Copyright (C) 2009 Google Inc. All rights reserved.
 *	Copyright (C) 2011 Igalia S.L.
 *	Copyright (C) 2012, 2013 Apple Inc. All Rights Reserved.
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
#include "DumpRenderTreeSupportFLTK.h"

#include "APICast.h"
#include "AXObjectCache.h"
#include "AccessibilityObject.h"
#include "AnimationController.h"
#include "ApplicationCacheStorage.h"
#include "Chrome.h"
#include "ChromeClientFLTK.h"
#include "DOMWrapperWorld.h"
#include "Document.h"
#include "Editor.h"
#include "EditorClientFLTK.h"
#include "Element.h"
#include "FocusController.h"
#include "FrameTree.h"
#include "FrameView.h"
#include "GCController.h"
#include "GeolocationClientMock.h"
#include "GeolocationController.h"
#include "GeolocationError.h"
#include "GeolocationPosition.h"
#include "GraphicsContext.h"
#include "HTMLInputElement.h"
#include "JSCJSValue.h"
#include "JSCSSStyleDeclaration.h"
#include "JSDOMWindow.h"
#include "JSDocument.h"
#include "JSElement.h"
#include "JSLock.h"
#include "JSNodeList.h"
#include "MemoryCache.h"
#include "MutationObserver.h"
#include "NodeList.h"
#include "PageGroup.h"
#include "PrintContext.h"
#include "RenderListItem.h"
#include "RenderTreeAsText.h"
#include "RenderView.h"
#include "ResourceLoadScheduler.h"
#include "RuntimeEnabledFeatures.h"
#include "SchemeRegistry.h"
#include "SecurityOrigin.h"
#include "SecurityPolicy.h"
#include "Settings.h"
#include "TextIterator.h"
#include "webkitglobalsprivate.h"
#include "webkitwebframe.h"
#include "webkitwebframeprivate.h"
#include "webkitwebview.h"
#include "webkitwebviewprivate.h"
#include <JavaScriptCore/APICast.h>
#include <wtf/text/WTFString.h>

using namespace JSC;
using namespace WebCore;
using namespace WebKit;

bool DumpRenderTreeSupportFLTK::s_drtRun = false;
bool DumpRenderTreeSupportFLTK::s_linksIncludedInTabChain = true;
DumpRenderTreeSupportFLTK::FrameLoadEventCallback DumpRenderTreeSupportFLTK::s_frameLoadEventCallback = 0;
DumpRenderTreeSupportFLTK::AuthenticationCallback DumpRenderTreeSupportFLTK::s_authenticationCallback = 0;

DumpRenderTreeSupportFLTK::DumpRenderTreeSupportFLTK()
{
}

DumpRenderTreeSupportFLTK::~DumpRenderTreeSupportFLTK()
{
}

void DumpRenderTreeSupportFLTK::setDumpRenderTreeModeEnabled(bool enabled)
{
	s_drtRun = enabled;
}

bool DumpRenderTreeSupportFLTK::dumpRenderTreeModeEnabled()
{
	return s_drtRun;
}
void DumpRenderTreeSupportFLTK::setLinksIncludedInFocusChain(bool enabled)
{
	s_linksIncludedInTabChain = enabled;
}

bool DumpRenderTreeSupportFLTK::linksIncludedInFocusChain()
{
	return s_linksIncludedInTabChain;
}

/**
 * getFrameChildren:
 * @frame: a #WebKitWebFrame
 *
 * Return value: child frames of @frame
 */
GSList* DumpRenderTreeSupportFLTK::getFrameChildren(WebKitWebFrame* frame)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_FRAME(frame), 0);

	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return 0;

	GSList* children = 0;
	for (Frame* child = coreFrame->tree().firstChild(); child; child = child->tree().nextSibling()) {
		WebKitWebFrame* kitFrame = kit(child);
		if (kitFrame)
		  children = g_slist_append(children, kitFrame);
	}

	return children;
}

/**
 * getInnerText:
 * @frame: a #WebKitWebFrame
 *
 * Return value: inner text of @frame
 */
CString DumpRenderTreeSupportFLTK::getInnerText(WebKitWebFrame* frame)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_FRAME(frame), CString(""));

	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return CString("");

	FrameView* view = coreFrame->view();
	if (view && view->layoutPending())
		view->layout();

	Element* documentElement = coreFrame->document()->documentElement();
	if (!documentElement)
		return CString("");
	return documentElement->innerText().utf8();
}

/**
 * dumpRenderTree:
 * @frame: a #WebKitWebFrame
 *
 * Return value: Non-recursive render tree dump of @frame
 */
CString DumpRenderTreeSupportFLTK::dumpRenderTree(WebKitWebFrame* frame)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_FRAME(frame), CString(""));

	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return CString("");

	FrameView* view = coreFrame->view();

	if (view && view->layoutPending())
		view->layout();

	return externalRepresentation(coreFrame).utf8();
}

void DumpRenderTreeSupportFLTK::addUserScript(WebKitWebFrame* frame, const char* sourceCode, bool runAtStart, bool allFrames)
{
	g_return_if_fail(WEBKIT_IS_WEB_FRAME(frame));

	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return;

	WebKitWebView* webView = getViewFromFrame(frame);
	Page* page = core(webView);
	page->group().addUserScriptToWorld(mainThreadNormalWorld(), sourceCode, URL(), Vector<String>(), Vector<String>(),
		runAtStart ? InjectAtDocumentStart : InjectAtDocumentEnd, allFrames ? InjectInAllFrames : InjectInTopFrameOnly);
}

/**
 * addUserStyleSheet
 * @frame: a #WebKitWebFrame
 * @sourceCode: code of a user stylesheet
 *
 */
void DumpRenderTreeSupportFLTK::addUserStyleSheet(WebKitWebFrame* frame, const char* sourceCode, bool allFrames)
{
	g_return_if_fail(WEBKIT_IS_WEB_FRAME(frame));

	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return;

	WebKitWebView* webView = getViewFromFrame(frame);
	Page* page = core(webView);
	page->group().addUserStyleSheetToWorld(mainThreadNormalWorld(), sourceCode, URL(), Vector<String>(), Vector<String>(), allFrames ? InjectInAllFrames : InjectInTopFrameOnly); 
}

void DumpRenderTreeSupportFLTK::clearMainFrameName(WebKitWebFrame* frame)
{
	g_return_if_fail(WEBKIT_IS_WEB_FRAME(frame));

	core(frame)->tree().clearName();
}

void DumpRenderTreeSupportFLTK::executeCoreCommandByName(WebKitWebView* webView, const gchar* name, const gchar* value)
{
	g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));
	g_return_if_fail(name);
	g_return_if_fail(value);

	core(webView)->focusController().focusedOrMainFrame().editor().command(name).execute(value);
}

bool DumpRenderTreeSupportFLTK::isCommandEnabled(WebKitWebView* webView, const gchar* name)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);
	g_return_val_if_fail(name, FALSE);

	return core(webView)->focusController().focusedOrMainFrame().editor().command(name).isEnabled();
}

void DumpRenderTreeSupportFLTK::setComposition(WebKitWebView* webView, const char* text, int start, int length)
{
	g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));
	g_return_if_fail(text);

	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	Editor& editor = frame.editor();
	if (!editor.canEdit() && !editor.hasComposition())
		return;

	String compositionString = String::fromUTF8(text);
	Vector<CompositionUnderline> underlines;
	underlines.append(CompositionUnderline(0, compositionString.length(), Color(0, 0, 0), false));
	editor.setComposition(compositionString, underlines, start, start + length);
}

bool DumpRenderTreeSupportFLTK::hasComposition(WebKitWebView* webView)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), false);
	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	return frame.editor().hasComposition();
}

bool DumpRenderTreeSupportFLTK::compositionRange(WebKitWebView* webView, int* start, int* length)
{
	g_return_val_if_fail(start && length, false);
	*start = *length = 0;

	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), false);
	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	Editor& editor = frame.editor();
	if (!editor.hasComposition())
		return false;

	*start = editor.compositionStart();
	*length = editor.compositionEnd() - *start;
	return true;
}

void DumpRenderTreeSupportFLTK::confirmComposition(WebKitWebView* webView, const char* text)
{
	g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));

	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	Editor& editor = frame.editor();

	if (!editor.hasComposition()) {
		editor.insertText(String::fromUTF8(text), 0);
		return;
	}
	if (text) {
		editor.confirmComposition(String::fromUTF8(text));
		return;
	}
	editor.confirmComposition();
}

void DumpRenderTreeSupportFLTK::doCommand(WebKitWebView* webView, const char* command)
{
	g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));
	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	Editor& editor = frame.editor();

	String commandString(command);
	// Remove ending : here.
	if (commandString.endsWith(":", true))
		commandString = commandString.left(commandString.length() - 1);

	// Make the first char in upper case.
	String firstChar = commandString.left(1);
	commandString = commandString.right(commandString.length() - 1);
	commandString.insert(firstChar.upper(), 0);

	editor.command(commandString).execute();
}

bool DumpRenderTreeSupportFLTK::firstRectForCharacterRange(WebKitWebView* webView, int location, int length, cairo_rectangle_int_t* rect)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), false);
	g_return_val_if_fail(rect, false);

	if ((location + length < location) && (location + length))
		length = 0;

	Frame& frame = core(webView)->focusController().focusedOrMainFrame();
	Editor& editor = frame.editor();

	RefPtr<Range> range = TextIterator::rangeFromLocationAndLength(frame.selection().rootEditableElementOrDocumentElement(), location, length);
	if (!range)
		return false;

	*rect = editor.firstRectForRange(range.get());
	return true;
}

bool DumpRenderTreeSupportFLTK::selectedRange(WebKitWebView* webView, int* start, int* length)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), false);
	g_return_val_if_fail(start && length, false);

	Frame& frame = core(webView)->focusController().focusedOrMainFrame();

	RefPtr<Range> range = frame.selection().toNormalizedRange().get();
	if (!range)
		return false;

	Element* selectionRoot = frame.selection().rootEditableElement();
	Element* scope = selectionRoot ? selectionRoot : frame.document()->documentElement();

	RefPtr<Range> testRange = Range::create(scope->document(), scope, 0, range->startContainer(), range->startOffset());
	ASSERT(testRange->startContainer() == scope);
	*start = TextIterator::rangeLength(testRange.get());

	ExceptionCode ec;
	testRange->setEnd(range->endContainer(), range->endOffset(), ec);
	ASSERT(testRange->startContainer() == scope);
	*length = TextIterator::rangeLength(testRange.get());

	return true;
}

void DumpRenderTreeSupportFLTK::setDefersLoading(WebKitWebView* webView, bool defers)
{
	core(webView)->setDefersLoading(defers);
}

void DumpRenderTreeSupportFLTK::forceWebViewPaint(WebKitWebView* webView)
{
	g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));

	static_cast<WebKit::ChromeClient&>(core(webView)->chrome().client()).forcePaint();
}

void DumpRenderTreeSupportFLTK::whiteListAccessFromOrigin(const gchar* sourceOrigin, const gchar* destinationProtocol, const gchar* destinationHost, bool allowDestinationSubdomains)
{
	SecurityPolicy::addOriginAccessWhitelistEntry(*SecurityOrigin::createFromString(sourceOrigin), destinationProtocol, destinationHost, allowDestinationSubdomains);
}

void DumpRenderTreeSupportFLTK::removeWhiteListAccessFromOrigin(const char* sourceOrigin, const char* destinationProtocol, const char* destinationHost, bool allowDestinationSubdomains)
{
	SecurityPolicy::removeOriginAccessWhitelistEntry(*SecurityOrigin::createFromString(sourceOrigin), destinationProtocol, destinationHost, allowDestinationSubdomains);
}

void DumpRenderTreeSupportFLTK::resetOriginAccessWhiteLists()
{
	SecurityPolicy::resetOriginAccessWhitelists();
}

void DumpRenderTreeSupportFLTK::gcCollectJavascriptObjects()
{
	gcController().garbageCollectNow();
}

void DumpRenderTreeSupportFLTK::gcCollectJavascriptObjectsOnAlternateThread(bool waitUntilDone)
{
	gcController().garbageCollectOnAlternateThreadForDebugging(waitUntilDone);
}

unsigned long DumpRenderTreeSupportFLTK::gcCountJavascriptObjects()
{
	JSC::JSLockHolder lock(JSDOMWindow::commonVM());
	return JSDOMWindow::commonVM()->heap.objectCount();
}

void DumpRenderTreeSupportFLTK::layoutFrame(WebKitWebFrame* frame)
{
	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return;

	FrameView* view = coreFrame->view();
	if (!view)
		return;

	view->layout();
}

void DumpRenderTreeSupportFLTK::clearOpener(WebKitWebFrame* frame)
{
	Frame* coreFrame = core(frame);
	if (coreFrame)
		coreFrame->loader().setOpener(0);
}

bool DumpRenderTreeSupportFLTK::findString(WebKitWebView* webView, const gchar* targetString, WebKitFindOptions findOptions)
{
	return core(webView)->findString(String::fromUTF8(targetString), findOptions);
}

void DumpRenderTreeSupportFLTK::setValueForUser(JSContextRef context, JSValueRef nodeObject, JSStringRef value)
{
	JSC::ExecState* exec = toJS(context);
	Element* element = toElement(toJS(exec, nodeObject));
	if (!element)
		return;
	HTMLInputElement* inputElement = element->toInputElement();
	if (!inputElement)
		return;

	size_t bufferSize = JSStringGetMaximumUTF8CStringSize(value);
	GUniquePtr<gchar> valueBuffer(static_cast<gchar*>(g_malloc(bufferSize)));
	JSStringGetUTF8CString(value, valueBuffer.get(), bufferSize);
	inputElement->setValueForUser(String::fromUTF8(valueBuffer.get()));
}

void DumpRenderTreeSupportFLTK::rectangleForSelection(WebKitWebFrame* frame, cairo_rectangle_int_t* rectangle)
{
	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return;

	IntRect bounds = enclosingIntRect(coreFrame->selection().bounds());
	rectangle->x = bounds.x();
	rectangle->y = bounds.y();
	rectangle->width = bounds.width();
	rectangle->height = bounds.height();
}

bool DumpRenderTreeSupportFLTK::shouldClose(WebKitWebFrame* frame)
{
	Frame* coreFrame = core(frame);
	if (!coreFrame)
		return true;
	return coreFrame->loader().shouldClose();
}

void DumpRenderTreeSupportFLTK::scalePageBy(WebKitWebView* webView, float scaleFactor, float x, float y)
{
	core(webView)->setPageScaleFactor(scaleFactor, IntPoint(x, y));
}

void DumpRenderTreeSupportFLTK::resetGeolocationClientMock(WebKitWebView* webView)
{
#if ENABLE(GEOLOCATION)
	GeolocationClientMock* mock = static_cast<GeolocationClientMock*>(GeolocationController::from(core(webView))->client());
	mock->reset();
#endif
}

void DumpRenderTreeSupportFLTK::setMockGeolocationPermission(WebKitWebView* webView, bool allowed)
{
#if ENABLE(GEOLOCATION)
	GeolocationClientMock* mock = static_cast<GeolocationClientMock*>(GeolocationController::from(core(webView))->client());
	mock->setPermission(allowed);
#endif
}

void DumpRenderTreeSupportFLTK::setMockGeolocationPosition(WebKitWebView* webView, double latitude, double longitude, double accuracy)
{
#if ENABLE(GEOLOCATION)
	GeolocationClientMock* mock = static_cast<GeolocationClientMock*>(GeolocationController::from(core(webView))->client());

	double timestamp = g_get_real_time() / 1000000.0;
	mock->setPosition(GeolocationPosition::create(timestamp, latitude, longitude, accuracy));
#endif
}

void DumpRenderTreeSupportFLTK::setMockGeolocationPositionUnavailableError(WebKitWebView* webView, const gchar* errorMessage)
{
#if ENABLE(GEOLOCATION)
	GeolocationClientMock* mock = static_cast<GeolocationClientMock*>(GeolocationController::from(core(webView))->client());
	mock->setPositionUnavailableError(errorMessage);
#endif
}

int DumpRenderTreeSupportFLTK::numberOfPendingGeolocationPermissionRequests(WebKitWebView* webView)
{
#if ENABLE(GEOLOCATION)
	GeolocationClientMock* mock = static_cast<GeolocationClientMock*>(GeolocationController::from(core(webView))->client());
	return mock->numberOfPendingPermissionRequests();
#else
	return 0;
#endif
}

void DumpRenderTreeSupportFLTK::setPageCacheSupportsPlugins(WebKitWebView* webView, bool enabled)
{
	core(webView)->settings().setPageCacheSupportsPlugins(enabled);
}

void DumpRenderTreeSupportFLTK::setCSSGridLayoutEnabled(WebKitWebView* webView, bool enabled)
{
	core(webView)->settings().setCSSGridLayoutEnabled(enabled);
}

void DumpRenderTreeSupportFLTK::setCSSRegionsEnabled(WebKitWebView* webView, bool enabled)
{
	RuntimeEnabledFeatures::sharedFeatures().setCSSRegionsEnabled(enabled);
}

void DumpRenderTreeSupportFLTK::setExperimentalContentSecurityPolicyFeaturesEnabled(bool enabled)
{
#if ENABLE(CSP_NEXT)
	RuntimeEnabledFeatures::sharedFeatures().setExperimentalContentSecurityPolicyFeaturesEnabled(enabled);
#endif
}

void DumpRenderTreeSupportFLTK::setSeamlessIFramesEnabled(bool enabled)
{
#if ENABLE(IFRAME_SEAMLESS)
	RuntimeEnabledFeatures::sharedFeatures().setSeamlessIFramesEnabled(enabled);
#endif
}

void DumpRenderTreeSupportFLTK::setShadowDOMEnabled(bool enabled)
{
#if ENABLE(SHADOW_DOM)
	RuntimeEnabledFeatures::sharedFeatures().setShadowDOMEnabled(enabled);
#endif
}

void DumpRenderTreeSupportFLTK::deliverAllMutationsIfNecessary()
{
	MutationObserver::deliverAllMutations();
}

void DumpRenderTreeSupportFLTK::setDomainRelaxationForbiddenForURLScheme(bool forbidden, const char* urlScheme)
{
	SchemeRegistry::setDomainRelaxationForbiddenForURLScheme(forbidden, String::fromUTF8(urlScheme));
}

void DumpRenderTreeSupportFLTK::setSerializeHTTPLoads(bool enabled)
{
	resourceLoadScheduler()->setSerialLoadingEnabled(enabled);
}

void DumpRenderTreeSupportFLTK::setTracksRepaints(WebKitWebFrame* frame, bool tracks)
{
	g_return_if_fail(WEBKIT_IS_WEB_FRAME(frame));

	Frame* coreFrame = core(frame);
	if (coreFrame && coreFrame->view())
		coreFrame->view()->setTracksRepaints(tracks);
}

bool DumpRenderTreeSupportFLTK::isTrackingRepaints(WebKitWebFrame* frame)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_FRAME(frame), false);

	Frame* coreFrame = core(frame);
	if (coreFrame && coreFrame->view())
		return coreFrame->view()->isTrackingRepaints();

	return false;
}

GSList* DumpRenderTreeSupportFLTK::trackedRepaintRects(WebKitWebFrame* frame)
{
	g_return_val_if_fail(WEBKIT_IS_WEB_FRAME(frame), 0);

	Frame* coreFrame = core(frame);
	if (!coreFrame || !coreFrame->view())
		return 0;

	GSList* rects = 0;
	const Vector<IntRect>& repaintRects = coreFrame->view()->trackedRepaintRects();
	for (unsigned i = 0; i < repaintRects.size(); i++) {
		GdkRectangle* rect = g_new0(GdkRectangle, 1);
		rect->x = repaintRects[i].x();
		rect->y = repaintRects[i].y();
		rect->width = repaintRects[i].width();
		rect->height = repaintRects[i].height();
		rects = g_slist_append(rects, rect);
	}

	return rects;
}

void DumpRenderTreeSupportFLTK::resetTrackedRepaints(WebKitWebFrame* frame)
{
	g_return_if_fail(WEBKIT_IS_WEB_FRAME(frame));

	Frame* coreFrame = core(frame);
	if (coreFrame && coreFrame->view())
		coreFrame->view()->resetTrackedRepaints();
}

void DumpRenderTreeSupportFLTK::clearMemoryCache()
{
	memoryCache()->evictResources();
}

void DumpRenderTreeSupportFLTK::clearApplicationCache()
{
	cacheStorage().empty();
	cacheStorage().vacuumDatabaseFile();
}

void DumpRenderTreeSupportFLTK::setFrameLoadEventCallback(FrameLoadEventCallback frameLoadEventCallback)
{
	s_frameLoadEventCallback = frameLoadEventCallback;
}

void DumpRenderTreeSupportFLTK::setAuthenticationCallback(AuthenticationCallback authenticationCallback)
{
	s_authenticationCallback = authenticationCallback;
}

void DumpRenderTreeSupportFLTK::setPageVisibility(WebKitWebView* webView, WebCore::PageVisibilityState visibilityState, bool isInitialState)
{
#if ENABLE(PAGE_VISIBILITY_API)
	Page* page = core(webView);
	if (!page)
		return;

	page->setIsVisible(visibilityState == PageVisibilityStateVisible, isInitialState);
	if (visibilityState == PageVisibilityStatePrerender)
		page->setIsPrerender();
#endif
}
