add_library(ir INTERFACE)

target_sources(ir PUBLIC type.cc builder.cc dump.cc)

target_include_directories(ir INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)
