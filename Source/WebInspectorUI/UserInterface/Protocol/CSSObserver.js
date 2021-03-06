/*
 * Copyright (C) 2013, 2015 Apple Inc. All rights reserved.
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

WebInspector.CSSObserver = class CSSObserver
{
    // Events defined by the "CSS" domain.

    mediaQueryResultChanged()
    {
        WebInspector.cssStyleManager.mediaQueryResultChanged();
    }

    styleSheetChanged(styleSheetId)
    {
        WebInspector.cssStyleManager.styleSheetChanged(styleSheetId);
    }

    styleSheetAdded(header)
    {
        // FIXME: Not implemented. <rdar://problem/13213680>
    }

    styleSheetRemoved(header)
    {
        // FIXME: Not implemented. <rdar://problem/13213680>
    }

    namedFlowCreated(namedFlow)
    {
        WebInspector.domTreeManager.namedFlowCreated(namedFlow);
    }

    namedFlowRemoved(documentNodeId, flowName)
    {
        WebInspector.domTreeManager.namedFlowRemoved(documentNodeId, flowName);
    }

    // COMPATIBILITY (iOS 7): regionLayoutUpdated was removed and replaced by regionOversetChanged.
    regionLayoutUpdated(namedFlow)
    {
        WebInspector.domTreeManager.regionLayoutUpdated(namedFlow);
    }

    regionOversetChanged(namedFlow)
    {
        WebInspector.domTreeManager.regionOversetChanged(namedFlow);
    }

    registeredNamedFlowContentElement(documentNodeId, flowName, contentNodeId, nextContentElementNodeId)
    {
        WebInspector.domTreeManager.registeredNamedFlowContentElement(documentNodeId, flowName, contentNodeId, nextContentElementNodeId);
    }

    unregisteredNamedFlowContentElement(documentNodeId, flowName, contentNodeId)
    {
        WebInspector.domTreeManager.unregisteredNamedFlowContentElement(documentNodeId, flowName, contentNodeId);
    }
};
