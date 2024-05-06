include(FetchContent)

set(TARGET_NAME "lcov")

FetchContent_Declare(
	${TARGET_NAME}
	GIT_REPOSITORY https://github.com/linux-test-project/lcov.git
	GIT_TAG v2.0
)

FetchContent_MakeAvailable(${TARGET_NAME})

set_property(GLOBAL PROPERTY LCOV_SOURCE_DIR "${${TARGET_NAME}_SOURCE_DIR}")
