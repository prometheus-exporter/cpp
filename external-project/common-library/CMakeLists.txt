set(ALIAS_NAME "lib::common")
set(BUILD_TARGET "common")
set(BUILD_DIR ${CMAKE_BINARY_DIR}/${BUILD_TARGET})

configure_file("CMakeLists.txt.in" "${BUILD_DIR}/download/CMakeLists.txt")
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
				RESULT_VARIABLE result
				WORKING_DIRECTORY ${BUILD_DIR}/download)
if(result)
	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
				RESULT_VARIABLE result
				WORKING_DIRECTORY ${BUILD_DIR}/download)
if(result)
	message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

add_library(${BUILD_TARGET} IMPORTED STATIC GLOBAL)
add_library(${ALIAS_NAME} ALIAS ${BUILD_TARGET})

file(MAKE_DIRECTORY ${BUILD_DIR}/integrate/include)

find_package(PkgConfig REQUIRED)
set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${BUILD_DIR}/integrate/lib/pkgconfig")
pkg_check_modules(COMMON REQUIRED common)

set(DEPENDENCIES ${COMMON_LIBRARIES})
LIST(REMOVE_AT DEPENDENCIES 0)

set_target_properties(
	${BUILD_TARGET} PROPERTIES
	IMPORTED_LOCATION ${COMMON_LIBRARY_DIRS}/libcommon.a
	INTERFACE_INCLUDE_DIRECTORIES ${COMMON_INCLUDE_DIRS}
	INTERFACE_LINK_LIBRARIES "${DEPENDENCIES}"
)
