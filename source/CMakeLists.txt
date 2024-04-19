cmake_minimum_required(
	VERSION 
	3.11
)
project(
	project_single_linked_list
	CXX
)
set( 
	CMAKE_CXX_STANDARD 
	17
)
set(
	SOURCE_DIR 
	src
)
set(
	FILES_MAIN 
	"${SOURCE_DIR}/main.cpp"
)
set(
	FILES_SINGLE_LINKED_LIST 
	"${SOURCE_DIR}/single_linked_list.h" 
)
add_executable(
	single_linked_list
	${FILES_MAIN}
	${FILES_SINGLE_LINKED_LIST}
)

source_group(
	"Main"
	FILES ${FILES_MAIN}
)
source_group(
	"Single linked list"
	FILES ${FILES_ARRAY_PTR}
)

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
	set(
		SYSTEM_LIBS 
		-lstdc++
	)
else()
	set(
		SYSTEM_LIBS
	)
endif()

target_link_libraries(
	single_linked_list
	${SYSTEM_LIBS}
)