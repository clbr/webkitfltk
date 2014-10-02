/*
 * Copyright (C) 2013-2014 Apple Inc. All rights reserved.
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
#include "MainFrame.h"

#include "PageOverlayController.h"
#include "ScrollLatchingState.h"
#include "WheelEventDeltaTracker.h"

namespace WebCore {

inline MainFrame::MainFrame(Page& page, FrameLoaderClient& client)
    : Frame(page, nullptr, client)
    , m_selfOnlyRefCount(0)
#if PLATFORM(MAC)
    , m_latchingState(std::make_unique<ScrollLatchingState>())
#endif
    , m_recentWheelEventDeltaTracker(std::make_unique<WheelEventDeltaTracker>())
    , m_pageOverlayController(std::make_unique<PageOverlayController>(*this))
{
}

MainFrame::~MainFrame()
{
}

RefPtr<MainFrame> MainFrame::create(Page& page, FrameLoaderClient& client)
{
    return adoptRef(new MainFrame(page, client));
}

void MainFrame::selfOnlyRef()
{
    if (m_selfOnlyRefCount++)
        return;

    ref();
}

void MainFrame::selfOnlyDeref()
{
    ASSERT(m_selfOnlyRefCount);
    if (--m_selfOnlyRefCount)
        return;

    if (hasOneRef())
        dropChildren();

    deref();
}

void MainFrame::dropChildren()
{
    while (Frame* child = tree().firstChild())
        tree().removeChild(child);
}

#if PLATFORM(MAC)
void MainFrame::resetLatchingState()
{
    if (!m_latchingState)
        return;

    m_latchingState->clear();
}
#endif

}
