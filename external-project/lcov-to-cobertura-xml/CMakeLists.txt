include(FetchContent)

set(TARGET_NAME "lcov-to-cobertura-xml")

FetchContent_Declare(
	${TARGET_NAME}
	GIT_REPOSITORY https://github.com/eriwen/lcov-to-cobertura-xml.git
	GIT_TAG v2.0.2
)

FetchContent_MakeAvailable(${TARGET_NAME})

set_property(GLOBAL PROPERTY LCOV_TO_COBERTURA_XML_SOURCE_DIR "${${TARGET_NAME}_SOURCE_DIR}")
