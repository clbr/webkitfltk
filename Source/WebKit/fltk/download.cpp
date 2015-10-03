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

#include "download.h"

#include <wtf/text/Base64.h>
#include <wtf/text/CString.h>
#include <string.h>

using namespace WTF;
using namespace WebCore;

extern void (*downloadfunc)(const char *url, const char *file);
extern void (*downloadrefreshfunc)();

static void saveData(const char *file, const char *data, const unsigned size) {
	FILE *f = fopen(file, "w");
	if (!f) return;

	fwrite(data, size, 1, f);

	fclose(f);
}

static unsigned handleData(const char *url, const char *file) {

	const char *comma = strchr(url, ',');
	if (!comma || comma - url < 6)
		return 0;

	comma -= 6;
	const bool base64 = strncmp(comma, "base64", 6) == 0;
	comma += 7;

	const String data = decodeURLEscapeSequences(comma);
	if (base64) {
		Vector<char> out;
		if (base64Decode(data, out, Base64IgnoreWhitespace) && out.size() > 0) {

			saveData(file, &out[0], out.size());

			return out.size();
		}
	} else {
		const CString utf8 = data.utf8();
		saveData(file, utf8.data(), utf8.length());
		return data.length();
	}

	return 0;
}

download::download(const char *url, const char *file,
			const ResourceRequest *req) {
	this->url = strdup(url);
	this->file = strdup(file);
	time = ::time(NULL);
	received = 0;
	size = -1;
	failed = finished = false;
	isData = false;

	// Is it a data: url? data:image/octet-stream;base64,iVBORw0KG
	if (!strncmp(url, "data:", 5)) {
		isData = true;

		size = received = handleData(url, file);

		if (size)
			didFinish();
		else
			didFail();

		return;
	}

	curl.init(this, URL(ParsedURLString, this->url));

	if (req)
		curl.addHeaders(*req);

	curl.setDestination(file);

	curl.start();
}

download::~download() {
	free((char *) url);
	free((char *) file);
}

void download::didReceiveResponse() {
	if (isData)
		return;

	const ResourceResponse &res = curl.getResponse();
	size = res.expectedContentLength();
}

void download::didReceiveDataOfLength(int size) {

	received += size;

	if (downloadrefreshfunc)
		downloadrefreshfunc();
}

void download::didFinish() {

	finished = true;

	if (downloadfunc)
		downloadfunc(url, file);
	if (downloadrefreshfunc)
		downloadrefreshfunc();
}

void download::didFail() {
	failed = true;
}

void download::stop() {
	if (isData)
		return;

	curl.cancel();
	failed = true;

	if (downloadrefreshfunc)
		downloadrefreshfunc();
}

bool download::isFailed() const {
	return failed;
}

bool download::isFinished() const {
	return finished;
}

void download::getStats(time_t *start, long long *size, long long *received) const {

	*start = time;
	*size = this->size;
	*received = this->received;
}
