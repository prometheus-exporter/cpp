#include "HistogramMetric.h"
#include <string>
#include <vector>

using namespace std;

HistogramMetric::HistogramMetric(const string &name, const string &help,
								 const prometheus::Labels &constantLabels,
								 const Histogram::BucketBoundaries &buckets)
	: Metric(MetricType::Histogram), buckets(buckets), family(BuildHistogram()
																  .Name(name)
																  .Help(help)
																  .Labels(constantLabels)
																  .Register(*this->GetRegistry())) {
}

void HistogramMetric::Observe(const prometheus::Labels &labels, double value) {
	this->GetHistogram(labels).Observe(value);
}

void HistogramMetric::ObserveMultiple(const prometheus::Labels &labels,
									  const vector<double> &bucketIncrements, double sumOfValues) {
	this->GetHistogram(labels).ObserveMultiple(bucketIncrements, sumOfValues);
}

void HistogramMetric::Reset(const prometheus::Labels &labels) {
	this->GetHistogram(labels).Reset();
}

Histogram &HistogramMetric::GetHistogram(const prometheus::Labels &labels) {
	if (auto histogram = this->histograms.find(labels); histogram != this->histograms.end()) {
		return histogram->second;
	} else {
		this->histograms.insert({labels, this->family.Add(labels, this->buckets)});
		return this->histograms.at(labels);
	}
}
