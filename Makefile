ROOTPATH:=$(abspath ../../sdk/)
# LIB:=$(ROOTPATH)/lib
GEN:=$(ROOTPATH)/gen/host/libnix

# PKG_ICU:=$(LIB)/libicu67/instdir/lib/pkgconfig/
# PKG_SQLITE:=$(LIB)/sqlite/instdir/lib/pkgconfig/
PKG_FONTCONFIG:=$(ROOTPATH)/morphoswb/libs/fontconfig/MorphOS/
PKG:=$(PKG_ICU):$(PKG_SQLITE)

DEBIAN_PKG:=libicu-dev ruby-dev clang-7
NATIVE_GCC:=/usr/bin/

CROSS_CC:=/opt/ppc-amigaos/bin/ppc-amigaos-gcc
CROSS_CXX:=/opt/ppc-amigaos/bin/ppc-amigaos-g++

OBJC:=$(ROOTPATH)/morphoswb/classes/frameworks/includes/

CMAKE = cmake
STRIP = ppc-amigaos-strip

USE_CLIB2=YES
ifeq ($(USE_CLIB2), YES)
LIBC_PATH=$(SDK_PATH)/local/clib2
else
LIBC_PATH=$(SDK_PATH)/local/newlib
endif
	
LIB:=$(LIBC_PATH)/lib
CMN_INC:=$(SDK_PATH)/local/common/include

all:

configure-native:
	rm -rf build
	mkdir build
	(cd build && \
		cmake -DCMAKE_MODULE_PATH=$(realpath Source/cmake) \
		-DCMAKE_BUILD_TYPE=Release -DPORT=JSCOnly -DUSE_SYSTEM_MALLOC=YES -DCMAKE_CXX_FLAGS="-O2 -fPIC" -DCMAKE_C_FLAGS="-O2 -fPIC" \
		-DCMAKE_C_COMPILER=$(NATIVE_GCC)gcc -DCMAKE_CXX_COMPILER=$(NATIVE_GCC)g++ \
		$(realpath ./))

jscore-native:
	rm -rf WebKitBuild build
	mkdir build
	(cd build && \
		$(realpath Tools/Scripts/run-javascriptcore-tests) --jsc-only \
		--cmakeargs='-DCMAKE_MODULE_PATH=$(realpath Source/cmake) -DJAVASCRIPTCORE_DIR=$(realpath Source/JavaScriptCore) \
			-DCMAKE_BUILD_TYPE=Release -DPORT=JSCOnly -DUSE_SYSTEM_MALLOC=YES -DCMAKE_CXX_FLAGS="-O2 -fPIC" -DCMAKE_C_FLAGS="-O2 -fPIC" \
			-DCMAKE_C_COMPILER=$(NATIVE_GCC)gcc -DCMAKE_CXX_COMPILER=$(NATIVE_GCC)g++')
	mkdir -p /opt/apps/webkitty/WebKitBuild/Release/Source/JavaScriptCore/shell/
	cp -a Source/JavaScriptCore/API/tests/testapiScripts /opt/apps/webkitty/WebKitBuild/Release/Source/JavaScriptCore/shell/
	Tools/Scripts/run-javascriptcore-tests --root WebKitBuild/Release/Source/JavaScriptCore/shell/ --no-jsc-stress --no-jit-stress-test

