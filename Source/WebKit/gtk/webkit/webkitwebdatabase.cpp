/*
 * Copyright (C) 2009 Martin Robinson
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
#include "webkitwebdatabase.h"

#include "DatabaseDetails.h"
#include "DatabaseManager.h"
#include "FileSystem.h"
#include "GroupSettings.h"
#include "PageGroup.h"
#include "webkitglobalsprivate.h"
#include "webkitsecurityoriginprivate.h"
#include <wtf/text/CString.h>

/**
 * SECTION:webkitwebdatabase
 * @short_description: A WebKit web application database
 *
 * #WebKitWebDatabase is a representation of a Web Database database. The
 * proposed Web Database standard introduces support for SQL databases that web
 * sites can create and access on a local computer through JavaScript.
 *
 * To get access to all databases defined by a security origin, use
 * #webkit_security_origin_get_databases. Each database has a canonical
 * name, as well as a user-friendly display name.
 *
 * WebKit uses SQLite to create and access the local SQL databases. The location
 * of a #WebKitWebDatabase can be accessed wth #WebKitWebDatabase::get_filename.
 * You can configure the location of all databases with
 * #webkit_set_database_directory_path.
 *
 * For each database the web site can define an estimated size which can be
 * accessed with #WebKitWebDatabase::get_expected_size. The current size of the
 * database in bytes is returned by #WebKitWebDatabase::get_size.
 *
 * For more information refer to the Web Database specification proposal at
 * http://dev.w3.org/html5/webdatabase
 */

using namespace WebKit;

static CString gWebKitWebDatabasePath;
static uint64_t webkit_default_database_quota = 5 * 1024 * 1024;


WebKitWebDatabase::WebKitWebDatabase() {
	name = displayName = filename = NULL;
	origin = NULL;
}

WebKitWebDatabase::~WebKitWebDatabase() {
	free(name);
	free(displayName);
	free(filename);
}

/**
 * WebKitWebDatabase::get_security_origin:
 * @webDatabase: a #WebKitWebDatabase
 *
 * Returns the security origin of the #WebKitWebDatabase.
 *
 * Returns: (transfer none): the security origin of the database
 *
 * Since: 1.1.14
 **/
WebKitSecurityOrigin* WebKitWebDatabase::get_security_origin()
{
	return origin;
}

/**
 * WebKitWebDatabase::get_name:
 * @webDatabase: a #WebKitWebDatabase
 *
 * Returns the canonical name of the #WebKitWebDatabase.
 *
 * Returns: the name of the database
 *
 * Since: 1.1.14
 **/
const char* WebKitWebDatabase::get_name()
{
	return name;
}

/**
 * WebKitWebDatabase::get_display_name:
 * @webDatabase: a #WebKitWebDatabase
 *
 * Returns the name of the #WebKitWebDatabase as seen by the user.
 *
 * Returns: the name of the database as seen by the user.
 *
 * Since: 1.1.14
 **/
const char* WebKitWebDatabase::get_display_name()
{
#if ENABLE(SQL_DATABASE)
	WebCore::DatabaseDetails details = WebCore::DatabaseManager::manager().detailsForNameAndOrigin(name, core(origin));
	WTF::String displayName = details.displayName();

	if (displayName.isEmpty())
		return "";

	free(displayName);
	displayName = strdup(displayName.utf8().data());
	return displayName;
#else
	return "";
#endif
}

/**
 * WebKitWebDatabase::get_expected_size:
 * @webDatabase: a #WebKitWebDatabase
 *
 * Returns the expected size of the #WebKitWebDatabase in bytes as defined by the
 * web author. The Web Database standard allows web authors to specify an expected
 * size of the database to optimize the user experience.
 *
 * Returns: the expected size of the database in bytes
 *
 * Since: 1.1.14
 **/
uint64_t WebKitWebDatabase::get_expected_size()
{
#if ENABLE(SQL_DATABASE)
	WebCore::DatabaseDetails details = WebCore::DatabaseManager::manager().detailsForNameAndOrigin(name, core(origin));
	return details.expectedUsage();
#else
	return 0;
#endif
}

