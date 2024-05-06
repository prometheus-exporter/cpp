#include "../main/Main.h"
#include "httplib.h"
#include "gtest/gtest.h"
#include <future>
#include <thread>

using namespace std;

TEST(MainTest, Run_1) {
	int argc = 1;
	char *argv[] = {(char *)"./MainTest"};

	EXPECT_FALSE(Main().Run(argc, argv));
}

TEST(MainTest, Run_2) {
	int argc = 3;
	char *argv[] = {(char *)"./MainTest", (char *)"-a", (char *)"0.0.0.0:20000"};

	future<void> f =
		async(launch::async, [&argc, &argv]() { EXPECT_TRUE(Main().Run(argc, argv)); });

	this_thread::sleep_for(3s);

	httplib::Client client("http://127.0.0.1:20000");

	const auto response = client.Get("/metrics");

	EXPECT_EQ(response->status, 200);

	const auto answers = {
		"metric_01_info{const_label_01=\"const-value-01\"} 1\n",
		"metric_02{const_label_01=\"const-value-01\",label_01=\"value-01\"} ",
		"metric_03{const_label_01=\"const-value-01\",label_01=\"value-01\"} ",
		"metric_04_count{const_label_01=\"const-value-01\",label_01=\"value-01\"} ",
		"metric_04_sum{const_label_01=\"const-value-01\",label_01=\"value-01\"} ",
		"metric_04_bucket{const_label_01=\"const-value-01\",label_01=\"value-01\",le=",
		"metric_05_count{const_label_01=\"const-value-01\",label_01=\"value-01\"} ",
		"metric_05_sum{const_label_01=\"const-value-01\",label_01=\"value-01\"} ",
	};

	for (const auto &answer : answers) {
		if (response->body.contains(answer) == false) {
			EXPECT_STREQ(answer, response->body.c_str());
		}
	}

	EXPECT_EQ(kill(getpid(), SIGTERM), 0);

	f.wait();
}
