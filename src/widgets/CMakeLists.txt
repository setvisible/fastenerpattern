set(MY_SOURCES ${MY_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/appliedloadwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/designobjectivewidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/designoptionwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/designspacewidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/designvariablewidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/fastenerwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/splicetoolbar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tablewidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/abstractspliceview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/mainwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/solverwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/resultwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/optimisationwidget.cpp
    )

# Rem: set here the headers related to the Qt MOC (i.e., with associated *.ui)
set(MY_HEADERS ${MY_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/appliedloadwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/designobjectivewidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/designoptionwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/designspacewidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/designvariablewidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/fastenerwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/splicetoolbar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tablewidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/abstractspliceview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/mainwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/solverwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/resultwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/optimisationwidget.h
    )

set(MY_FORMS ${MY_FORMS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/appliedloadwidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/designobjectivewidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/designoptionwidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/designspacewidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/designvariablewidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/fastenerwidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tablewidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/mainwidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/solverwidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/resultwidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/optimisationwidget.ui
    )
