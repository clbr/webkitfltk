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

#ifndef webkitwebdatabase_h
#define webkitwebdatabase_h

#include <webkit/webkitdefines.h>

class WebKitSecurityOrigin;

class WebKitWebDatabase {
public:
	WebKitWebDatabase();
	~WebKitWebDatabase();

	const char *get_name();
	const char *get_display_name();
	const char *get_filename();

	uint64_t get_expected_size();
	uint64_t get_size();

	void remove();
	WebKitSecurityOrigin *get_security_origin();

private:
	WebKitSecurityOrigin* origin;
	const char* name;
	const char* displayName;
	const char* filename;
};


void webkit_remove_all_web_databases(void);

const char *webkit_get_web_database_directory_path(void);
void webkit_set_web_database_directory_path(const char *path);

uint64_t webkit_get_default_web_database_quota(void);
void webkit_set_default_web_database_quota(uint64_t defaultQuota);

#endif				/* webkitwebdatabase_h */
