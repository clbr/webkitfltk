/*
 * Copyright (C) 2014, 2015 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DFANode_h
#define DFANode_h

#if ENABLE(CONTENT_EXTENSIONS)

#include "ContentExtensionsDebugging.h"
#include <wtf/HashMap.h>
#include <wtf/Vector.h>

namespace WebCore {

namespace ContentExtensions {

// A DFANode abstract the transition table out of a DFA state. If a state is accepting, the DFANode also have
// the actions for that state.

typedef HashMap<uint16_t, unsigned, DefaultHash<uint16_t>::Hash, WTF::UnsignedWithZeroKeyHashTraits<uint16_t>> DFANodeTransitions;

class DFANode {
public:
    DFANodeTransitions transitions;
    bool hasFallbackTransition { false };
    bool isKilled { false };
    unsigned fallbackTransition;
    Vector<uint64_t> actions;

#if CONTENT_EXTENSIONS_STATE_MACHINE_DEBUGGING
    Vector<unsigned> correspondingNFANodes;
#endif
};

}

} // namespace WebCore

#endif // ENABLE(CONTENT_EXTENSIONS)

#endif // DFANode_h
