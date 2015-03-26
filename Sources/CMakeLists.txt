add_definitions(-O2 -fPIC)

file(GLOB_RECURSE PROJECT_SRCS 
${CMAKE_CURRENT_SOURCE_DIR}/*.c
${CMAKE_CURRENT_SOURCE_DIR}/ebox/*.c
${CMAKE_CURRENT_SOURCE_DIR}/eclass/*.c
${CMAKE_CURRENT_SOURCE_DIR}/ecommons/*.c
${CMAKE_CURRENT_SOURCE_DIR}/egraphics/*.c
${CMAKE_CURRENT_SOURCE_DIR}/eobj/*.c
${CMAKE_CURRENT_SOURCE_DIR}/epopup/*.c
)

add_library(${PROJECT_NAME} ${PROJECT_SRCS}) 

#[[
add_subdirectory(ebox)
add_subdirectory(eclass)
add_subdirectory(ecommon)
add_subdirectory(egraphics)
add_subdirectory(eobj)
add_subdirectory(epopup)

add_target_libraries(${PROJECT_NAME} ebox eclass ecommon egraphics eobj epopup)
]]

