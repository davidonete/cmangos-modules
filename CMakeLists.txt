set(LIBRARY_NAME Modules)



# Base files
file(GLOB modules_source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
    source_group("Modules" FILES ${modules_source})
	
# Define base source library
set(LIBRARY_SRCS ${modules_source})

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SRCS})

target_link_libraries(${LIBRARY_NAME}
	shared
	RecastNavigation::Detour
)

if(UNIX)
	# Both systems don't have libdl and don't need them
	if (NOT (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "NetBSD"))
		target_link_libraries(${LIBRARY_NAME} dl)
	endif()
endif()