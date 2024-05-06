include(FetchContent)

set(TARGET_NAME "prometheus")
set(ALIAS_NAME "lib::${TARGET_NAME}")

FetchContent_Declare(
	${TARGET_NAME}
	GIT_REPOSITORY https://github.com/jupp0r/prometheus-cpp.git
	GIT_TAG v1.2.4
)

set(ENABLE_COMPRESSION OFF)
set(ENABLE_TESTING OFF)

FetchContent_MakeAvailable(${TARGET_NAME})

add_library(${TARGET_NAME} INTERFACE)
add_library(${ALIAS_NAME} ALIAS ${TARGET_NAME})
set_target_properties(${TARGET_NAME} PROPERTIES
	INTERFACE_LINK_LIBRARIES "prometheus-cpp::core;prometheus-cpp::pull;prometheus-cpp::push"
)
