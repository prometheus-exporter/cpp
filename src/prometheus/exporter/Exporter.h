#pragma once

#include "Metric.h"
#include "prometheus/exposer.h"
#include <string>
#include <vector>

using namespace std;
using namespace prometheus;

class Exporter {
	private:
		Exposer exposer;

	public:
		Exporter(const string &address, const size_t &threadSize = 2);
		virtual ~Exporter() = default;

		void RegisterAuth(function<bool(const string &, const string &)> authCB,
						  const string &realm = "prometheus exporter",
						  const string &uri = "/metrics");

		void RegisterMetric(const Metric &metric, const string &uri = "/metrics");
		void RemoveMetric(const Metric &metric, const string &uri = "/metrics");

		vector<int> GetListeningPorts() const;
};