jscore-amigaos: amigaos.cmake
#rm -rf WebKitBuild cross-build
	mkdir -p cross-build WebKitBuild/Release/bin
	(cd cross-build && \
		$(realpath Tools/Scripts/run-javascriptcore-tests) --jsc-only \
		--no-testmasm --no-testair --no-testb3 --no-testdfg --no-testapi \
		--no-jsc-stress --no-mozilla-tests --no-jit-stress-tests --no-quick \
		--cmakeargs='-DUSE_CLIB2=$(USE_CLIB2) \
		-DCMAKE_CROSSCOMPILING=ON -DCMAKE_TOOLCHAIN_FILE=$(realpath amigaos.cmake) -DCMAKE_MODULE_PATH=$(realpath Source/cmake) \
		-DJAVASCRIPTCORE_DIR=$(realpath Source/JavaScriptCore) -DBUILD_SHARED_LIBS=NO \
		-DICU_INCLUDE_DIR=$(LIBC_PATH)/include \
		-DICU_LIBRARIES=$(LIBC_PATH)/lib \
		-DICU_DATA_LIBRARY_RELEASE=$(LIBC_PATH)/lib/libicudata.a \
		-DICU_I18N_LIBRARY_RELEASE=$(LIBC_PATH)/lib/libicui18n.a \
		-DICU_UC_LIBRARY_RELEASE=$(LIBC_PATH)/lib/libicuuc.a \
		-DJPEG_LIBRARY=$(LIB)/libjpeg -DJPEG_INCLUDE_DIR=$(LIB) \
		-DLIBXML2_LIBRARY=$(LIB)/libxml2/instdir/lib -DLIBXML2_INCLUDE_DIR=$(LIB)/libxml2/instdir/include/libxml2 \
		-DPNG_LIBRARY=$(GEN)/libpng16/lib/ -DPNG_INCLUDE_DIR=$(GEN)/libpng16/include \
		-DLIBXSLT_LIBRARIES=$(LIB)/libxslt/instdir/lib -DLIBXSLT_INCLUDE_DIR=$(LIB)/libxslt/instdir/include \
		-DSQLITE_LIBRARIES=$(LIB)/sqlite/instdir/lib -DSQLITE_INCLUDE_DIR=$(LIB)/sqlite/instdir/include \
		-DCairo_INCLUDE_DIR=$(LIBC_PATH)/include/cairo \
		-DCMAKE_BUILD_TYPE=Release -DPORT=JSCOnly -DUSE_SYSTEM_MALLOC=YES \
		-DCMAKE_FIND_LIBRARY_SUFFIXES=".a" ')
	cp -a Source/JavaScriptCore/API/tests/testapiScripts ./WebKitBuild/Release/Source/JavaScriptCore/shell/
#	Tools/Scripts/run-javascriptcore-tests --root WebKitBuild/Release/Source/JavaScriptCore/shell/ --no-jsc-stress --no-jit-stress-test

jscore-pack:
	$(STRIP) ./WebKitBuild/Release/Source/JavaScriptCore/shell/jsc
	$(STRIP) ./WebKitBuild/Release/Source/JavaScriptCore/shell/testRegExp
	$(STRIP) ./WebKitBuild/Release/Source/JavaScriptCore/shell/testair
	$(STRIP) ./WebKitBuild/Release/Source/JavaScriptCore/shell/testapi
	$(STRIP) ./WebKitBuild/Release/Source/JavaScriptCore/shell/testb3
	$(STRIP) ./WebKitBuild/Release/Source/JavaScriptCore/shell/testdfg
	$(STRIP) ./WebKitBuild/Release/Source/JavaScriptCore/shell/testmasm
	tar cJf testsuite.tar.xz Tools ./WebKitBuild/Release/Source/JavaScriptCore/shell/jsc \
		./WebKitBuild/Release/Source/JavaScriptCore/shell/testRegExp \
		./WebKitBuild/Release/Source/JavaScriptCore/shell/testair \
		./WebKitBuild/Release/Source/JavaScriptCore/shell/testapi \
		./WebKitBuild/Release/Source/JavaScriptCore/shell/testb3 \
		./WebKitBuild/Release/Source/JavaScriptCore/shell/testdfg \
		./WebKitBuild/Release/Source/JavaScriptCore/shell/testmasm \
		 JSTests LayoutTests PerformanceTests

