/*
 * Copyright (C) 2014 Samsung Electronics
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "PlatformSpeechSynthesizer.h"

#if ENABLE(SPEECH_SYNTHESIS)

#include <PlatformSpeechSynthesisProviderEfl.h>
#include <PlatformSpeechSynthesisUtterance.h>
#include <PlatformSpeechSynthesisVoice.h>

namespace WebCore {

PlatformSpeechSynthesizer::PlatformSpeechSynthesizer(PlatformSpeechSynthesizerClient* client)
    : m_voiceListIsInitialized(false)
    , m_speechSynthesizerClient(client)
    , m_platformSpeechWrapper(std::make_unique<PlatformSpeechSynthesisProviderEfl>(this))
{
}

PlatformSpeechSynthesizer::~PlatformSpeechSynthesizer()
{
}

void PlatformSpeechSynthesizer::initializeVoiceList()
{
    ASSERT(m_platformSpeechWrapper);
    m_platformSpeechWrapper->initializeVoiceList(m_voiceList);
}

void PlatformSpeechSynthesizer::pause()
{
    ASSERT(m_platformSpeechWrapper);
    m_platformSpeechWrapper->pause();
}

void PlatformSpeechSynthesizer::resume()
{
    ASSERT(m_platformSpeechWrapper);
    m_platformSpeechWrapper->resume();
}

void PlatformSpeechSynthesizer::speak(PassRefPtr<PlatformSpeechSynthesisUtterance> utterance)
{
    ASSERT(m_platformSpeechWrapper);
    m_platformSpeechWrapper->speak(utterance);
}

void PlatformSpeechSynthesizer::cancel()
{
    ASSERT(m_platformSpeechWrapper);
    m_platformSpeechWrapper->cancel();
}

} // namespace WebCore

#endif // ENABLE(SPEECH_SYNTHESIS)
