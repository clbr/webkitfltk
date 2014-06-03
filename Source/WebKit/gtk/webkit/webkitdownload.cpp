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

#include "config.h"
#include "webkitdownload.h"

#include "ErrorsFLTK.h"
#include "NotImplemented.h"

DownloadClient::DownloadClient() {
	currentSize = 0;
	status = WEBKIT_DOWNLOAD_STATUS_CREATED;
	output = NULL;

	webkitInit();
}

DownloadClient::DownloadClient(WebKitNetworkRequest *request) {
	DownloadClient();
}

DownloadClient::DownloadClient(WebKitNetworkRequest *request,
				WebCore::ResourceHandle* resource,
				const WebCore::ResourceResponse &response) {
	DownloadClient();
}

static void webkitDownloadEmitError(DownloadClient* download, const ResourceError& error)
{
/*	  DownloadClientError errorCode;
	switch (error.errorCode()) {
	case DownloadErrorNetwork:
		errorCode = WEBKIT_DOWNLOAD_ERROR_NETWORK;
		break;
	case DownloadErrorCancelledByUser:
		errorCode = WEBKIT_DOWNLOAD_ERROR_CANCELLED_BY_USER;
		break;
	case DownloadErrorDestination:
		errorCode = WEBKIT_DOWNLOAD_ERROR_DESTINATION;
		break;
	default:
		g_assert_not_reached();
	}

	bool handled;
	g_signal_emit_by_name(download, "error", 0, errorCode, error.localizedDescription().utf8().data(), &handled);
	*/
}

static bool webkit_download_open_stream_for_uri(DownloadClient* download, const char* uri, bool append=FALSE)
{
/*
	DownloadClientPrivate* priv = download->priv;
	GRefPtr<GFile> file = adoptGRef(g_file_new_for_uri(uri));
	GUniqueOutPtr<GError> error;

	if (append)
		priv->outputStream = g_file_append_to(file.get(), G_FILE_CREATE_NONE, NULL, &error.outPtr());
	else
		priv->outputStream = g_file_replace(file.get(), NULL, TRUE, G_FILE_CREATE_NONE, NULL, &error.outPtr());

	if (error) {
		webkitDownloadEmitError(download, downloadDestinationError(core(priv->networkResponse), error->message));
		return FALSE;
	}

	GRefPtr<GFileInfo> info = adoptGRef(g_file_info_new());
	const char* uri_string = webkit_download_get_uri(download);
	g_file_info_set_attribute_string(info.get(), "metadata::download-uri", uri_string);
	g_file_info_set_attribute_string(info.get(), "xattr::xdg.origin.url", uri_string);
	g_file_set_attributes_async(file.get(), info.get(), G_FILE_QUERY_INFO_NONE, G_PRIORITY_DEFAULT, 0, 0, 0);
*/
	return TRUE;
}

/**
 * webkit_download_start:
 * @download: the #DownloadClient
 *
 * Initiates the download. Notice that you must have set the
 * destination-uri property before calling this method.
 *
 * Since: 1.1.2
 */
void DownloadClient::download_start()
{
#if 0
	DownloadClientPrivate* priv = download->priv;
	g_return_if_fail(priv->destinationURI);
	g_return_if_fail(priv->status == WEBKIT_DOWNLOAD_STATUS_CREATED);
	g_return_if_fail(priv->timer == NULL);

	// For GTK, when downloading a file NetworkingContext is null
	if (!priv->resourceHandle)
		priv->resourceHandle = ResourceHandle::create(/* Null NetworkingContext */ NULL, core(priv->networkRequest), priv->downloadClient, false, false);
	else {
		priv->resourceHandle->setClient(priv->downloadClient);
		priv->resourceHandle->setDefersLoading(false);
	}

	priv->timer = g_timer_new();
	webkit_download_open_stream_for_uri(download, priv->destinationURI);
#endif
}

