#pragma once

#include "../prometheus/exporter/Exporter.h"
#include <atomic>
#include <future>
#include <string>

using namespace std;

class Main {
	private:
		atomic_bool stop;

		string address;
		string uri;

		bool ParseCommandLineFlag(int argc, char *argv[]);

		future<void> SetMetric01(Exporter &exporter);
		future<void> SetMetric02(Exporter &exporter);
		future<void> SetMetric03(Exporter &exporter);
		future<void> SetMetric04(Exporter &exporter);
		future<void> SetMetric05(Exporter &exporter);

	public:
		Main();
		~Main() = default;

		bool Run(int argc, char *argv[]);
};
