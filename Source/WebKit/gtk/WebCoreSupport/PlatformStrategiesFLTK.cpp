/*
 * Copyright (C) 2012 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "PlatformStrategiesFLTK.h"

#include "NotImplemented.h"
#include "Page.h"
#include "PageGroup.h"
#include "PlatformCookieJar.h"
#include "PluginDatabase.h"
#include "PluginPackage.h"

using namespace WebCore;

void PlatformStrategiesFLTK::initialize()
{
	DEPRECATED_DEFINE_STATIC_LOCAL(PlatformStrategiesFLTK, platformStrategies, ());
	setPlatformStrategies(&platformStrategies);
}

PlatformStrategiesFLTK::PlatformStrategiesFLTK()
{
}

CookiesStrategy* PlatformStrategiesFLTK::createCookiesStrategy()
{
	return this;
}

DatabaseStrategy* PlatformStrategiesFLTK::createDatabaseStrategy()
{
	return this;
}

LoaderStrategy* PlatformStrategiesFLTK::createLoaderStrategy()
{
	return this;
}

PasteboardStrategy* PlatformStrategiesFLTK::createPasteboardStrategy()
{
	// This is currently used only by Mac code.
	notImplemented();
	return 0;
}

PluginStrategy* PlatformStrategiesFLTK::createPluginStrategy()
{
	return this;
}

SharedWorkerStrategy* PlatformStrategiesFLTK::createSharedWorkerStrategy()
{
	return this;
}

StorageStrategy* PlatformStrategiesFLTK::createStorageStrategy()
{
	return this;
}

// CookiesStrategy
String PlatformStrategiesFLTK::cookiesForDOM(const NetworkStorageSession& session, const URL& firstParty, const URL& url)
{
	return WebCore::cookiesForDOM(session, firstParty, url);
}

void PlatformStrategiesFLTK::setCookiesFromDOM(const NetworkStorageSession& session, const URL& firstParty, const URL& url, const String& cookieString)
{
	WebCore::setCookiesFromDOM(session, firstParty, url, cookieString);
}

bool PlatformStrategiesFLTK::cookiesEnabled(const NetworkStorageSession& session, const URL& firstParty, const URL& url)
{
	return WebCore::cookiesEnabled(session, firstParty, url);
}

String PlatformStrategiesFLTK::cookieRequestHeaderFieldValue(const NetworkStorageSession& session, const URL& firstParty, const URL& url)
{
	return WebCore::cookieRequestHeaderFieldValue(session, firstParty, url);
}

bool PlatformStrategiesFLTK::getRawCookies(const NetworkStorageSession& session, const URL& firstParty, const URL& url, Vector<Cookie>& rawCookies)
{
	return WebCore::getRawCookies(session, firstParty, url, rawCookies);
}

void PlatformStrategiesFLTK::deleteCookie(const NetworkStorageSession& session, const URL& url, const String& cookieName)
{
	WebCore::deleteCookie(session, url, cookieName);
}

// PluginStrategy
void PlatformStrategiesFLTK::refreshPlugins()
{
	PluginDatabase::installedPlugins()->refresh();
}

void PlatformStrategiesFLTK::getPluginInfo(const Page* page, Vector<PluginInfo>& outPlugins)
{
	PluginDatabase* database = PluginDatabase::installedPlugins();
	const Vector<PluginPackage*> &plugins = database->plugins();

	for (size_t i = 0; i < plugins.size(); ++i) {
		PluginPackage* package = plugins[i];

		PluginInfo pluginInfo;
		pluginInfo.name = package->name();
		pluginInfo.file = package->fileName();
		pluginInfo.desc = package->description();

		const MIMEToDescriptionsMap& mimeToDescriptions = package->mimeToDescriptions();
		MIMEToDescriptionsMap::const_iterator end = mimeToDescriptions.end();
		for (MIMEToDescriptionsMap::const_iterator it = mimeToDescriptions.begin(); it != end; ++it) {
			MimeClassInfo mime;
			mime.type = it->key;
			mime.desc = it->value;
			mime.extensions = package->mimeToExtensions().get(mime.type);
			pluginInfo.mimes.append(mime);
		}

		outPlugins.append(pluginInfo);
	}
}
