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

#ifndef download_h
#define download_h

#include <CurlDownload.h>
#include <ResourceRequest.h>

class download: public WebCore::CurlDownloadListener {
public:
	download(const char *url, const char *file,
			const WebCore::ResourceRequest *req = NULL);
	virtual ~download();

	void stop();

	void didReceiveResponse() override;
	void didReceiveDataOfLength(int size) override;
	void didFinish() override;
	void didFail() override;

	void getStats(time_t *start, long long *size, long long *received) const;
	bool isFailed() const;
	bool isFinished() const;

	const char *url, *file;
private:
	WebCore::CurlDownload curl;
	time_t time;
	long long size, received;
	bool failed, finished;
};

#endif
