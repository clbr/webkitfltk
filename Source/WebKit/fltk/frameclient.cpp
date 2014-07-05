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

#include "config.h"
#include "frameclient.h"
#include "webviewpriv.h"
#include "dirlisting.h"

#include <wtf/text/CString.h>
#include <DocumentLoader.h>
#include <FrameNetworkingContext.h>
#include <ErrorsFLTK.h>
#include <MainFrame.h>
#include <MIMETypeRegistry.h>
#include <NotImplemented.h>
#include <ResourceError.h>
#include <ResourceLoader.h>
#include <ResourceRequest.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace WebCore;
using namespace WTF;

extern const char * (*uafunc)(const char *);
extern int (*urlblockfunc)(const char *);
extern const char * (*aboutpagefunc)(const char*);

FlFrameLoaderClient::FlFrameLoaderClient(webview *inview, Frame *inframe) {
	view = inview;
	frame = inframe ? inframe : &inview->priv->page->mainFrame();
}

bool FlFrameLoaderClient::hasHTMLView() const {
	return true;
}

void FlFrameLoaderClient::frameLoaderDestroyed() {
	delete this;
}

bool FlFrameLoaderClient::hasWebView() const {
	return true;
}

void FlFrameLoaderClient::makeRepresentation(DocumentLoader*) {
	notImplemented();
}

void FlFrameLoaderClient::forceLayoutForNonHTML() {
	notImplemented();
}

void FlFrameLoaderClient::setCopiesOnScroll() {
	notImplemented();
}

void FlFrameLoaderClient::detachedFromParent2() {
	notImplemented();
}

void FlFrameLoaderClient::detachedFromParent3() {
	notImplemented();
}

void FlFrameLoaderClient::assignIdentifierToInitialRequest(unsigned long identifier, DocumentLoader*, const ResourceRequest&) {
	notImplemented();
}

void FlFrameLoaderClient::dispatchWillSendRequest(DocumentLoader*, unsigned long identifier,
		ResourceRequest &req, const ResourceResponse& redirectResponse) {

	if (urlblockfunc && urlblockfunc(req.url().string().utf8().data()))
		req.setURL(URL(URL(), "about:blank"));
}

bool FlFrameLoaderClient::shouldUseCredentialStorage(DocumentLoader*, unsigned long identifier) {
	return false;
}

