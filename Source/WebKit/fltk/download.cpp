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

#include <string.h>

using namespace WTF;
using namespace WebCore;

extern void (*downloadfunc)(const char *url, const char *file);

download::download(const char *url, const char *file,
			const ResourceRequest *req) {
	this->url = strdup(url);
	this->file = strdup(file);

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

}

void download::didReceiveDataOfLength(int size) {

}

void download::didFinish() {
	if (downloadfunc)
		downloadfunc(url, file);
}

void download::didFail() {

}

void download::stop() {
	curl.cancel();
}
