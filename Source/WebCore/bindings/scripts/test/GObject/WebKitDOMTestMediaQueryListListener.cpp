/*
 *  This file is part of the WebKit open source project.
 *  This file has been generated by generate-bindings.pl. DO NOT MODIFY!
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "WebKitDOMTestMediaQueryListListener.h"

#include "CSSImportRule.h"
#include "DOMObjectCache.h"
#include "Document.h"
#include "ExceptionCode.h"
#include "JSMainThreadExecState.h"
#include "WebKitDOMPrivate.h"
#include "WebKitDOMTestMediaQueryListListenerPrivate.h"
#include "gobject/ConvertToUTF8String.h"
#include <wtf/GetPtr.h>
#include <wtf/RefPtr.h>

#define WEBKIT_DOM_TEST_MEDIA_QUERY_LIST_LISTENER_GET_PRIVATE(obj) G_TYPE_INSTANCE_GET_PRIVATE(obj, WEBKIT_DOM_TYPE_TEST_MEDIA_QUERY_LIST_LISTENER, WebKitDOMTestMediaQueryListListenerPrivate)

typedef struct _WebKitDOMTestMediaQueryListListenerPrivate {
    RefPtr<WebCore::TestMediaQueryListListener> coreObject;
} WebKitDOMTestMediaQueryListListenerPrivate;

namespace WebKit {

WebKitDOMTestMediaQueryListListener* kit(WebCore::TestMediaQueryListListener* obj)
{
    if (!obj)
        return 0;

    if (gpointer ret = DOMObjectCache::get(obj))
        return WEBKIT_DOM_TEST_MEDIA_QUERY_LIST_LISTENER(ret);

    return wrapTestMediaQueryListListener(obj);
}

WebCore::TestMediaQueryListListener* core(WebKitDOMTestMediaQueryListListener* request)
{
    return request ? static_cast<WebCore::TestMediaQueryListListener*>(WEBKIT_DOM_OBJECT(request)->coreObject) : 0;
}

WebKitDOMTestMediaQueryListListener* wrapTestMediaQueryListListener(WebCore::TestMediaQueryListListener* coreObject)
{
    ASSERT(coreObject);
    return WEBKIT_DOM_TEST_MEDIA_QUERY_LIST_LISTENER(g_object_new(WEBKIT_DOM_TYPE_TEST_MEDIA_QUERY_LIST_LISTENER, "core-object", coreObject, NULL));
}

} // namespace WebKit

G_DEFINE_TYPE(WebKitDOMTestMediaQueryListListener, webkit_dom_test_media_query_list_listener, WEBKIT_DOM_TYPE_OBJECT)

static void webkit_dom_test_media_query_list_listener_finalize(GObject* object)
{
    WebKitDOMTestMediaQueryListListenerPrivate* priv = WEBKIT_DOM_TEST_MEDIA_QUERY_LIST_LISTENER_GET_PRIVATE(object);

    WebKit::DOMObjectCache::forget(priv->coreObject.get());

    priv->~WebKitDOMTestMediaQueryListListenerPrivate();
    G_OBJECT_CLASS(webkit_dom_test_media_query_list_listener_parent_class)->finalize(object);
}

static GObject* webkit_dom_test_media_query_list_listener_constructor(GType type, guint constructPropertiesCount, GObjectConstructParam* constructProperties)
{
    GObject* object = G_OBJECT_CLASS(webkit_dom_test_media_query_list_listener_parent_class)->constructor(type, constructPropertiesCount, constructProperties);

    WebKitDOMTestMediaQueryListListenerPrivate* priv = WEBKIT_DOM_TEST_MEDIA_QUERY_LIST_LISTENER_GET_PRIVATE(object);
    priv->coreObject = static_cast<WebCore::TestMediaQueryListListener*>(WEBKIT_DOM_OBJECT(object)->coreObject);
    WebKit::DOMObjectCache::put(priv->coreObject.get(), object);

    return object;
}

static void webkit_dom_test_media_query_list_listener_class_init(WebKitDOMTestMediaQueryListListenerClass* requestClass)
{
    GObjectClass* gobjectClass = G_OBJECT_CLASS(requestClass);
    g_type_class_add_private(gobjectClass, sizeof(WebKitDOMTestMediaQueryListListenerPrivate));
    gobjectClass->constructor = webkit_dom_test_media_query_list_listener_constructor;
    gobjectClass->finalize = webkit_dom_test_media_query_list_listener_finalize;
}

static void webkit_dom_test_media_query_list_listener_init(WebKitDOMTestMediaQueryListListener* request)
{
    WebKitDOMTestMediaQueryListListenerPrivate* priv = WEBKIT_DOM_TEST_MEDIA_QUERY_LIST_LISTENER_GET_PRIVATE(request);
    new (priv) WebKitDOMTestMediaQueryListListenerPrivate();
}

