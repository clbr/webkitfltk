/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "JSTestOverloadedConstructors.h"

#include "ExceptionCode.h"
#include "JSBlob.h"
#include "JSDOMBinding.h"
#include "TestOverloadedConstructors.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

// Attributes

JSC::EncodedJSValue jsTestOverloadedConstructorsConstructor(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);

class JSTestOverloadedConstructorsPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSTestOverloadedConstructorsPrototype* create(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSTestOverloadedConstructorsPrototype* ptr = new (NotNull, JSC::allocateCell<JSTestOverloadedConstructorsPrototype>(vm.heap)) JSTestOverloadedConstructorsPrototype(vm, globalObject, structure);
        ptr->finishCreation(vm);
        return ptr;
    }

    DECLARE_INFO;
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    JSTestOverloadedConstructorsPrototype(JSC::VM& vm, JSC::JSGlobalObject*, JSC::Structure* structure)
        : JSC::JSNonFinalObject(vm, structure)
    {
    }

    void finishCreation(JSC::VM&);
};

class JSTestOverloadedConstructorsConstructor : public DOMConstructorObject {
private:
    JSTestOverloadedConstructorsConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::VM&, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSTestOverloadedConstructorsConstructor* create(JSC::VM& vm, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSTestOverloadedConstructorsConstructor* ptr = new (NotNull, JSC::allocateCell<JSTestOverloadedConstructorsConstructor>(vm.heap)) JSTestOverloadedConstructorsConstructor(structure, globalObject);
        ptr->finishCreation(vm, globalObject);
        return ptr;
    }

    DECLARE_INFO;
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }
protected:
    static JSC::EncodedJSValue JSC_HOST_CALL constructJSTestOverloadedConstructors(JSC::ExecState*);
    static JSC::EncodedJSValue JSC_HOST_CALL constructJSTestOverloadedConstructors1(JSC::ExecState*);
    static JSC::EncodedJSValue JSC_HOST_CALL constructJSTestOverloadedConstructors2(JSC::ExecState*);
    static JSC::EncodedJSValue JSC_HOST_CALL constructJSTestOverloadedConstructors3(JSC::ExecState*);
    static JSC::EncodedJSValue JSC_HOST_CALL constructJSTestOverloadedConstructors4(JSC::ExecState*);
    static JSC::ConstructType getConstructData(JSC::JSCell*, JSC::ConstructData&);
};

EncodedJSValue JSC_HOST_CALL JSTestOverloadedConstructorsConstructor::constructJSTestOverloadedConstructors1(ExecState* exec)
{
    JSTestOverloadedConstructorsConstructor* castedThis = jsCast<JSTestOverloadedConstructorsConstructor*>(exec->callee());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    ArrayBuffer* arrayBuffer(toArrayBuffer(exec->argument(0)));
    if (UNLIKELY(exec->hadException()))
        return JSValue::encode(jsUndefined());
    RefPtr<TestOverloadedConstructors> object = TestOverloadedConstructors::create(arrayBuffer);
    return JSValue::encode(asObject(toJS(exec, castedThis->globalObject(), object.get())));
}

EncodedJSValue JSC_HOST_CALL JSTestOverloadedConstructorsConstructor::constructJSTestOverloadedConstructors2(ExecState* exec)
{
    JSTestOverloadedConstructorsConstructor* castedThis = jsCast<JSTestOverloadedConstructorsConstructor*>(exec->callee());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    RefPtr<ArrayBufferView> arrayBufferView(toArrayBufferView(exec->argument(0)));
    if (UNLIKELY(exec->hadException()))
        return JSValue::encode(jsUndefined());
    RefPtr<TestOverloadedConstructors> object = TestOverloadedConstructors::create(arrayBufferView);
    return JSValue::encode(asObject(toJS(exec, castedThis->globalObject(), object.get())));
}

EncodedJSValue JSC_HOST_CALL JSTestOverloadedConstructorsConstructor::constructJSTestOverloadedConstructors3(ExecState* exec)
{
    JSTestOverloadedConstructorsConstructor* castedThis = jsCast<JSTestOverloadedConstructorsConstructor*>(exec->callee());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    Blob* blob(JSBlob::toWrapped(exec->argument(0)));
    if (UNLIKELY(exec->hadException()))
        return JSValue::encode(jsUndefined());
    RefPtr<TestOverloadedConstructors> object = TestOverloadedConstructors::create(blob);
    return JSValue::encode(asObject(toJS(exec, castedThis->globalObject(), object.get())));
}

