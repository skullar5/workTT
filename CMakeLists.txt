cmake_minimum_required(VERSION 3.16)
project(NoomyPlan
  VERSION 0.1
  DESCRIPTION "Budget Management Application"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "C:/Qt_msvc/6.8.2/msvc2022_64")

find_package(Qt6 COMPONENTS Core Gui Widgets Charts REQUIRED)

set(SOURCES
    main.cpp
    user.cpp
    authenticatesystem.cpp
    loginpage.cpp
    financialreport.cpp
    cashflowtracking.cpp
    dashboard.cpp
    budgetpage.cpp
    budgetpagebudget.cpp
    budgetpageexpenses.cpp
    inventory.cpp
    invoice.cpp
    invoicemanager.cpp
)

set(HEADERS
    user.h
    mainwindow.h
    authenticatesystem.h
    loginpage.h
    financialreport.h
    cashflowtracking.h
    dashboard.h
    budgetpage.h
    budgetpagebudget.h
    budgetpageexpenses.h
    inventory.h
    invoice.h
    invoicemanager.h
)

add_executable(NoomyPlan ${SOURCES} ${HEADERS})
target_link_libraries(NoomyPlan PRIVATE 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets
    Qt6::Charts
)

target_include_directories(NoomyPlan PRIVATE
    "${CMAKE_PREFIX_PATH}/include"
    "${CMAKE_PREFIX_PATH}/include/QtCore"
    "${CMAKE_PREFIX_PATH}/include/QtGui"
    "${CMAKE_PREFIX_PATH}/include/QtWidgets"
    "${CMAKE_PREFIX_PATH}/include/QtCharts"
)

# Add MSVC-specific flags to enable exception handling
if(MSVC)
    target_compile_options(NoomyPlan PRIVATE /EHsc)
endif()

set_target_properties(NoomyPlan PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)