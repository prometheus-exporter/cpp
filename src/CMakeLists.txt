set(BUILD_TARGET "prometheus-exporter")
set(ALIAS_NAME "bin::${BUILD_TARGET}")
set(TEST_BUILD_TARGET "${BUILD_TARGET}${POSTFIX_TEST_NAME}")

set(SOURCE
	main/Main.cpp

	prometheus/exporter/Exporter.cpp
	prometheus/exporter/Metric.cpp
	prometheus/exporter/CounterMetric.cpp
	prometheus/exporter/GaugeMetric.cpp
	prometheus/exporter/HistogramMetric.cpp
	prometheus/exporter/InfoMetric.cpp
	prometheus/exporter/SummaryMetric.cpp
)

set(SOURCE_MAIN
	main.cpp
)

set(TEST_SOURCE
	test/main.cpp

	test/MainTest.cpp

	prometheus/exporter/test/ExporterTest.cpp
	prometheus/exporter/test/MetricTest.cpp
	prometheus/exporter/test/CounterMetricTest.cpp
	prometheus/exporter/test/GaugeMetricTest.cpp
	prometheus/exporter/test/HistogramMetricTest.cpp
	prometheus/exporter/test/InfoMetricTest.cpp
	prometheus/exporter/test/SummaryMetricTest.cpp
)

set(DEPENDENCIES
	lib::common
	lib::prometheus
)

list(APPEND DEPENDENCIES_TEST ${DEPENDENCIES} lib::httplib)

set(CONFIGURE_FILE_TEST
)

func_build_binary(${ALIAS_NAME} ${BUILD_TARGET} "${SOURCE};${SOURCE_MAIN}" "${DEPENDENCIES}")
func_build_unit_test("${TEST_BUILD_TARGET}" "${TEST_SOURCE}" "${SOURCE}" "${DEPENDENCIES_TEST}" "${CONFIGURE_FILE_TEST}")
