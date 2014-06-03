/*
WebkitFLTK
Copyright (C) 2014 Lauri Kasanen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef frameclient_h
#define frameclient_h

#include <FrameLoaderClient.h>
#include "webview.h"

class FlFrameLoaderClient: public WebCore::FrameLoaderClient {
public:
	FlFrameLoaderClient(webview *, WebCore::Frame *inframe = NULL);

	bool hasHTMLView() const override;
	void frameLoaderDestroyed() override;
	bool hasWebView() const override; // mainly for assertions
	void makeRepresentation(WebCore::DocumentLoader*) override;
	void forceLayoutForNonHTML() override;

	void setCopiesOnScroll() override;

	void detachedFromParent2() override;
	void detachedFromParent3() override;

	void assignIdentifierToInitialRequest(unsigned long identifier, WebCore::DocumentLoader*, const WebCore::ResourceRequest&) override;

	void dispatchWillSendRequest(WebCore::DocumentLoader*, unsigned long identifier, WebCore::ResourceRequest&, const WebCore::ResourceResponse& redirectResponse) override;
	bool shouldUseCredentialStorage(WebCore::DocumentLoader*, unsigned long identifier) override;
	void dispatchDidReceiveAuthenticationChallenge(WebCore::DocumentLoader*, unsigned long identifier, const WebCore::AuthenticationChallenge&) override;
	void dispatchDidCancelAuthenticationChallenge(WebCore::DocumentLoader*, unsigned long identifier, const WebCore::AuthenticationChallenge&) override;

	void dispatchDidReceiveResponse(WebCore::DocumentLoader*, unsigned long identifier, const WebCore::ResourceResponse&) override;
	void dispatchDidReceiveContentLength(WebCore::DocumentLoader*, unsigned long identifier, int dataLength) override;
	void dispatchDidFinishLoading(WebCore::DocumentLoader*, unsigned long identifier) override;
	void dispatchDidFailLoading(WebCore::DocumentLoader*, unsigned long identifier, const WebCore::ResourceError&) override;
	bool dispatchDidLoadResourceFromMemoryCache(WebCore::DocumentLoader*, const WebCore::ResourceRequest&, const WebCore::ResourceResponse&, int length) override;

	void dispatchDidHandleOnloadEvents() override;
	void dispatchDidReceiveServerRedirectForProvisionalLoad() override;
	void dispatchDidCancelClientRedirect() override;
	void dispatchWillPerformClientRedirect(const WebCore::URL&, double interval, double fireDate) override;
	void dispatchDidChangeLocationWithinPage() override;
	void dispatchDidPushStateWithinPage() override;
	void dispatchDidReplaceStateWithinPage() override;
	void dispatchDidPopStateWithinPage() override;
	void dispatchWillClose() override;
	void dispatchDidReceiveIcon() override;
	void dispatchDidStartProvisionalLoad() override;
	void dispatchDidReceiveTitle(const WebCore::StringWithDirection&) override;
	void dispatchDidChangeIcons(WebCore::IconType) override;
	void dispatchDidCommitLoad() override;
	void dispatchDidFailProvisionalLoad(const WebCore::ResourceError&) override;
	void dispatchDidFailLoad(const WebCore::ResourceError&) override;
	void dispatchDidFinishDocumentLoad() override;
	void dispatchDidFinishLoad() override;

	WebCore::Frame* dispatchCreatePage(const WebCore::NavigationAction&) override;
	void dispatchShow() override;

	void dispatchDecidePolicyForResponse(const WebCore::ResourceResponse&, const WebCore::ResourceRequest&, WebCore::FramePolicyFunction) override;
	void dispatchDecidePolicyForNewWindowAction(const WebCore::NavigationAction&, const WebCore::ResourceRequest&, PassRefPtr<WebCore::FormState>, const WTF::String& frameName, WebCore::FramePolicyFunction) override;
	void dispatchDecidePolicyForNavigationAction(const WebCore::NavigationAction&, const WebCore::ResourceRequest&, PassRefPtr<WebCore::FormState>, WebCore::FramePolicyFunction) override;
	void cancelPolicyCheck() override;

	void dispatchUnableToImplementPolicy(const WebCore::ResourceError&) override;

	void dispatchWillSendSubmitEvent(PassRefPtr<WebCore::FormState>) override;
	void dispatchWillSubmitForm(PassRefPtr<WebCore::FormState>, WebCore::FramePolicyFunction) override;

	void revertToProvisionalState(WebCore::DocumentLoader*) override;
	void setMainDocumentError(WebCore::DocumentLoader*, const WebCore::ResourceError&) override;

	void setMainFrameDocumentReady(bool) override;

	void startDownload(const WebCore::ResourceRequest&, const WTF::String& suggestedName = WTF::String()) override;

	void willChangeTitle(WebCore::DocumentLoader*) override;
	void didChangeTitle(WebCore::DocumentLoader*) override;

	void committedLoad(WebCore::DocumentLoader*, const char*, int) override;
	void finishedLoading(WebCore::DocumentLoader*) override;

	void updateGlobalHistory() override;
	void updateGlobalHistoryRedirectLinks() override;

	bool shouldGoToHistoryItem(WebCore::HistoryItem*) const override;

	// This frame has displayed inactive content (such as an image) from an
	// insecure source.  Inactive content cannot spread to other frames.
	void didDisplayInsecureContent() override;

	// The indicated security origin has run active content (such as a
	// script) from an insecure source.  Note that the insecure content can
	// spread to other frames in the same origin.
	void didRunInsecureContent(WebCore::SecurityOrigin*, const WebCore::URL&) override;
	void didDetectXSS(const WebCore::URL&, bool didBlockEntirePage) override;

	WebCore::ResourceError cancelledError(const WebCore::ResourceRequest&) override;
	WebCore::ResourceError blockedError(const WebCore::ResourceRequest&) override;
	WebCore::ResourceError cannotShowURLError(const WebCore::ResourceRequest&) override;
	WebCore::ResourceError interruptedForPolicyChangeError(const WebCore::ResourceRequest&) override;

	WebCore::ResourceError cannotShowMIMETypeError(const WebCore::ResourceResponse&) override;
	WebCore::ResourceError fileDoesNotExistError(const WebCore::ResourceResponse&) override;
	WebCore::ResourceError pluginWillHandleLoadError(const WebCore::ResourceResponse&) override;

	bool shouldFallBack(const WebCore::ResourceError&) override;

	bool canHandleRequest(const WebCore::ResourceRequest&) const override;
	bool canShowMIMEType(const WTF::String& MIMEType) const override;
	bool canShowMIMETypeAsHTML(const WTF::String& MIMEType) const override;
	bool representationExistsForURLScheme(const WTF::String& URLScheme) const override;
	WTF::String generatedMIMETypeForURLScheme(const WTF::String& URLScheme) const override;

	void frameLoadCompleted() override;
	void saveViewStateToItem(WebCore::HistoryItem*) override;
	void restoreViewState() override;
	void provisionalLoadStarted() override;
	void didFinishLoad() override;
	void prepareForDataSourceReplacement() override;

	PassRefPtr<WebCore::DocumentLoader> createDocumentLoader(const WebCore::ResourceRequest&, const WebCore::SubstituteData&) override;
	void setTitle(const WebCore::StringWithDirection&, const WebCore::URL&) override;

	WTF::String userAgent(const WebCore::URL&) override;
	void savePlatformDataToCachedFrame(WebCore::CachedFrame*) override;
	void transitionToCommittedFromCachedFrame(WebCore::CachedFrame*) override;
	void transitionToCommittedForNewPage() override;

	void didSaveToPageCache() override;
	void didRestoreFromPageCache() override;

	void dispatchDidBecomeFrameset(bool) override; // Can change due to navigation or DOM modification.

	bool canCachePage() const override;
	void convertMainResourceLoadToDownload(WebCore::DocumentLoader*, const WebCore::ResourceRequest&, const WebCore::ResourceResponse&) override;

	PassRefPtr<WebCore::Frame> createFrame(const WebCore::URL& url, const WTF::String& name, WebCore::HTMLFrameOwnerElement* ownerElement, const WTF::String& referrer, bool allowsScrolling, int marginWidth, int marginHeight) override;
	PassRefPtr<WebCore::Widget> createPlugin(const WebCore::IntSize&, WebCore::HTMLPlugInElement*, const WebCore::URL&, const Vector<WTF::String>&, const Vector<WTF::String>&, const WTF::String&, bool loadManually) override;
	void recreatePlugin(WebCore::Widget*) override;
	void redirectDataToPlugin(WebCore::Widget* pluginWidget) override;

	PassRefPtr<WebCore::Widget> createJavaAppletWidget(const WebCore::IntSize&, WebCore::HTMLAppletElement*, const WebCore::URL& baseURL, const Vector<WTF::String>& paramNames, const Vector<WTF::String>& paramValues) override;

	WebCore::ObjectContentType objectContentType(const WebCore::URL&, const WTF::String& mimeType, bool shouldPreferPlugInsForImages) override;
	WTF::String overrideMediaType() const override;

	void dispatchDidClearWindowObjectInWorld(WebCore::DOMWrapperWorld&) override;

	void registerForIconNotification(bool listen = true) override;

	PassRefPtr<WebCore::FrameNetworkingContext> createNetworkingContext() override;

#if ENABLE(MEDIA_STREAM)
	void dispatchWillStartUsingPeerConnectionHandler(WebCore::RTCPeerConnectionHandler*) { }
#endif

	void setFrame(WebCore::Frame *inframe);
	WebCore::Frame *getFrame() const;
private:
	webview *view;
	WebCore::Frame *frame;
};

#endif
