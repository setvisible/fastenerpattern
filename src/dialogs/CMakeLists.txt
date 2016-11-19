set(MY_SOURCES ${MY_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dialogs/propertiesdialog.cpp
    )

# Rem: set here the headers related to the Qt MOC (i.e., with associated *.ui)
set(MY_HEADERS ${MY_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dialogs/propertiesdialog.h
    )

set(MY_FORMS ${MY_FORMS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dialogs/propertiesdialog.ui
    )