EncodedJSValue JSC_HOST_CALL JSTestOverloadedConstructorsConstructor::constructJSTestOverloadedConstructors4(ExecState* exec)
{
    JSTestOverloadedConstructorsConstructor* castedThis = jsCast<JSTestOverloadedConstructorsConstructor*>(exec->callee());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    const String& string(exec->argument(0).isEmpty() ? String() : exec->argument(0).toString(exec)->value(exec));
    if (UNLIKELY(exec->hadException()))
        return JSValue::encode(jsUndefined());
    RefPtr<TestOverloadedConstructors> object = TestOverloadedConstructors::create(string);
    return JSValue::encode(asObject(toJS(exec, castedThis->globalObject(), object.get())));
}

EncodedJSValue JSC_HOST_CALL JSTestOverloadedConstructorsConstructor::constructJSTestOverloadedConstructors(ExecState* exec)
{
    size_t argsCount = exec->argumentCount();
    JSValue arg0(exec->argument(0));
    if ((argsCount == 1 && (arg0.isObject() && asObject(arg0)->inherits(JSArrayBuffer::info()))))
        return JSTestOverloadedConstructorsConstructor::constructJSTestOverloadedConstructors1(exec);
    if ((argsCount == 1 && (arg0.isObject() && asObject(arg0)->inherits(JSArrayBufferView::info()))))
        return JSTestOverloadedConstructorsConstructor::constructJSTestOverloadedConstructors2(exec);
    if ((argsCount == 1 && (arg0.isObject() && asObject(arg0)->inherits(JSBlob::info()))))
        return JSTestOverloadedConstructorsConstructor::constructJSTestOverloadedConstructors3(exec);
    if (argsCount == 1)
        return JSTestOverloadedConstructorsConstructor::constructJSTestOverloadedConstructors4(exec);
    if (argsCount < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    return throwVMTypeError(exec);
}

const ClassInfo JSTestOverloadedConstructorsConstructor::s_info = { "TestOverloadedConstructorsConstructor", &Base::s_info, 0, CREATE_METHOD_TABLE(JSTestOverloadedConstructorsConstructor) };

JSTestOverloadedConstructorsConstructor::JSTestOverloadedConstructorsConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSTestOverloadedConstructorsConstructor::finishCreation(VM& vm, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(vm);
    ASSERT(inherits(info()));
    putDirect(vm, vm.propertyNames->prototype, JSTestOverloadedConstructors::getPrototype(vm, globalObject), DontDelete | ReadOnly);
    putDirect(vm, vm.propertyNames->length, jsNumber(1), ReadOnly | DontDelete | DontEnum);
}

ConstructType JSTestOverloadedConstructorsConstructor::getConstructData(JSCell*, ConstructData& constructData)
{
    constructData.native.function = constructJSTestOverloadedConstructors;
    return ConstructTypeHost;
}

/* Hash table for prototype */

static const HashTableValue JSTestOverloadedConstructorsPrototypeTableValues[] =
{
    { "constructor", DontEnum | ReadOnly, NoIntrinsic, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestOverloadedConstructorsConstructor), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(0) },
};

WEBCORE_EXPORT const ClassInfo JSTestOverloadedConstructorsPrototype::s_info = { "TestOverloadedConstructorsPrototype", &Base::s_info, 0, CREATE_METHOD_TABLE(JSTestOverloadedConstructorsPrototype) };

void JSTestOverloadedConstructorsPrototype::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    reifyStaticProperties(vm, JSTestOverloadedConstructorsPrototypeTableValues, *this);
}

WEBCORE_EXPORT const ClassInfo JSTestOverloadedConstructors::s_info = { "TestOverloadedConstructors", &Base::s_info, 0, CREATE_METHOD_TABLE(JSTestOverloadedConstructors) };

