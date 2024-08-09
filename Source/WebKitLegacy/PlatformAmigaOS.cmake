add_definitions(-DUSE_CAIRO=1 -DUSE_CURL=1 -DWEBKIT_EXPORTS=1 -DWEBCORE_EXPORT=WTF_EXPORT_DECLARATION -DPAL_EXPORT=WTF_EXPORT_DECLARATION -DWTF_EXPORT=WTF_EXPORT_DECLARATION
	-DJS_EXPORT_PRIVATE=WTF_EXPORT -DUSE_SYSTEM_MALLOC -DAMIGAOS_MINIMAL=${AMIGAOS_MINIMAL})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")

list(APPEND WebKitLegacy_PRIVATE_INCLUDE_DIRECTORIES
    ${CAIRO_INCLUDE_DIRS}
    "${WEBKIT_LIBRARIES_DIR}/include"
    "${ROOTPATH}/morphoswb/development/tools/eventprofiler/"
)

list(APPEND WebKitLegacy_LIBRARIES PRIVATE WTF${DEBUG_SUFFIX})

list(APPEND WebKitLegacy_PRIVATE_INCLUDE_DIRECTORIES
    "${CMAKE_BINARY_DIR}/../include/private"
    "${CMAKE_BINARY_DIR}/../include/private/JavaScriptCore"
    "${CMAKE_BINARY_DIR}/../include/private/WebCore"
    "${WEBKITLEGACY_DIR}/amigaos"
    "${WEBKITLEGACY_DIR}/amigaos/WebCoreSupport"
    "${WEBKITLEGACY_DIR}/amigaos/ServiceWorker"
    "${WebKitLegacy_DERIVED_SOURCES_DIR}/include"
    "${WebKitLegacy_DERIVED_SOURCES_DIR}/Interfaces"
    ${SQLITE_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS}
    ${WPE_INCLUDE_DIRS}
    ${OBJC_INCLUDE}
)

list(APPEND WebKitLegacy_SOURCES_Classes
    amigaos/WebFrame.cpp
    amigaos/WebPage.cpp
    amigaos/WebProcess.cpp
    amigaos/BackForwardClient.cpp
    amigaos/WebApplicationCache.cpp
    amigaos/storage/WebDatabaseProvider.cpp
    amigaos/storage/WebStorageTrackerClient.cpp
    amigaos/WebDocumentLoader.cpp
    amigaos/CacheModel.cpp
    amigaos/WebDragClient.cpp
    amigaos/PopupMenu.cpp
    amigaos/Gamepad.cpp
    amigaos/NetworkSession.cpp
)

# list(APPEND WebKitLegacy_SOURCES_Classes
#     morphos/WkWebView.mm
#     morphos/WkNetworkRequestMutable.mm
#     morphos/WkHistory.mm
#     morphos/WkSettings.mm
#     morphos/WkCertificate.mm
#     morphos/WkCertificateViewer.mm
#     morphos/WkError.mm
#     morphos/WkDownload.mm
#     morphos/WkFileDialog.mm
#     morphos/WkHitTest.mm
#     morphos/WkFavIcon.mm
#     morphos/WkPrinting.mm
#     morphos/WkUserScript.mm
#     morphos/WkMedia.mm
#     morphos/WkNotification.mm
#     morphos/WkResourceResponse.mm
#     morphos/WkCache.mm
# )

list(APPEND WebKitLegacy_SOURCES_WebCoreSupport
    amigaos/WebCoreSupport/WebVisitedLinkStore.cpp
    amigaos/WebCoreSupport/WebEditorClient.cpp
    amigaos/WebCoreSupport/WebChromeClient.cpp
    amigaos/WebCoreSupport/WebPluginInfoProvider.cpp
    amigaos/WebCoreSupport/WebPlatformStrategies.cpp
    amigaos/WebCoreSupport/WebInspectorClient.cpp
    amigaos/WebCoreSupport/WebFrameLoaderClient.cpp
    amigaos/WebCoreSupport/WebFrameNetworkingContext.cpp
    amigaos/WebCoreSupport/WebContextMenuClient.cpp
    amigaos/WebCoreSupport/WebPageGroup.cpp
    amigaos/WebCoreSupport/WebProgressTrackerClient.cpp
    amigaos/WebCoreSupport/WebNotificationClient.cpp
)

