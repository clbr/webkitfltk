/*
 *  Copyright (C) 2005, 2006, 2007, 2008, 2014 Apple Inc. All rights reserved.
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
 *
 */

#ifndef WTF_Vector_h
#define WTF_Vector_h

#include <initializer_list>
#include <limits>
#include <string.h>
#include <type_traits>
#include <utility>
#include <wtf/CheckedArithmetic.h>
#include <wtf/FastMalloc.h>
#include <wtf/MallocPtr.h>
#include <wtf/Noncopyable.h>
#include <wtf/StdLibExtras.h>
#include <wtf/ValueCheck.h>
#include <wtf/VectorTraits.h>

namespace WTF {

const size_t notFound = static_cast<size_t>(-1);

template <bool needsDestruction, typename T>
struct VectorDestructor;

template<typename T>
struct VectorDestructor<false, T>
{
    static void destruct(T*, T*) {}
};

template<typename T>
struct VectorDestructor<true, T>
{
    static void destruct(T* begin, T* end) 
    {
        for (T* cur = begin; cur != end; ++cur)
            cur->~T();
    }
};

template <bool needsInitialization, bool canInitializeWithMemset, typename T>
struct VectorInitializer;

template<bool ignore, typename T>
struct VectorInitializer<false, ignore, T>
{
    static void initialize(T*, T*) {}
};

template<typename T>
struct VectorInitializer<true, false, T>
{
    static void initialize(T* begin, T* end) 
    {
        for (T* cur = begin; cur != end; ++cur)
            new (NotNull, cur) T;
    }
};

template<typename T>
struct VectorInitializer<true, true, T>
{
    static void initialize(T* begin, T* end) 
    {
        memset(begin, 0, reinterpret_cast<char*>(end) - reinterpret_cast<char*>(begin));
    }
};

template <bool canMoveWithMemcpy, typename T>
struct VectorMover;

template<typename T>
struct VectorMover<false, T>
{
    static void move(T* src, T* srcEnd, T* dst)
    {
        while (src != srcEnd) {
            new (NotNull, dst) T(WTF::move(*src));
            src->~T();
            ++dst;
            ++src;
        }
    }
    static void moveOverlapping(T* src, T* srcEnd, T* dst)
    {
        if (src > dst)
            move(src, srcEnd, dst);
        else {
            T* dstEnd = dst + (srcEnd - src);
            while (src != srcEnd) {
                --srcEnd;
                --dstEnd;
                new (NotNull, dstEnd) T(WTF::move(*srcEnd));
                srcEnd->~T();
            }
        }
    }
};

template<typename T>
struct VectorMover<true, T>
{
    static void move(const T* src, const T* srcEnd, T* dst) 
    {
        memcpy(dst, src, reinterpret_cast<const char*>(srcEnd) - reinterpret_cast<const char*>(src));
    }
    static void moveOverlapping(const T* src, const T* srcEnd, T* dst) 
    {
        memmove(dst, src, reinterpret_cast<const char*>(srcEnd) - reinterpret_cast<const char*>(src));
    }
};

template <bool canCopyWithMemcpy, typename T>
struct VectorCopier;

template<typename T>
struct VectorCopier<false, T>
{
    template<typename U>
    static void uninitializedCopy(const T* src, const T* srcEnd, U* dst)
    {
        while (src != srcEnd) {
            new (NotNull, dst) U(*src);
            ++dst;
            ++src;
        }
    }
};

template<typename T>
struct VectorCopier<true, T>
{
    static void uninitializedCopy(const T* src, const T* srcEnd, T* dst)
    {
        memcpy(dst, src, reinterpret_cast<const char*>(srcEnd) - reinterpret_cast<const char*>(src));
    }
    template<typename U>
    static void uninitializedCopy(const T* src, const T* srcEnd, U* dst)
    {
        VectorCopier<false, T>::uninitializedCopy(src, srcEnd, dst);
    }
};

template <bool canFillWithMemset, typename T>
struct VectorFiller;

template<typename T>
struct VectorFiller<false, T>
{
    static void uninitializedFill(T* dst, T* dstEnd, const T& val) 
    {
        while (dst != dstEnd) {
            new (NotNull, dst) T(val);
            ++dst;
        }
    }
};

template<typename T>
struct VectorFiller<true, T>
{
    static void uninitializedFill(T* dst, T* dstEnd, const T& val) 
    {
        static_assert(sizeof(T) == 1, "Size of type T should be equal to one!");
#if COMPILER(GCC) && defined(_FORTIFY_SOURCE)
        if (!__builtin_constant_p(dstEnd - dst) || (!(dstEnd - dst)))
#endif
            memset(dst, val, dstEnd - dst);
    }
};

template<bool canCompareWithMemcmp, typename T>
struct VectorComparer;

template<typename T>
struct VectorComparer<false, T>
{
    static bool compare(const T* a, const T* b, unsigned size)
    {
        for (unsigned i = 0; i < size; ++i) {
            if (!(a[i] == b[i]))
                return false;
        }
        return true;
    }
};

template<typename T>
struct VectorComparer<true, T>
{
    static bool compare(const T* a, const T* b, unsigned size)
    {
        return memcmp(a, b, sizeof(T) * size) == 0;
    }
};

template<typename T>
struct VectorTypeOperations
{
    static void destruct(T* begin, T* end)
    {
        VectorDestructor<!std::is_trivially_destructible<T>::value, T>::destruct(begin, end);
    }

