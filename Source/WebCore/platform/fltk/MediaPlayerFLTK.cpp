/*
 * Copyright (C) 2014 Lauri Kasanen. All rights reserved.
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
#include "MediaPlayerFLTK.h"

#if ENABLE(FIFTH_VIDEO)

#include "ExceptionCodePlaceholder.h"
#include "MediaPlayer.h"
#include "MediaSourcePrivateClient.h"
#include <wtf/Functional.h>
#include <wtf/MainThread.h>
#include <wtf/text/WTFString.h>

namespace WebCore {

// MediaPlayer Enigne Support
void MediaPlayerFLTK::registerMediaEngine(MediaEngineRegistrar registrar)
{
    registrar(create, getSupportedTypes, supportsType, 0, 0, 0, 0);
}

PassOwnPtr<MediaPlayerPrivateInterface> MediaPlayerFLTK::create(MediaPlayer* player)
{
    return adoptPtr(new MediaPlayerFLTK(player));
}

static HashSet<String> mimeTypeCache()
{
    DEPRECATED_DEFINE_STATIC_LOCAL(HashSet<String>, cache, ());
    static bool isInitialized = false;

    if (!isInitialized) {
        isInitialized = true;

        const char * const mimeTypes[] = {
            "application/ogg",
            "application/vnd.apple.mpegurl",
            "application/vnd.rn-realmedia",
            "application/x-3gp",
            "application/x-pn-realaudio",
            "audio/3gpp",
            "audio/aac",
            "audio/flac",
            "audio/iLBC-sh",
            "audio/midi",
            "audio/mobile-xmf",
            "audio/mp1",
            "audio/mp2",
            "audio/mp3",
            "audio/mp4",
            "audio/mpeg",
            "audio/ogg",
            "audio/opus",
            "audio/qcelp",
            "audio/riff-midi",
            "audio/speex",
            "audio/wav",
            "audio/webm",
            "audio/x-ac3",
            "audio/x-aiff",
            "audio/x-amr-nb-sh",
            "audio/x-amr-wb-sh",
            "audio/x-au",
            "audio/x-ay",
            "audio/x-celt",
            "audio/x-dts",
            "audio/x-flac",
            "audio/x-gbs",
            "audio/x-gsm",
            "audio/x-gym",
            "audio/x-imelody",
            "audio/x-ircam",
            "audio/x-kss",
            "audio/x-m4a",
            "audio/x-mod",
            "audio/x-mp3",
            "audio/x-mpeg",
            "audio/x-musepack",
            "audio/x-nist",
            "audio/x-nsf",
            "audio/x-paris",
            "audio/x-sap",
            "audio/x-sbc",
            "audio/x-sds",
            "audio/x-shorten",
            "audio/x-sid",
            "audio/x-spc",
            "audio/x-speex",
            "audio/x-svx",
            "audio/x-ttafile",
            "audio/x-vgm",
            "audio/x-voc",
            "audio/x-vorbis+ogg",
            "audio/x-w64",
            "audio/x-wav",
            "audio/x-wavpack",
            "audio/x-wavpack-correction",
            "video/3gpp",
            "video/mj2",
            "video/mp4",
            "video/mpeg",
            "video/mpegts",
            "video/ogg",
            "video/quicktime",
            "video/vivo",
            "video/webm",
            "video/x-cdxa",
            "video/x-dirac",
            "video/x-dv",
            "video/x-fli",
            "video/x-flv",
            "video/x-h263",
            "video/x-ivf",
            "video/x-m4v",
            "video/x-matroska",
            "video/x-mng",
            "video/x-ms-asf",
            "video/x-msvideo",
            "video/x-mve",
            "video/x-nuv",
            "video/x-vcd"
        };

        for (unsigned i = 0; i < (sizeof(mimeTypes) / sizeof(*mimeTypes)); ++i)
            cache.add(String(mimeTypes[i]));
    }

    return cache;
}

void MediaPlayerFLTK::getSupportedTypes(HashSet<String>& supportedTypes)
{
    supportedTypes = mimeTypeCache();
}

MediaPlayer::SupportsType MediaPlayerFLTK::supportsType(const MediaEngineSupportParameters& parameters)
{
    return MediaPlayer::IsSupported;
}

MediaPlayerFLTK::MediaPlayerFLTK(MediaPlayer* player)
    : m_player(player)
    , m_currentTime(MediaTime::zeroTime())
    , m_duration(MediaTime::zeroTime())
    , m_readyState(MediaPlayer::HaveNothing)
    , m_networkState(MediaPlayer::Empty)
    , m_playing(false)
{
}

MediaPlayerFLTK::~MediaPlayerFLTK()
{
}

void MediaPlayerFLTK::load(const String&)
{
}

#if ENABLE(MEDIA_SOURCE)
void MediaPlayerFLTK::load(const String&, MediaSourcePrivateClient* source)
{
    m_mediaSource = source;
    m_mediaSourcePrivate = MockMediaSourcePrivate::create(this);
    m_mediaSource->setPrivateAndOpen(*m_mediaSourcePrivate);
}
#endif

void MediaPlayerFLTK::cancelLoad()
{
}

void MediaPlayerFLTK::play()
{
    m_playing = 1;
}

void MediaPlayerFLTK::pause()
{
    m_playing = 0;
}

IntSize MediaPlayerFLTK::naturalSize() const
{
    return IntSize();
}

bool MediaPlayerFLTK::hasVideo() const
{
    return false;
}

bool MediaPlayerFLTK::hasAudio() const
{
    return false;
}

void MediaPlayerFLTK::setVisible(bool)
{
}

bool MediaPlayerFLTK::seeking() const
{
    return false;
}

bool MediaPlayerFLTK::paused() const
{
    return !m_playing;
}

MediaPlayer::NetworkState MediaPlayerFLTK::networkState() const
{
    return m_networkState;
}

MediaPlayer::ReadyState MediaPlayerFLTK::readyState() const
{
    return m_readyState;
}

double MediaPlayerFLTK::maxTimeSeekableDouble() const
{
    return m_duration.toDouble();
}

std::unique_ptr<PlatformTimeRanges> MediaPlayerFLTK::buffered() const
{
//    if (m_mediaSource)
//        return m_mediaSource->buffered();

    return PlatformTimeRanges::create();
}

bool MediaPlayerFLTK::didLoadingProgress() const
{
    return false;
}

void MediaPlayerFLTK::setSize(const IntSize&)
{
}

void MediaPlayerFLTK::paint(GraphicsContext*, const IntRect&)
{
}

double MediaPlayerFLTK::currentTimeDouble() const
{
    return m_currentTime.toDouble();
}

double MediaPlayerFLTK::durationDouble() const
{
    return m_duration.toDouble();
}

void MediaPlayerFLTK::seekWithTolerance(double time, double negativeTolerance, double positiveTolerance)
{
}

void MediaPlayerFLTK::advanceCurrentTime()
{
}

void MediaPlayerFLTK::updateDuration(const MediaTime& duration)
{
    if (m_duration == duration)
        return;

    m_duration = duration;
    m_player->durationChanged();
}

void MediaPlayerFLTK::setReadyState(MediaPlayer::ReadyState readyState)
{
    if (readyState == m_readyState)
        return;

    m_readyState = readyState;
    m_player->readyStateChanged();
}

void MediaPlayerFLTK::setNetworkState(MediaPlayer::NetworkState networkState)
{
    if (networkState == m_networkState)
        return;

    m_networkState = networkState;
    m_player->networkStateChanged();
}

#if ENABLE(MEDIA_SOURCE)
unsigned long MediaPlayerFLTK::totalVideoFrames()
{
    return m_mediaSourcePrivate ? m_mediaSourcePrivate->totalVideoFrames() : 0;
}

unsigned long MediaPlayerFLTK::droppedVideoFrames()
{
    return m_mediaSourcePrivate ? m_mediaSourcePrivate->droppedVideoFrames() : 0;
}

unsigned long MediaPlayerFLTK::corruptedVideoFrames()
{
    return m_mediaSourcePrivate ? m_mediaSourcePrivate->corruptedVideoFrames() : 0;
}

double MediaPlayerFLTK::totalFrameDelay()
{
    return m_mediaSourcePrivate ? m_mediaSourcePrivate->totalFrameDelay() : 0;
}
#endif

}

#endif