configure: amigaos.cmake link.sh CMakeLists.txt Dummy/libdummy.a ffmpeg/.buildstamp
	rm -rf cross-build
	mkdir cross-build
	(cd cross-build \
		cmake -DUSE_CLIB2=$(USE_CLIB2) \
		-DCMAKE_CROSSCOMPILING=ON -DCMAKE_BUILD_TYPE=RelWithDebugInfo -DCMAKE_TOOLCHAIN_FILE=$(realpath amigaos.cmake) -DCMAKE_DL_LIBS="syscall" \
		-DBUILD_SHARED_LIBS=NO -DPORT=AmigaOS -DENABLE_WEBCORE=1 -DENABLE_WEBKIT_LEGACY=1 -DLOG_DISABLED=0 -DAMIGAOS_MINIMAL=0 -DROOTPATH="$(ROOTPATH)" \
		-DJPEG_LIBRARY=$(LIB)/libjpeg/libjpeg.a \
		-DJPEG_INCLUDE_DIR=$(LIB)/libjpeg \
		-DLIBXML2_LIBRARY=$(LIB)/libxml2/instdir/lib/libxml2.a \
		-DLIBXML2_INCLUDE_DIR="$(LIB)/libxml2/instdir/include/libxml2/" \
		-DPNG_LIBRARIES=$(GEN)/libpng16/lib/libpng16.a \
		-DPNG_PNG_INCLUDE_DIR=$(GEN)/libpng16/include/libpng16/ \
		-DPNG_INCLUDE_DIRS=$(GEN)/libpng16/include/libpng16/ \
		-DLIBXSLT_LIBRARIES=$(LIB)/libxslt/instdir/lib/libxslt.a \
		-DLIBXSLT_INCLUDE_DIR=$(LIB)/libxslt/instdir/include \
		-DSQLITE_LIBRARIES=$(LIB)/sqlite/instdir/lib/libsqlite3.a \
		-DSQLITE_INCLUDE_DIR=$(LIB)/sqlite/instdir/include \
		-DSQLite3_LIBRARY=$(LIB)/sqlite/instdir/include \
		-DSQLite3_INCLUDE_DIR=$(LIB)/sqlite/instdir/include \
		-DCAIRO_INCLUDE_DIRS=$(ROOTPATH)/amigaoswb/libs/cairo/AmigaOS/os-include/cairo \
		-DCAIRO_LIBRARIES="$(ROOTPATH)/amigaoswb/libs/cairo/AmigaOS/lib/libnix/libcairo.a" \
		-DCairo_INCLUDE_DIR=$(ROOTPATH)/amigaoswb/libs/cairo/AmigaOS/os-include/cairo \
		-DCairo_LIBRARY="$(ROOTPATH)/amigaoswb/libs/cairo/AmigaOS/lib/libnix/libcairo.a" \
		-DHarfBuzz_INCLUDE_DIR="$(realpath Dummy)"\
		-DHarfBuzz_LIBRARY=$(GEN)/lib/libnghttp2.a \
		-DICU_ROOT="$(LIB)/libicu67/instdir/" \
		-DICU_UC_LIBRARY_RELEASE="$(LIB)/libicu67/instdir/lib/libicuuc.a" \
		-DICU_DATA_LIBRARY_RELEASE="$(LIB)/libicu67/instdir/lib/libicudata.a" \
		-DICU_I18N_LIBRARY_RELEASE="$(LIB)/libicu67/instdir/lib/libicui18n.a" \
		-DHarfBuzz_ICU_LIBRARY="$(realpath Dummy)/libdummy.a" \
		-DFREETYPE_INCLUDE_DIRS="$(ROOTPATH)/amigaoswb/libs/freetype/include" \
		-DFREETYPE_LIBRARY="$(ROOTPATH)/amigaoswb/libs/freetype/library/lib/libfreetype.a" \
		-DFontconfig_LIBRARY="$(ROOTPATH)/amigaoswb/libs/fontconfig/MorphOS/libfontconfig-glue.a" \
		-DFontconfig_INCLUDE_DIR="$(ROOTPATH)/amigaoswb/libs/fontconfig" \
		-DOpenJPEG_INCLUDE_DIR="$(GEN)/include/openjpeg-2.5" \
		-DWebP_INCLUDE_DIR="$(GEN)/include" -DWebP_LIBRARY="$(GEN)/lib/libwebp.a" -DWebP_DEMUX_LIBRARY="$(GEN)/lib/libwebpdemux.a"\
		-DAVFORMAT_LIBRARY="ffmpeg/instdir/lib/libavformat.a" -DAVFORMAT_INCLUDE_DIR="$(realpath ffmpeg/instdir/include)" \
		-DAVCODEC_LIBRARY="ffmpeg/instdir/lib/libavcodec.a" -DAVCODEC_INCLUDE_DIR="$(realpath ffmpeg/instdir/include)" \
		-DAVUTIL_LIBRARY="ffmpeg/instdir/lib/libavutil.a" -DAVUTIL_INCLUDE_DIR="$(realpath ffmpeg/instdir/include)" \
		-DSWSCALE_LIBRARY="ffmpeg/instdir/lib/libswscale.a" -DSWSCALE_INCLUDE_DIR="$(realpath ffmpeg/instdir/include)" \
		-DWOFF2_LIBRARY="$(GEN)/lib/libwoff2dec.a $(GEN)/lib/libwoff2common.a" -DWOFF2_INCLUDE_DIR="$(GEN)/include/" \
		-DOBJC_INCLUDE="$(OBJC)" \
		-DCMAKE_MODULE_PATH=$(realpath Source/cmake) $(realpath ./))

