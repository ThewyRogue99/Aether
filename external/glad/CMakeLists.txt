add_library(glad STATIC)

if(APPLE)
    target_sources(glad PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/4.1/src/gl.c)

    target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/4.1/include)
else()
    target_sources(glad PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/4.6/src/gl.c)

    target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/4.6/include)
endif()

target_compile_definitions(glad PRIVATE $<$<CONFIG:Debug>:GLAD_DEBUG>)
