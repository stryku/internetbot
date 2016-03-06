set( SRC_FILES 
	 main.cpp
	 Trie.cpp )

include_directories(${COMMON_HEADERS})
	
add_executable(bot ${SRC_FILES})

target_link_libraries(bot curlpp curl pthread)

set( CMAKE_INSTALL_PREFIX ${INSTALL_DIR} )

install(TARGETS bot DESTINATION ${INSTALL_DIR})
