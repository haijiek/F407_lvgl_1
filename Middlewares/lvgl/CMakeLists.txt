
add_library(lvgl STATIC)

file(GLOB_RECURSE LVGL_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c" #LVGL下面的所有源文件
)

target_sources(lvgl PUBLIC
    ${LVGL_SOURCES}

)

target_include_directories(lvgl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    src
    src/core
    src/draw
    src/widgets
    src/draw/sw
    src/extra
    src/font
    src/hal
    src/misc
    src/widgets
    src/extra/widgets/calendar
    examples/porting
)







