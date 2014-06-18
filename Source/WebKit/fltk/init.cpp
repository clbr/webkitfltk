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

#include "ApplicationCacheStorage.h"
#include "Logging.h"
#include "Page.h"
#include "PageGroup.h"
#include "ResourceHandle.h"
#include "TextEncodingRegistry.h"
#include "webkit.h"

#include "platformstrategy.h"

#include <runtime/InitializeThreading.h>
#include <wtf/MainThread.h>

using namespace WebCore;

const char * (*uafunc)(const char *) = NULL;
int (*urlblockfunc)(const char *) = NULL;
const char * (*uploaddirfunc)() = NULL;

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
