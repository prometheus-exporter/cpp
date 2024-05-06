#pragma once

#include "Metric.h"
#include "prometheus/summary.h"
#include <map>
#include <string>

using namespace std;
using namespace prometheus;

class SummaryMetric : public Metric {
	private:
		const Summary::Quantiles quantiles;
		const chrono::milliseconds maxAge;
		const int ageBuckets;

		Family<Summary> &family;
		map<prometheus::Labels, Summary &> summaries;

		Summary &GetSummary(const prometheus::Labels &labels);

	public:
		SummaryMetric(const string &name, const string &help,
					  const prometheus::Labels &constantLabels,
					  const Summary::Quantiles &quantiles = {},
					  const chrono::milliseconds &maxAge = chrono::seconds{60},
					  const int &ageBuckets = 5);
		virtual ~SummaryMetric() = default;

		void Observe(const prometheus::Labels &labels, const double &value);
};
