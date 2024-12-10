add_executable(ir_fact fact.cc)

target_link_libraries(ir_fact PRIVATE ir)
target_link_libraries(ir_fact PRIVATE util)

add_executable(dom1 domtree1.cc)

target_link_libraries(dom1 PRIVATE ir)
target_link_libraries(dom1 PRIVATE util)

add_executable(loop1 loop1.cc)

target_link_libraries(loop1 PRIVATE ir)
target_link_libraries(loop1 PRIVATE util)
