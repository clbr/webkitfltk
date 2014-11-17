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

#include "dirlisting.h"

#include <dirent.h>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int filter(const struct dirent * const cur) {
	if (!strcmp(cur->d_name, ".") || !strcmp(cur->d_name, ".."))
		return 0;
	return 1;
}

#if defined(__GLIBC__) && __GLIBC_MINOR__ < 10
static int comp(const void *ap, const void *bp) {
#else
static int comp(const struct dirent **ap, const struct dirent **bp) {
#endif

	const struct dirent **a = (const struct dirent **) ap;
	const struct dirent **b = (const struct dirent **) bp;

	if ((*a)->d_type == DT_DIR && (*b)->d_type != DT_DIR)
		return -1;
	if ((*a)->d_type != DT_DIR && (*b)->d_type == DT_DIR)
		return 1;

	return alphasort(a, b);
}

const char *gendirlisting(const char * const dir) {

	// Using std::string since wtfstring is horribly inefficient in append.
	std::string str("<html><head><title>");
	str.reserve(16384);
	str += dir;
	str += "</title>";
	str += "<style>";
	str += "html { width: 100%; }\n"
		"body { background-color: #eee;"
			"width: 50%;"
			"margin-left: auto;"
			"margin-right: auto;"
			"margin-top: 2em; }\n"
		"table { width: 100%;"
			"border-collapse: collapse; }\n"
		"tr, td, th { border-top: 1px solid green;"
			"border-bottom: 1px solid green;"
			"padding: 0.1em; }\n"
		"th { padding: 0.2em;"
			"padding-bottom: 0.5em;"
			"text-align: right;"
			"background-color: #f0f0f0; }\n"
		"td { text-align: right; }\n"
		"th:first-child, td:first-child { text-align: left;"
			"width: 60%; }\n"
		"tr:nth-child(even) { background: #e0e0e0; }\n";
	str += "</style></head><body>\n";

	str += "<h1>Directory listing</h1>\n";
	str += "<h2>";
	str += dir;
	str += "</h2>";
	str += "<table>";
	str += "<tr><th>Name</th><th>Type</th><th>Size</th></tr>\n";

	struct dirent **names;
	const int num = scandir(dir, &names, filter, comp);

	if (num < 1)
		return strdup("");

	struct dirent *cur;
	int i;
	for (i = 0; i < num; i++) {
		bool isdir = false;
		cur = names[i];
		if (cur->d_type == DT_DIR)
			isdir = true;

		if (isdir)
			str += "<tr class=\"dir\"><td>";
		else
			str += "<tr><td>";

		std::string fullpath(dir);
		fullpath += "/";
		fullpath += cur->d_name;

		str += "<a href=\"file://";
		str += fullpath;
		str += "\">";
		str += cur->d_name;
		str += "</a></td><td>";

		if (isdir)
			str += "Directory";
		else
			str += "File";

		str += "</td><td>";


		struct stat st;
		unsigned long long size = 0;
		if (stat(fullpath.c_str(), &st) == 0)
			size = st.st_size;

		char tmp[80];
		if (size > 1024*1024*1024) {
			snprintf(tmp, 80, "%llu", size / (1024*1024*1024));
			str += tmp;
			str += " GB";
		} else if (size > 1024*1024) {
			snprintf(tmp, 80, "%llu", size / (1024*1024));
			str += tmp;
			str += " MB";
		} else if (size > 1024) {
			snprintf(tmp, 80, "%llu", size / (1024));
			str += tmp;
			str += " KB";
		} else {
			snprintf(tmp, 80, "%llu", size);
			str += tmp;
			str += " B";
		}

		str += "</td></tr>\n";

		free(cur);
	}
	free(names);

	str += "</table></body></html>";

	return strdup(str.c_str());
}
