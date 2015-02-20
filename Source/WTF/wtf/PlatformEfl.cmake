list(APPEND WTF_SOURCES
    efl/MainThreadEfl.cpp
    efl/RunLoopEfl.cpp
    efl/WorkQueueEfl.cpp

    gobject/GMainLoopSource.cpp
    gobject/GRefPtr.cpp
    gobject/GThreadSafeMainLoopSource.cpp
)

list(APPEND WTF_LIBRARIES
    ${ECORE_EVAS_LIBRARIES}
    ${ECORE_IMF_LIBRARIES}
    ${ECORE_LIBRARIES}
    ${EINA_LIBRARIES}
    ${EO_LIBRARIES}
    ${EVAS_LIBRARIES}
    ${GLIB_GIO_LIBRARIES}
    ${GLIB_GOBJECT_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${ZLIB_LIBRARIES}
    pthread
)

list(APPEND WTF_INCLUDE_DIRECTORIES
    "${WEBKIT2_DIR}/Platform/efl"
    ${ECORE_EVAS_INCLUDE_DIRS}
    ${ECORE_INCLUDE_DIRS}
    ${ECORE_IMF_INCLUDE_DIRS}
    ${EINA_INCLUDE_DIRS}
    ${EO_INCLUDE_DIRS}
    ${EVAS_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
)
