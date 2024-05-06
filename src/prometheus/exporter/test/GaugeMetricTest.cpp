#include "../GaugeMetric.h"
#include "../Exporter.h"
#include "httplib.h"
#include "gtest/gtest.h"
#include <experimental/random>
#include <string>

using namespace std;

static void test(function<void(const string &port, GaugeMetric &metric)> job) {
	const string port = to_string(experimental::randint(10000, 20000));

	GaugeMetric metric01{"metric_01", "metric 01", {{"const_label_01", "const-value-01"}}};

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

TEST(GaugeMetric, Increment) {
	auto job = [](const string &port, GaugeMetric &metric01) {
		metric01.Increment({{"label_01", "value-01"}});
		check(port, "metric_01{const_label_01=\"const-value-01\",label_01=\"value-01\"} 1\n");

		metric01.Increment({{"label_01", "value-01"}}, 2);
		check(port, "metric_01{const_label_01=\"const-value-01\",label_01=\"value-01\"} 3\n");
	};

	test(job);
}

TEST(GaugeMetric, Decrement) {
	auto job = [](const string &port, GaugeMetric &metric01) {
		metric01.Decrement({{"label_01", "value-01"}});
		check(port, "metric_01{const_label_01=\"const-value-01\",label_01=\"value-01\"} -1\n");

		metric01.Decrement({{"label_01", "value-01"}}, 2);
		check(port, "metric_01{const_label_01=\"const-value-01\",label_01=\"value-01\"} -3\n");
	};

	test(job);
}

TEST(GaugeMetric, Set) {
	auto job = [](const string &port, GaugeMetric &metric01) {
		metric01.Set({{"label_01", "value-01"}}, 5);
		check(port, "metric_01{const_label_01=\"const-value-01\",label_01=\"value-01\"} 5\n");

		metric01.Set({{"label_01", "value-01"}}, 10);
		check(port, "metric_01{const_label_01=\"const-value-01\",label_01=\"value-01\"} 10\n");
	};

	test(job);
}

TEST(GaugeMetric, SetToCurrentTime) {
	auto job = [](const string &port, GaugeMetric &metric01) {
		metric01.SetToCurrentTime({{"label_01", "value-01"}});
		check(port, "metric_01{const_label_01=\"const-value-01\",label_01=\"value-01\"} ");
	};

	test(job);
}

TEST(GaugeMetric, GetValue) {
	auto job = [](const string &port, GaugeMetric &metric01) {
		metric01.Set({{"label_01", "value-01"}}, 5);
		EXPECT_EQ(metric01.GetValue({{"label_01", "value-01"}}), 5);
	};

	test(job);
}
