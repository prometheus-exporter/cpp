#include "../CounterMetric.h"
#include "../Exporter.h"
#include "httplib.h"
#include "gtest/gtest.h"
#include <experimental/random>
#include <string>

using namespace std;

static void test(function<void(const string &port, CounterMetric &metric)> job) {
	const string port = to_string(experimental::randint(10000, 20000));

	CounterMetric metric01{"metric_01", "metric 01", {{"const_label_01", "const-value-01"}}};

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

TEST(CounterMetric, Increment) {
	auto job = [](const string &port, CounterMetric &metric01) {
		metric01.Increment({{"label_01", "value-01"}});
		check(port, "metric_01{const_label_01=\"const-value-01\",label_01=\"value-01\"} 1\n");

		metric01.Increment({{"label_01", "value-01"}}, 2);
		check(port, "metric_01{const_label_01=\"const-value-01\",label_01=\"value-01\"} 3\n");
	};

	test(job);
}

TEST(CounterMetric, Reset) {
	auto job = [](const string &port, CounterMetric &metric01) {
		metric01.Increment({{"label_01", "value-01"}});
		check(port, "metric_01{const_label_01=\"const-value-01\",label_01=\"value-01\"} 1\n");

		metric01.Reset({{"label_01", "value-01"}});
		check(port, "metric_01{const_label_01=\"const-value-01\",label_01=\"value-01\"} 0\n");
	};

	test(job);
}

TEST(CounterMetric, GetValue) {
	auto job = [](const string &port, CounterMetric &metric01) {
		metric01.Increment({{"label_01", "value-01"}});
		EXPECT_EQ(metric01.GetValue({{"label_01", "value-01"}}), 1);

		metric01.Increment({{"label_01", "value-01"}}, 2);
		EXPECT_EQ(metric01.GetValue({{"label_01", "value-01"}}), 3);
	};

	test(job);
}
