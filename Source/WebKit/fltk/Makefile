SRC = $(wildcard *.cpp ../Storage/*.cpp)

OBJ := $(SRC:.cpp=.o)
OBJ := $(OBJ:.cc=.o)

CXXFLAGS += -I .. -I ../.. -I . -I ../../WTF -I webkit -I WebCoreSupport -I ../Storage
CXXFLAGS += -std=gnu++11 -g -Wall
CXXFLAGS += -DBUILDING_FLTK__
CXXFLAGS += -D_GLIBCXX_USE_SCHED_YIELD -D_GLIBCXX_USE_NANOSLEEP
CXXFLAGS += $(shell icu-config --cppflags)
CXXFLAGS += $(shell pkg-config --cflags cairo libxml-2.0 harfbuzz)

WEBC = ../../WebCore
JSC = ../../JavaScriptCore
CXXFLAGS += -I $(JSC)/ForwardingHeaders \
	-I $(WEBC) \
	-I $(WEBC)/ForwardingHeaders \
	-I $(WEBC)/Modules/geolocation \
	-I $(WEBC)/Modules/filesystem \
	-I $(WEBC)/Modules/mediastream \
	-I $(WEBC)/Modules/navigatorcontentutils \
	-I $(WEBC)/Modules/networkinfo \
	-I $(WEBC)/Modules/webdatabase \
	-I $(WEBC)/Modules/websockets \
	-I $(WEBC)/accessibility \
	-I $(WEBC)/bindings/generic \
	-I $(WEBC)/bindings/js \
	-I $(WEBC)/bindings \
	-I $(WEBC)/bridge \
	-I $(WEBC)/bridge/c \
	-I $(WEBC)/bridge/jsc \
	-I $(WEBC)/css \
	-I $(WEBC)/dom \
	-I $(WEBC)/dom/default \
	-I $(WEBC)/editing \
	-I $(WEBC)/fileapi \
	-I $(WEBC)/history \
	-I $(WEBC)/html \
	-I $(WEBC)/html/forms \
	-I $(WEBC)/html/shadow \
	-I $(WEBC)/html/track \
	-I $(WEBC)/inspector \
	-I $(WEBC)/loader \
	-I $(WEBC)/loader/archive \
	-I $(WEBC)/loader/appcache \
	-I $(WEBC)/loader/icon \
	-I $(WEBC)/loader/cache \
	-I $(WEBC)/page \
	-I $(WEBC)/page/animation \
	-I $(WEBC)/page/scrolling \
	-I $(WEBC)/platform \
	-I $(WEBC)/platform/animation \
	-I $(WEBC)/platform/audio \
	-I $(WEBC)/platform/fltk \
	-I $(WEBC)/platform/cairo \
	-I $(WEBC)/platform/graphics \
	-I $(WEBC)/platform/graphics/filters \
	-I $(WEBC)/platform/graphics/harfbuzz \
	-I $(WEBC)/platform/graphics/harfbuzz/ng \
	-I $(WEBC)/platform/graphics/cairo \
	-I $(WEBC)/platform/graphics/texmap \
	-I $(WEBC)/platform/graphics/opentype \
	-I $(WEBC)/platform/graphics/transforms \
	-I $(WEBC)/platform/mediastream \
	-I $(WEBC)/platform/mock \
	-I $(WEBC)/platform/network \
	-I $(WEBC)/platform/network/curl \
	-I $(WEBC)/platform/sql \
	-I $(WEBC)/platform/text \
	-I $(WEBC)/plugins \
	-I $(WEBC)/rendering \
	-I $(WEBC)/rendering/line \
	-I $(WEBC)/rendering/shapes \
	-I $(WEBC)/rendering/style \
	-I $(WEBC)/storage \
	-I $(WEBC)/style \
	-I $(WEBC)/svg \
	-I $(WEBC)/svg/animation \
	-I $(WEBC)/svg/graphics \
	-I $(WEBC)/svg/properties \
	-I $(WEBC)/workers \
	-I $(JSC) \
	-I $(JSC)/API \
	-I $(JSC)/assembler \
	-I $(JSC)/bytecode \
	-I $(JSC)/bytecompiler \
	-I $(JSC)/disassembler \
	-I $(JSC)/dfg \
	-I $(JSC)/heap \
	-I $(JSC)/debugger \
	-I $(JSC)/interpreter \
	-I $(JSC)/jit \
	-I $(JSC)/llint \
	-I $(JSC)/parser \
	-I $(JSC)/profiler \
	-I $(JSC)/runtime


include ../../Makefile.fltk.shared
CXXFLAGS += $(shell $(FLTKCONFIG) --cxxflags)

.PHONY: all clean install library tests

# /tmp tends to be in RAM. When building on a HD, this saves several seconds.
NAME = /tmp/libwebkitfltk.a

LIBS = -lz -pthread -lxslt -lxml2 -ldl -lsqlite3 \
	`icu-config --ldflags` -lharfbuzz -lharfbuzz-icu \
	-lfreetype -lfontconfig -lcairo \
	-lpng -ljpeg -lrt -lcurl -lssl -lcrypto -lglib-2.0 \
	`$(FLTKCONFIG) --ldflags --use-images` \
	-static-libgcc -static-libstdc++

all: library tests

library: $(NAME)

tests: testapp/testapp bench/webkitbench

-include $(OBJ:.o=.d)

$(NAME): $(OBJ) ../../WTF/wtf/libwtf.a ../../JavaScriptCore/libjsc.a \
		../../WebCore/libwebcore.a ../../bmalloc/bmalloc/libbmalloc.a
	rm -f $(NAME)
	ar cru $(NAME) $(OBJ)
	ranlib $(NAME)
	printf "CREATE tmp.a\n\
	ADDLIB $(NAME)\n\
	ADDLIB ../../WTF/wtf/libwtf.a\n\
	ADDLIB ../../bmalloc/bmalloc/libbmalloc.a\n\
	ADDLIB ../../JavaScriptCore/libjsc.a\n\
	ADDLIB ../../WebCore/libwebcore.a\n\
	SAVE\n\
	END" | ar -M
	mv tmp.a $(NAME)

testapp/testapp: $(NAME) Makefile $(wildcard testapp/*.cpp)
	$(CXX) -o testapp/testapp testapp/*.cpp $(CXXFLAGS) $(NAME) \
		$(LIBS)

bench/webkitbench: $(NAME) Makefile $(wildcard bench/*.cpp)
	$(CXX) -o bench/webkitbench bench/*.cpp $(CXXFLAGS) $(NAME) \
		$(LIBS)

clean:
	rm -f $(OBJ)

%.o: %.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $<
	@$(CXX) -MM -MP $(CXXFLAGS) $< -o $*.d

PREFIX ?= /usr

install: library
	mkdir -p $(DESTDIR)/$(PREFIX)/lib/pkgconfig
	mkdir -p $(DESTDIR)/$(PREFIX)/include/webkitfltk
	install -m644 $(NAME) $(DESTDIR)/$(PREFIX)/lib
	insthdr=`grep 'include "' webkit.h | cut -d\" -f2`; \
	for hdr in $$insthdr webkit.h; do \
		install -m644 $$hdr $(DESTDIR)/$(PREFIX)/include/webkitfltk; \
	done
	majorver=`grep WK_FLTK_MAJOR webkit.h | cut -d" " -f3`; \
	minorver=`grep WK_FLTK_MINOR webkit.h | cut -d" " -f3`; \
	patchver=`grep WK_FLTK_PATCH webkit.h | cut -d" " -f3`; \
	sed -e "s@PREFIX@$(PREFIX)@" -e "s@VERSION@$$majorver.$$minorver.$$patchver@" \
		webkitfltk.pc > $(DESTDIR)/$(PREFIX)/lib/pkgconfig/webkitfltk.pc
