#include "CounterMetric.h"
#include <string>

using namespace std;

CounterMetric::CounterMetric(const string &name, const string &help,
							 const prometheus::Labels &constantLabels)
	: Metric(MetricType::Counter), family(BuildCounter()
											  .Name(name)
											  .Help(help)
											  .Labels(constantLabels)
											  .Register(*this->GetRegistry())) {}

void CounterMetric::Increment(const prometheus::Labels &labels) {
	this->GetCounter(labels).Increment();
}

void CounterMetric::Increment(const prometheus::Labels &labels, const double &value) {
	this->GetCounter(labels).Increment(value);
}

void CounterMetric::Reset(const prometheus::Labels &labels) { this->GetCounter(labels).Reset(); }

double CounterMetric::GetValue(const prometheus::Labels &labels) {
	return this->GetCounter(labels).Value();
}

Counter &CounterMetric::GetCounter(const prometheus::Labels &labels) {
	if (auto counter = this->counters.find(labels); counter != this->counters.end()) {
		return counter->second;
	} else {
		this->counters.insert({labels, this->family.Add(labels)});
		return this->counters.at(labels);
	}
}
