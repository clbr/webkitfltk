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
#include <platform/PlatformExportMacros.h>
#include <runtime/JSExportMacros.h>

#include <ApplicationCacheStorage.h>
#include <CrossOriginPreflightResultCache.h>
#include <FontCache.h>
#include <GCController.h>
#include <IconDatabase.h>
#include <IconDatabaseClient.h>
#include <ImageSource.h>
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
#include <wtf/spoofing.h>

#include <cairo.h>
#include <openssl/crypto.h>

using namespace WebCore;

const char * (*uafunc)(const char *) = NULL;
int (*urlblockfunc)(const char *) = NULL;
int (*inlineblockfunc)(const char *, const char *) = NULL;
const char * (*uploaddirfunc)() = NULL;
const char * (*downloaddirfunc)() = NULL;
const char * (*aboutpagefunc)(const char *) = NULL;
void (*downloadfunc)(const char *url, const char *file) = NULL;
int (*sslfunc)(const char *, const char *) = NULL;
void (*sslerrfunc)(webview *, const char *, const bool) = NULL;
webview *(*popupfunc)(const char *) = NULL;
void (*downloadrefreshfunc)() = NULL;
void (*newdownloadfunc)() = NULL;
void (*bgtabfunc)(const char*) = NULL;
void (*persitesettingsfunc)(const char*) = NULL;

int (*spoofedTZ)() = NULL;
const char *(*spoofedAccept)(const char *) = NULL;
const char *(*spoofedLanguage)(const char *) = NULL;

const char *wk_stream_exec = NULL;
const char *wk_cookiepath = NULL;
int wheelspeed = 100;

#if OPENSSL_VERSION_NUMBER < 0x10100000

static pthread_mutex_t *mutexes;

static unsigned long openssl_id_cb(void) {
	return (unsigned long) pthread_self();
}

static void openssl_lock_cb(int mode, int n, const char *file, int line) {
	if (mode & CRYPTO_LOCK)
		pthread_mutex_lock(&mutexes[n]);
	else
		pthread_mutex_unlock(&mutexes[n]);
}

#endif

void webkitInit() {
	static bool init = false;
	if (init)
		return;
	init = true;

	JSC::initializeThreading();
	WTF::initializeMainThread();

#if !LOG_DISABLED
	WebCore::initializeLoggingChannelsIfNecessary();
#endif

	PlatformStrategiesFLTK::initialize();
	atomicCanonicalTextEncodingName("UTF-8");

	Fl::lock();

	// Make sure the runtime cairo version is new enough
	const int runtime = cairo_version();
	const int required = CAIRO_VERSION_ENCODE(1, 12, 18);
	if (runtime < required) {
		printf("The Cairo version on this system is too old, 1.12.18 required (%d, %d)\n",
			runtime, required);
		exit(1);
	}

#if OPENSSL_VERSION_NUMBER < 0x10100000
	// OpenSSL < 1.1 needs thread protection
	const unsigned num_locks = CRYPTO_num_locks();
	mutexes = (pthread_mutex_t *) calloc(num_locks, sizeof(pthread_mutex_t));
	unsigned i;
	for (i = 0; i < num_locks; i++) {
		pthread_mutex_init(&mutexes[i], NULL);
	}
	CRYPTO_set_id_callback(openssl_id_cb);
	CRYPTO_set_locking_callback(openssl_lock_cb);
#endif
}

void wk_set_useragent_func(const char * (*func)(const char *)) {
	uafunc = func;
}

void wk_set_urlblock_func(int (*func)(const char *)) {
	urlblockfunc = func;
}

