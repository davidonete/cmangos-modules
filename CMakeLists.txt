set(LIBRARY_NAME modules)

function(capitalize_first_letter INPUT_STRING OUTPUT_VARIABLE)
  string(SUBSTRING ${INPUT_STRING} 0 1 FIRST_LETTER)
  string(SUBSTRING ${INPUT_STRING} 1 -1 REMAINING_STRING)
  string(TOUPPER ${FIRST_LETTER} FIRST_LETTER_UPPER)
  set(${OUTPUT_VARIABLE} "${FIRST_LETTER_UPPER}${REMAINING_STRING}" PARENT_SCOPE)
endfunction()

# Get all active modules
set(SKIP_FOLDERS "PlayerBots" "modules")
set(MODULE_NAMES "")
file(GLOB SUBDIRECTORIES LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/src/modules/*)
foreach(SUBDIR ${SUBDIRECTORIES})
  get_filename_component(FOLDER_NAME ${SUBDIR} NAME)
  # Check if the folder should be skipped
  list(FIND SKIP_FOLDERS ${FOLDER_NAME} SKIP_INDEX)
  if (SKIP_INDEX EQUAL -1)
    string(TOUPPER ${FOLDER_NAME} UPPER_FOLDER_NAME)
    if(BUILD_MODULE_${UPPER_FOLDER_NAME})
	  list(APPEND MODULE_NAMES ${FOLDER_NAME})
	endif()
  endif()
endforeach()

# Generate files that initialize all active modules
set(OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/Modules.cpp)
file(WRITE ${OUTPUT_FILE} "// Auto-generated file\n\n")
foreach(MODULE_NAME ${MODULE_NAMES})
  capitalize_first_letter(${MODULE_NAME} CAPITALIZED_MODULE_NAME)
  file(APPEND ${OUTPUT_FILE} "#include \"${CAPITALIZED_MODULE_NAME}Module.h\"\n")
endforeach()
file(WRITE ${OUTPUT_FILE} "{\n")
file(WRITE ${OUTPUT_FILE} "void AddModules()\n")
file(WRITE ${OUTPUT_FILE} "{\n")
foreach(MODULE_NAME ${MODULE_NAMES})
  capitalize_first_letter(${MODULE_NAME} CAPITALIZED_MODULE_NAME)
  file(APPEND ${OUTPUT_FILE} "    new ${CAPITALIZED_MODULE_NAME}Module();\n")
endforeach()
file(WRITE ${OUTPUT_FILE} "}\n")

# Base files
include_directories(${CMAKE_SOURCE_DIR}/src/modules/modules/src/)
file(GLOB modules_source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
  source_group("modules" FILES ${modules_source})
	
# Define base source library
set(LIBRARY_SRCS ${modules_source})

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SRCS})

target_link_libraries(${LIBRARY_NAME}
  shared
  RecastNavigation::Detour
)

# Include and link all modules
foreach(MODULE_NAME ${MODULE_NAMES})
  include_directories(${CMAKE_SOURCE_DIR}/src/modules/${MODULE_NAME}/src/)
  target_link_libraries(${LIBRARY_NAME} ${MODULE_NAME})
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/dep/g3dlite)
add_definitions(-DENABLE_MODULES)

set_target_properties(${LIBRARY_NAME} PROPERTIES PROJECT_LABEL "Modules")

if(UNIX)
  # Both systems don't have libdl and don't need them
  if (NOT (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "NetBSD"))
    target_link_libraries(${LIBRARY_NAME} dl)
  endif()
endif()