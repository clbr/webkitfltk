/*
 * Copyright (C) 2008 Collabora Ltd.
 * Copyright (C) 2009 Gustavo Noronha Silva <gns@gnome.org>
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

#ifndef webkitdownload_h
#define webkitdownload_h

#include <webkit/webkitdefines.h>

#include "ResourceHandleClient.h"
#include "ResourceHandleInternal.h"
#include "ResourceRequest.h"
#include "ResourceResponse.h"
#include "SharedBuffer.h"
#include "webkitglobals.h"
#include "webkitglobalsprivate.h"
#include "webkitnetworkrequestprivate.h"
#include "webkitnetworkresponse.h"
#include "webkitnetworkresponseprivate.h"
#include <wtf/Noncopyable.h>
#include <wtf/text/CString.h>

using namespace WebKit;
using namespace WebCore;

/**
 * DownloadClientStatus:
 * @WEBKIT_DOWNLOAD_STATUS_ERROR: The download failed because of an error other than user cancellation.
 * @WEBKIT_DOWNLOAD_STATUS_CREATED: The download has not started yet.
 * @WEBKIT_DOWNLOAD_STATUS_STARTED: The download has started, but has not completed yet.
 * @WEBKIT_DOWNLOAD_STATUS_CANCELLED: The user canceled the download.
 * @WEBKIT_DOWNLOAD_STATUS_FINISHED: The download completed successfully.
 *
 * Enum values used to denote the various states of a download.
 **/
typedef enum {
	WEBKIT_DOWNLOAD_STATUS_ERROR = -1,
	WEBKIT_DOWNLOAD_STATUS_CREATED = 0,
	WEBKIT_DOWNLOAD_STATUS_STARTED,
	WEBKIT_DOWNLOAD_STATUS_CANCELLED,
	WEBKIT_DOWNLOAD_STATUS_FINISHED
} DownloadClientStatus;

/**
 * DownloadClientError:
 * @WEBKIT_DOWNLOAD_ERROR_CANCELLED_BY_USER: The download failed due to user cancellation.
 * @WEBKIT_DOWNLOAD_ERROR_DESTINATION: The download failed due to disk write failure.
 * @WEBKIT_DOWNLOAD_ERROR_NETWORK: The download failed due to a network error.
 *
 * Enum values used to denote the various download errors.
 **/
typedef enum {
	WEBKIT_DOWNLOAD_ERROR_CANCELLED_BY_USER,
	WEBKIT_DOWNLOAD_ERROR_DESTINATION,
	WEBKIT_DOWNLOAD_ERROR_NETWORK
} DownloadClientError;

/**
 * SECTION:webkitdownload
 * @short_description: Object used to communicate with the application when downloading.
 *
 * #DownloadClient carries information about a download request,
 * including a #WebKitNetworkRequest object. The application may use
 * this object to control the download process, or to simply figure
 * out what is to be downloaded, and do it itself.
 */

class DownloadClient:public ResourceHandleClient {
	WTF_MAKE_NONCOPYABLE(DownloadClient);

 public:
	DownloadClient();
	DownloadClient(WebKitNetworkRequest * request);
	DownloadClient(WebKitNetworkRequest *,
		       WebCore::ResourceHandle *,
		       const WebCore::ResourceResponse &);

	virtual void didReceiveResponse(ResourceHandle *,
					const ResourceResponse &) override;
	virtual void didReceiveData(ResourceHandle *, const char *, unsigned,
				    int) override;
	virtual void didReceiveBuffer(ResourceHandle *,
				      PassRefPtr < SharedBuffer > buffer,
				      int encodedLength) override;
	virtual void didFinishLoading(ResourceHandle *, double) override;
	virtual void didFail(ResourceHandle *, const ResourceError &) override;
	virtual void wasBlocked(ResourceHandle *) override;
	virtual void cannotShowURL(ResourceHandle *) override;

	void download_start();
	void download_cancel();
	const char *download_get_uri();
	WebKitNetworkRequest *download_get_network_request();
	WebKitNetworkResponse *download_get_network_response();
	const char *download_get_suggested_filename();
	const char *download_get_destination_uri();
	void download_set_destination_uri(const char *destination_uri);
	double download_get_progress();
	double download_get_elapsed_time();
	uint64_t download_get_total_size();
	uint64_t download_get_current_size();
	DownloadClientStatus download_get_status();

	void set_suggested_filename(const char *suggestedFilename);

 private:
	char *destinationURI;
	char *suggestedFilename;
	uint64_t currentSize;
	struct timeval start;
	DownloadClientStatus status;
	FILE *output;
	DownloadClient *downloadClient;
	WebKitNetworkRequest *networkRequest;
	WebKitNetworkResponse *networkResponse;
	 RefPtr < ResourceHandle > resourceHandle;

	void download_error(const ResourceError & error);
	void download_received_data(const char *data, int length);
	void close_stream();
};

#endif
