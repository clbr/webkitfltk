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
#include "platformstrategy.h"

#include "PlatformCookieJar.h"

using namespace WebCore;
using namespace WTF;

static PlatformStrategiesFLTK *str = NULL;

PlatformStrategiesFLTK::PlatformStrategiesFLTK() {

}

void PlatformStrategiesFLTK::initialize() {
	str = new PlatformStrategiesFLTK;
	setPlatformStrategies(str);
}

// cookies

String PlatformStrategiesFLTK::cookiesForDOM(const NetworkStorageSession &sess,
		const URL &firstparty, const URL &url) {
	return WebCore::cookiesForDOM(sess, firstparty, url);
}

void PlatformStrategiesFLTK::setCookiesFromDOM(const NetworkStorageSession &sess,
			const URL &firstparty, const URL&url, const String &str) {
	WebCore::setCookiesFromDOM(sess, firstparty, url, str);
}

bool PlatformStrategiesFLTK::cookiesEnabled(const NetworkStorageSession &sess,
		const URL &firstparty, const URL &url) {
	return WebCore::cookiesEnabled(sess, firstparty, url);
}

String PlatformStrategiesFLTK::cookieRequestHeaderFieldValue(
		const NetworkStorageSession &sess,
		const URL &firstparty, const URL &url) {
	return WebCore::cookieRequestHeaderFieldValue(sess, firstparty, url);
}

bool PlatformStrategiesFLTK::getRawCookies(const NetworkStorageSession &sess,
		const URL &firstparty, const URL &url,
		Vector<Cookie> &out) {
	return WebCore::getRawCookies(sess, firstparty, url, out);
}

void PlatformStrategiesFLTK::deleteCookie(const NetworkStorageSession &sess,
		const URL &url, const String &name) {
	WebCore::deleteCookie(sess, url, name);
}

void PlatformStrategiesFLTK::refreshPlugins() {

}

void PlatformStrategiesFLTK::getPluginInfo(const Page *page, Vector<PluginInfo>&info) {

}
