###################################################################
# CMAKE config
###################################################################

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_BUILD_TYPE Debug)

project(Slender2D LANGUAGES CXX)

###################################################################
# Complier config
###################################################################

# set linker flags
if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:library")


	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

endif()

###################################################################
# Folder Definitons
###################################################################

#set basic folder macros
set(ROOT_FOLDER ${CMAKE_SOURCE_DIR})
set(CMAKE_BINARY_DIR ${ROOT_FOLDER}/vs_project)
set(EXECUTABLE_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PROJECT_SOURCE_DIR ${ROOT_FOLDER}/src)
set(PROJECT_TESTS_DIR ${ROOT_FOLDER}/tests)
set(PROJECT_HEADERS_DIR ${ROOT_FOLDER}/headers)
set(PROJECT_INCLUDE_DIR ${ROOT_FOLDER}/include)
set(PROJECT_LIB_DIR ${ROOT_FOLDER}/lib)
set(PROJECT_EXTERN_DIR ${ROOT_FOLDER}/extern)

###################################################################
# Extern Libraries
###################################################################

# BOOST
set ( Boost_USE_STATIC_LIBS ON )
set ( Boost_USE_MULTITHREADED ON )
set (BOOST_ROOT ${PROJECT_EXTERN_DIR})
find_package( Boost 1.60 COMPONENTS unit_test_framework REQUIRED )

link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

# SDL2
set (SDL2_DIR ${PROJECT_EXTERN_DIR}/SDL2-2.0.7)
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

set (SDL2_IMAGE_DIR ${PROJECT_EXTERN_DIR}/SDL2_image-2.0.3)
find_package(SDL2_IMAGE REQUIRED)

include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

###################################################################
# Setup internal Files
###################################################################

# if(MSVC) sets up filters according to folder structure
function(setup_filters root_folder files)
	source_group(TREE ${root_folder} FILES ${files})
endfunction()

function(prefix_list outputList prefix list)
	set(tempList "")
	foreach(file ${list})
		list(APPEND tempList "${prefix}/${file}")
	endforeach()
	set(${outputList} "${tempList}" PARENT_SCOPE)
endfunction()

# The following macro contains all Slender2D .hpp files
set(HEADER_FILES 
	Utility/Fileutility.h
	Utility/IdDispatcher.h
	Core/Framework.h
	Core/Event/PostSubscribeEvent.h
)

# The following macro contains all Slender2D .cpp files rel. to source directory
set(SOURCE_FILES 
	Utility/FileUtility.cpp
	Core/Framework.cpp

)

# the following macro contains all Slender2D test files
set(TEST_FILES
	FileUtilityTest.cpp
	PublisherSubscriberTest.cpp
)

# Produce list of absolute path 
prefix_list(ABS_SOURCE_FILES ${PROJECT_SOURCE_DIR} "${SOURCE_FILES}")
prefix_list(ABS_TEST_FILES ${PROJECT_TESTS_DIR} "${TEST_FILES}")
prefix_list(ABS_HEADER_FILES ${PROJECT_HEADERS_DIR} "${HEADER_FILES}")

# set filters
setup_filters(${ROOT_FOLDER} "${ABS_SOURCE_FILES}")
setup_filters(${ROOT_FOLDER} "${ABS_TEST_FILES}")
setup_filters(${ROOT_FOLDER} "${ABS_HEADER_FILES}")

# let the IDE find the header files
include_directories(${PROJECT_HEADERS_DIR})

###################################################################
# Create a Slender2D Package
###################################################################
add_library(Slender2D SHARED ${ABS_SOURCE_FILES} ${ABS_HEADER_FILES})


# link libraries together
target_link_libraries(Slender2D 
	${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES}
)

###################################################################
# Create Unit Test Project for Slender2D
###################################################################
add_executable(Slender2DTest ${ABS_TEST_FILES})

set_target_properties(Slender2DTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)
set_target_properties(Slender2DTest PROPERTIES LIBRARY_OUTPUT_DIRECTORY lib)
target_link_libraries(Slender2DTest 
	Slender2D ${Boost_LIBRARIES}
)

###################################################################
# Create Game Project using Slender2D
###################################################################