/**
 * webkit_download_cancel:
 * @download: the #DownloadClient
 *
 * Cancels the download. Calling this will not free the
 * #DownloadClient object, so you still need to call
 * g_object_unref() on it, if you are the owner of a reference. Notice
 * that cancelling the download provokes the emission of the
 * DownloadClient::error signal, reporting that the download was
 * cancelled.
 *
 * Since: 1.1.2
 */
void DownloadClient::download_cancel()
{
#if 0
	DownloadClientPrivate* priv = download->priv;

	// Cancel may be called even if start was not called, so we need
	// to make sure timer is non-NULL.
	if (priv->timer)
		g_timer_stop(priv->timer);

	if (priv->resourceHandle)
		priv->resourceHandle->cancel();

	webkit_download_set_status(download, WEBKIT_DOWNLOAD_STATUS_CANCELLED);
	webkitDownloadEmitError(download, downloadCancelledByUserError(core(priv->networkResponse)));
#endif
}

/**
 * webkit_download_get_uri:
 * @download: the #DownloadClient
 *
 * Convenience method to retrieve the URI from the
 * #WebKitNetworkRequest which is being downloaded.
 *
 * Returns: the URI
 *
 * Since: 1.1.2
 */
const char* DownloadClient::download_get_uri()
{
	return webkit_network_request_get_uri(networkRequest);
}

/**
 * webkit_download_get_network_request:
 * @download: the #DownloadClient
 *
 * Retrieves the #WebKitNetworkRequest object that backs the download
 * process.
 *
 * Returns: (transfer none): the #WebKitNetworkRequest instance
 *
 * Since: 1.1.2
 */
WebKitNetworkRequest* DownloadClient::download_get_network_request()
{
	return networkRequest;
}

/**
 * webkit_download_get_network_response:
 * @download: the #DownloadClient
 *
 * Retrieves the #WebKitNetworkResponse object that backs the download
 * process.
 *
 * Returns: (transfer none): the #WebKitNetworkResponse instance
 *
 * Since: 1.1.16
 */
WebKitNetworkResponse* DownloadClient::download_get_network_response()
{
	return networkResponse;
}

static void webkit_download_set_response(DownloadClient* download, const ResourceResponse& response)
{
/*
	DownloadClientPrivate* priv = download->priv;
	priv->networkResponse = kitNew(response);

	if (!response.isNull() && !response.suggestedFilename().isEmpty())
		webkit_download_set_suggested_filename(download, response.suggestedFilename().utf8().data());
	*/
}

/**
 * webkit_download_get_suggested_filename:
 * @download: the #DownloadClient
 *
 * Retrieves the filename that was suggested by the server, or the one
 * derived by WebKit from the URI.
 *
 * Returns: the suggested filename
 *
 * Since: 1.1.2
 */
const char* DownloadClient::download_get_suggested_filename()
{
/*
	DownloadClientPrivate* priv = download->priv;
	if (priv->suggestedFilename)
		return priv->suggestedFilename;

	URL url = URL(URL(), webkit_network_request_get_uri(priv->networkRequest));
	url.setQuery(String());
	url.removeFragmentIdentifier();
	priv->suggestedFilename = strdup(decodeURLEscapeSequences(url.lastPathComponent()).utf8().data());
	return priv->suggestedFilename;*/
}

// for internal use only
void DownloadClient::download_set_suggested_filename(const char* suggestedFilename)
{
	free(suggestedFilename);
	suggestedFilename = strdup(suggestedFilename);
}


/**
 * webkit_download_get_destination_uri:
 * @download: the #DownloadClient
 *
 * Obtains the URI to which the downloaded file will be written. This
 * must have been set by the application before calling
 * webkit_download_start(), and may be %NULL.
 *
 * Returns: the destination URI or %NULL
 *
 * Since: 1.1.2
 */
const char* DownloadClient::download_get_destination_uri()
{
/*	  g_return_val_if_fail(WEBKIT_IS_DOWNLOAD(download), NULL);

	DownloadClientPrivate* priv = download->priv;
	return priv->destinationURI;*/
}

