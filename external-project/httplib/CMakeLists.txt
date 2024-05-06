include(FetchContent)

set(TARGET_NAME "httplib")
set(ALIAS_NAME "lib::${TARGET_NAME}")

FetchContent_Declare(
	${TARGET_NAME}
	GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
	GIT_TAG v0.15.3
)

FetchContent_MakeAvailable(${TARGET_NAME})

add_library(${ALIAS_NAME} ALIAS httplib)

