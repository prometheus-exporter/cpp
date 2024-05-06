#pragma once

#include "Metric.h"
#include "prometheus/family.h"
#include "prometheus/gauge.h"
#include <map>
#include <string>

using namespace std;
using namespace prometheus;

class GaugeMetric : public Metric {
	private:
		Family<Gauge> &family;
		map<prometheus::Labels, Gauge &> gauges;

		Gauge &GetGauge(const prometheus::Labels &labels);

	public:
		GaugeMetric(const string &name, const string &help,
					const prometheus::Labels &constantLabels);
		virtual ~GaugeMetric() = default;

		void Increment(const prometheus::Labels &labels);
		void Increment(const prometheus::Labels &labels, const double &value);

		void Decrement(const prometheus::Labels &labels);
		void Decrement(const prometheus::Labels &labels, const double &value);

		void Set(const prometheus::Labels &labels, const double &value);

		void SetToCurrentTime(const prometheus::Labels &labels);

		double GetValue(const prometheus::Labels &labels);
};
