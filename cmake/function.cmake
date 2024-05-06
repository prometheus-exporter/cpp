function(func_build_binary ALIAS_NAME BUILD_TARGET SOURCE DEPENDENCIES)
	if(NOT "${SOURCE}" STREQUAL "")
		add_executable(${BUILD_TARGET} ${SOURCE})
		add_executable(${ALIAS_NAME} ALIAS ${BUILD_TARGET})
	endif()

	if(TARGET ${BUILD_TARGET})
		foreach(ITEM ${DEPENDENCIES})
			if(ENABLE_TEST)
				target_link_libraries(${BUILD_TARGET} ${ITEM} gcov)
			else()
				target_link_libraries(${BUILD_TARGET} ${ITEM})
			endif(ENABLE_TEST)
		endforeach()

		install(TARGETS ${BUILD_TARGET} DESTINATION ${CMAKE_INSTALL_PREFIX})
	endif()
endfunction()

function(func_build_library ALIAS_NAME BUILD_TARGET HEADER SOURCE DEPENDENCIES)
	if(NOT "${SOURCE}" STREQUAL "")
		add_library(${BUILD_TARGET} STATIC ${SOURCE})
		add_library(${BUILD_TARGET}${POSTFIX_OBJECT_NAME} OBJECT ${SOURCE})
	else()
		add_library(${BUILD_TARGET} INTERFACE)
	endif()

	add_library(${ALIAS_NAME} ALIAS ${BUILD_TARGET})

	if(TARGET ${BUILD_TARGET})
		get_target_property(TARGET_TYPE ${BUILD_TARGET} TYPE)
		if("${TARGET_TYPE}" STREQUAL "STATIC_LIBRARY")
			target_include_directories(${BUILD_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
		elseif("${TARGET_TYPE}" STREQUAL "INTERFACE_LIBRARY")
			target_include_directories(${BUILD_TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
		endif()

		foreach(ITEM ${DEPENDENCIES})
			target_link_libraries(${BUILD_TARGET} ${ITEM})
			target_link_libraries(${BUILD_TARGET}${POSTFIX_OBJECT_NAME} ${ITEM})
		endforeach()
	endif()
endfunction()

function(func_build_unit_test TEST_BUILD_TARGET SOURCE_TEST SOURCE DEPENDENCIES CONFIGURE_FILE)
	if(ENABLE_TEST)
		foreach(ITEM ${CONFIGURE_FILE})
			string(LENGTH "${ITEM}" ITEM_SIZE)

			math(EXPR DESTINATION_FILE_SIZE "${ITEM_SIZE} - 3")

			string(SUBSTRING "${ITEM}" 0 ${DESTINATION_FILE_SIZE} DESTINATION_FILE)

			configure_file("${ITEM}" "${CMAKE_CURRENT_SOURCE_DIR}/${DESTINATION_FILE}")
		endforeach()

		add_executable(${TEST_BUILD_TARGET} ${SOURCE_TEST} ${SOURCE})

		set_source_files_properties(${SOURCE} PROPERTIES COMPILE_FLAGS ${CMAKE_CXX_FLAGS_COVERAGE})

		target_link_libraries(${TEST_BUILD_TARGET} gcov lib::googletest)
		foreach(ITEM ${DEPENDENCIES})
			target_link_libraries(${TEST_BUILD_TARGET} ${ITEM})
		endforeach()

		add_test(NAME ${TEST_BUILD_TARGET} COMMAND ${TEST_BUILD_TARGET})
	endif(ENABLE_TEST)
endfunction()

function(func_make_coverage)
	get_property(LCOV_SOURCE_DIR GLOBAL PROPERTY LCOV_SOURCE_DIR)
	get_property(LCOV_TO_COBERTURA_XML_SOURCE_DIR GLOBAL PROPERTY LCOV_TO_COBERTURA_XML_SOURCE_DIR)

	set(BINARY_LCOV ${LCOV_SOURCE_DIR}/bin/lcov)
	set(BINARY_GENHTML ${LCOV_SOURCE_DIR}/bin/genhtml)

	set(LCOV_FOLDER ${CMAKE_BINARY_DIR}/coverage/lcov)
	set(COVERAGE_RESULT_FOLDER ${CMAKE_BINARY_DIR}/coverage/html)

	set(LCOV_FILE_BASE ${LCOV_FOLDER}/coverage.base)
	set(LCOV_FILE_TEST ${LCOV_FOLDER}/coverage.test)
	set(LCOV_FILE_TOTAL ${LCOV_FOLDER}/coverage.total)
	set(LCOV_EXCEPT_PATH "build" "13")

	add_custom_target(lcov_run
			COMMAND rm -rf ${LCOV_FOLDER} ${COVERAGE_RESULT_FOLDER}
			COMMAND mkdir -p ${LCOV_FOLDER} ${COVERAGE_RESULT_FOLDER}
			COMMAND ${BINARY_LCOV} --capture --initial --directory ${CMAKE_BINARY_DIR} --output-file ${LCOV_FILE_BASE}
			COMMAND ${BINARY_LCOV} --capture --directory ${CMAKE_BINARY_DIR} --output-file ${LCOV_FILE_TEST} --ignore-errors empty,negative
			COMMAND ${BINARY_LCOV} --add-tracefile ${LCOV_FILE_BASE} --add-tracefile ${LCOV_FILE_TEST} --output-file ${LCOV_FILE_TOTAL}
			COMMAND ${BINARY_LCOV} --remove ${LCOV_FILE_TOTAL} ${LCOV_EXCEPT_PATH} --output-file ${LCOV_FILE_TOTAL}
			COMMAND ${BINARY_GENHTML} ${LCOV_FILE_TOTAL} --output-directory ${COVERAGE_RESULT_FOLDER}
			COMMAND python3 ${LCOV_TO_COBERTURA_XML_SOURCE_DIR}/lcov_cobertura/lcov_cobertura.py ${LCOV_FILE_TOTAL} -o ${COVERAGE_RESULT_FOLDER}/coverage.xml
		)
endfunction()
