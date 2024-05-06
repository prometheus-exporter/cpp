#include "../InfoMetric.h"
#include "../Exporter.h"
#include "../GaugeMetric.h"
#include "httplib.h"
#include "gtest/gtest.h"
#include <experimental/random>
#include <string>

using namespace std;

static void test(function<void(const string &port, InfoMetric &metric)> job) {
	const string port = to_string(experimental::randint(10000, 20000));

	InfoMetric metric01{"metric_01", "metric 01", {{"const_label_01", "const-value-01"}}};

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

TEST(InfoMetric, InfoMetric) {
	auto job = [](const string &port, InfoMetric &metric01) {
		check(port, "metric_01_info{const_label_01=\"const-value-01\"} 1\n");
	};

	test(job);
}
