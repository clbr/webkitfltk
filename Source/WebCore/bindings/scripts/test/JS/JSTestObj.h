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

#ifndef JSTestObj_h
#define JSTestObj_h

#include "JSDOMBinding.h"
#include "TestObj.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class JSTestObj : public JSDOMWrapper {
public:
    typedef JSDOMWrapper Base;
    static JSTestObj* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<TestObj> impl)
    {
        JSTestObj* ptr = new (NotNull, JSC::allocateCell<JSTestObj>(globalObject->vm().heap)) JSTestObj(structure, globalObject, impl);
        ptr->finishCreation(globalObject->vm());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::VM&, JSC::JSGlobalObject*);
    static bool getOwnPropertySlot(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);
    static void destroy(JSC::JSCell*);
    ~JSTestObj();
    DECLARE_INFO;

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

    static JSC::JSValue getConstructor(JSC::VM&, JSC::JSGlobalObject*);
    JSC::WriteBarrier<JSC::Unknown> m_cachedAttribute1;
    JSC::WriteBarrier<JSC::Unknown> m_cachedAttribute2;
    static void visitChildren(JSCell*, JSC::SlotVisitor&);


    // Custom attributes
    JSC::JSValue customAttr(JSC::ExecState*) const;
    void setCustomAttr(JSC::ExecState*, JSC::JSValue);

    // Custom functions
    JSC::JSValue customMethod(JSC::ExecState*);
    JSC::JSValue customMethodWithArgs(JSC::ExecState*);
    static JSC::JSValue classMethod2(JSC::ExecState*);
    TestObj& impl() const { return *m_impl; }
    void releaseImpl() { m_impl->deref(); m_impl = 0; }

    void releaseImplIfNotNull()
    {
        if (m_impl) {
            m_impl->deref();
            m_impl = 0;
        }
    }

private:
    TestObj* m_impl;
protected:
    JSTestObj(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<TestObj>);

    void finishCreation(JSC::VM& vm)
    {
        Base::finishCreation(vm);
        ASSERT(inherits(info()));
    }

    static const unsigned StructureFlags = JSC::InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | JSC::OverridesGetOwnPropertySlot | JSC::OverridesVisitChildren | Base::StructureFlags;
};

class JSTestObjOwner : public JSC::WeakHandleOwner {
public:
    virtual bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::SlotVisitor&);
    virtual void finalize(JSC::Handle<JSC::Unknown>, void* context);
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld&, TestObj*)
{
    DEPRECATED_DEFINE_STATIC_LOCAL(JSTestObjOwner, jsTestObjOwner, ());
    return &jsTestObjOwner;
}

inline void* wrapperContext(DOMWrapperWorld& world, TestObj*)
{
    return &world;
}

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, TestObj*);
TestObj* toTestObj(JSC::JSValue);

class JSTestObjPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::VM&, JSC::JSGlobalObject*);
    static JSTestObjPrototype* create(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSTestObjPrototype* ptr = new (NotNull, JSC::allocateCell<JSTestObjPrototype>(vm.heap)) JSTestObjPrototype(vm, globalObject, structure);
        ptr->finishCreation(vm);
        return ptr;
    }

    DECLARE_INFO;
    void finishCreation(JSC::VM&);
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    JSTestObjPrototype(JSC::VM& vm, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(vm, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesVisitChildren | Base::StructureFlags;
};

class JSTestObjConstructor : public DOMConstructorObject {
private:
    JSTestObjConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::VM&, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSTestObjConstructor* create(JSC::VM& vm, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSTestObjConstructor* ptr = new (NotNull, JSC::allocateCell<JSTestObjConstructor>(vm.heap)) JSTestObjConstructor(structure, globalObject);
        ptr->finishCreation(vm, globalObject);
        return ptr;
    }

    DECLARE_INFO;
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::ImplementsHasInstance | DOMConstructorObject::StructureFlags;
    static JSC::EncodedJSValue JSC_HOST_CALL constructJSTestObj(JSC::ExecState*);
    static JSC::ConstructType getConstructData(JSC::JSCell*, JSC::ConstructData&);
};

// Functions

JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionVoidMethod(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionVoidMethodWithArgs(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionByteMethod(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionByteMethodWithArgs(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionOctetMethod(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionOctetMethodWithArgs(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionLongMethod(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionLongMethodWithArgs(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionObjMethod(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionObjMethodWithArgs(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithSequenceArg(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodReturningSequence(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithEnumArg(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodThatRequiresAllArgsAndThrows(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionSerializedValue(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionOptionsObject(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithException(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionCustomMethod(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionCustomMethodWithArgs(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionAddEventListener(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionRemoveEventListener(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionWithScriptStateVoid(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionWithScriptStateObj(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionWithScriptStateVoidException(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionWithScriptStateObjException(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionWithScriptExecutionContext(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionWithScriptExecutionContextAndScriptState(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionWithScriptExecutionContextAndScriptStateObjException(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionWithScriptExecutionContextAndScriptStateWithSpaces(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionWithScriptArgumentsAndCallStack(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithOptionalArg(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithNonOptionalArgAndOptionalArg(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithNonOptionalArgAndTwoOptionalArgs(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithOptionalString(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithOptionalStringIsUndefined(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithOptionalStringIsNullString(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithCallbackArg(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithNonCallbackArgAndCallbackArg(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithCallbackAndOptionalArg(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjConstructorFunctionStaticMethodWithCallbackAndOptionalArg(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjConstructorFunctionStaticMethodWithCallbackArg(JSC::ExecState*);
#if ENABLE(Condition1)
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionConditionalMethod1(JSC::ExecState*);
#endif
#if ENABLE(Condition1) && ENABLE(Condition2)
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionConditionalMethod2(JSC::ExecState*);
#endif
#if ENABLE(Condition1) || ENABLE(Condition2)
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionConditionalMethod3(JSC::ExecState*);
#endif
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionOverloadedMethod(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjConstructorFunctionClassMethod(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjConstructorFunctionClassMethodWithOptional(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjConstructorFunctionClassMethod2(JSC::ExecState*);
#if ENABLE(Condition1)
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjConstructorFunctionOverloadedMethod1(JSC::ExecState*);
#endif
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionClassMethodWithClamp(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithUnsignedLongSequence(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionStringArrayFunction(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionDomStringListFunction(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithAndWithoutNullableSequence(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMethodWithAndWithoutNullableSequence2(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionGetSVGDocument(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionConvert1(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionConvert2(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionConvert4(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionConvert5(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionMutablePointFunction(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionImmutablePointFunction(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionOrange(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionStrictFunction(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionStrictFunctionWithSequence(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionStrictFunctionWithArray(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionVariadicStringMethod(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionVariadicDoubleMethod(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionVariadicNodeMethod(JSC::ExecState*);
JSC::EncodedJSValue JSC_HOST_CALL jsTestObjPrototypeFunctionAny(JSC::ExecState*);
// Attributes

JSC::EncodedJSValue jsTestObjReadOnlyLongAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjReadOnlyStringAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjReadOnlyTestObjAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjConstructorStaticReadOnlyLongAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjConstructorStaticStringAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjConstructorStaticStringAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjConstructorTestSubObj(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjTestSubObjEnabledBySettingConstructor(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjTestSubObjEnabledBySettingConstructor(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjEnumAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjEnumAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjByteAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjByteAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjOctetAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjOctetAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjShortAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjShortAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjUnsignedShortAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjUnsignedShortAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjLongAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjLongAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjLongLongAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjLongLongAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjUnsignedLongLongAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjUnsignedLongLongAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjStringAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjStringAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjTestObjAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjTestObjAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjXMLObjAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjXMLObjAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjCreate(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjCreate(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjReflectedStringAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjReflectedStringAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjReflectedIntegralAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjReflectedIntegralAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjReflectedUnsignedIntegralAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjReflectedUnsignedIntegralAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjReflectedBooleanAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjReflectedBooleanAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjReflectedURLAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjReflectedURLAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjReflectedStringAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjReflectedStringAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjReflectedCustomIntegralAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjReflectedCustomIntegralAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjReflectedCustomBooleanAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjReflectedCustomBooleanAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjReflectedCustomURLAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjReflectedCustomURLAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjTypedArrayAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjTypedArrayAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjAttrWithGetterException(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjAttrWithGetterException(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjAttrWithSetterException(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjAttrWithSetterException(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjStringAttrWithGetterException(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjStringAttrWithGetterException(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjStringAttrWithSetterException(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjStringAttrWithSetterException(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjCustomAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjCustomAttr(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjWithScriptStateAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjWithScriptStateAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjWithScriptExecutionContextAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjWithScriptExecutionContextAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjWithScriptStateAttributeRaises(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjWithScriptStateAttributeRaises(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjWithScriptExecutionContextAttributeRaises(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjWithScriptExecutionContextAttributeRaises(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjWithScriptExecutionContextAndScriptStateAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjWithScriptExecutionContextAndScriptStateAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjWithScriptExecutionContextAndScriptStateAttributeRaises(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjWithScriptExecutionContextAndScriptStateAttributeRaises(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjWithScriptExecutionContextAndScriptStateWithSpacesAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjWithScriptExecutionContextAndScriptStateWithSpacesAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjWithScriptArgumentsAndCallStackAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjWithScriptArgumentsAndCallStackAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
#if ENABLE(Condition1)
JSC::EncodedJSValue jsTestObjConditionalAttr1(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjConditionalAttr1(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
#endif
#if ENABLE(Condition1) && ENABLE(Condition2)
JSC::EncodedJSValue jsTestObjConditionalAttr2(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjConditionalAttr2(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
#endif
#if ENABLE(Condition1) || ENABLE(Condition2)
JSC::EncodedJSValue jsTestObjConditionalAttr3(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjConditionalAttr3(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
#endif
#if ENABLE(Condition1)
JSC::EncodedJSValue jsTestObjConditionalAttr4Constructor(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjConditionalAttr4Constructor(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
#endif
#if ENABLE(Condition1) && ENABLE(Condition2)
JSC::EncodedJSValue jsTestObjConditionalAttr5Constructor(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjConditionalAttr5Constructor(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
#endif
#if ENABLE(Condition1) || ENABLE(Condition2)
JSC::EncodedJSValue jsTestObjConditionalAttr6Constructor(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjConditionalAttr6Constructor(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
#endif
JSC::EncodedJSValue jsTestObjCachedAttribute1(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjCachedAttribute2(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjAnyAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjAnyAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjContentDocument(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjMutablePoint(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjMutablePoint(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjImmutablePoint(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjImmutablePoint(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjStrawberry(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjStrawberry(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjStrictFloat(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjStrictFloat(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjDescription(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjId(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjId(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjHash(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjReplaceableAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjReplaceableAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjNullableDoubleAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjNullableLongAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjNullableBooleanAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjNullableStringAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjNullableLongSettableAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjNullableLongSettableAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjNullableStringValue(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjNullableStringValue(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjAttribute(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
JSC::EncodedJSValue jsTestObjAttributeWithReservedEnumType(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);
void setJSTestObjAttributeWithReservedEnumType(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::EncodedJSValue);
JSC::EncodedJSValue jsTestObjConstructor(JSC::ExecState*, JSC::JSObject*, JSC::EncodedJSValue, JSC::PropertyName);

} // namespace WebCore

#endif
