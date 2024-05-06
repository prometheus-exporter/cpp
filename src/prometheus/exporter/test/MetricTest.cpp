#include "../Metric.h"
#include "gtest/gtest.h"

TEST(Metric, GetMetricType) {
	const MetricType metricType = MetricType::Info;

	Metric metric{MetricType::Info};

	EXPECT_EQ(metric.GetMetricType(), metricType);
}

TEST(Metric, GetRegistry) {
	Metric metric{MetricType::Info};

	EXPECT_TRUE(metric.GetRegistry() != nullptr);
}