/**
 * webkit_download_set_destination_uri:
 * @download: the #DownloadClient
 * @destination_uri: the destination URI
 *
 * Defines the URI that should be used to save the downloaded file to.
 *
 * Since: 1.1.2
 */
void DownloadClient::download_set_destination_uri(const char* destination_uri)
{
/*
	g_return_if_fail(WEBKIT_IS_DOWNLOAD(download));
	g_return_if_fail(destination_uri);

	DownloadClientPrivate* priv = download->priv;
	if (priv->destinationURI && !strcmp(priv->destinationURI, destination_uri))
		return;

	if (priv->status != WEBKIT_DOWNLOAD_STATUS_CREATED && priv->status != WEBKIT_DOWNLOAD_STATUS_CANCELLED) {
		ASSERT(priv->destinationURI);

		bool downloading = priv->outputStream != NULL;
		if (downloading)
			webkit_download_close_stream(download);

		GRefPtr<GFile> src = adoptGRef(g_file_new_for_uri(priv->destinationURI));
		GRefPtr<GFile> dest = adoptGRef(g_file_new_for_uri(destination_uri));
		GUniqueOutPtr<GError> error;

		g_file_move(src.get(), dest.get(), G_FILE_COPY_BACKUP, 0, 0, 0, &error.outPtr());

		free(priv->destinationURI);
		priv->destinationURI = strdup(destination_uri);

		if (error) {
			webkitDownloadEmitError(download, downloadDestinationError(core(priv->networkResponse), error->message));
			return;
		}

		if (downloading) {
			if (!webkit_download_open_stream_for_uri(download, destination_uri, TRUE)) {
				webkit_download_cancel(download);
				return;
			}
		}
	} else {
		free(priv->destinationURI);
		priv->destinationURI = strdup(destination_uri);
	}

	// Only notify change if everything went fine.
	g_object_notify(G_OBJECT(download), "destination-uri");*/
}

void DownloadClient::close_stream() {
	fclose(output);
	output = NULL;
}

/**
 * webkit_download_get_status:
 * @download: the #DownloadClient
 *
 * Obtains the current status of the download, as a
 * #DownloadClientStatus.
 *
 * Returns: the current #DownloadClientStatus
 *
 * Since: 1.1.2
 */
DownloadClientStatus DownloadClient::download_get_status()
{
	return status;
}

/**
 * webkit_download_get_total_size:
 * @download: the #DownloadClient
 *
 * Returns the expected total size of the download. This is expected
 * because the server may provide incorrect or missing
 * Content-Length. Notice that this may grow over time, as it will be
 * always the same as current_size in the cases where current size
 * surpasses it.
 *
 * Returns: the expected total size of the downloaded file
 *
 * Since: 1.1.2
 */
uint64_t DownloadClient::download_get_total_size()
{
/*
	DownloadClientPrivate* priv = download->priv;
	SoupMessage* message = priv->networkResponse ? webkit_network_response_get_message(priv->networkResponse) : NULL;

	if (!message)
		return 0;

	return MAX(priv->currentSize, static_cast<uint64_t>(soup_message_headers_get_content_length(message->response_headers)));
	*/
}

/**
 * webkit_download_get_current_size:
 * @download: the #DownloadClient
 *
 * Current already downloaded size.
 *
 * Returns: the already downloaded size
 *
 * Since: 1.1.2
 */
uint64_t DownloadClient::download_get_current_size()
{
	return currentSize;
}

/**
 * webkit_download_get_progress:
 * @download: a #DownloadClient
 *
 * Determines the current progress of the download.
 *
 * Returns: a #double ranging from 0.0 to 1.0.
 *
 * Since: 1.1.2
 */
double DownloadClient::download_get_progress()
{
	if (!download->networkResponse)
		return 0.0;

	double total_size = static_cast<double>(webkit_download_get_total_size(download));

	if (total_size == 0)
		return 1.0;

	return ((double)currentSize) / total_size;
}

