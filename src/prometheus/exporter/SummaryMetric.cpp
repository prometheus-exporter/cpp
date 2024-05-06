#include "SummaryMetric.h"
#include <string>

using namespace std;

SummaryMetric::SummaryMetric(const string &name, const string &help,
							 const prometheus::Labels &constantLabels,
							 const Summary::Quantiles &quantiles,
							 const chrono::milliseconds &maxAge, const int &ageBuckets)
	: Metric(MetricType::Summary), quantiles(quantiles), maxAge(maxAge), ageBuckets(ageBuckets),
	  family(BuildSummary()
				 .Name(name)
				 .Help(help)
				 .Labels(constantLabels)
				 .Register(*this->GetRegistry())) {}

void SummaryMetric::Observe(const prometheus::Labels &labels, const double &value) {
	this->GetSummary(labels).Observe(value);
}

Summary &SummaryMetric::GetSummary(const prometheus::Labels &labels) {
	if (auto summary = this->summaries.find(labels); summary != this->summaries.end()) {
		return summary->second;
	} else {
		this->summaries.insert(
			{labels, this->family.Add(labels, this->quantiles, this->maxAge, this->ageBuckets)});
		return this->summaries.at(labels);
	}
}