JSTestOverloadedConstructors::JSTestOverloadedConstructors(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<TestOverloadedConstructors> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

JSObject* JSTestOverloadedConstructors::createPrototype(VM& vm, JSGlobalObject* globalObject)
{
    return JSTestOverloadedConstructorsPrototype::create(vm, globalObject, JSTestOverloadedConstructorsPrototype::createStructure(vm, globalObject, globalObject->objectPrototype()));
}

JSObject* JSTestOverloadedConstructors::getPrototype(VM& vm, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSTestOverloadedConstructors>(vm, globalObject);
}

void JSTestOverloadedConstructors::destroy(JSC::JSCell* cell)
{
    JSTestOverloadedConstructors* thisObject = static_cast<JSTestOverloadedConstructors*>(cell);
    thisObject->JSTestOverloadedConstructors::~JSTestOverloadedConstructors();
}

JSTestOverloadedConstructors::~JSTestOverloadedConstructors()
{
    releaseImplIfNotNull();
}

EncodedJSValue jsTestOverloadedConstructorsConstructor(ExecState* exec, JSObject* baseValue, EncodedJSValue, PropertyName)
{
    JSTestOverloadedConstructorsPrototype* domObject = jsDynamicCast<JSTestOverloadedConstructorsPrototype*>(baseValue);
    if (!domObject)
        return throwVMTypeError(exec);
    return JSValue::encode(JSTestOverloadedConstructors::getConstructor(exec->vm(), domObject->globalObject()));
}

JSValue JSTestOverloadedConstructors::getConstructor(VM& vm, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestOverloadedConstructorsConstructor>(vm, jsCast<JSDOMGlobalObject*>(globalObject));
}

bool JSTestOverloadedConstructorsOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    UNUSED_PARAM(handle);
    UNUSED_PARAM(visitor);
    return false;
}

void JSTestOverloadedConstructorsOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSTestOverloadedConstructors* jsTestOverloadedConstructors = jsCast<JSTestOverloadedConstructors*>(handle.slot()->asCell());
    DOMWrapperWorld& world = *static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, &jsTestOverloadedConstructors->impl(), jsTestOverloadedConstructors);
    jsTestOverloadedConstructors->releaseImpl();
}

#if ENABLE(BINDING_INTEGRITY)
#if PLATFORM(WIN)
#pragma warning(disable: 4483)
extern "C" { extern void (*const __identifier("??_7TestOverloadedConstructors@WebCore@@6B@")[])(); }
#else
extern "C" { extern void* _ZTVN7WebCore26TestOverloadedConstructorsE[]; }
#endif
#endif
JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject* globalObject, TestOverloadedConstructors* impl)
{
    if (!impl)
        return jsNull();
    if (JSValue result = getExistingWrapper<JSTestOverloadedConstructors>(globalObject, impl))
        return result;

#if ENABLE(BINDING_INTEGRITY)
    void* actualVTablePointer = *(reinterpret_cast<void**>(impl));
#if PLATFORM(WIN)
    void* expectedVTablePointer = reinterpret_cast<void*>(__identifier("??_7TestOverloadedConstructors@WebCore@@6B@"));
#else
    void* expectedVTablePointer = &_ZTVN7WebCore26TestOverloadedConstructorsE[2];
#if COMPILER(CLANG)
    // If this fails TestOverloadedConstructors does not have a vtable, so you need to add the
    // ImplementationLacksVTable attribute to the interface definition
    COMPILE_ASSERT(__is_polymorphic(TestOverloadedConstructors), TestOverloadedConstructors_is_not_polymorphic);
#endif
#endif
    // If you hit this assertion you either have a use after free bug, or
    // TestOverloadedConstructors has subclasses. If TestOverloadedConstructors has subclasses that get passed
    // to toJS() we currently require TestOverloadedConstructors you to opt out of binding hardening
    // by adding the SkipVTableValidation attribute to the interface IDL definition
    RELEASE_ASSERT(actualVTablePointer == expectedVTablePointer);
#endif
    return createNewWrapper<JSTestOverloadedConstructors>(globalObject, impl);
}

TestOverloadedConstructors* JSTestOverloadedConstructors::toWrapped(JSC::JSValue value)
{
    if (auto* wrapper = jsDynamicCast<JSTestOverloadedConstructors*>(value))
        return &wrapper->impl();
    return nullptr;
}

}