/**
 * webkit_download_get_elapsed_time:
 * @download: a #DownloadClient
 *
 * Elapsed time for the download in seconds, including any fractional
 * part. If the download is finished, had an error or was cancelled
 * this is the time between its start and the event.
 *
 * Returns: seconds since the download was started, as a #double
 *
 * Since: 1.1.2
 */
double DownloadClient::download_get_elapsed_time()
{
/*
	DownloadClientPrivate* priv = download->priv;
	if (!priv->timer)
		return 0;

	return g_timer_elapsed(priv->timer, NULL);*/
}

void DownloadClient::download_received_data(const char* data, int length)
{
	if (currentSize == 0)
		status = WEBKIT_DOWNLOAD_STATUS_STARTED;

	ssize_t bytes_written;
/*
	g_output_stream_write_all(G_OUTPUT_STREAM(download->outputStream),
							  data, length, &bytes_written, NULL, &error.outPtr());

	if (error) {
		webkitDownloadEmitError(download, downloadDestinationError(core(download->networkResponse), error->message));
		return;
	}

	download->currentSize += length;
	g_object_notify(G_OBJECT(download), "current-size");

	ASSERT(download->networkResponse);
	if (download->currentSize > webkit_download_get_total_size(download))
		g_object_notify(G_OBJECT(download), "total-size");

	// Throttle progress notification to not consume high amounts of
	// CPU on fast links, except when the last notification occured
	// in more then 0.7 secs from now, or the last notified progress
	// is passed in 1% or we reached the end.
	static double lastProgress = 0;
	static double lastElapsed = 0;
	double currentElapsed = g_timer_elapsed(download->timer, NULL);
	double currentProgress = webkit_download_get_progress(download);

	if (lastElapsed
		&& lastProgress
		&& (currentElapsed - lastElapsed) < 0.7
		&& (currentProgress - lastProgress) < 0.01
		&& currentProgress < 1.0) {
		return;
	}
	lastElapsed = currentElapsed;
	lastProgress = currentProgress;
*/
}

void DownloadClient::download_error(const ResourceError& error)
{
/*	  webkit_download_close_stream(download);

	DownloadClientPrivate* priv = download->priv;
	GRefPtr<DownloadClient> protect(download);

	g_timer_stop(download->timer);
	webkit_download_set_status(download, WEBKIT_DOWNLOAD_STATUS_ERROR);
	webkitDownloadEmitError(download, downloadNetworkError(error));*/
}

void DownloadClient::didReceiveResponse(ResourceHandle*, const ResourceResponse& response)
{
	webkit_download_set_response(m_download, response);
	if (response.httpStatusCode() >= 400) {
		m_download->download->resourceHandle->cancel();
		download_error(ResourceError(errorDomainDownload, response.httpStatusCode(),
				response.url().string(), response.httpStatusText()));
	}
}

void DownloadClient::didReceiveData(ResourceHandle*, const char* data, unsigned length, int encodedDataLength)
{
	ASSERT_NOT_REACHED();
}

void DownloadClient::didReceiveBuffer(ResourceHandle*, PassRefPtr<SharedBuffer> buffer, int encodedLength)
{
	// This pattern is suggested by SharedBuffer.h.
	const char* segment;
	unsigned position = 0;
	while (unsigned length = buffer->getSomeData(segment, position)) {
		download_received_data(segment, length);
		position += length;
	}
}

void DownloadClient::didFinishLoading(ResourceHandle*, double)
{
/*
	webkit_download_close_stream(download);

	DownloadClientPrivate* download = download->priv;

	g_object_notify(G_OBJECT(download), "progress");
	webkit_download_set_status(download, WEBKIT_DOWNLOAD_STATUS_FINISHED);*/
}

void DownloadClient::didFail(ResourceHandle*, const ResourceError& error)
{
	download_error(error);
}

void DownloadClient::wasBlocked(ResourceHandle*)
{
	// FIXME: Implement this when we have the new frame loader signals
	// and error handling.
	notImplemented();
}

void DownloadClient::cannotShowURL(ResourceHandle*)
{
	// FIXME: Implement this when we have the new frame loader signals
	// and error handling.
	notImplemented();
}
