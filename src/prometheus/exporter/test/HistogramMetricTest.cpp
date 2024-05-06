#include "../HistogramMetric.h"
#include "../Exporter.h"
#include "httplib.h"
#include "gtest/gtest.h"
#include <experimental/random>
#include <string>

using namespace std;

static void test(function<void(const string &port, HistogramMetric &metric)> job) {
	const string port = to_string(experimental::randint(10000, 20000));

	HistogramMetric metric01{"metric_01", "metric 01", {{"const_label_01", "const-value-01"}}};

	Exporter exporter{"0.0.0.0:" + port};
	exporter.RegisterMetric(metric01);

	job(port, metric01);
}

static void check(const string &port, const string &answer) {
	httplib::Client client("http://127.0.0.1:" + port);

	const auto response = client.Get("/metrics");

	EXPECT_EQ(response->status, 200);
	if (response->body.contains(answer) == false) {
		EXPECT_STREQ(response->body.c_str(), "");
	}
}

TEST(HistogramMetric, Observe) {
	auto job = [](const string &port, HistogramMetric &metric01) {
		metric01.Observe({{"label_01", "value-01"}}, 1);

		check(port, "metric_01_count{const_label_01=\"const-value-01\",label_01=\"value-01\"} 1\n");
		check(port, "metric_01_sum{const_label_01=\"const-value-01\",label_01=\"value-01\"} 1\n");
		check(port, "metric_01_bucket{const_label_01=\"const-value-01\",label_01=\"value-01\",le=");
	};

	test(job);
}

TEST(HistogramMetric, ObserveMultiple) {
	auto job = [](const string &port, HistogramMetric &metric01) {
		metric01.ObserveMultiple({{"label_01", "value-01"}}, {1}, 3);

		check(port, "metric_01_count{const_label_01=\"const-value-01\",label_01=\"value-01\"} 1\n");
		check(port, "metric_01_sum{const_label_01=\"const-value-01\",label_01=\"value-01\"} 3\n");
		check(port, "metric_01_bucket{const_label_01=\"const-value-01\",label_01=\"value-01\",le=");
	};

	test(job);
}

TEST(HistogramMetric, Reset) {
	auto job = [](const string &port, HistogramMetric &metric01) {
		metric01.Observe({{"label_01", "value-01"}}, 1);

		metric01.Reset({{"label_01", "value-01"}});

		check(port, "metric_01_count{const_label_01=\"const-value-01\",label_01=\"value-01\"} 0\n");
		check(port, "metric_01_sum{const_label_01=\"const-value-01\",label_01=\"value-01\"} 0\n");
		check(port, "metric_01_bucket{const_label_01=\"const-value-01\",label_01=\"value-01\",le="
					"\"+Inf\"} 0\n");
	};

	test(job);
}
