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

#ifndef platformstrategy_h
#define platformstrategy_h

#include "CookiesStrategy.h"
#include "DatabaseStrategy.h"
#include "LoaderStrategy.h"
#include "PasteboardStrategy.h"
#include "PlatformStrategies.h"
#include "PluginStrategy.h"
#include "SharedWorkerStrategy.h"
#include "StorageStrategy.h"

class PlatformStrategiesFLTK: public WebCore::PlatformStrategies,
	private WebCore::CookiesStrategy, private WebCore::DatabaseStrategy,
	private WebCore::LoaderStrategy, private WebCore::PasteboardStrategy,
	private WebCore::PluginStrategy, private WebCore::SharedWorkerStrategy,
	private WebCore::StorageStrategy {
public:
	static void initialize();
private:
	PlatformStrategiesFLTK();

	// cookies
	virtual WTF::String cookiesForDOM(const WebCore::NetworkStorageSession&,
			const WebCore::URL&, const WebCore::URL&);
	virtual void setCookiesFromDOM(const WebCore::NetworkStorageSession&,
			const WebCore::URL&, const WebCore::URL&, const WTF::String&);
	virtual bool cookiesEnabled(const WebCore::NetworkStorageSession&,
			const WebCore::URL&, const WebCore::URL&);
	virtual String cookieRequestHeaderFieldValue(const WebCore::NetworkStorageSession&,
			const WebCore::URL&, const WebCore::URL&);
	virtual bool getRawCookies(const WebCore::NetworkStorageSession&,
			const WebCore::URL&, const WebCore::URL&,
			WTF::Vector<WebCore::Cookie>&);
	virtual void deleteCookie(const WebCore::NetworkStorageSession&,
			const WebCore::URL&, const WTF::String&);

	// plugins
	virtual void refreshPlugins();
	virtual void getPluginInfo(const WebCore::Page*, WTF::Vector<WebCore::PluginInfo>&);

	// creations
	virtual CookiesStrategy* createCookiesStrategy() { return this; }
	virtual DatabaseStrategy* createDatabaseStrategy() { return this; }
	virtual LoaderStrategy* createLoaderStrategy() { return this; }
	virtual PasteboardStrategy* createPasteboardStrategy() { return this; }
	virtual PluginStrategy* createPluginStrategy() { return this; }
	virtual SharedWorkerStrategy* createSharedWorkerStrategy() { return this; }
	virtual StorageStrategy* createStorageStrategy() { return this; }
};

#endif
