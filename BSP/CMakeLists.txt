# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.

file(GLOB_RECURSE BSP_SRC CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c" #BSP下面的所有源文件
)

set(BSP_INC
    24CXX
    delay
    LCD
    LED
    TOUCH
    SRAM
)
add_library(BSP OBJECT)

#SRC
target_sources(BSP PUBLIC
    ${BSP_SRC}
)

#INC
target_include_directories(BSP PUBLIC
    ${BSP_INC}
)
#Link_Lib
target_link_libraries(BSP PUBLIC
    stm32cubemx
)