void FlFrameLoaderClient::dispatchDidReceiveAuthenticationChallenge(DocumentLoader*, unsigned long identifier, const AuthenticationChallenge&) {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidCancelAuthenticationChallenge(DocumentLoader*, unsigned long identifier, const AuthenticationChallenge&) {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidReceiveResponse(DocumentLoader*, unsigned long identifier, const ResourceResponse&) {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidReceiveContentLength(DocumentLoader*, unsigned long identifier, int dataLength) {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidFinishLoading(DocumentLoader*, unsigned long identifier) {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidFailLoading(DocumentLoader*, unsigned long identifier, const ResourceError&) {
	notImplemented();
}

bool FlFrameLoaderClient::dispatchDidLoadResourceFromMemoryCache(DocumentLoader*, const ResourceRequest&, const ResourceResponse&, int length) {
	notImplemented();
	return false;
}

void FlFrameLoaderClient::dispatchDidHandleOnloadEvents() {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidReceiveServerRedirectForProvisionalLoad() {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidCancelClientRedirect() {
	notImplemented();
}

void FlFrameLoaderClient::dispatchWillPerformClientRedirect(const URL&, double interval, double fireDate) {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidChangeLocationWithinPage() {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidPushStateWithinPage() {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidReplaceStateWithinPage() {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidPopStateWithinPage() {
	notImplemented();
}

void FlFrameLoaderClient::dispatchWillClose() {
}

void FlFrameLoaderClient::dispatchDidReceiveIcon() {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidStartProvisionalLoad() {
}

void FlFrameLoaderClient::dispatchDidReceiveTitle(const StringWithDirection&) {
}

void FlFrameLoaderClient::dispatchDidChangeIcons(IconType) {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidCommitLoad() {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidFailProvisionalLoad(const ResourceError &error) {
	dispatchDidFailLoad(error);
}

void FlFrameLoaderClient::dispatchDidFailLoad(const ResourceError &err) {

	// The download conversion dispatches an error for some reason. Ignore and back.
	if (err.domain() == "WebKitPolicyError" &&
		err.errorCode() == PolicyErrorFrameLoadInterruptedByPolicyChange) {
		return;
	}

	char *instructions = NULL;
	if (err.errorCode() == CURLE_COULDNT_RESOLVE_HOST ||
		err.errorCode() == CURLE_COULDNT_CONNECT ||
		err.errorCode() == CURLE_REMOTE_ACCESS_DENIED ||
		err.errorCode() == CURLE_OPERATION_TIMEDOUT) {
		asprintf(&instructions,
			"This issue could be caused by either end. "
			"<a href=\"http://isup.me/%s\">"
			"Check with DownForEveryoneOrJustMe</a>?",
			err.domain().utf8().data());

		if (!instructions)
			return;
	}

	char *ptr = NULL;
	asprintf(&ptr, "<html><body><h2>%s%serror %u, %s</h2>%s</body></html>",
		err.failingURL().utf8().data(),
		err.failingURL().length() > 1 ? ": " : "",
		err.errorCode(),
		err.localizedDescription().utf8().data(),
		instructions ? instructions : "");

	if (!ptr) {
		free(instructions);
		return;
	}

	view->loadString(ptr, NULL, NULL, NULL, frame);
	free(ptr);
	free(instructions);
}

void FlFrameLoaderClient::dispatchDidFinishDocumentLoad() {
}

void FlFrameLoaderClient::dispatchDidFinishLoad() {
	view->redraw();
}

Frame* FlFrameLoaderClient::dispatchCreatePage(const NavigationAction&) {
	return 0;
}

void FlFrameLoaderClient::dispatchShow() {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDecidePolicyForResponse(const ResourceResponse &resp,
		const ResourceRequest &req, FramePolicyFunction policyfunc) {
	if (!policyfunc)
		return;
	if (req.isNull()) {
		policyfunc(PolicyIgnore);
		return;
	}

	if (canShowMIMEType(resp.mimeType()))
		policyfunc(PolicyUse);
	else
		policyfunc(PolicyDownload);
}

void FlFrameLoaderClient::dispatchDecidePolicyForNewWindowAction(const NavigationAction&, const ResourceRequest&, PassRefPtr<FormState>,
		const String& frameName, FramePolicyFunction policyfunc) {
	policyfunc(PolicyUse);
}

void FlFrameLoaderClient::dispatchDecidePolicyForNavigationAction(const NavigationAction&,
		const ResourceRequest &req,
		PassRefPtr<FormState>, FramePolicyFunction policyfunc) {

	if (req.url().string().startsWith("file://")) {

		const char *path = strdup(req.url().string().utf8().data() + 7);
		struct stat st;
		static bool byus = false;

		if (stat(path, &st) == 0 && S_ISDIR(st.st_mode) && !byus) {
			const char *list = gendirlisting(path);
			policyfunc(PolicyIgnore);

			byus = true;
			view->loadString(list, "text/html", "UTF-8",
						req.url().string().utf8().data());

			byus = false;

			free((char *) list);
			free((char *) path);

			return;
		}
		free((char *) path);
	} else if (req.url().string().startsWith("about://")) {

		if (aboutpagefunc) {
			const char * const page =
				aboutpagefunc(req.url().string().utf8().data() + 8);
			if (page) {
				view->loadString(page);
				free((char *) page);
			}
		}

		policyfunc(PolicyIgnore);
		return;
	}

	policyfunc(PolicyUse);
}

void FlFrameLoaderClient::cancelPolicyCheck() {
}

void FlFrameLoaderClient::dispatchUnableToImplementPolicy(const ResourceError&) {
	notImplemented();
}

void FlFrameLoaderClient::dispatchWillSendSubmitEvent(PassRefPtr<FormState>) {
	notImplemented();
}

void FlFrameLoaderClient::dispatchWillSubmitForm(PassRefPtr<FormState>,
			FramePolicyFunction policyfunc) {
	if (!policyfunc)
		return;

	policyfunc(PolicyUse);
}

void FlFrameLoaderClient::revertToProvisionalState(DocumentLoader*) {
	notImplemented();
}

void FlFrameLoaderClient::setMainDocumentError(DocumentLoader*, const ResourceError&) {
	notImplemented();
}

void FlFrameLoaderClient::setMainFrameDocumentReady(bool) {
	notImplemented();
}

void FlFrameLoaderClient::startDownload(const ResourceRequest &req,
					const String& suggestedName) {
	view->download(req.url().string().utf8().data(),
			suggestedName.utf8().data());
}

void FlFrameLoaderClient::willChangeTitle(DocumentLoader*) {
}

void FlFrameLoaderClient::didChangeTitle(DocumentLoader *documentLoader) {
}

void FlFrameLoaderClient::committedLoad(DocumentLoader *loader, const char *data, int len) {
	loader->commitData(data, len);
}

void FlFrameLoaderClient::finishedLoading(DocumentLoader*) {
	notImplemented();
}

void FlFrameLoaderClient::updateGlobalHistory() {
	notImplemented();
}

void FlFrameLoaderClient::updateGlobalHistoryRedirectLinks() {
	notImplemented();
}

bool FlFrameLoaderClient::shouldGoToHistoryItem(HistoryItem *item) const {
	return item != 0;
}

// FlFrameLoaderClient::This frame has displayed inactive content (such as an image) from an
// insecure source.  Inactive content cannot spread to other frames.
void FlFrameLoaderClient::didDisplayInsecureContent() {
	notImplemented();
}

// FlFrameLoaderClient::The indicated security origin has run active content (such as a
// script) from an insecure source.  Note that the insecure content can
// spread to other frames in the same origin.
void FlFrameLoaderClient::didRunInsecureContent(SecurityOrigin*, const URL&) {
	notImplemented();
}

void FlFrameLoaderClient::didDetectXSS(const URL&, bool didBlockEntirePage) {
	notImplemented();
}

ResourceError FlFrameLoaderClient::cancelledError(const ResourceRequest &req) {
	return WebCore::cancelledError(req);
}

ResourceError FlFrameLoaderClient::blockedError(const ResourceRequest &req) {
	return WebCore::blockedError(req);
}

ResourceError FlFrameLoaderClient::cannotShowURLError(const ResourceRequest &req) {
	return WebCore::cannotShowURLError(req);
}

ResourceError FlFrameLoaderClient::interruptedForPolicyChangeError(const ResourceRequest &req) {
	return WebCore::interruptedForPolicyChangeError(req);
}

ResourceError FlFrameLoaderClient::cannotShowMIMETypeError(const ResourceResponse &resp) {
	return WebCore::cannotShowMIMETypeError(resp);
}

ResourceError FlFrameLoaderClient::fileDoesNotExistError(const ResourceResponse &resp) {
	return WebCore::fileDoesNotExistError(resp);
}

ResourceError FlFrameLoaderClient::pluginWillHandleLoadError(const ResourceResponse &resp) {
	return WebCore::pluginWillHandleLoadError(resp);
}

bool FlFrameLoaderClient::shouldFallBack(const ResourceError &error) {
	return !(error.isCancellation());
}

bool FlFrameLoaderClient::canHandleRequest(const ResourceRequest&) const {
	notImplemented();
	return true;
}

bool FlFrameLoaderClient::canShowMIMEType(const String& MIMEType) const {
	return MIMETypeRegistry::canShowMIMEType(MIMEType);
}

bool FlFrameLoaderClient::canShowMIMETypeAsHTML(const String& MIMEType) const {
	notImplemented();
	return false;
}

bool FlFrameLoaderClient::representationExistsForURLScheme(const String& URLScheme) const {
	notImplemented();
	return false;
}

String FlFrameLoaderClient::generatedMIMETypeForURLScheme(const String& URLScheme) const {
	notImplemented();
	return String();
}

void FlFrameLoaderClient::frameLoadCompleted() {
}

void FlFrameLoaderClient::saveViewStateToItem(HistoryItem*) {
	notImplemented();
}

void FlFrameLoaderClient::restoreViewState() {
	notImplemented();
}

void FlFrameLoaderClient::provisionalLoadStarted() {
}

void FlFrameLoaderClient::didFinishLoad() {
}

void FlFrameLoaderClient::prepareForDataSourceReplacement() {
	notImplemented();
}

PassRefPtr<DocumentLoader> FlFrameLoaderClient::createDocumentLoader(const ResourceRequest& req,
		const SubstituteData &data) {
	RefPtr<DocumentLoader> loader = DocumentLoader::create(req, data);
	return loader;
}

void FlFrameLoaderClient::setTitle(const StringWithDirection &title, const URL &url) {

	// We don't care about iframe titles.
	if (frame != &view->priv->page->mainFrame())
		return;

	free((char *) view->priv->title);
	free((char *) view->priv->url);
	view->priv->title = strdup(title.string().utf8().data());
	view->priv->url = strdup(url.string().utf8().data());

	if (view->priv->m_titleChanged)
		view->priv->m_titleChanged();
}

String FlFrameLoaderClient::userAgent(const URL &url) {
	if (uafunc)
		return uafunc(decodeURLEscapeSequences(url.string()).utf8().data());
	return String("Mozilla/5.0 (Windows NT 5.1; rv:31.0) Gecko/20100101 Firefox/31.0");
}

void FlFrameLoaderClient::savePlatformDataToCachedFrame(CachedFrame*) {
	notImplemented();
}

void FlFrameLoaderClient::transitionToCommittedFromCachedFrame(CachedFrame*) {
	notImplemented();
}

void FlFrameLoaderClient::transitionToCommittedForNewPage() {
	IntSize size(view->w(), view->h());
	frame->createView(size, Color::white, false);
}

void FlFrameLoaderClient::didSaveToPageCache() {
	notImplemented();
}

void FlFrameLoaderClient::didRestoreFromPageCache() {
	notImplemented();
}

void FlFrameLoaderClient::dispatchDidBecomeFrameset(bool) {
	notImplemented();
}

bool FlFrameLoaderClient::canCachePage() const {
	return true;
}

void FlFrameLoaderClient::setFrame(Frame *inframe) {
	frame = inframe;
}

Frame *FlFrameLoaderClient::getFrame() const {
	return frame;
}

void FlFrameLoaderClient::convertMainResourceLoadToDownload(DocumentLoader *dl,
		const ResourceRequest &req, const ResourceResponse&) {
	dl->mainResourceLoader()->handle()->setDefersLoading(true);
	view->download(req.url().string().utf8().data(),
			req.url().lastPathComponent().utf8().data(),
			&req);
}

PassRefPtr<Frame> FlFrameLoaderClient::createFrame(const URL& url,
		const String& name, HTMLFrameOwnerElement* ownerElement,
		const String& referrer, bool allowsScrolling,
		int marginWidth, int marginHeight) {

	Frame *parent = frame;
	FlFrameLoaderClient *newl = new FlFrameLoaderClient(view);
	RefPtr<Frame> kid = Frame::create(view->priv->page, ownerElement, newl);
	newl->setFrame(kid.get());

	kid->tree().setName(name);
	parent->tree().appendChild(kid);
	kid->init();

	// Bad JS may nuke it instantly
	if (!kid->page())
		return NULL;

	parent->loader().loadURLIntoChildFrame(url, referrer, kid.get());

	// Bad JS again
	if (!kid->tree().parent())
		return NULL;

	return kid.release();
}

PassRefPtr<Widget> FlFrameLoaderClient::createPlugin(const IntSize&, HTMLPlugInElement*, const URL&, const Vector<String>&, const Vector<String>&, const String&, bool loadManually) {
	return 0;
}

void FlFrameLoaderClient::recreatePlugin(Widget*) {
}

void FlFrameLoaderClient::redirectDataToPlugin(Widget* pluginWidget) {
}

PassRefPtr<Widget> FlFrameLoaderClient::createJavaAppletWidget(const IntSize&, HTMLAppletElement*, const URL& baseURL, const Vector<String>& paramNames, const Vector<String>& paramValues) {
	return NULL;
}

ObjectContentType FlFrameLoaderClient::objectContentType(const URL &url,
		const String& mimeType, bool preferPlugInsForImages) {
	return FrameLoader::defaultObjectContentType(url, mimeType, preferPlugInsForImages);
}

String FlFrameLoaderClient::overrideMediaType() const {
	return String();
}

void FlFrameLoaderClient::dispatchDidClearWindowObjectInWorld(DOMWrapperWorld&) {
	notImplemented();
}

void FlFrameLoaderClient::registerForIconNotification(bool listen) {
	notImplemented();
}

class netctx: public FrameNetworkingContext {
public:
	netctx(Frame *f): FrameNetworkingContext(f) {}
};

PassRefPtr<FrameNetworkingContext> FlFrameLoaderClient::createNetworkingContext() {
	return adoptRef(new netctx(frame));
}
