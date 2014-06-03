SRC = WebCoreSupport/FLTKAdjustmentWatcher.cpp \
	WebCoreSupport/PlatformStrategiesFLTK.cpp \
	WebCoreSupport/TextCheckerClientFLTK.cpp \
	WebCoreSupport/DocumentLoaderFLTK.cpp \
	WebCoreSupport/AssertMatchingEnums.cpp \
	WebCoreSupport/EditorClientFLTK.cpp \
	WebCoreSupport/FrameNetworkingContextFLTK.cpp \
	WebCoreSupport/DumpRenderTreeSupportFLTK.cpp \
	WebCoreSupport/ContextMenuClientFLTK.cpp \
	WebCoreSupport/UserMediaClientFLTK.cpp \
	WebCoreSupport/FrameLoaderClientFLTK.cpp \
	WebCoreSupport/BatteryClientFLTK.cpp \
	WebCoreSupport/NavigatorContentUtilsClientFLTK.cpp \
	WebCoreSupport/InspectorClientFLTK.cpp \
	WebCoreSupport/DeviceOrientationClientFLTK.cpp \
	WebCoreSupport/GeolocationClientFLTK.cpp \
	WebCoreSupport/ChromeClientFLTK.cpp \
	WebCoreSupport/DeviceMotionClientFLTK.cpp \
	WebCoreSupport/DragClientFLTK.cpp \
	WebCoreSupport/ProgressTrackerClientFLTK.cpp \
	webkit/webkitwebdatasource.cpp \
	webkit/webkitwebsettings.cpp \
	webkit/webkitfavicondatabase.cpp \
	webkit/webkitwebpolicydecision.cpp \
	webkit/webkitapplicationcache.cpp \
	webkit/webkitgeolocationpolicydecision.cpp \
	webkit/webkitdownload.cpp \
	webkit/webkitwebhistoryitem.cpp \
	webkit/webkitwebbackforwardlist.cpp \
	webkit/webkitwebresource.cpp \
	webkit/webkitsecurityorigin.cpp \
	webkit/webkitnetworkrequest.cpp \
	webkit/webkithittestresult.cpp \
	webkit/webkitviewportattributes.cpp \
	webkit/webkitwebinspector.cpp \
	webkit/webkitwebdatabase.cpp \
	webkit/webkitauthenticationdialog.cpp \
	webkit/webkitwebnavigationaction.cpp \
	webkit/webkitwebwindowfeatures.cpp \
	webkit/webkitglobals.cpp \
	webkit/webkitwebview.cpp \
	webkit/webkitnetworkresponse.cpp \
	webkit/webkitwebframe.cpp

OBJ := $(SRC:.cpp=.o)
OBJ := $(OBJ:.cc=.o)

CXXFLAGS += -I .. -I ../.. -I . -I ../../WTF -I webkit -I WebCoreSupport
CXXFLAGS += -std=gnu++11
CXXFLAGS += -DBUILDING_FLTK__
CXXFLAGS += -D_GLIBCXX_USE_SCHED_YIELD -D_GLIBCXX_USE_NANOSLEEP
CXXFLAGS += $(shell icu-config --cppflags)
CXXFLAGS += $(shell pkg-config --cflags cairo libxml-2.0 harfbuzz)
CXXFLAGS += $(shell fltk-config13 --cxxflags)

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

.PHONY: all clean

NAME = libwebkitfltk.a

LIBS = -lz -pthread -lxslt -lxml2 -ldl -lsqlite3 `icu-config --ldflags` \
	-lharfbuzz -lharfbuzz-icu -lfreetype -lfontconfig -lcairo \
	-lpng -ljpeg -lrt -lcurl -lssl -lcrypto \
	`fltk-config13 --ldflags`

all: $(NAME)

$(NAME): $(OBJ)
	rm -f $(NAME)
	ar cru $(NAME) $(OBJ)
	ranlib $(NAME)
	echo -e "CREATE tmp.a\n\
	ADDLIB $(NAME)\n\
	ADDLIB ../WTF/wtf/libwtf.a\n\
	ADDLIB ../JavaScriptCore/libjsc.a\n\
	ADDLIB ../WebCore/libwebcore.a\n\
	SAVE\n\
	END" | ar -M
	mv tmp.a $(NAME)

clean:
	rm -f $(OBJ)
