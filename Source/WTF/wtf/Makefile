SRC = Assertions.cpp \
    Atomics.cpp \
    BitVector.cpp \
    CompilationThread.cpp \
    Compression.cpp \
    CryptographicUtilities.cpp \
    CryptographicallyRandomNumber.cpp \
    CurrentTime.cpp \
    DataLog.cpp \
    DateMath.cpp \
    DecimalNumber.cpp \
    FastBitVector.cpp \
    FastMalloc.cpp \
    FilePrintStream.cpp \
    FunctionDispatcher.cpp \
    GregorianDateTime.cpp \
    HashTable.cpp \
    MD5.cpp \
    MainThread.cpp \
    MediaTime.cpp \
    MetaAllocator.cpp \
    NumberOfCores.cpp \
    OSRandomSource.cpp \
    PageAllocationAligned.cpp \
    PageBlock.cpp \
    ParallelJobsGeneric.cpp \
    PrintStream.cpp \
    RAMSize.cpp \
    RandomNumber.cpp \
    RefCountedLeakCounter.cpp \
    RunLoop.cpp \
    SHA1.cpp \
    SixCharacterHash.cpp \
    StackStats.cpp \
    StackBounds.cpp \
    StringPrintStream.cpp \
    TCSystemAlloc.cpp \
    Threading.cpp \
    WTFThreadData.cpp \
    dtoa.cpp \
    dtoa/bignum-dtoa.cc \
    dtoa/bignum.cc \
    dtoa/cached-powers.cc \
    dtoa/diy-fp.cc \
    dtoa/double-conversion.cc \
    dtoa/fast-dtoa.cc \
    dtoa/fixed-dtoa.cc \
    dtoa/strtod.cc \
    text/AtomicString.cpp \
    text/AtomicStringTable.cpp \
    text/Base64.cpp \
    text/CString.cpp \
    text/StringBuilder.cpp \
    text/StringImpl.cpp \
    text/StringStatics.cpp \
    text/WTFString.cpp \
    threads/BinarySemaphore.cpp \
    unicode/UTF8.cpp \
    unicode/icu/CollatorICU.cpp \
    OSAllocatorPosix.cpp \
    ThreadIdentifierDataPthreads.cpp \
    ThreadingPthreads.cpp \
    fltk/MainThreadFLTK.cpp \
    fltk/RunLoopFLTK.cpp

OBJ := $(SRC:.cpp=.o)
OBJ := $(OBJ:.cc=.o)

CXXFLAGS += -I dtoa -I threads -I unicode -I .. -I .
CXXFLAGS += -std=gnu++11
CXXFLAGS += -DBUILDING_FLTK__
CXXFLAGS += $(shell icu-config --cppflags)
CXXFLAGS += -fno-rtti -fno-exceptions

include ../../Makefile.fltk.shared
CXXFLAGS += $(shell $(FLTKCONFIG) --cxxflags)

.PHONY: all clean

NAME = libwtf.a

all: $(OBJ)
	rm -f $(NAME)
	ar cru $(NAME) $(OBJ)
	ranlib $(NAME)

clean:
	rm -f $(OBJ)
