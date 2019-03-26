cmake_minimum_required(VERSION 2.8)

project(tbeditor)

set(TBEDITOR_TBENGINE_INC "" CACHE PATH "Include")
set(TBEDITOR_BOOST_INC "" CACHE PATH "Include")
set(TBEDITOR_ZLIB_INC "" CACHE PATH "Include")
set(TBEDITOR_QUAZIP_INC "" CACHE PATH "Include")

set(TBEDITOR_BOOST_LIB_DIR "" CACHE PATH "Lib Dir")

set(TBEDITOR_TBENGINE_LIB "" CACHE FILEPATH "Lib")
set(TBEDITOR_BOOST_LIB "" CACHE FILEPATH "Lib")
set(TBEDITOR_QUAZIP_LIB "" CACHE FILEPATH "Lib")
set(TBEDITOR_ZLIB_LIB "" CACHE FILEPATH "Lib")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Qt bin must be on PATH var
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5OpenGL)

# qt5_use_modules(tbeditor Core OpenGL Gui Widgets)

string(TOLOWER ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE} OUTPUT_PATH)
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH})

include_directories(${TBEDITOR_TBENGINE_INC})
include_directories(${TBEDITOR_BOOST_INC})
include_directories(${TBEDITOR_DEVIL_INC})
include_directories(${TBEDITOR_ZLIB_INC})
include_directories(${TBEDITOR_QUAZIP_INC})
include_directories(sources)

link_directories(${TBEDITOR_BOOST_LIB_DIR})

file(GLOB_RECURSE SOURCES sources/*.cpp sources/*.rc)
file(GLOB_RECURSE HEADERS sources/*.h)
file(GLOB_RECURSE UI sources/*.ui)
file(GLOB_RECURSE QRC sources/*.qrc)

qt5_wrap_ui(UI_SOURCE ${UI})
qt5_add_resources(QRC_SOURCE ${QRC})

add_executable(tbeditor ${SOURCES} ${UI_SOURCE} ${QRC_SOURCE})

target_link_libraries(tbeditor
  Qt5::Widgets
  Qt5::OpenGL
  ${TBEDITOR_TBENGINE_LIB}
  ${TBEDITOR_BOOST_LIB}
  ${TBEDITOR_DEVIL_LIB}
  ${TBEDITOR_QUAZIP_LIB}
  ${TBEDITOR_ZLIB_LIB}
  opengl32
  glu32
)
