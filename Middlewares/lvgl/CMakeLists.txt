
#------------SRC_INC-------
set(lvgl_SRC
)

set(lvgl_INC
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
        ${CMAKE_SOURCE_DIR}/BSP/LCD
)

set(UI_SRC
        demos/screens/ui_Screen1.c
        demos/screens/ui_Screen2.c
        #    screens/ui_Screen3.c
        demos/ui.c
        demos/components/ui_comp_hook.c
        demos/ui_helpers.c
)

set(UI_INC
)


add_library(lvgl STATIC)
add_library(ui STATIC)


file(GLOB_RECURSE LVGL_SRC CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c" #LVGL??????????????
)


#--------------target_source---------

target_sources(lvgl PUBLIC
        ${LVGL_SRC}
)
target_sources(ui PUBLIC
        ${UI_SRC}
)
#----------target_include---------

target_include_directories(lvgl PUBLIC
    ${lvgl_INC}
)


#----------link-------

target_link_libraries(lvgl PUBLIC stm32cubemx)







