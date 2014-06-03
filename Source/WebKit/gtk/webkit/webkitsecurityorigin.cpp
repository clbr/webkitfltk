/*
 * Copyright (C) 2009 Martin Robinson, Jan Michael C. Alonzo
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
#include "webkitsecurityorigin.h"

#include "DatabaseManager.h"
#include "webkitglobalsprivate.h"
#include "webkitsecurityoriginprivate.h"
#include <wtf/text/CString.h>
#include <wtf/text/WTFString.h>

/**
 * SECTION:webkitsecurityorigin
 * @short_description: A security boundary for web sites
 *
 * #WebKitSecurityOrigin is a representation of a security domain defined
 * by web sites. An origin consists of a host name, a protocol, and a port
 * number. Web sites with the same security origin can access each other's
 * resources for client-side scripting or database access.
 *
 * Use #webkit_web_frame_get_security_origin to get the security origin of a
 * #WebKitWebFrame.
 *
 * Database quotas and usages are also defined per security origin. The
 * cumulative disk usage of an origin's databases may be retrieved with
 * #webkit_security_origin_get_web_database_usage. An origin's quota can be
 * adjusted with #webkit_security_origin_set_web_database_quota.
 */

using namespace WebKit;

WebKitSecurityOrigin::WebKitSecurityOrigin() {
	protocol = host = NULL;
}

WebKitSecurityOrigin::~WebKitSecurityOrigin() {
	free(protocol);
	free(host);
}

const char* WebKitSecurityOrigin::get_protocol()
{
	WTF::String protocol = coreOrigin->protocol();

	if (!this->protocol)
		this->protocol = strdup(protocol.utf8().data());

	return protocol;
}

const char* WebKitSecurityOrigin::get_host()
{
	WTF::String host = coreOrigin->host();

	if (!this->host)
		this->host = strdup(host.utf8().data());

	return host;
}

unsigned WebKitSecurityOrigin::get_port()
{
	return coreOrigin->port();
}

/**
 * webkit_security_origin_get_web_database_usage:
 * @securityOrigin: a #WebKitSecurityOrigin
 *
 * Returns the cumulative size of all Web Database database's in the origin
 * in bytes.
 *
 * Returns: the cumulative size of all databases
 *
 * Since: 1.1.14
 **/
uint64_t WebKitSecurityOrigin::get_webdb_usage()
{
#if ENABLE(SQL_DATABASE)
	return WebCore::DatabaseManager::manager().usageForOrigin(coreOrigin);
#else
	return 0;
#endif
}

/**
 * webkit_security_origin_get_web_database_quota:
 * @securityOrigin: a #WebKitSecurityOrigin
 *
 * Returns the quota for Web Database storage of the security origin
 * in bytes.
 *
 * Returns: the Web Database quota
 *
 * Since: 1.1.14
 **/
uint64_t WebKitSecurityOrigin::get_webdb_quota()
{
#if ENABLE(SQL_DATABASE)
	return WebCore::DatabaseManager::manager().quotaForOrigin(coreOrigin);
#else
	return 0;
#endif
}

/**
 * webkit_security_origin_set_web_database_quota:
 * @securityOrigin: a #WebKitSecurityOrigin
 * @quota: a new Web Database quota in bytes
 *
 * Adjust the quota for Web Database storage of the security origin
 *
 * Since: 1.1.14
 **/
void WebKitSecurityOrigin::set_webdb_quota(uint64_t quota)
{
#if ENABLE(SQL_DATABASE)
	WebCore::DatabaseManager::manager().setQuota(coreOrigin, quota);
#endif
}

/**
 * webkit_security_origin_get_all_web_databases:
 * @securityOrigin: a #WebKitSecurityOrigin
 *
 * Returns a list of all Web Databases in the security origin.
 *
 * Returns: (transfer container) (element-type WebKitWebDatabase): a
 * #GList of databases in the security origin.
 *
 * Since: 1.1.14
 **/
vector<WebKitWebDatabase*> WebKitSecurityOrigin::get_all_webdbs()
{
	vector<WebKitWebDatabase*> databases;

#if ENABLE(SQL_DATABASE)
	Vector<WTF::String> databaseNames;

	if (!WebCore::DatabaseManager::manager().databaseNamesForOrigin(coreOrigin, databaseNames))
		return databases;

	const unsigned max = databaseNames.size();
	for (unsigned i = 0; i < max; ++i) {
		WebKitWebDatabase* database = WebKitSecurityOrigin::get_webdb(databaseNames[i].utf8().data());
		databases.push_back(database);
	}
#endif

	return databases;
}

WebKitWebDatabase* WebKitSecurityOrigin::get_webdb(const char* databaseName)
{
	const unsigned max = webDatabases.size();
	for (unsigned i = 0; i < max; i++) {
		if (!strcmp(databaseName, webDatabases[i]->get_name())
			return webDatabases[i];
	}

	WebKitWebDatabase *database = new WebKitWebDatabase(this, databaseName);
	webDatabases.push_back(database);

	return database;
}

namespace WebKit {

WebKitSecurityOrigin* kit(WebCore::SecurityOrigin* coreOrigin)
{
	ASSERT(coreOrigin);

	GHashTable* table = webkit_security_origins();
	WebKitSecurityOrigin* origin = (WebKitSecurityOrigin*) g_hash_table_lookup(table, coreOrigin);

	if (!origin) {
		origin = WEBKIT_SECURITY_ORIGIN(g_object_new(WEBKIT_TYPE_SECURITY_ORIGIN, NULL));
		origin->priv->coreOrigin = coreOrigin;
		g_hash_table_insert(table, coreOrigin, origin);
	}

	return origin;
}

}
