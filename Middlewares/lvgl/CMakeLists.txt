
#------------SRC_INC-------
set(lvgl_SRC
)

set(lvgl_INC
        ${CMAKE_SOURCE_DIR}/Middlewares
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

file(GLOB_RECURSE UI_SRC CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/demos/*.c" #LVGL??????????????
)

set(UI_INC
    demos
    demos/components
    demos/fonts
    demos/screens
)


add_library(lvgl STATIC)
add_library(ui STATIC)


file(GLOB_RECURSE LVGL_SRC CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" #LVGL??????????????
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.c" #LVGL??????????????

)


#--------------target_source---------

target_sources(lvgl PRIVATE
        ${LVGL_SRC}
)
target_sources(ui PRIVATE
        ${UI_SRC}
)
#----------target_include---------

target_include_directories(lvgl PUBLIC
    ${lvgl_INC}
)
target_include_directories(ui PUBLIC
    ${UI_INC}
)


#----------link-------

target_link_libraries(lvgl PUBLIC
    stm32cubemx
    BSP
)
target_link_libraries(ui PUBLIC lvgl)