void wk_set_inlineblock_func(int (*func)(const char *, const char *)) {
	inlineblockfunc = func;
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

#ifdef None
#undef None
// We don't want the X11 definition in this file.
#endif

void wk_drop_caches() {

	// Turn the cache on and off.  Disabling the object cache will remove all
	// resources from the cache.  They may still live on if they are referenced
	// by some Web page though.
	auto &memoryCache = WebCore::MemoryCache::singleton();
	auto &pageCache = WebCore::PageCache::singleton();
	auto &fontCache = WebCore::FontCache::singleton();

	if (!memoryCache.disabled()) {
		memoryCache.setDisabled(true);
		memoryCache.setDisabled(false);
	}

	pageCache.pruneToSizeNow(0, PruningReason::None);

	// Invalidating the font cache and freeing all inactive font data.
	fontCache.invalidate();

	// Empty the Cross-Origin Preflight cache
	WebCore::CrossOriginPreflightResultCache::singleton().empty();

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

void wk_set_ssl_err_func(void (*func)(webview *, const char *, const bool)) {
	sslerrfunc = func;
}

// Return 1 if ok, 0 to abort SSL connection
int fl_check_cert(const String &str, const String &host) {

	// If there was a typo, e.g. 'https:/foo.com', the host will be empty.
	// It would fail anyhow, but fail it here too.
	if (!host.length())
		return 0;

	if (sslfunc)
		return sslfunc(str.utf8().data(), host.utf8().data());
	return 1;
}

void wk_set_popup_func(webview *(*func)(const char*)) {
	popupfunc = func;
}

void wk_set_download_refresh_func(void (*func)()) {
	downloadrefreshfunc = func;
}

void wk_set_new_download_func(void (*func)()) {
	newdownloadfunc = func;
}

void wk_set_bgtab_func(void (*func)(const char*)) {
	bgtabfunc = func;
}

class dbclient: public IconDatabaseClient {
public:
	dbclient(void (*func)()): done(func) {}
	void didImportIconURLForPageURL(const String&) {}
	void didImportIconDataForPageURL(const String&) {}
	void didChangeIconForPageURL(const String&) {}
	void didRemoveAllIcons() {}
	void didFinishURLImport() {
		done();
	}
private:
	void (*done)();
};

static dbclient *dbc;

void wk_set_favicon_dir(const char *dir, const std::vector<const char*> *preloads,
			void (*done)()) {
	if (!dir)
		return;

	if (iconDatabase().isEnabled()) {
		printf("Tried to open favicon db twice\n");
		return;
	}

	if (done) {
		dbc = new dbclient(done);
		iconDatabase().setClient(dbc);
	}

	iconDatabase().setEnabled(true);

	if (preloads && preloads->size()) {
		unsigned i;
		const unsigned max = preloads->size();
		for (i = 0; i < max; i++) {
			iconDatabase().retainIconForPageURL(String::fromUTF8((*preloads)[i]));
		}
		iconDatabase().retainedPageURLCount();
	}

	iconDatabase().open(String::fromUTF8(dir), IconDatabase::defaultDatabaseFilename());
}

Fl_RGB_Image *wk_get_favicon(const char *url, const unsigned targetsize) {

	if (!url)
		return NULL;

	const URL parsed(URL(), String::fromUTF8(url));
	cairo_surface_t *surf =
		iconDatabase().synchronousNativeIconForPageURL(parsed.string(),
						IntSize(targetsize, targetsize)).get();
	if (!surf)
		return NULL;

	const cairo_surface_type_t type = cairo_surface_get_type(surf);
	if (type != CAIRO_SURFACE_TYPE_IMAGE) {
		printf("Surface type not image (%u)\n", type);
		return NULL;
	}

	const cairo_format_t format = cairo_image_surface_get_format(surf);
	if (format != CAIRO_FORMAT_ARGB32 && format != CAIRO_FORMAT_RGB24) {
		printf("Unknown format %u\n", format);
		return NULL;
	}

	const int w = cairo_image_surface_get_width(surf);
	const int h = cairo_image_surface_get_height(surf);
	const int s = cairo_image_surface_get_stride(surf);
	if (w <= 0 || h <= 0 || s <= 0) {
		printf("Invalid icon %dx%d stride %d\n", w, h, s);
		return NULL;
	}

	unsigned char *data = cairo_image_surface_get_data(surf);
	if (!data)
		return NULL;
	int depth = 3;
	if (format == CAIRO_FORMAT_ARGB32)
		depth = 4;

	int ld = 0;
	if (s != depth * w)
		ld = s - depth * w;

	unsigned char *copy = new unsigned char[s * h];
	memcpy(copy, data, s * h);
	data = copy;

	// FLTK 1.3 does not support BGRA data. Convert it to RGBA.
	// TODO: this only handles little endian.
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			unsigned char * const px = &data[y * s + x * depth];
			unsigned char tmp = px[0];
			px[0] = px[2];
			px[2] = tmp;
		}
	}

	Fl_RGB_Image *pic = new Fl_RGB_Image(data, w, h, depth, ld);
	pic->alloc_array = 1;

	return pic;
}

void wk_exit() {
	iconDatabase().close();
	wk_drop_caches();
}

void wk_set_cache_dir(const char *dir) {
	WebCore::ApplicationCacheStorage::singleton().setCacheDirectory(dir);
}

void wk_set_cache_max(const unsigned bytes) {
	WebCore::ApplicationCacheStorage::singleton().setMaximumSize(bytes);
}

void wk_set_tz_func(int (*func)()) {
	spoofedTZ = func;
}

void wk_set_accept_func(const char *(*func)(const char*)) {
	spoofedAccept = func;
}

void wk_set_language_func(const char *(*func)(const char *)) {
	spoofedLanguage = func;
}

void wk_set_streaming_prog(const char *prog) {
	wk_stream_exec = prog;
}

void wk_set_persite_settings_func(void (*func)(const char*)) {
	persitesettingsfunc = func;
}

void wk_set_cookie_path(const char *path) {
	free((char *) wk_cookiepath);
	asprintf((char **) &wk_cookiepath, "%s/cookies.dat", path);
}

void wk_set_image_max(const unsigned size) {
#if ENABLE(IMAGE_DECODER_DOWN_SAMPLING)
	ImageSource::setMaxPixelsPerDecodedImage(size * size);
#endif
}
