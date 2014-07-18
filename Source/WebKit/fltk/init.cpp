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

#include <ApplicationCacheStorage.h>
#include <CrossOriginPreflightResultCache.h>
#include <FontCache.h>
#include <GCController.h>
#include <Logging.h>
#include <MemoryCache.h>
#include <Page.h>
#include <PageCache.h>
#include <PageGroup.h>
#include <ResourceHandle.h>
#include <TextEncodingRegistry.h>
#include "webkit.h"

#include "platformstrategy.h"

#include <runtime/InitializeThreading.h>
#include <wtf/MainThread.h>

using namespace WebCore;

const char * (*uafunc)(const char *) = NULL;
int (*urlblockfunc)(const char *) = NULL;
const char * (*uploaddirfunc)() = NULL;
const char * (*downloaddirfunc)() = NULL;
const char * (*aboutpagefunc)(const char *) = NULL;
void (*downloadfunc)(const char *url, const char *file) = NULL;
int (*sslfunc)(const char *, const char *) = NULL;
void (*sslerrfunc)(webview *, const char *) = NULL;

int wheelspeed = 100;

void webkitInit() {
	static bool init = false;
	if (init)
		return;
	init = true;

	JSC::initializeThreading();
	WTF::initializeMainThread();

	WebCore::initializeLoggingChannelsIfNecessary();

	PlatformStrategiesFLTK::initialize();
	atomicCanonicalTextEncodingName("UTF-8");
	cacheStorage().setCacheDirectory("cache");
	PageGroup::setShouldTrackVisitedLinks(true);

	Fl::lock();
}

void wk_set_useragent_func(const char * (*func)(const char *)) {
	uafunc = func;
}

void wk_set_urlblock_func(int (*func)(const char *)) {
	urlblockfunc = func;
}

void wk_set_uploaddir_func(const char * (*func)()) {
	uploaddirfunc = func;
}

void wk_set_downloaddir_func(const char * (*func)()) {
	downloaddirfunc = func;
}

void wk_set_wheel_speed(const int in) {
	wheelspeed = in;
}

void wk_set_aboutpage_func(const char * (*func)(const char*)) {
	aboutpagefunc = func;
}

void wk_set_download_func(void (*func)(const char *url, const char *file)) {
	downloadfunc = func;
}

void wk_drop_caches() {

	// Turn the cache on and off.  Disabling the object cache will remove all
	// resources from the cache.  They may still live on if they are referenced
	// by some Web page though.
	if (!WebCore::memoryCache()->disabled()) {
		WebCore::memoryCache()->setDisabled(true);
		WebCore::memoryCache()->setDisabled(false);
	}

	WebCore::pageCache()->pruneToCapacityNow(0);

	// Invalidating the font cache and freeing all inactive font data.
	WebCore::fontCache().invalidate();

	// Empty the Cross-Origin Preflight cache
	WebCore::CrossOriginPreflightResultCache::shared().empty();

	// Drop JIT compiled code from ExecutableAllocator.
	WebCore::gcController().discardAllCompiledCode();

	// Run GC
	WebCore::gcController().garbageCollectNow();
}

char *wk_urlencode(const char *in) {

	String s = encodeWithURLEscapeSequences(String::fromUTF8(in));
	return strdup(s.utf8().data());
}

void wk_set_ssl_func(int (*func)(const char *, const char *)) {
	sslfunc = func;
}

void wk_set_ssl_err_func(void (*func)(webview *, const char *)) {
	sslerrfunc = func;
}

// Return 1 if ok, 0 to abort SSL connection
int fl_check_cert(const String &str, const String &host) {
	if (sslfunc)
		return sslfunc(str.utf8().data(), host.utf8().data());
	return 1;
}
