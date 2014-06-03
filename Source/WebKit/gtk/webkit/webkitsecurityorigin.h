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

#ifndef webkitsecurityorigin_h
#define webkitsecurityorigin_h

#include "webkitwebdatabase.h"
#include <vector>

namespace WebCore {
	class SecurityOrigin;
}

class WebKitSecurityOrigin {
public:
	WebKitSecurityOrigin();
	~WebKitSecurityOrigin();

	const char *get_protocol();
	const char *get_host();
	unsigned get_port();
	uint64_t get_webdb_usage();
	uint64_t get_webdb_quota();
	void set_webdb_quota(uint64_t);
	std::vector<WebKitWebDatabase*> get_all_webdbs();
	WebKitWebDatabase *get_webdb(const char *);

private:
	WebCore::SecurityOrigin *coreOrigin;
	const char *protocol;
	const char *host;
	std::vector<WebKitWebDatabase*> webDatabases;

	bool disposed;
};

#endif				/* __WEBKIT_SECURITY_ORIGIN_H__ */