list(APPEND WebKitLegacy_SOURCES_WebCoreSupport
    amigaos/cache/CacheStorageEngine.cpp
    amigaos/cache/CacheStorageEngineCache.cpp
    amigaos/cache/CacheStorageEngineCaches.cpp
    amigaos/cache/NetworkCacheCoders.cpp
    amigaos/cache/NetworkCacheDataCurl.cpp
    amigaos/cache/NetworkCacheIOChannelCurl.cpp
    amigaos/cache/NetworkCacheSubresourcesEntry.cpp
    amigaos/cache/NetworkCacheBlobStorage.cpp
    amigaos/cache/NetworkCacheEntry.cpp
    amigaos/cache/NetworkCacheKey.cpp
    amigaos/cache/PrefetchCache.cpp
    amigaos/cache/NetworkCacheData.cpp
    amigaos/cache/NetworkCacheFileSystem.cpp
    amigaos/cache/NetworkCacheStorage.cpp
    amigaos/cache/NetworkCache.cpp
    amigaos/cache/CacheStorageEngineConnection.cpp
    amigaos/cache/WebCacheStorageProvider.cpp
    amigaos/cache/WebCacheStorageConnection.cpp
)

if (NOT AMIGAOS_MINIMAL)
    list(APPEND WebKitLegacy_ABP
        amigaos/ABPFilterParser/ABPFilterParser.cpp
        amigaos/ABPFilterParser/BloomFilter.cpp
        amigaos/ABPFilterParser/cosmeticFilter.cpp
        amigaos/ABPFilterParser/filter.cpp
        amigaos/ABPFilterParser/hashFn.cpp
    )
    add_definitions(-DENABLE_WEB_AUDIO=1)
    list(APPEND WebKitLegacy_SOURCES_Classes
        amigaos/WebDatabaseManager.cpp
        amigaos/WebCoreSupport/WebUserMediaClient.cpp

        amigaos/ServiceWorker/WebServiceWorkerProvider.cpp
        amigaos/ServiceWorker/SharedMemory.cpp
        amigaos/ServiceWorker/SharedStringHashTable.cpp
        amigaos/ServiceWorker/SharedStringHashTableReadOnly.cpp
        amigaos/ServiceWorker/SharedStringHashStore.cpp
        amigaos/ServiceWorker/WebSWOriginTable.cpp
        amigaos/ServiceWorker/WebSWOriginStore.cpp
        amigaos/ServiceWorker/WebSWServerConnection.cpp
        amigaos/ServiceWorker/WebSWServerToContextConnection.cpp
        amigaos/ServiceWorker/WebSWContextManagerConnection.cpp
        amigaos/ServiceWorker/ServiceWorkerSoftUpdateLoader.cpp
    )
endif()

list(APPEND WebKitLegacy_SOURCES ${WebKitLegacy_INCLUDES} ${WebKitLegacy_SOURCES_Classes} ${WebKitLegacy_SOURCES_WebCoreSupport} ${WebKitLegacy_ABP})

# set(MM_FLAGS "-Wno-ignored-attributes -Wno-protocol -Wundeclared-selector -fobjc-call-cxx-cdtors -fobjc-exceptions -fconstant-string-class=OBConstantString -DDEBUG=0")

# set_source_files_properties(morphos/WkWebView.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkNetworkRequestMutable.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkHistory.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkSettings.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkCertificate.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkCertificateViewer.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkError.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkDownload.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkFileDialog.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkHitTest.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkFavIcon.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkPrinting.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkUserScript.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkMedia.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkNotification.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkResourceResponse.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})
# set_source_files_properties(morphos/WkCache.mm PROPERTIES COMPILE_FLAGS ${MM_FLAGS})

set(WebKitLegacy_OUTPUT_NAME
    WebKit${DEBUG_SUFFIX}
)
