/*
 * Copyright (C) 2007, 2008, 2009, 2010, 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2011, 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#if ENABLE(FIFTH_VIDEO)
#include "MediaControlsFifth.h"

#include "ContextMenuClient.h"
#include "CSSValueKeywords.h"
#include "ExceptionCodePlaceholder.h"
#include "HTMLNames.h"
#include "MediaController.h"
#include "WheelEvent.h"

#include <FL/fl_ask.H>
#include <unistd.h>

extern const char *wk_stream_exec;

namespace WebCore {

MediaControlsFifth::MediaControlsFifth(Document& document)
    : MediaControls(document)
    , m_dlButton(0)
    , m_streamButton(0)
{
}

PassRefPtr<MediaControls> MediaControls::create(Document& document)
{
    return MediaControlsFifth::createControls(document);
}

typedef void (*eventfunc)(const char *url, ContextMenuClient *ctx);

class FlMediaEventListener: public EventListener {
public:
	FlMediaEventListener(MediaControlsFifth *ptr, eventfunc f,
				ContextMenuClient *ctx):
		EventListener(FlEventListenerType),
		m_ptr(ptr), m_ctx(ctx), m_func(f) {}

	void handleEvent(ScriptExecutionContext*, Event*) override {
		m_func(m_ptr->getVideoURL(), m_ctx);
	}

	bool operator==(const EventListener &other) {
		if (other.type() == FlEventListenerType) {
			const FlMediaEventListener &e = (const FlMediaEventListener &) other;
			return m_ptr == e.m_ptr && m_func == e.m_func;
		}
		return false;
	}

private:
	MediaControlsFifth *m_ptr;
	ContextMenuClient *m_ctx;
	eventfunc m_func;
};

static void dl(const char *url, ContextMenuClient *ctx) {
	if (!url || !ctx || strlen(url) < 4) {
		free((char *) url);
		fl_alert("Couldn't get the video url");
		return;
	}

	ctx->downloadURL(URL(URL(), url));

	free((char *) url);
}

static void stream(const char *url, ContextMenuClient *) {
	if (!url || strlen(url) < 4) {
		free((char *) url);
		fl_alert("Couldn't get the video url");
		return;
	}

	if (!wk_stream_exec) {
		free((char *) url);
		fl_alert("No streaming program set!");
		return;
	}

	if (fork() == 0) {
		char tmp[1360];
		snprintf(tmp, 1360, "%s %s", wk_stream_exec, url);
		tmp[1359] = '\0';
		system(tmp);
		exit(0);
	}

	free((char *) url);
}

using HTMLNames::inputTag;
using HTMLNames::valueAttr;

PassRefPtr<MediaControlsFifth> MediaControlsFifth::createControls(Document& document)
{
    if (!document.page())
        return 0;

    ContextMenuClient *ctx = document.page()->contextMenuClient();

    RefPtr<MediaControlsFifth> controls = adoptRef(new MediaControlsFifth(document));

    ExceptionCode ec;

    RefPtr<HTMLInputElement> dlButton = HTMLInputElement::create(inputTag, document, NULL, false);
    controls->m_dlButton = dlButton.get();
    dlButton->setAttribute(valueAttr, "Download");
    dlButton->setType("button");
    RefPtr<FlMediaEventListener> l(adoptRef(new FlMediaEventListener(controls.get(), dl, ctx)));
    dlButton->addEventListener("click", l.release(), false);
    controls->appendChild(dlButton.release(), ec);
    if (ec)
        return 0;

    RefPtr<HTMLInputElement> streamButton = HTMLInputElement::create(inputTag, document, NULL, false);
    controls->m_streamButton = streamButton.get();
    streamButton->setAttribute(valueAttr, "Stream");
    streamButton->setType("button");
    RefPtr<FlMediaEventListener> l2(adoptRef(new FlMediaEventListener(controls.get(), stream, ctx)));
    streamButton->addEventListener("click", l2.release(), false);
    controls->appendChild(streamButton.release(), ec);
    if (ec)
        return 0;

    RefPtr<MediaControlPanelElement> panel = MediaControlPanelElement::create(document);
    controls->m_panel = panel.get();
    controls->appendChild(panel.release(), ec);
    if (ec)
        return 0;

    return controls.release();
}

void MediaControlsFifth::setMediaController(MediaControllerInterface* controller)
{
    if (m_mediaController == controller)
        return;

    MediaControls::setMediaController(controller);
}

void MediaControlsFifth::defaultEventHandler(Event* event)
{
    MediaControls::defaultEventHandler(event);
}

void MediaControlsFifth::hide()
{
    MediaControls::hide();
}

void MediaControlsFifth::makeTransparent()
{
    MediaControls::makeTransparent();
}

void MediaControlsFifth::changedClosedCaptionsVisibility()
{
    MediaControls::changedClosedCaptionsVisibility();

}

void MediaControlsFifth::reset()
{
    Page* page = document().page();
    if (!page)
        return;

    makeOpaque();
}

void MediaControlsFifth::updateCurrentTimeDisplay()
{
}

void MediaControlsFifth::reportedError()
{
    Page* page = document().page();
    if (!page)
        return;
}

void MediaControlsFifth::updateStatusDisplay()
{
}

void MediaControlsFifth::loadedMetadata()
{
    MediaControls::loadedMetadata();
}

void MediaControlsFifth::changedMute()
{
    MediaControls::changedMute();
}

void MediaControlsFifth::changedVolume()
{
    MediaControls::changedVolume();
}

void MediaControlsFifth::enteredFullscreen()
{
    MediaControls::enteredFullscreen();
}

void MediaControlsFifth::exitedFullscreen()
{
    MediaControls::exitedFullscreen();
}

void MediaControlsFifth::showVolumeSlider()
{
    if (!m_mediaController->hasAudio())
        return;
}

void MediaControlsFifth::toggleClosedCaptionTrackList()
{
    if (!m_mediaController->hasClosedCaptions())
        return;
}

void MediaControlsFifth::showClosedCaptionTrackList()
{
}

void MediaControlsFifth::hideClosedCaptionTrackList()
{
}

void MediaControlsFifth::setFullscreenSliderVolume()
{
}

bool MediaControlsFifth::shouldClosedCaptionsContainerPreventPageScrolling(int wheelDeltaY)
{
    return false;
}

void MediaControlsFifth::handleClickEvent(Event* event)
{
    Node* currentTarget = event->currentTarget()->toNode();
    Node* target = event->target()->toNode();
}

void MediaControlsFifth::closedCaptionTracksChanged()
{
}

PassRefPtr<MediaControlsFifthEventListener> MediaControlsFifth::eventListener()
{
    if (!m_eventListener)
        m_eventListener = MediaControlsFifthEventListener::create(this);
    return m_eventListener;
}

// --------

void MediaControlsFifthEventListener::handleEvent(ScriptExecutionContext*, Event* event)
{
    if (event->type() == eventNames().clickEvent)
        m_mediaControls->handleClickEvent(event);
    else if ((event->type() == eventNames().wheelEvent || event->type() == eventNames().mousewheelEvent) && event->eventInterface() == WheelEventInterfaceType) {
        WheelEvent* wheelEvent = toWheelEvent(event);
        if (m_mediaControls->shouldClosedCaptionsContainerPreventPageScrolling(wheelEvent->wheelDeltaY()))
            event->preventDefault();
    }
}

bool MediaControlsFifthEventListener::operator==(const EventListener& listener)
{
    if (const MediaControlsFifthEventListener* mediaControlsFifthEventListener = MediaControlsFifthEventListener::cast(&listener))
        return m_mediaControls == mediaControlsFifthEventListener->m_mediaControls;
    return false;
}

const char *MediaControlsFifth::getVideoURL() const {
    if (!m_mediaController || !m_mediaController->hasCurrentSrc())
        return strdup("");

    return strdup(m_mediaController->currentSrc().string().utf8().data());
}

}

#endif
