cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(TMD)
include(GNUInstallDirs)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGES_RELEASE} -DRELEASE")

find_package(wxWidgets COMPONENTS core base richtext REQUIRED)
include("${wxWidgets_USE_FILE}")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.49 COMPONENTS system REQUIRED)

find_package(OpenSSL REQUIRED)

include_directories(
	"${Boost_INCLUDE_DIR}"
	"${OPENSSL_INCLUDE_DIR}"
	"${PROJECT_SOURCE_DIR}/headers"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE bin/Release)

set(EXEC_NAME TMD-${CMAKE_SYSTEM}-${CMAKE_CXX_COMPILER_ID})

add_executable(${EXEC_NAME}
	headers/debug_gui.hpp
	headers/gui_client.hpp
	headers/main_frame.hpp
	headers/tamandua_textctrl.hpp
	headers/tamandua_box.hpp
	src/debug_gui.cpp
	src/gui_client.cpp
	src/main_frame.cpp
	src/tamandua_textctrl.cpp
	main.cpp
)
target_link_libraries("${EXEC_NAME}" "${wxWidgets_LIBRARIES}" "${OPENSSL_LIBRARIES}" "${Boost_LIBRARIES}" tamandua)

if (UNIX)
	install(TARGETS ${EXEC_NAME}
		RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
	)
endif(UNIX)
