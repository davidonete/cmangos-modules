set(LIBRARY_NAME modules)

function(capitalize_first_letter INPUT_STRING OUTPUT_VARIABLE)
  string(SUBSTRING ${INPUT_STRING} 0 1 FIRST_LETTER)
  string(SUBSTRING ${INPUT_STRING} 1 -1 REMAINING_STRING)
  string(TOUPPER ${FIRST_LETTER} FIRST_LETTER_UPPER)
  set(${OUTPUT_VARIABLE} "${FIRST_LETTER_UPPER}${REMAINING_STRING}" PARENT_SCOPE)
endfunction()

# Generate global include for all active modules
set(SKIP_FOLDERS "PlayerBots" "modules")
set(ALL_MODULE_HEADERS "")

file(GLOB SUBDIRECTORIES LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/src/modules/*)
foreach(SUBDIR ${SUBDIRECTORIES})
  get_filename_component(FOLDER_NAME ${SUBDIR} NAME)
  # Check if the folder should be skipped
  list(FIND SKIP_FOLDERS ${FOLDER_NAME} SKIP_INDEX)
  if (SKIP_INDEX EQUAL -1)
    string(TOUPPER ${FOLDER_NAME} UPPER_FOLDER_NAME)
	capitalize_first_letter(${FOLDER_NAME} CAPITALIZED_FOLDER_NAME)
    if(BUILD_MODULE_${UPPER_FOLDER_NAME})
      #list(APPEND ALL_MODULE_HEADERS "${CMAKE_SOURCE_DIR}/src/modules/${FOLDER_NAME}/src/${CAPITALIZED_FOLDER_NAME}Module.h")
	  list(APPEND ALL_MODULE_HEADERS "${CAPITALIZED_FOLDER_NAME}Module.h")
	  include_directories(${CMAKE_SOURCE_DIR}/src/modules/${FOLDER_NAME}/src/)
	endif()
  endif()
endforeach()

set(OUTPUT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Modules.h)
file(WRITE ${OUTPUT_HEADER} "// Auto-generated file\n")
foreach(HEADER ${ALL_MODULE_HEADERS})
  #file(RELATIVE_PATH RELATIVE_PATH_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src ${HEADER})
  #file(APPEND ${OUTPUT_HEADER} "#include \"${RELATIVE_PATH_HEADER}\"\n")
  file(APPEND ${OUTPUT_HEADER} "#include \"${HEADER}\"\n")
endforeach()

# Base files
file(GLOB modules_source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
    source_group("modules" FILES ${modules_source})
	
# Define base source library
set(LIBRARY_SRCS ${modules_source})

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SRCS})

target_link_libraries(${LIBRARY_NAME}
	shared
	RecastNavigation::Detour
)

include_directories(${CMAKE_SOURCE_DIR}/dep/g3dlite)
add_definitions(-DENABLE_MODULES)

set_target_properties(${LIBRARY_NAME} PROPERTIES PROJECT_LABEL "Modules")

if(UNIX)
	# Both systems don't have libdl and don't need them
	if (NOT (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "NetBSD"))
		target_link_libraries(${LIBRARY_NAME} dl)
	endif()
endif()