    static void initialize(T* begin, T* end)
    {
        VectorInitializer<VectorTraits<T>::needsInitialization, VectorTraits<T>::canInitializeWithMemset, T>::initialize(begin, end);
    }

    static void move(T* src, T* srcEnd, T* dst)
    {
        VectorMover<VectorTraits<T>::canMoveWithMemcpy, T>::move(src, srcEnd, dst);
    }

    static void moveOverlapping(T* src, T* srcEnd, T* dst)
    {
        VectorMover<VectorTraits<T>::canMoveWithMemcpy, T>::moveOverlapping(src, srcEnd, dst);
    }

    static void uninitializedCopy(const T* src, const T* srcEnd, T* dst)
    {
        VectorCopier<VectorTraits<T>::canCopyWithMemcpy, T>::uninitializedCopy(src, srcEnd, dst);
    }

    static void uninitializedFill(T* dst, T* dstEnd, const T& val)
    {
        VectorFiller<VectorTraits<T>::canFillWithMemset, T>::uninitializedFill(dst, dstEnd, val);
    }
    
    static bool compare(const T* a, const T* b, unsigned size)
    {
        return VectorComparer<VectorTraits<T>::canCompareWithMemcmp, T>::compare(a, b, size);
    }
};

template<typename T>
class VectorBufferBase {
    WTF_MAKE_NONCOPYABLE(VectorBufferBase);
public:
    void allocateBuffer(unsigned newCapacity)
    {
        ASSERT(newCapacity);
        if (newCapacity > std::numeric_limits<unsigned>::max() / sizeof(T))
            CRASH();
        unsigned sizeToAllocate = fastMallocGoodSize(newCapacity * sizeof(T));
        m_capacity = sizeToAllocate / sizeof(T);
        m_buffer = static_cast<T*>(fastMalloc(sizeToAllocate));
    }

    bool tryAllocateBuffer(unsigned newCapacity)
    {
        ASSERT(newCapacity);
        if (newCapacity > std::numeric_limits<unsigned>::max() / sizeof(T))
            return false;

        unsigned sizeToAllocate = fastMallocGoodSize(newCapacity * sizeof(T));
        T* newBuffer;
        if (tryFastMalloc(sizeToAllocate).getValue(newBuffer)) {
            m_capacity = sizeToAllocate / sizeof(T);
            m_buffer = newBuffer;
            return true;
        }
        return false;
    }

    bool shouldReallocateBuffer(unsigned newCapacity) const
    {
        return VectorTraits<T>::canMoveWithMemcpy && m_capacity && newCapacity;
    }

    void reallocateBuffer(unsigned newCapacity)
    {
        ASSERT(shouldReallocateBuffer(newCapacity));
        if (newCapacity > std::numeric_limits<unsigned>::max() / sizeof(T))
            CRASH();
        unsigned sizeToAllocate = fastMallocGoodSize(newCapacity * sizeof(T));
        m_capacity = sizeToAllocate / sizeof(T);
        m_buffer = static_cast<T*>(fastRealloc(m_buffer, sizeToAllocate));
    }

    void deallocateBuffer(T* bufferToDeallocate)
    {
        if (!bufferToDeallocate)
            return;
        
        if (m_buffer == bufferToDeallocate) {
            m_buffer = 0;
            m_capacity = 0;
        }

        fastFree(bufferToDeallocate);
    }

    T* buffer() { return m_buffer; }
    const T* buffer() const { return m_buffer; }
    static ptrdiff_t bufferMemoryOffset() { return OBJECT_OFFSETOF(VectorBufferBase, m_buffer); }
    unsigned capacity() const { return m_capacity; }

    MallocPtr<T> releaseBuffer()
    {
        T* buffer = m_buffer;
        m_buffer = 0;
        m_capacity = 0;
        return adoptMallocPtr(buffer);
    }

protected:
    VectorBufferBase()
        : m_buffer(0)
        , m_capacity(0)
        , m_size(0)
    {
    }

