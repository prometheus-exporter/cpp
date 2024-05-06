#pragma once

#include "Metric.h"
#include "prometheus/counter.h"
#include "prometheus/family.h"
#include <map>
#include <string>

using namespace std;
using namespace prometheus;

class CounterMetric : public Metric {
	private:
		Family<Counter> &family;
		map<prometheus::Labels, Counter &> counters;

		Counter &GetCounter(const prometheus::Labels &labels);

	public:
		CounterMetric(const string &name, const string &help,
					  const prometheus::Labels &constantLabels);
		virtual ~CounterMetric() = default;

		void Increment(const prometheus::Labels &labels);
		void Increment(const prometheus::Labels &labels, const double &value);

		void Reset(const prometheus::Labels &labels);

		double GetValue(const prometheus::Labels &labels);
};
