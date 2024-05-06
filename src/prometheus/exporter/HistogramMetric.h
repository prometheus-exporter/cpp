#pragma once

#include "Metric.h"
#include "prometheus/histogram.h"
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace prometheus;

class HistogramMetric : public Metric {
	private:
		const Histogram::BucketBoundaries buckets;

		Family<Histogram> &family;
		map<prometheus::Labels, Histogram &> histograms;

		Histogram &GetHistogram(const prometheus::Labels &labels);

	public:
		HistogramMetric(const string &name, const string &help,
						const prometheus::Labels &constantLabels,
						const Histogram::BucketBoundaries &buckets = {});
		virtual ~HistogramMetric() = default;

		void Observe(const prometheus::Labels &labels, double value);
		void ObserveMultiple(const prometheus::Labels &labels,
							 const vector<double> &bucketIncrements, double sumOfValues);
		void Reset(const prometheus::Labels &labels);
};
