// bsls_atomicoperations_x64_win_msvc.h                               -*-C++-*-
#ifndef INCLUDED_BSLS_ATOMICOPERATIONS_X64_WIN_MSVC
#define INCLUDED_BSLS_ATOMICOPERATIONS_X64_WIN_MSVC

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide implementations of atomic operations for X86_64/MSVC/Win64.
//
//@CLASSES:
//  bsls::AtomicOperations_X64_WIN_MSVC: implementation of atomics for
//                                       X86_64/MSVC/Windows
//
//@DESCRIPTION: This component provides classes necessary to implement atomics
// on the Windows X86_64 platform with MSVC compiler.  The classes are for
// private use only.  See 'bsls_atomicoperations' and 'bsls_atomic' for the
// public interface to atomics.

#ifndef INCLUDED_BSLS_ATOMICOPERATIONS_DEFAULT
#include <bsls_atomicoperations_default.h>
#endif

#ifndef INCLUDED_BSLS_PLATFORM
#include <bsls_platform.h>
#endif

#ifndef INCLUDED_BSLS_TYPES
#include <bsls_types.h>
#endif

#if defined(BSLS_PLATFORM_CPU_X86_64) && defined(BSLS_PLATFORM_CMP_MSVC)

#include <intrin.h>

// Visual C++ implementation exploits the fact that 'volatile' loads and stores
// have acquire and release semantics (load - acquire, store - release).  So
// these memory ordering guarantees come for free (and accidentally they are
// no-op on x86).  However the implementation of operations providing
// the sequential consistency guarantee still requires a memory barrier.
//
// As for interlocked intrinsics, they provide the sequential consistency
// guarantee, so no additional memory barrier is needed.
//
// For some explanations, see
// http://blogs.msdn.com/b/kangsu/archive/2007/07/16/
//                       volatile-acquire-release-memory-fences-and-vc2005.aspx
// and also MSDN documentation for 'volatile' and interlocked intrinsics in
// VC++ 2005 and later.

namespace BloombergLP {

namespace bsls {

struct AtomicOperations_X64_WIN_MSVC;
typedef AtomicOperations_X64_WIN_MSVC  AtomicOperations_Imp;

           // =======================================================
           // struct Atomic_TypeTraits<AtomicOperations_X64_WIN_MSVC>
           // =======================================================

template <>
struct Atomic_TypeTraits<AtomicOperations_X64_WIN_MSVC>
{
    struct Int
    {
        __declspec(align(4))
        volatile int d_value;
    };

    struct Int64
    {
        __declspec(align(8))
        volatile Types::Int64 d_value;
    };

    struct Pointer
    {
        __declspec(align(8))
        void * volatile d_value;
    };
};

                    // ====================================
                    // struct AtomicOperations_X64_WIN_MSVC
                    // ====================================

struct AtomicOperations_X64_WIN_MSVC
    : AtomicOperations_Default64<AtomicOperations_X64_WIN_MSVC>
{
    typedef Atomic_TypeTraits<AtomicOperations_X64_WIN_MSVC> AtomicTypes;

        // *** atomic functions for int ***

    static int getInt(const AtomicTypes::Int *atomicInt);

    static int getIntAcquire(const AtomicTypes::Int *atomicInt);

    static void setInt(AtomicTypes::Int *atomicInt, int value);

    static void setIntRelease(AtomicTypes::Int *atomicInt, int value);

    static int swapInt(AtomicTypes::Int *atomicInt, int swapValue);

    static int testAndSwapInt(AtomicTypes::Int *atomicInt,
                              int compareValue,
                              int swapValue);

    static int addIntNv(AtomicTypes::Int *atomicInt, int value);

        // *** atomic functions for Int64 ***

    static Types::Int64 getInt64(const AtomicTypes::Int64 *atomicInt);

    static Types::Int64 getInt64Acquire(const AtomicTypes::Int64 *atomicInt);

    static void setInt64(AtomicTypes::Int64 *atomicInt, Types::Int64 value);

    static void setInt64Release(AtomicTypes::Int64 *atomicInt,
                                Types::Int64 value);

    static Types::Int64 swapInt64(AtomicTypes::Int64 *atomicInt,
                                  Types::Int64 swapValue);

    static Types::Int64 testAndSwapInt64(AtomicTypes::Int64 *atomicInt,
                                         Types::Int64 compareValue,
                                         Types::Int64 swapValue);

    static Types::Int64 addInt64Nv(AtomicTypes::Int64 *atomicInt,
                                   Types::Int64 value);
};

// ===========================================================================
//                        INLINE FUNCTION DEFINITIONS
// ===========================================================================

