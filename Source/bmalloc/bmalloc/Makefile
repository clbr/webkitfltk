SRC = Allocator.cpp \
	Cache.cpp \
	Deallocator.cpp \
	Environment.cpp \
	FreeList.cpp \
	Heap.cpp \
	ObjectType.cpp \
	SegregatedFreeList.cpp \
	StaticMutex.cpp \
	VMHeap.cpp \
	mbmalloc.cpp

OBJ := $(SRC:.cpp=.o)
OBJ := $(OBJ:.cc=.o)

CXXFLAGS += -I .. -I .
CXXFLAGS += -std=gnu++11
CXXFLAGS += -DBUILDING_FLTK__
CXXFLAGS += $(shell icu-config --cppflags)
CXXFLAGS += -fno-rtti -fno-exceptions

include ../../Makefile.fltk.shared

.PHONY: all clean

NAME = libbmalloc.a

all: $(OBJ)
	rm -f $(NAME)
	ar cru $(NAME) $(OBJ)
	ranlib $(NAME)

clean:
	rm -f $(OBJ)
