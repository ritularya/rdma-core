add_library(dummy SHARED example3.c)
#target_link_libraries(dummy PRIVATE ibverbs)
target_include_directories(dummy PRIVATE ${CMAKE_SOURCE_DIR}/libibverbs)
target_link_libraries(dummy PRIVATE ibverbs)
set_target_properties(dummy PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	PREFIX "lib"
	OUTPUT_NAME "dummy")