    VectorBufferBase(T* buffer, unsigned capacity, unsigned size)
        : m_buffer(buffer)
        , m_capacity(capacity)
        , m_size(size)
    {
    }

    ~VectorBufferBase()
    {
        // FIXME: It would be nice to find a way to ASSERT that m_buffer hasn't leaked here.
    }

    T* m_buffer;
    unsigned m_capacity;
    unsigned m_size; // Only used by the Vector subclass, but placed here to avoid padding the struct.
};

template<typename T, unsigned inlineCapacity>
class VectorBuffer;

template<typename T>
class VectorBuffer<T, 0> : private VectorBufferBase<T> {
private:
    typedef VectorBufferBase<T> Base;
public:
    VectorBuffer()
    {
    }

    VectorBuffer(unsigned capacity, unsigned size = 0)
    {
        m_size = size;
        // Calling malloc(0) might take a lock and may actually do an
        // allocation on some systems.
        if (capacity)
            allocateBuffer(capacity);
    }

    ~VectorBuffer()
    {
        deallocateBuffer(buffer());
    }
    
    void swap(VectorBuffer<T, 0>& other, unsigned, unsigned)
    {
        std::swap(m_buffer, other.m_buffer);
        std::swap(m_capacity, other.m_capacity);
    }
    
    void restoreInlineBufferIfNeeded() { }

    using Base::allocateBuffer;
    using Base::tryAllocateBuffer;
    using Base::shouldReallocateBuffer;
    using Base::reallocateBuffer;
    using Base::deallocateBuffer;

    using Base::buffer;
    using Base::capacity;
    using Base::bufferMemoryOffset;

    using Base::releaseBuffer;

protected:
    using Base::m_size;

private:
    using Base::m_buffer;
    using Base::m_capacity;
};

template<typename T, unsigned inlineCapacity>
class VectorBuffer : private VectorBufferBase<T> {
    WTF_MAKE_NONCOPYABLE(VectorBuffer);
private:
    typedef VectorBufferBase<T> Base;
public:
    VectorBuffer()
        : Base(inlineBuffer(), inlineCapacity, 0)
    {
    }

    VectorBuffer(unsigned capacity, unsigned size = 0)
        : Base(inlineBuffer(), inlineCapacity, size)
    {
        if (capacity > inlineCapacity)
            Base::allocateBuffer(capacity);
    }

    ~VectorBuffer()
    {
        deallocateBuffer(buffer());
    }

    void allocateBuffer(unsigned newCapacity)
    {
        // FIXME: This should ASSERT(!m_buffer) to catch misuse/leaks.
        if (newCapacity > inlineCapacity)
            Base::allocateBuffer(newCapacity);
        else {
            m_buffer = inlineBuffer();
            m_capacity = inlineCapacity;
        }
    }

    bool tryAllocateBuffer(unsigned newCapacity)
    {
        if (newCapacity > inlineCapacity)
            return Base::tryAllocateBuffer(newCapacity);
        m_buffer = inlineBuffer();
        m_capacity = inlineCapacity;
        return true;
    }

    void deallocateBuffer(T* bufferToDeallocate)
    {
        if (bufferToDeallocate == inlineBuffer())
            return;
        Base::deallocateBuffer(bufferToDeallocate);
    }

    bool shouldReallocateBuffer(unsigned newCapacity) const
    {
        // We cannot reallocate the inline buffer.
        return Base::shouldReallocateBuffer(newCapacity) && std::min(m_capacity, newCapacity) > inlineCapacity;
    }

    void reallocateBuffer(unsigned newCapacity)
    {
        ASSERT(shouldReallocateBuffer(newCapacity));
        Base::reallocateBuffer(newCapacity);
    }

    void swap(VectorBuffer& other, unsigned mySize, unsigned otherSize)
    {
        if (buffer() == inlineBuffer() && other.buffer() == other.inlineBuffer()) {
            swapInlineBuffer(other, mySize, otherSize);
            std::swap(m_capacity, other.m_capacity);
        } else if (buffer() == inlineBuffer()) {
            m_buffer = other.m_buffer;
            other.m_buffer = other.inlineBuffer();
            swapInlineBuffer(other, mySize, 0);
            std::swap(m_capacity, other.m_capacity);
        } else if (other.buffer() == other.inlineBuffer()) {
            other.m_buffer = m_buffer;
            m_buffer = inlineBuffer();
            swapInlineBuffer(other, 0, otherSize);
            std::swap(m_capacity, other.m_capacity);
        } else {
            std::swap(m_buffer, other.m_buffer);
            std::swap(m_capacity, other.m_capacity);
        }
    }

    void restoreInlineBufferIfNeeded()
    {
        if (m_buffer)
            return;
        m_buffer = inlineBuffer();
        m_capacity = inlineCapacity;
    }

