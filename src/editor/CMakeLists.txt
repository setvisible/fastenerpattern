set(MY_SOURCES ${MY_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/items/appliedloaditem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/items/arcarrowitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/items/arrowitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/items/fasteneritem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/items/scalableimageitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/items/scalablepointitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/items/symbolitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/items/tensoritem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/backgroundwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/backgroundview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/flexiblescrollbar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/splicegraphicswidget.cpp
    )

# Rem: set here the headers related to the Qt MOC (i.e., with associated *.ui)
set(MY_HEADERS ${MY_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/backgroundwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/flexiblescrollbar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/splicegraphicswidget.h
    )
