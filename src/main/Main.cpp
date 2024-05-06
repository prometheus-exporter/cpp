#include "Main.h"
#include "../prometheus/exporter/CounterMetric.h"
#include "../prometheus/exporter/Exporter.h"
#include "../prometheus/exporter/GaugeMetric.h"
#include "../prometheus/exporter/HistogramMetric.h"
#include "../prometheus/exporter/InfoMetric.h"
#include "../prometheus/exporter/SummaryMetric.h"
#include "Signal.h"
#include <future>
#include <getopt.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

Main::Main() : stop(true), address(""), uri("/metrics") {}

bool Main::ParseCommandLineFlag(int argc, char *argv[]) {
	auto help = [&argv]() {
		cout << "Usage of " << argv[0] << endl;
		cout << "  --address[-a] 0.0.0.0:10000 : address" << endl;
		cout << "  --uri[-u] metrics           : uri     : default(metrics)" << endl;
		cout << "  --help[-h]                  : usage" << endl;
	};

	if (argc == 1) {
		help();
		return false;
	}

	const char *shortOptions = "a:u:h";

	const option longOptions[] = {{"address", required_argument, nullptr, 'a'},
								  {"uri", required_argument, nullptr, 'u'},
								  {"help", no_argument, nullptr, 'h'}};

	while (true) {
		const auto opt = getopt_long(argc, argv, shortOptions, longOptions, nullptr);
		if (opt == -1) {
			break;
		}

		switch (opt) {
		case 'a':
			this->address = optarg;
			break;
		case 'u':
			this->uri = "/" + string(optarg);
			break;
		case 'h':
		default:
			help();

			return false;
		}
	}

	return true;
}

future<void> Main::SetMetric01(Exporter &exporter) {
	return async(launch::async, [this, &exporter]() {
		InfoMetric metric{"metric_01", "metric 01 help", {{"const_label_01", "const-value-01"}}};
		exporter.RegisterMetric(metric, this->uri);

		while (this->stop == false) {
			this_thread::sleep_for(1ms);
		}
	});
}

future<void> Main::SetMetric02(Exporter &exporter) {
	return async(launch::async, [this, &exporter]() {
		CounterMetric metric{"metric_02", "metric 02 help", {{"const_label_01", "const-value-01"}}};

		exporter.RegisterMetric(metric, this->uri);

		while (this->stop == false) {
			metric.Increment({{"label_01", "value-01"}});

			this_thread::sleep_for(1s);
		}
	});
}

future<void> Main::SetMetric03(Exporter &exporter) {
	return async(launch::async, [this, &exporter]() {
		GaugeMetric metric{"metric_03", "metric 03 help", {{"const_label_01", "const-value-01"}}};

		exporter.RegisterMetric(metric, this->uri);

		while (this->stop == false) {
			metric.Decrement({{"label_01", "value-01"}});

			this_thread::sleep_for(1s);
		}
	});
}

future<void> Main::SetMetric04(Exporter &exporter) {
	return async(launch::async, [this, &exporter]() {
		HistogramMetric metric{
			"metric_04", "metric 04 help", {{"const_label_01", "const-value-01"}}};

		exporter.RegisterMetric(metric, this->uri);

		while (this->stop == false) {
			metric.Observe({{"label_01", "value-01"}}, 1);

			this_thread::sleep_for(1s);
		}
	});
}

future<void> Main::SetMetric05(Exporter &exporter) {
	return async(launch::async, [this, &exporter]() {
		SummaryMetric metric{"metric_05", "metric 05 help", {{"const_label_01", "const-value-01"}}};

		exporter.RegisterMetric(metric, this->uri);

		while (this->stop == false) {
			metric.Observe({{"label_01", "value-01"}}, 1);

			this_thread::sleep_for(1s);
		}
	});
}

bool Main::Run(int argc, char *argv[]) {
	this->stop.store(false);

	if (this->ParseCommandLineFlag(argc, argv) == false) {
		return false;
	}

	Exporter exporter{this->address};

	vector<future<void>> futures = {};
	futures.push_back(this->SetMetric01(exporter));
	futures.push_back(this->SetMetric02(exporter));
	futures.push_back(this->SetMetric03(exporter));
	futures.push_back(this->SetMetric04(exporter));
	futures.push_back(this->SetMetric05(exporter));

	auto stop = [this](int) { this->stop.store(true); };
	Signal::Instance().Add(SIGINT, stop);
	Signal::Instance().Add(SIGTERM, stop);

	while (this->stop == false) {
		this_thread::sleep_for(1s);
	}

	for (const auto &iter : futures) {
		iter.wait();
	}

	Signal::Instance().Delete();

	return true;
}
