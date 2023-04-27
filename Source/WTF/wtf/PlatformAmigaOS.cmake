list(APPEND WTF_SOURCES
    generic/WorkQueueGeneric.cpp
)

    list(APPEND WTF_SOURCES
        generic/MainThreadGeneric.cpp

        posix/FileSystemPOSIX.cpp
        posix/OSAllocatorPOSIX.cpp
        posix/ThreadingPOSIX.cpp
        posix/CPUTimePOSIX.cpp

        text/unix/TextBreakIteratorInternalICUUnix.cpp

        unix/UniStdExtrasUnix.cpp

        amigaos/LanguageAmigaOS.cpp
        amigaos/Misc.cpp
        amigaos/MD5.cpp

        amigaos/MemoryPressureHandlerAmigaOS.cpp
    )

    list(APPEND WTF_SOURCES
        generic/MemoryFootprintGeneric.cpp
    )

    list(APPEND WTF_SOURCES
        generic/RunLoopGeneric.cpp
    )

list(APPEND WTF_LIBRARIES
    ${CMAKE_THREAD_LIBS_INIT}
)

if (AMIGAOS_MINIMAL)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
endif()