configure-mini: amigaos.cmake link.sh CMakeLists.txt Dummy/libdummy.a ffmpeg/.buildstamp
	rm -rf cross-build-mini
	mkdir cross-build-mini
	(cd cross-build-mini && \
		cmake -DUSE_CLIB2=$(USE_CLIB2) \
		-DCMAKE_CROSSCOMPILING=ON -DCMAKE_BUILD_TYPE=RelWithDebugInfo -DCMAKE_TOOLCHAIN_FILE=$(realpath amigaos.cmake) -DCMAKE_DL_LIBS="syscall" \
		-DCMAKE_C_COMPILER=$(CROSS_CC) -DCMAKE_CXX_COMPILER=$(CROSS_CXX) \
		-DBUILD_SHARED_LIBS=NO -DPORT=AmigaOS -DENABLE_WEBCORE=1 -DENABLE_WEBKIT_LEGACY=1 -DLOG_DISABLED=0 \
		-DENABLE_OPENTYPE_MATH=0 \
		-DAMIGAOS_MINIMAL=1 -DROOTPATH="$(ROOTPATH)" \
		-DJPEG_LIBRARY=$(LIBC_PATH)/lib/libjpeg.a \
		-DJPEG_INCLUDE_DIR=$(LIBC_PATH)/include \
		-DLIBXML2_LIBRARY=$(LIBC_PATH)/lib/libxml2.a \
		-DLIBXML2_INCLUDE_DIR="$(LIBC_PATH)/include/libxml2/" \
		-DPNG_LIBRARY="$(LIBC_PATH)/lib/libpng16.a" \
		-DPNG_INCLUDE_DIR=$(LIBC_PATH)/include/libpng16/ \
		-DLIBXSLT_LIBRARIES=$(LIB)/libxslt/instdir/lib/libxslt.a \
		-DLIBXSLT_INCLUDE_DIR=$(CMN_INC) \
		-DSQLite3_LIBRARY=$(LIB)/sqlite/instdir/include \
		-DSQLite3_INCLUDE_DIR=$(CMN_INC) \
		-DCairo_INCLUDE_DIR=$(LIBC_PATH)/include/cairo/ \
		-DCairo_LIBRARY="$(LIBC_PATH)/lib/libcairo.a" \
		-DHarfBuzz_INCLUDE_DIR=$(LIBC_PATH)/include/harfbuzz/ \
		-DHarfBuzz_LIBRARY=$(LIBC_PATH)/lib/libnghttp2.a \
		-DHarfBuzz_ICU_LIBRARY="$(LIBC_PATH)/lib/libharfbuzz-icu.a" \
		-DICU_ROOT="$(LIB)/libicu67/instdir/" \
		-DICU_INCLUDE_DIR=$(LIBC_PATH)/include \
		-DICU_UC_LIBRARY_RELEASE="$(LIBC_PATH)/lib/libicuuc.a" \
		-DICU_DATA_LIBRARY_RELEASE="$(LIBC_PATH)/lib/libicudata.a" \
		-DICU_I18N_LIBRARY_RELEASE="$(LIBC_PATH)/lib/libicui18n.a" \
		-DFREETYPE_INCLUDE_DIRS="$(CMN_INC)" \
		-DFREETYPE_LIBRARY="$(LIBC_PATH)/lib/libfreetype.a" \
		-DFontconfig_LIBRARY="$(LIBC_PATH)/lib/libfontconfig.a" \
		-DFontconfig_INCLUDE_DIR="$(LIBC_PATH)/include/" \
		-DOpenJPEG_INCLUDE_DIR="$(LIBC_PATH)/include/openjpeg-2.5" \
		-DWebP_INCLUDE_DIR="$(CMN_INC)" \
		-DWebP_LIBRARY="$(LIBC_PATH)/lib/libwebp.a" \
		-DWebP_DEMUX_LIBRARY="$(LIBC_PATH)/lib/libwebpdemux.a"\
		-DAVFORMAT_LIBRARY="ffmpeg/instdir/lib/libavformat.a" -DAVFORMAT_INCLUDE_DIR="$(realpath ffmpeg/instdir/include)" \
		-DAVCODEC_LIBRARY="ffmpeg/instdir/lib/libavcodec.a" -DAVCODEC_INCLUDE_DIR="$(realpath ffmpeg/instdir/include)" \
		-DAVUTIL_LIBRARY="ffmpeg/instdir/lib/libavutil.a" -DAVUTIL_INCLUDE_DIR="$(realpath ffmpeg/instdir/include)" \
		-DSWSCALE_LIBRARY="ffmpeg/instdir/lib/libswscale.a" -DSWSCALE_INCLUDE_DIR="$(realpath ffmpeg/instdir/include)" \
		-DOBJC_INCLUDE="$(OBJC)" \
		-DCURL_INCLUDE_DIR="$(CMN_INC)" \
		-DOPENSSL_INCLUDE_DIR="$(CMN_INC)" \
		-DZLIB_INCLUDE_DIR="$(CMN_INC)" \
		-DNGHTTP2_INCLUDE_DIRS="$(CMN_INC)" \
		-DHYPHEN_INCLUDE_DIR="$(CMN_INC)" \
		-DLibPSL_LIBRARY=$(LIBC_PATH)/lib/libpsl.a \
		-DLibPSL_INCLUDE_DIR="$(CMN_INC)" \
		-DOpenJPEG_LIBRARY=$(LIBC_PATH)/lib/libopenjp2.a \
		-DCMAKE_MODULE_PATH=$(realpath Source/cmake) $(realpath ./))