    using Base::buffer;
    using Base::capacity;
    using Base::bufferMemoryOffset;

    MallocPtr<T> releaseBuffer()
    {
        if (buffer() == inlineBuffer())
            return nullptr;
        return Base::releaseBuffer();
    }

protected:
    using Base::m_size;

private:
    using Base::m_buffer;
    using Base::m_capacity;
    
    void swapInlineBuffer(VectorBuffer& other, unsigned mySize, unsigned otherSize)
    {
        // FIXME: We could make swap part of VectorTypeOperations
        // https://bugs.webkit.org/show_bug.cgi?id=128863
        
        if (std::is_pod<T>::value)
            std::swap(m_inlineBuffer, other.m_inlineBuffer);
        else
            swapInlineBuffers(inlineBuffer(), other.inlineBuffer(), mySize, otherSize);
    }
    
    static void swapInlineBuffers(T* left, T* right, unsigned leftSize, unsigned rightSize)
    {
        if (left == right)
            return;
        
        ASSERT(leftSize <= inlineCapacity);
        ASSERT(rightSize <= inlineCapacity);
        
        unsigned swapBound = std::min(leftSize, rightSize);
        for (unsigned i = 0; i < swapBound; ++i)
            std::swap(left[i], right[i]);
        VectorTypeOperations<T>::move(left + swapBound, left + leftSize, right + swapBound);
        VectorTypeOperations<T>::move(right + swapBound, right + rightSize, left + swapBound);
    }

    T* inlineBuffer() { return reinterpret_cast_ptr<T*>(m_inlineBuffer); }
    const T* inlineBuffer() const { return reinterpret_cast_ptr<const T*>(m_inlineBuffer); }

    typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type m_inlineBuffer[inlineCapacity];
};

struct UnsafeVectorOverflow {
    static NO_RETURN_DUE_TO_ASSERT void overflowed()
    {
        ASSERT_NOT_REACHED();
    }
};

template<typename T, unsigned inlineCapacity = 0, typename OverflowHandler = CrashOnOverflow>
class Vector : private VectorBuffer<T, inlineCapacity> {
    WTF_MAKE_FAST_ALLOCATED;
private:
    typedef VectorBuffer<T, inlineCapacity> Base;
    typedef VectorTypeOperations<T> TypeOperations;

public:
    typedef T ValueType;

    typedef T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    Vector()
    {
    }

    // Unlike in std::vector, this constructor does not initialize POD types.
    explicit Vector(unsigned size)
        : Base(size, size)
    {
        if (begin())
            TypeOperations::initialize(begin(), end());
    }

    Vector(unsigned size, const T& val)
        : Base(size, size)
    {
        if (begin())
            TypeOperations::uninitializedFill(begin(), end(), val);
    }

    Vector(std::initializer_list<T> initializerList)
    {
        reserveInitialCapacity(initializerList.size());
        for (const auto& element : initializerList)
            uncheckedAppend(element);
    }

    ~Vector()
    {
        if (m_size)
            shrink(0);
    }

    Vector(const Vector&);
    template<unsigned otherCapacity, typename otherOverflowBehaviour>
    Vector(const Vector<T, otherCapacity, otherOverflowBehaviour>&);

    Vector& operator=(const Vector&);
    template<unsigned otherCapacity, typename otherOverflowBehaviour>
    Vector& operator=(const Vector<T, otherCapacity, otherOverflowBehaviour>&);

    Vector(Vector&&);
    Vector& operator=(Vector&&);

    size_t size() const { return m_size; }
    static ptrdiff_t sizeMemoryOffset() { return OBJECT_OFFSETOF(Vector, m_size); }
    unsigned capacity() const { return Base::capacity(); }
    bool isEmpty() const { return !size(); }

    T& at(unsigned i)
    {
        if (UNLIKELY(i >= size()))
            OverflowHandler::overflowed();
        return Base::buffer()[i];
    }
    const T& at(unsigned i) const
    {
        if (UNLIKELY(i >= size()))
            OverflowHandler::overflowed();
        return Base::buffer()[i];
    }
    T& at(Checked<unsigned> i)
    {
        RELEASE_ASSERT(i < size());
        return Base::buffer()[i];
    }
    const T& at(Checked<unsigned> i) const
    {
        RELEASE_ASSERT(i < size());
        return Base::buffer()[i];
    }

    T& operator[](unsigned i) { return at(i); }
    const T& operator[](unsigned i) const { return at(i); }
    T& operator[](Checked<unsigned> i) { return at(i); }
    const T& operator[](Checked<unsigned> i) const { return at(i); }

    T* data() { return Base::buffer(); }
    const T* data() const { return Base::buffer(); }
    static ptrdiff_t dataMemoryOffset() { return Base::bufferMemoryOffset(); }