                    // ------------------------------------
                    // struct AtomicOperations_X64_WIN_MSVC
                    // ------------------------------------

// Memory barrier for atomic operations with sequential consistency.
#define BSLS_ATOMIC_FENCE()  \
    _mm_mfence()

inline
int AtomicOperations_X64_WIN_MSVC::
    getInt(const AtomicTypes::Int *atomicInt)
{
    BSLS_ATOMIC_FENCE();
    return atomicInt->d_value;
}

inline
int AtomicOperations_X64_WIN_MSVC::
    getIntAcquire(const AtomicTypes::Int *atomicInt)
{
    return atomicInt->d_value;
}

inline
void AtomicOperations_X64_WIN_MSVC::
    setInt(AtomicTypes::Int *atomicInt, int value)
{
    atomicInt->d_value = value;
    BSLS_ATOMIC_FENCE();
}

inline
void AtomicOperations_X64_WIN_MSVC::
    setIntRelease(AtomicTypes::Int *atomicInt, int value)
{
    atomicInt->d_value = value;
}

inline
int AtomicOperations_X64_WIN_MSVC::
    swapInt(AtomicTypes::Int *atomicInt, int swapValue)
{
    return _InterlockedExchange(
            reinterpret_cast<long volatile *>(&atomicInt->d_value),
            swapValue);
}

inline
int AtomicOperations_X64_WIN_MSVC::
    testAndSwapInt(AtomicTypes::Int *atomicInt,
                   int compareValue,
                   int swapValue)
{
    return _InterlockedCompareExchange(
            reinterpret_cast<long volatile *>(&atomicInt->d_value),
            swapValue,
            compareValue);
}

inline
int AtomicOperations_X64_WIN_MSVC::
    addIntNv(AtomicTypes::Int *atomicInt, int value)
{
    return _InterlockedExchangeAdd(
            reinterpret_cast<long volatile *>(&atomicInt->d_value),
            value)
        + value;
}

inline
Types::Int64 AtomicOperations_X64_WIN_MSVC::
    getInt64(const AtomicTypes::Int64 *atomicInt)
{
    BSLS_ATOMIC_FENCE();
    return atomicInt->d_value;
}

inline
Types::Int64 AtomicOperations_X64_WIN_MSVC::
    getInt64Acquire(const AtomicTypes::Int64 *atomicInt)
{
    return atomicInt->d_value;
}

inline
void AtomicOperations_X64_WIN_MSVC::
    setInt64(AtomicTypes::Int64 *atomicInt, Types::Int64 value)
{
    atomicInt->d_value = value;
    BSLS_ATOMIC_FENCE();
}

inline
void AtomicOperations_X64_WIN_MSVC::
    setInt64Release(AtomicTypes::Int64 *atomicInt,
                    Types::Int64 value)
{
    atomicInt->d_value = value;
}

inline
Types::Int64 AtomicOperations_X64_WIN_MSVC::
    swapInt64(AtomicTypes::Int64 *atomicInt,
              Types::Int64 swapValue)
{
    return _InterlockedExchange64(&atomicInt->d_value, swapValue);
}

inline
Types::Int64 AtomicOperations_X64_WIN_MSVC::
    testAndSwapInt64(AtomicTypes::Int64 *atomicInt,
                     Types::Int64 compareValue,
                     Types::Int64 swapValue)
{
    return _InterlockedCompareExchange64(
            &atomicInt->d_value,
            swapValue,
            compareValue);
}

inline
Types::Int64 AtomicOperations_X64_WIN_MSVC::
    addInt64Nv(AtomicTypes::Int64 *atomicInt,
               Types::Int64 value)
{
    return _InterlockedExchangeAdd64(
            &atomicInt->d_value,
            value)
        + value;
}

#undef BSLS_ATOMIC_FENCE

}  // close package namespace

}  // close enterprise namespace

#endif  // X86_64 && MSVC

#endif

// ----------------------------------------------------------------------------
// Copyright (C) 2013 Bloomberg L.P.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------- END-OF-FILE ----------------------------------