build:
	(cd cross-build && make -j$(shell nproc))
	echo "Link done"
	$(STRIP) cross-build/Tools/morphos/MiniBrowser.db -o cross-build/Tools/morphos/MiniBrowser
	echo "Stripped binary in cross-build/Tools/morphos/MiniBrowser"

#		-Wdev --debug-output --trace --trace-expand \

build-mini:
	(cd cross-build-mini && make -j$(shell nproc))

cross-build:
	make configure

.build: cross-build build

cross-build-mini:
	make configure-mini

.build-mini: cross-build-mini build-mini

amigaos.cmake: amigaos.cmake.in
	gcc -xc -E -P -C -o$@ -nostdinc $@.in -D_IN_ROOTPATH=$(ROOTPATH) -D_IN_DUMMYPATH=$(realpath Dummy)

link.sh: link.sh.in
	gcc -xc -E -P -C -o$@ -nostdinc $@.in -D_IN_ROOTPATH=$(ROOTPATH)
	chmod u+x $@

libwebkit.a:
	(cd cross-build/Source/WebKitLegacy && make)

clean:
	rm -rf amigaos.cmake cross-build cross-build-mini WebKitBuild build link.sh

install:

install-iso:

source:

sdk:

$(CMAKE):
	rm -rf cmake-3.16.2
	tar xf cmake-3.16.2.tar.gz
	(cd cmake-3.16.2 && ./bootstrap -- -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_USE_OPENSSL=OFF )
	(cd cmake-3.16.2 && make -j$(shell nproc))

Dummy/libdummy.a:
	ppc-amigaos-gcc -c -o Dummy/dummy.o Dummy/dummy.c
	ppc-amigaos-ar rc Dummy/libdummy.a Dummy/dummy.o
	ppc-amigaos-ranlib Dummy/libdummy.a
	cp Dummy/libdummy.a Dummy/libdl.a