    iterator begin() { return data(); }
    iterator end() { return begin() + m_size; }
    const_iterator begin() const { return data(); }
    const_iterator end() const { return begin() + m_size; }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    T& first() { return at(0); }
    const T& first() const { return at(0); }
    T& last() { return at(size() - 1); }
    const T& last() const { return at(size() - 1); }
    
    T takeLast()
    {
        T result = WTF::move(last());
        removeLast();
        return result;
    }
    
    template<typename U> bool contains(const U&) const;
    template<typename U> size_t find(const U&) const;
    template<typename U> size_t reverseFind(const U&) const;

    void shrink(unsigned size);
    void grow(unsigned size);
    void resize(unsigned size);
    void resizeToFit(unsigned size);
    void reserveCapacity(unsigned newCapacity);
    bool tryReserveCapacity(unsigned newCapacity);
    void reserveInitialCapacity(unsigned initialCapacity);
    void shrinkCapacity(unsigned newCapacity);
    void shrinkToFit() { shrinkCapacity(size()); }

    void clear() { shrinkCapacity(0); }

    template<typename U> void append(const U*, unsigned);
    template<typename U> void append(U&&);
    template<typename U> void uncheckedAppend(U&& val);
    template<typename U, unsigned otherCapacity> void appendVector(const Vector<U, otherCapacity>&);
    template<typename U> bool tryAppend(const U*, unsigned);

    template<typename U> void insert(unsigned position, const U*, unsigned);
    template<typename U> void insert(unsigned position, U&&);
    template<typename U, unsigned c> void insertVector(unsigned position, const Vector<U, c>&);

    void remove(unsigned position);
    void remove(unsigned position, unsigned length);

    void removeLast() 
    {
        if (UNLIKELY(isEmpty()))
            OverflowHandler::overflowed();
        shrink(size() - 1); 
    }

    void fill(const T&, unsigned);
    void fill(const T& val) { fill(val, size()); }

    template<typename Iterator> void appendRange(Iterator start, Iterator end);

    MallocPtr<T> releaseBuffer();

    void swap(Vector<T, inlineCapacity, OverflowHandler>& other)
    {
        Base::swap(other, m_size, other.m_size);
        std::swap(m_size, other.m_size);
    }

    void reverse();

    void checkConsistency();

private:
    void expandCapacity(unsigned newMinCapacity);
    T* expandCapacity(unsigned newMinCapacity, T*);
    bool tryExpandCapacity(unsigned newMinCapacity);
    const T* tryExpandCapacity(unsigned newMinCapacity, const T*);
    template<typename U> U* expandCapacity(unsigned newMinCapacity, U*);
    template<typename U> void appendSlowCase(U&&);

