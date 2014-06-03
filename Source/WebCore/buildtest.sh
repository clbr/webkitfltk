#!/bin/sh

/opt/gcc48/bin/g++ -o test test.cpp -std=gnu++11 -I .. -I . \
-I ../WTF -I Modules/battery -I Modules/encryptedmedia -I Modules/gamepad \
-I Modules/geolocation -I Modules/indexeddb -I Modules/indexeddb/leveldb \
-I Modules/indieui -I Modules/mediacontrols/ -I Modules/mediasource \
-I Modules/mediastream -I Modules/navigatorcontentutils -I Modules/notifications \
-I Modules/plugins -I Modules/proximity -I Modules/quota -I Modules/vibration \
-I Modules/webaudio -I Modules/webdatabase -I Modules/websockets -I accessibility \
-I bindings -I bindings/generic -I bindings/js -I bridge -I bridge/c -I bridge/jsc \
-I crypto -I crypto/keys -I css -I cssjit -I dom -I dom/default -I editing -I fileapi \
-I history -I html -I html/canvas -I html/forms -I html/parser -I html/shadow \
-I html/track -I inspector -I loader -I loader/appcache -I loader/archive \
-I loader/archive/mhtml -I loader/cache -I loader/icon -I mathml -I page \
-I page/animation -I page/scrolling -I platform -I platform/animation \
-I platform/audio -I platform/graphics -I platform/graphics/cpu/arm \
-I platform/graphics/cpu/arm/filters -I platform/graphics/filters \
-I platform/graphics/filters/texmap -I platform/graphics/harfbuzz \
-I platform/graphics/harfbuzz/ng -I platform/graphics/opentype \
-I platform/graphics/texmap -I platform/graphics/transforms \
-I platform/image-decoders -I platform/image-decoders/bmp \
-I platform/image-decoders/gif -I platform/image-decoders/ico \
-I platform/image-decoders/jpeg -I platform/image-decoders/png \
-I platform/image-decoders/webp -I platform/leveldb -I platform/mediastream \
-I platform/mock -I platform/network -I platform/sql -I platform/text \
-I platform/text/icu -I plugins -I rendering -I rendering/line -I rendering/mathml \
-I rendering/shapes -I rendering/style -I rendering/svg -I replay -I storage -I style \
-I svg -I svg/animation -I svg/graphics -I svg/graphics/filters -I svg/properties \
-I websockets -I workers -I xml -I xml/parser -I ../JavaScriptCore \
-I ../JavaScriptCore/ForwardingHeaders -I ../JavaScriptCore/API \
-I ../JavaScriptCore/assembler -I ../JavaScriptCore/bytecode \
-I ../JavaScriptCore/bytecompiler -I ../JavaScriptCore/dfg \
-I ../JavaScriptCore/disassembler -I ../JavaScriptCore/heap \
-I ../JavaScriptCore/debugger -I ../JavaScriptCore/interpreter \
-I ../JavaScriptCore/jit -I ../JavaScriptCore/llint -I ../JavaScriptCore/parser \
-I ../JavaScriptCore/profiler -I ../JavaScriptCore/runtime -I ../JavaScriptCore/yarr \
-I ForwardingHeaders -I platform/cairo -I platform/graphics/cairo \
-I platform/graphics/freetype -I platform/linux -I platform/network/curl \
-I platform/text/icu -I /opt/icu/include/ \
`pkg-config --cflags cairo libxml-2.0 harfbuzz`  \
-DBUILDING_FLTK__ \
-DENABLE_CANVAS_PATH  -DENABLE_CHANNEL_MESSAGING  -DENABLE_CONTEXT_MENUS  \
-DENABLE_CSS_BOX_DECORATION_BREAK  -DENABLE_CSS_TRANSFORMS_ANIMATIONS_UNPREFIXED  \
-DENABLE_DETAILS_ELEMENT  -DENABLE_FTPDIR  -DENABLE_HIDDEN_PAGE_DOM_TIMER_THROTTLING  \
-DENABLE_ICONDATABASE  -DENABLE_IMAGE_DECODER_DOWN_SAMPLING  -DENABLE_INSPECTOR  \
-DENABLE_JIT  -DENABLE_LEGACY_VENDOR_PREFIXES  -DENABLE_LINK_PREFETCH  -DENABLE_LLINT  \
-DENABLE_METER_ELEMENT  -DENABLE_NAVIGATOR_HWCONCURRENCY  -DENABLE_PROMISES  \
-DENABLE_PROGRESS_ELEMENT  -DENABLE_SVG_FONTS  -DENABLE_TEMPLATE_ELEMENT  \
-DENABLE_WEB_SOCKETS  -DENABLE_XSLT  -DENABLE_SUBPIXEL_LAYOUT  -DENABLE_VIEW_MODE_CSS_MEDIA \
-DENABLE_CURSOR_SUPPORT  -DENABLE_DRAG_SUPPORT  -DENABLE_MATHML  -DENABLE_TEXT_CARET  \
-DENABLE_TEXT_SELECTION  -DENABLE_WILL_REVEAL_EDGE_EVENTS  -DWTF_USE_TEXTURE_MAPPER \
-DENABLE_NETSCAPE_PLUGIN_API=0 -DENABLE_SQL_DATABASE=0 -DENABLE_DATE_AND_TIME_INPUT_TYPES=0 \
libwebcore.a ../JavaScriptCore/libjsc.a ../WTF/wtf/libwtf.a \
-pthread -Wl,-zorigin,-rpath,$$ORIGIN -Wl,-rpath,/opt/icu/lib \
-L/opt/icu/lib \
-Wl,-Bstatic \
-licui18n -licuuc -licudata \
-lharfbuzz -lharfbuzz-icu \
-Wl,-Bdynamic \
-ldl \
/opt/fltk13/lib/libfltk_images.a /opt/fltk13/lib/libfltk_cairo.a /opt/fltk13/lib/libfltk.a \
`pkg-config --libs cairo` -lfontconfig -lfreetype -lpng -ljpeg -lz \
-lsqlite3 -lxml2 -lxslt \
-lssl -lcrypto -ldl -lcurl \
-L /usr/X11R7/lib -lXfixes -lXext -lXft -lXinerama -lX11 -lglib-2.0 \
-static-libstdc++ -static-libgcc
