#include "../Exporter.h"
#include "../CounterMetric.h"
#include "httplib.h"
#include "gtest/gtest.h"
#include <experimental/random>
#include <string>

using namespace std;

static void check(const string &port, const string &uri, const string &answer,
				  const bool &containsAnswer = true) {
	httplib::Client client("http://127.0.0.1:" + port);

	auto response = client.Get(uri);

	EXPECT_EQ(response->status, 200);
	if (response->body.contains(answer) == !containsAnswer) {
		EXPECT_STREQ(response->body.c_str(), answer.c_str());
	}
};

TEST(Exporter, RegisterAuth) {
	const string USER = "user";
	const string PASSWORD = "password";
	const string port = to_string(experimental::randint(10000, 20000));

	CounterMetric metric01{"metric_01", "metric 01", {{"const_label_01", "const-value-01"}}};

	Exporter exporter{"0.0.0.0:" + port};

	auto authCB = [&](const std::string &user, const std::string &password) {
		return user == USER && password == PASSWORD;
	};
	exporter.RegisterAuth(authCB);

	exporter.RegisterMetric(metric01);

	httplib::Client client("http://127.0.0.1:" + port);

	{
		client.set_basic_auth(USER, PASSWORD);

		auto response = client.Get("/metrics");
		EXPECT_EQ(response->status, 200);
	}

	{
		client.set_basic_auth(USER, "invalid");

		auto response = client.Get("/metrics");
		EXPECT_EQ(response->status, 401);
	}
}

TEST(Exporter, RegisterMetric) {
	const string port = to_string(experimental::randint(10000, 20000));

	CounterMetric metric01{"metric_01", "metric 01", {{"const_label_01", "const-value-01"}}};
	CounterMetric metric02{"metric_02", "metric 02", {{"const_label_01", "const-value-01"}}};

	Exporter exporter{"0.0.0.0:" + port};
	exporter.RegisterMetric(metric01);
	exporter.RegisterMetric(metric02, "/metrics-02");

	metric01.Increment({{"label_01", "value-01"}});
	check(port, "/metrics",
		  "metric_01{const_label_01=\"const-value-01\",label_01=\"value-01\"} 1\n");

	metric02.Increment({{"label_01", "value-01"}});
	check(port, "/metrics-02",
		  "metric_02{const_label_01=\"const-value-01\",label_01=\"value-01\"} 1\n");
}

TEST(Exporter, RemoveMetric) {
	const string port = to_string(experimental::randint(10000, 20000));

	CounterMetric metric01{"metric_01", "metric 01", {{"const_label_01", "const-value-01"}}};

	Exporter exporter{"0.0.0.0:" + port};
	exporter.RegisterMetric(metric01);

	metric01.Increment({{"label_01", "value-01"}});
	check(port, "/metrics",
		  "metric_01{const_label_01=\"const-value-01\",label_01=\"value-01\"} 1\n");

	exporter.RemoveMetric(metric01);
	check(port, "/metrics",
		  "metric_01{const_label_01=\"const-value-01\",label_01=\"value-01\"} 1\n", false);
}

TEST(Exporter, GetListeningPorts) {
	const string port = to_string(experimental::randint(10000, 20000));

	Exporter exporter{"0.0.0.0:" + port};

	const auto ports = exporter.GetListeningPorts();
	EXPECT_EQ(ports.size(), 1);
	EXPECT_EQ(ports[0], stoi(port));
}
