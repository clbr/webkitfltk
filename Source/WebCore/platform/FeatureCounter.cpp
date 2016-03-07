/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "FeatureCounter.h"

#include "Page.h"
#include "Settings.h"

namespace WebCore {

bool FeatureCounter::shouldUseForPage(Page* page)
{
    if (!page)
        return false;

    // Log only if the setting is enabled.
    if (!page->settings().featureCounterEnabled())
        return false;

    // Do not log if this is a private session.
    if (page->usesEphemeralSession())
        return false;

    return true;
}

#if !PLATFORM(IOS) || !USE(APPLE_INTERNAL_SDK)

void FeatureCounter::incrementKey(Page* page, const char* const key)
{
    UNUSED_PARAM(page);
    ASSERT_UNUSED(key, key);
}

void FeatureCounter::setKey(Page* page, const char* const key, int64_t value)
{
    UNUSED_PARAM(page);
    ASSERT_UNUSED(key, key);
    UNUSED_PARAM(value);
}

#endif

} // namespace WebCore
