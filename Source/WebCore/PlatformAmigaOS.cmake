include(platform/Cairo.cmake)
include(platform/Curl.cmake)
include(platform/FreeType.cmake)
include(platform/ImageDecoders.cmake)

if (NOT AMIGAOS_MINIMAL)
	include(platform/GCrypt.cmake)
endif()

list(APPEND WebCore_PRIVATE_INCLUDE_DIRECTORIES
    "${WEBKIT_LIBRARIES_DIR}/include"
)

list(APPEND WebCore_PRIVATE_INCLUDE_DIRECTORIES
    ${WEBCORE_DIR}/platform
    ${WEBCORE_DIR}/platform/adwaita
    ${WEBCORE_DIR}/platform/amigaos
    ${WEBCORE_DIR}/platform/generic
    ${WEBCORE_DIR}/platform/graphics/amigaos
    ${WEBCORE_DIR}/platform/mediacapabilities
)

list(APPEND WebCore_SYSTEM_INCLUDE_DIRECTORIES
    ${ICU_INCLUDE_DIRS}
    ${LIBXML2_INCLUDE_DIR}
    ${SQLITE_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS}
    ${WPE_INCLUDE_DIRS}
    ${HYPHEN_INCLUDE_DIRS}
    ${AVCODEC_INCLUDE_DIR}
    "${ROOTPATH}/morphoswb/development/tools/eventprofiler/"
)

list(APPEND WebCore_LIBRARIES
    ${ICU_LIBRARIES}
    ${LIBXML2_LIBRARIES}
    ${SQLITE_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${WPE_LIBRARIES}
    ${HYPHEN_LIBRARIES}
)

list(APPEND WebCore_SOURCES
    editing/amigaos/EditorAmigaOS.cpp
    editing/amigaos/AutofillElements.cpp
    platform/amigaos/Altivec.cpp
    platform/amigaos/PasteboardAmigaOS.cpp
    platform/amigaos/CursorAmigaOS.cpp
    platform/amigaos/PlatformKeyboardEvent.cpp
    platform/amigaos/PlatformScreenAmigaOS.cpp
    platform/amigaos/MIMETypeRegistryAmigaOS.cpp
    platform/amigaos/DragDataAmigaOS.cpp
    platform/amigaos/DragImageAmigaOS.cpp
    platform/amigaos/SelectionData.cpp
    platform/amigaos/UserAgentAmigaOS.cpp
    platform/generic/KeyedDecoderGeneric.cpp
    platform/generic/KeyedEncoderGeneric.cpp
    platform/graphics/amigaos/GraphicsLayerAmigaOS.cpp
    platform/graphics/amigaos/ImageAmigaOS.cpp
    platform/graphics/amigaos/DisplayRefreshMonitorAmigaOS.cpp
    platform/network/amigaos/CurlSSLHandleAmigaOS.cpp
    platform/network/amigaos/NetworkStateNotifierAmigaOS.cpp
    #platform/posix/SharedBufferPOSIX.cpp
    platform/text/LocaleICU.cpp
    platform/text/hyphen/HyphenationLibHyphen.cpp
    rendering/RenderThemeAmigaOS.cpp
    rendering/RenderThemeAdwaita.cpp
    page/amigaos/DragControllerAmigaOS.cpp
    platform/adwaita/ThemeAdwaita.cpp
    platform/adwaita/ScrollbarThemeAdwaita.cpp
)

list(APPEND WebCore_PRIVATE_FRAMEWORK_HEADERS
    platform/adwaita/ScrollbarThemeAdwaita.h
    platform/graphics/amigaos/MediaPlayerAmigaOS.h
    platform/amigaos/SelectionData.h
)

if (NOT AMIGAOS_MINIMAL)
	list(APPEND WebCore_SOURCES
		platform/audio/amigaos/AudioDestinationAmigaOS.cpp
		platform/audio/amigaos/AudioBusAmigaOS.cpp
		platform/audio/amigaos/AudioFileReaderAmigaOS.cpp
		platform/audio/amigaos/AudioDestinationOutputAmigaOS.cpp
		platform/audio/amigaos/FFTFrameAmigaOS.cpp
		platform/graphics/amigaos/acinerella.c
		platform/graphics/amigaos/AcinerellaPointer.cpp
		platform/graphics/amigaos/AcinerellaBuffer.cpp
		platform/graphics/amigaos/AcinerellaMuxer.cpp
		platform/graphics/amigaos/AcinerellaHLS.cpp
		platform/graphics/amigaos/AcinerellaDecoder.cpp
		platform/graphics/amigaos/AcinerellaAudioDecoder.cpp
		platform/graphics/amigaos/AcinerellaVideoDecoder.cpp
		platform/graphics/amigaos/AcinerellaContainer.cpp
		platform/graphics/amigaos/MediaPlayerPrivateAmigaOS.cpp
		platform/graphics/amigaos/MediaSourcePrivateAmigaOS.cpp
		platform/graphics/amigaos/MediaSourceBufferPrivateAmigaOS.cpp
		platform/graphics/amigaos/AudioTrackPrivateAmigaOS.cpp
		platform/graphics/amigaos/VideoTrackPrivateAmigaOS.cpp
		platform/graphics/amigaos/MediaDescriptionAmigaOS.cpp
		platform/graphics/amigaos/MediaSampleAmigaOS.cpp
		platform/mediastream/amigaos/RealtimeMediaSourceCenterAmigaOS.cpp
	)
endif()

list(APPEND WebCore_USER_AGENT_STYLE_SHEETS
	${WEBCORE_DIR}/Modules/mediacontrols/mediaControlsAdwaita.css
	${WEBCORE_DIR}/css/themeAdwaita.css
)

set(WebCore_USER_AGENT_SCRIPTS
	${WEBCORE_DIR}/Modules/mediacontrols/mediaControlsAdwaita.js
)
set(WebCore_USER_AGENT_SCRIPTS_DEPENDENCIES ${WEBCORE_DIR}/rendering/RenderThemeAdwaita.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -DAMIGAOS_MINIMAL=${AMIGAOS_MINIMAL}")

set_source_files_properties(platform/morphos/Altivec.cpp PROPERTIES COMPILE_FLAGS "-maltivec ${COMPILE_FLAGS}")