ffmpeg/.buildstamp:
	cd ffmpeg && make

miniscp:
	scp cross-build/Tools/morphos/MiniBrowser jaca@192.168.2.5:/Users/jaca

minidump:
	@read -p "Address:" address; \
	ppc-morphos-objdump --demangle --disassemble -l --source cross-build/Tools/morphos/MiniBrowser.db --start-address $$address | less

minirelease:
	rm -rf WebKitty webkitty.tar webkitty.tar.xz webkitty.lha
	mkdir -p WebKitty/MOSSYS/Data/ICU
	cp cross-build/Tools/morphos/MiniBrowser WebKitty/
	cp -a Source/WebCore/Resources WebKitty/Resources
	cp -a $(ROOTPATH)/lib/libicu/instdir/icu/54.2/icudt54b WebKitty/MOSSYS/Data/ICU/icudt54b
#	( cd WebKitty/Resources && wget https://easylist.to/easylist/easylist.txt )
	cp easylist/easylist.dat WebKitty/Resources
	mkdir WebKitty/MiniResources
	cp Tools/morphos/MiniResources/*.png WebKitty/MiniResources
	cp Tools/morphos/MiniResources/MiniBrowser.info WebKitty/
	cp MUSTREAD.txt WebKitty/
	lha ao5 webkitty.lha WebKitty
	rm -rf WebKitty

putrelease: minirelease
	scp webkitty.lha jaca@tunkki.dk:/home/jaca/public_html

LINKFILES := \
	cross-build-mini/lib/libWebKit.a \
	cross-build-mini/lib/libWebCore.a \
	cross-build-mini/lib/libPAL.a \
	cross-build-mini/lib/libJavaScriptCore.a \
	cross-build-mini/lib/libWTF.a \
	$(ROOTPATH)/lib/libxml2/instdir/lib/libxml2.a \
	$(ROOTPATH)/lib/libxslt/instdir/lib/libxslt.a \
	$(ROOTPATH)/lib/sqlite/instdir/lib/libsqlite3.a \
	$(ROOTPATH)/gen/host/libnix/lib/libz.a \
	$(ROOTPATH)/morphoswb/libs/cairo/MorphOS/lib/libnix/libcairo.a \
	$(ROOTPATH)/gen/host/libnix/lib/libcurl.a \
	$(ROOTPATH)/gen/host/libnix/lib/libssl.a \
	$(ROOTPATH)/morphoswb/libs/freetype/library/lib/libfreetype.a \
	$(ROOTPATH)/gen/host/libnix/lib/libnghttp2.a \
	$(ROOTPATH)/lib/libjpeg/libjpeg.a \
	$(ROOTPATH)/gen/host/libnix/lib/libpsl.a \
	$(ROOTPATH)/gen/host/libnix/libpng16/lib/libpng16.a  \
	$(ROOTPATH)/gen/host/libnix/lib/libhyphen.a \
	$(ROOTPATH)/gen/host/libnix/lib/libcrypto.a \
	$(ROOTPATH)/lib/libicu67/instdir/lib/libicui18n.a \
	$(ROOTPATH)/lib/libicu67/instdir/lib/libicuuc.a \
	$(ROOTPATH)/lib/libicu67/instdir/lib/libicudata.a \
	$(ROOTPATH)/lib/libwebp/objects/host-libnix/tmpinstalldir/lib/libwebp.a \
	$(ROOTPATH)/lib/libwebp/objects/host-libnix/tmpinstalldir/lib/libwebpdemux.a \
	$(ROOTPATH)/gen/host/libnix/lib/libopenjp2.a

.PHONY: linkpackage
linkpackage:
	@rm -rf linkpackage
	@mkdir linkpackage
	@for i in $(LINKFILES); \
	do echo -n "$(STRIP) --strip-debug $$i -o linkpackage/">.run.sh; \
	echo $$i | rev | cut -d'/' -f-1 | rev >>.run.sh ; \
	echo "Copying and stripping $$i"; \
	bash ./.run.sh; \
	done
	@rm ./.run.sh
