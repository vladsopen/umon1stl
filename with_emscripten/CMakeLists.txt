cmake_minimum_required (VERSION 2.8)

project(Umon)

set(CMAKE_CXX_STANDARD 14)
set(EMCC_LINKER_FLAGS "--bind")

include_directories(
    ${PROJECT_SOURCE_DIR}/../portable/app/main
    ${PROJECT_SOURCE_DIR}/../portable/app/storage
    ${PROJECT_SOURCE_DIR}/../portable/app/streaming
    ${PROJECT_SOURCE_DIR}/../portable/app/ui/controls
    ${PROJECT_SOURCE_DIR}/../portable/app/ui/drawing
    ${PROJECT_SOURCE_DIR}/../portable/app/ui/view
    ${PROJECT_SOURCE_DIR}/../portable/tests
    ${PROJECT_SOURCE_DIR}/../portable/uiframework
    ${PROJECT_SOURCE_DIR}/../portable/useful
    ${PROJECT_SOURCE_DIR}/../with_emscripten/emsource
)

file(GLOB_RECURSE v_allsources
    ${PROJECT_SOURCE_DIR}/../portable/app/main/*.cpp
    ${PROJECT_SOURCE_DIR}/../portable/app/storage/*.cpp
    ${PROJECT_SOURCE_DIR}/../portable/app/streaming/*.cpp
    ${PROJECT_SOURCE_DIR}/../portable/app/ui/controls/*.cpp
    ${PROJECT_SOURCE_DIR}/../portable/app/ui/drawing/*.cpp
    ${PROJECT_SOURCE_DIR}/../portable/app/ui/view/*.cpp
    ${PROJECT_SOURCE_DIR}/../portable/tests/*.cpp
    ${PROJECT_SOURCE_DIR}/../portable/uiframework/*.cpp
    ${PROJECT_SOURCE_DIR}/../portable/useful/*.cpp
    ${PROJECT_SOURCE_DIR}/../with_emscripten/emsource/*.cpp
)

message(STATUS "::: Source top: " ${PROJECT_SOURCE_DIR})
message(STATUS "::: Found: " ${v_allsources})

add_executable(Umon ${v_allsources})

#add_definitions(" -Os -s ASSERTIONS=1 ")
# see .bat

set_target_properties(
    Umon PROPERTIES LINK_FLAGS "--bind -s DISABLE_EXCEPTION_CATCHING=2")


#$(allsources)) ../App/Main/MainApp.cpp
