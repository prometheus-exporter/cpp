add_subdirectory(common-library)
add_subdirectory(httplib)
add_subdirectory(prometheus)

if(ENABLE_TEST)
	add_subdirectory(googletest)
	add_subdirectory(lcov)
	add_subdirectory(lcov-to-cobertura-xml)
endif(ENABLE_TEST)