    using Base::m_size;
    using Base::buffer;
    using Base::capacity;
    using Base::swap;
    using Base::allocateBuffer;
    using Base::deallocateBuffer;
    using Base::tryAllocateBuffer;
    using Base::shouldReallocateBuffer;
    using Base::reallocateBuffer;
    using Base::restoreInlineBufferIfNeeded;
    using Base::releaseBuffer;
};

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
Vector<T, inlineCapacity, OverflowHandler>::Vector(const Vector& other)
    : Base(other.capacity(), other.size())
{
    if (begin())
        TypeOperations::uninitializedCopy(other.begin(), other.end(), begin());
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
template<unsigned otherCapacity, typename otherOverflowBehaviour>
Vector<T, inlineCapacity, OverflowHandler>::Vector(const Vector<T, otherCapacity, otherOverflowBehaviour>& other)
    : Base(other.capacity(), other.size())
{
    if (begin())
        TypeOperations::uninitializedCopy(other.begin(), other.end(), begin());
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
Vector<T, inlineCapacity, OverflowHandler>& Vector<T, inlineCapacity, OverflowHandler>::operator=(const Vector<T, inlineCapacity, OverflowHandler>& other)
{
    if (&other == this)
        return *this;
    
    if (size() > other.size())
        shrink(other.size());
    else if (other.size() > capacity()) {
        clear();
        reserveCapacity(other.size());
        ASSERT(begin());
    }
    
    std::copy(other.begin(), other.begin() + size(), begin());
    TypeOperations::uninitializedCopy(other.begin() + size(), other.end(), end());
    m_size = other.size();

    return *this;
}

inline bool typelessPointersAreEqual(const void* a, const void* b) { return a == b; }

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
template<unsigned otherCapacity, typename otherOverflowBehaviour>
Vector<T, inlineCapacity, OverflowHandler>& Vector<T, inlineCapacity, OverflowHandler>::operator=(const Vector<T, otherCapacity, otherOverflowBehaviour>& other)
{
    // If the inline capacities match, we should call the more specific
    // template.  If the inline capacities don't match, the two objects
    // shouldn't be allocated the same address.
    ASSERT(!typelessPointersAreEqual(&other, this));

    if (size() > other.size())
        shrink(other.size());
    else if (other.size() > capacity()) {
        clear();
        reserveCapacity(other.size());
        ASSERT(begin());
    }
    
    std::copy(other.begin(), other.begin() + size(), begin());
    TypeOperations::uninitializedCopy(other.begin() + size(), other.end(), end());
    m_size = other.size();

    return *this;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
inline Vector<T, inlineCapacity, OverflowHandler>::Vector(Vector<T, inlineCapacity, OverflowHandler>&& other)
{
    swap(other);
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
inline Vector<T, inlineCapacity, OverflowHandler>& Vector<T, inlineCapacity, OverflowHandler>::operator=(Vector<T, inlineCapacity, OverflowHandler>&& other)
{
    swap(other);
    return *this;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
template<typename U>
bool Vector<T, inlineCapacity, OverflowHandler>::contains(const U& value) const
{
    return find(value) != notFound;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
template<typename U>
size_t Vector<T, inlineCapacity, OverflowHandler>::find(const U& value) const
{
    for (unsigned i = 0; i < size(); ++i) {
        if (at(i) == value)
            return i;
    }
    return notFound;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
template<typename U>
size_t Vector<T, inlineCapacity, OverflowHandler>::reverseFind(const U& value) const
{
    for (unsigned i = 1; i <= size(); ++i) {
        const unsigned index = size() - i;
        if (at(index) == value)
            return index;
    }
    return notFound;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
void Vector<T, inlineCapacity, OverflowHandler>::fill(const T& val, unsigned newSize)
{
    if (size() > newSize)
        shrink(newSize);
    else if (newSize > capacity()) {
        clear();
        reserveCapacity(newSize);
        ASSERT(begin());
    }
    
    std::fill(begin(), end(), val);
    TypeOperations::uninitializedFill(end(), begin() + newSize, val);
    m_size = newSize;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
template<typename Iterator>
void Vector<T, inlineCapacity, OverflowHandler>::appendRange(Iterator start, Iterator end)
{
    for (Iterator it = start; it != end; ++it)
        append(*it);
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
void Vector<T, inlineCapacity, OverflowHandler>::expandCapacity(unsigned newMinCapacity)
{
    reserveCapacity(std::max(newMinCapacity, std::max(16u, capacity() + capacity() / 4 + 1)));
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
T* Vector<T, inlineCapacity, OverflowHandler>::expandCapacity(unsigned newMinCapacity, T* ptr)
{
    if (ptr < begin() || ptr >= end()) {
        expandCapacity(newMinCapacity);
        return ptr;
    }
    unsigned index = ptr - begin();
    expandCapacity(newMinCapacity);
    return begin() + index;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
bool Vector<T, inlineCapacity, OverflowHandler>::tryExpandCapacity(unsigned newMinCapacity)
{
    return tryReserveCapacity(std::max(newMinCapacity, std::max(16u, capacity() + capacity() / 4 + 1)));
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
const T* Vector<T, inlineCapacity, OverflowHandler>::tryExpandCapacity(unsigned newMinCapacity, const T* ptr)
{
    if (ptr < begin() || ptr >= end()) {
        if (!tryExpandCapacity(newMinCapacity))
            return 0;
        return ptr;
    }
    unsigned index = ptr - begin();
    if (!tryExpandCapacity(newMinCapacity))
        return 0;
    return begin() + index;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler> template<typename U>
inline U* Vector<T, inlineCapacity, OverflowHandler>::expandCapacity(unsigned newMinCapacity, U* ptr)
{
    expandCapacity(newMinCapacity);
    return ptr;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
inline void Vector<T, inlineCapacity, OverflowHandler>::resize(unsigned size)
{
    if (size <= m_size)
        TypeOperations::destruct(begin() + size, end());
    else {
        if (size > capacity())
            expandCapacity(size);
        if (begin())
            TypeOperations::initialize(end(), begin() + size);
    }
    
    m_size = size;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
void Vector<T, inlineCapacity, OverflowHandler>::resizeToFit(unsigned size)
{
    reserveCapacity(size);
    resize(size);
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
void Vector<T, inlineCapacity, OverflowHandler>::shrink(unsigned size)
{
    ASSERT(size <= m_size);
    TypeOperations::destruct(begin() + size, end());
    m_size = size;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
void Vector<T, inlineCapacity, OverflowHandler>::grow(unsigned size)
{
    ASSERT(size >= m_size);
    if (size > capacity())
        expandCapacity(size);
    if (begin())
        TypeOperations::initialize(end(), begin() + size);
    m_size = size;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
void Vector<T, inlineCapacity, OverflowHandler>::reserveCapacity(unsigned newCapacity)
{
    if (newCapacity <= capacity())
        return;
    T* oldBuffer = begin();
    T* oldEnd = end();
    Base::allocateBuffer(newCapacity);
    ASSERT(begin());
    TypeOperations::move(oldBuffer, oldEnd, begin());
    Base::deallocateBuffer(oldBuffer);
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
bool Vector<T, inlineCapacity, OverflowHandler>::tryReserveCapacity(unsigned newCapacity)
{
    if (newCapacity <= capacity())
        return true;
    T* oldBuffer = begin();
    T* oldEnd = end();
    if (!Base::tryAllocateBuffer(newCapacity))
        return false;
    ASSERT(begin());
    TypeOperations::move(oldBuffer, oldEnd, begin());
    Base::deallocateBuffer(oldBuffer);
    return true;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
inline void Vector<T, inlineCapacity, OverflowHandler>::reserveInitialCapacity(unsigned initialCapacity)
{
    ASSERT(!m_size);
    ASSERT(capacity() == inlineCapacity);
    if (initialCapacity > inlineCapacity)
        Base::allocateBuffer(initialCapacity);
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
void Vector<T, inlineCapacity, OverflowHandler>::shrinkCapacity(unsigned newCapacity)
{
    if (newCapacity >= capacity())
        return;

    if (newCapacity < size()) 
        shrink(newCapacity);

    T* oldBuffer = begin();
    if (newCapacity > 0) {
        if (Base::shouldReallocateBuffer(newCapacity)) {
            Base::reallocateBuffer(newCapacity);
            return;
        }

        T* oldEnd = end();
        Base::allocateBuffer(newCapacity);
        if (begin() != oldBuffer)
            TypeOperations::move(oldBuffer, oldEnd, begin());
    }

    Base::deallocateBuffer(oldBuffer);
    Base::restoreInlineBufferIfNeeded();
}

// Templatizing these is better than just letting the conversion happen implicitly,
// because for instance it allows a PassRefPtr to be appended to a RefPtr vector
// without refcount thrash.
template<typename T, unsigned inlineCapacity, typename OverflowHandler> template<typename U>
void Vector<T, inlineCapacity, OverflowHandler>::append(const U* data, unsigned dataSize)
{
    unsigned newSize = m_size + dataSize;
    if (newSize > capacity()) {
        data = expandCapacity(newSize, data);
        ASSERT(begin());
    }
    if (newSize < m_size)
        CRASH();
    T* dest = end();
    VectorCopier<std::is_trivial<T>::value, U>::uninitializedCopy(data, &data[dataSize], dest);
    m_size = newSize;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler> template<typename U>
bool Vector<T, inlineCapacity, OverflowHandler>::tryAppend(const U* data, unsigned dataSize)
{
    unsigned newSize = m_size + dataSize;
    if (newSize > capacity()) {
        data = tryExpandCapacity(newSize, data);
        if (!data)
            return false;
        ASSERT(begin());
    }
    if (newSize < m_size)
        return false;
    T* dest = end();
    VectorCopier<std::is_trivial<T>::value, U>::uninitializedCopy(data, &data[dataSize], dest);
    m_size = newSize;
    return true;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler> template<typename U>
ALWAYS_INLINE void Vector<T, inlineCapacity, OverflowHandler>::append(U&& value)
{
    if (size() != capacity()) {
        new (NotNull, end()) T(std::forward<U>(value));
        ++m_size;
        return;
    }

    appendSlowCase(std::forward<U>(value));
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler> template<typename U>
void Vector<T, inlineCapacity, OverflowHandler>::appendSlowCase(U&& value)
{
    ASSERT(size() == capacity());

    auto ptr = const_cast<typename std::remove_const<typename std::remove_reference<U>::type>::type*>(std::addressof(value));
    ptr = expandCapacity(size() + 1, ptr);
    ASSERT(begin());

    new (NotNull, end()) T(std::forward<U>(*ptr));
    ++m_size;
}

// This version of append saves a branch in the case where you know that the
// vector's capacity is large enough for the append to succeed.

template<typename T, unsigned inlineCapacity, typename OverflowHandler> template<typename U>
inline void Vector<T, inlineCapacity, OverflowHandler>::uncheckedAppend(U&& value)
{
    ASSERT(size() < capacity());

    auto ptr = std::addressof(value);
    new (NotNull, end()) T(std::forward<U>(*ptr));
    ++m_size;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler> template<typename U, unsigned otherCapacity>
inline void Vector<T, inlineCapacity, OverflowHandler>::appendVector(const Vector<U, otherCapacity>& val)
{
    append(val.begin(), val.size());
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler> template<typename U>
void Vector<T, inlineCapacity, OverflowHandler>::insert(unsigned position, const U* data, unsigned dataSize)
{
    ASSERT_WITH_SECURITY_IMPLICATION(position <= size());
    unsigned newSize = m_size + dataSize;
    if (newSize > capacity()) {
        data = expandCapacity(newSize, data);
        ASSERT(begin());
    }
    if (newSize < m_size)
        CRASH();
    T* spot = begin() + position;
    TypeOperations::moveOverlapping(spot, end(), spot + dataSize);
    VectorCopier<std::is_trivial<T>::value, U>::uninitializedCopy(data, &data[dataSize], spot);
    m_size = newSize;
}
 
template<typename T, unsigned inlineCapacity, typename OverflowHandler> template<typename U>
inline void Vector<T, inlineCapacity, OverflowHandler>::insert(unsigned position, U&& value)
{
    ASSERT_WITH_SECURITY_IMPLICATION(position <= size());

    auto ptr = const_cast<typename std::remove_const<typename std::remove_reference<U>::type>::type*>(std::addressof(value));
    if (size() == capacity()) {
        ptr = expandCapacity(size() + 1, ptr);
        ASSERT(begin());
    }

    T* spot = begin() + position;
    TypeOperations::moveOverlapping(spot, end(), spot + 1);
    new (NotNull, spot) T(std::forward<U>(*ptr));
    ++m_size;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler> template<typename U, unsigned c>
inline void Vector<T, inlineCapacity, OverflowHandler>::insertVector(unsigned position, const Vector<U, c>& val)
{
    insert(position, val.begin(), val.size());
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
inline void Vector<T, inlineCapacity, OverflowHandler>::remove(unsigned position)
{
    ASSERT_WITH_SECURITY_IMPLICATION(position < size());
    T* spot = begin() + position;
    spot->~T();
    TypeOperations::moveOverlapping(spot + 1, end(), spot);
    --m_size;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
inline void Vector<T, inlineCapacity, OverflowHandler>::remove(unsigned position, unsigned length)
{
    ASSERT_WITH_SECURITY_IMPLICATION(position <= size());
    ASSERT_WITH_SECURITY_IMPLICATION(position + length <= size());
    T* beginSpot = begin() + position;
    T* endSpot = beginSpot + length;
    TypeOperations::destruct(beginSpot, endSpot); 
    TypeOperations::moveOverlapping(endSpot, end(), beginSpot);
    m_size -= length;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
inline void Vector<T, inlineCapacity, OverflowHandler>::reverse()
{
    for (unsigned i = 0; i < m_size / 2; ++i)
        std::swap(at(i), at(m_size - 1 - i));
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
inline MallocPtr<T> Vector<T, inlineCapacity, OverflowHandler>::releaseBuffer()
{
    auto buffer = Base::releaseBuffer();
    if (inlineCapacity && !buffer && m_size) {
        // If the vector had some data, but no buffer to release,
        // that means it was using the inline buffer. In that case,
        // we create a brand new buffer so the caller always gets one.
        size_t bytes = m_size * sizeof(T);
        buffer = adoptMallocPtr(static_cast<T*>(fastMalloc(bytes)));
        memcpy(buffer.get(), data(), bytes);
    }
    m_size = 0;
    return buffer;
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
inline void Vector<T, inlineCapacity, OverflowHandler>::checkConsistency()
{
#if !ASSERT_DISABLED
    for (unsigned i = 0; i < size(); ++i)
        ValueCheck<T>::checkConsistency(at(i));
#endif
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
inline void swap(Vector<T, inlineCapacity, OverflowHandler>& a, Vector<T, inlineCapacity, OverflowHandler>& b)
{
    a.swap(b);
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
bool operator==(const Vector<T, inlineCapacity, OverflowHandler>& a, const Vector<T, inlineCapacity, OverflowHandler>& b)
{
    if (a.size() != b.size())
        return false;

    return VectorTypeOperations<T>::compare(a.data(), b.data(), a.size());
}

template<typename T, unsigned inlineCapacity, typename OverflowHandler>
inline bool operator!=(const Vector<T, inlineCapacity, OverflowHandler>& a, const Vector<T, inlineCapacity, OverflowHandler>& b)
{
    return !(a == b);
}

#if !ASSERT_DISABLED
template<typename T> struct ValueCheck<Vector<T>> {
    typedef Vector<T> TraitType;
    static void checkConsistency(const Vector<T>& v)
    {
        v.checkConsistency();
    }
};
#endif

} // namespace WTF

using WTF::Vector;
using WTF::UnsafeVectorOverflow;
using WTF::notFound;

#endif // WTF_Vector_h
