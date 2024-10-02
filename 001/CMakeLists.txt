#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.27)
project(Tutorials2024)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)

if(WITH_GEANT4_UIVIS)
	find_package(Geant4 REQUIRED ui_all vis_all)
else()
	find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(
	${Geant4_INCLUDE_DIR}	
	#${CMAKE_CURRENT_SOURCE_DIR}/include
					)


#file(GLOB sources ${PROJECT_SOURCE_DIR}/source/*.cpp)
#file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hpp)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(tutorial 
	main.cpp 
#	${sources} 
#	${headers}
	)

target_link_libraries(tutorial ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(Tutorials2024 DEPENDS tutorial)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS tutorial DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