/**
 * WebKitWebDatabase::get_size:
 * @webDatabase: a #WebKitWebDatabase
 *
 * Returns the actual size of the #WebKitWebDatabase space on disk in bytes.
 *
 * Returns: the actual size of the database in bytes
 *
 * Since: 1.1.14
 **/
uint64_t WebKitWebDatabase::get_size()
{
#if ENABLE(SQL_DATABASE)
	WebCore::DatabaseDetails details = WebCore::DatabaseManager::manager().detailsForNameAndOrigin(name, core(origin));
	return details.currentUsage();
#else
	return 0;
#endif
}

/**
 * WebKitWebDatabase::get_filename:
 * @webDatabase: a #WebKitWebDatabase
 *
 * Returns the absolute filename to the #WebKitWebDatabase file on disk.
 *
 * Returns: the absolute filename of the database
 *
 * Since: 1.1.14
 **/
const char* WebKitWebDatabase::get_filename()
{
#if ENABLE(SQL_DATABASE)
	WTF::String coreName = WTF::String::fromUTF8(name);
	WTF::String corePath = WebCore::DatabaseManager::manager().fullPathForDatabase(core(origin), coreName);

	if (corePath.isEmpty())
		return"";

	free(filename);
	filename = strdup(corePath.utf8().data());
	return filename;

#else
	return "";
#endif
}

/**
 * WebKitWebDatabase::remove:
 * @webDatabase: a #WebKitWebDatabase
 *
 * Removes the #WebKitWebDatabase from its security origin and destroys all data
 * stored in the database.
 *
 * Since: 1.1.14
 **/
void WebKitWebDatabase::remove()
{
#if ENABLE(SQL_DATABASE)
	WebCore::DatabaseManager::manager().deleteDatabase(core(origin), name);
#endif
}

/**
 * webkit_remove_all_web_databases:
 *
 * Removes all web databases from the current database directory path.
 *
 * Since: 1.1.14
 **/
void webkit_remove_all_web_databases()
{
#if ENABLE(SQL_DATABASE)
	WebCore::DatabaseManager::manager().deleteAllDatabases();
#endif
}

/**
 * webkit_get_web_database_directory_path:
 *
 * Returns the current path to the directory WebKit will write Web
 * Database and Indexed Database databases. By default this path will
 * be in the user data directory.
 *
 * Returns: the current database directory path in the filesystem encoding
 *
 * Since: 1.1.14
 **/
const char* webkit_get_web_database_directory_path()
{
	return gWebKitWebDatabasePath.data();
}

/**
 * webkit_set_web_database_directory_path:
 * @path: the new database directory path in the filesystem encoding
 *
 * Sets the current path to the directory WebKit will write Web
 * Database and Indexed Database databases.
 *
 * Since: 1.1.14
 **/
void webkit_set_web_database_directory_path(const char* path)
{
	gWebKitWebDatabasePath = path;

	String pathString = WebCore::filenameToString(path);
#if ENABLE(SQL_DATABASE)
	WebCore::DatabaseManager::manager().setDatabaseDirectoryPath(pathString);
#endif

#if ENABLE(INDEXED_DATABASE)
	WebCore::PageGroup::pageGroup(webkitPageGroupName())->groupSettings().setIndexedDBDatabasePath(pathString);
#endif

}

/**
 * webkit_get_default_web_database_quota:
 *
 * Returns the default quota for Web Database databases. By default
 * this value is 5MB.
 *
 * Returns: the current default database quota in bytes
 *
 * Since: 1.1.14
 **/
uint64_t webkit_get_default_web_database_quota()
{
	return webkit_default_database_quota;
}

/**
 * webkit_set_default_web_database_quota:
 * @defaultQuota: the new default database quota
 *
 * Sets the default quota for Web Database databases.
 *
 * Since: 1.1.14
 **/
void webkit_set_default_web_database_quota(uint64_t defaultQuota)
{
	webkit_default_database_quota = defaultQuota;
}
