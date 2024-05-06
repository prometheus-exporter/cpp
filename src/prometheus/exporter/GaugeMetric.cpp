#include "GaugeMetric.h"
#include <string>

using namespace std;

GaugeMetric::GaugeMetric(const string &name, const string &help,
						 const prometheus::Labels &constantLabels)
	: Metric(MetricType::Gauge), family(BuildGauge()
											.Name(name)
											.Help(help)
											.Labels(constantLabels)
											.Register(*this->GetRegistry())) {}

void GaugeMetric::Increment(const prometheus::Labels &labels) {
	this->GetGauge(labels).Increment();
}

void GaugeMetric::Increment(const prometheus::Labels &labels, const double &value) {
	this->GetGauge(labels).Increment(value);
}

void GaugeMetric::Decrement(const prometheus::Labels &labels) {
	this->GetGauge(labels).Decrement();
}

void GaugeMetric::Decrement(const prometheus::Labels &labels, const double &value) {
	this->GetGauge(labels).Decrement(value);
}

void GaugeMetric::Set(const prometheus::Labels &labels, const double &value) {
	this->GetGauge(labels).Set(value);
}

void GaugeMetric::SetToCurrentTime(const prometheus::Labels &labels) {
	this->GetGauge(labels).SetToCurrentTime();
}

double GaugeMetric::GetValue(const prometheus::Labels &labels) {
	return this->GetGauge(labels).Value();
}

Gauge &GaugeMetric::GetGauge(const prometheus::Labels &labels) {
	if (auto gauge = this->gauges.find(labels); gauge != this->gauges.end()) {
		return gauge->second;
	} else {
		this->gauges.insert({labels, this->family.Add(labels)});
		return this->gauges.at(labels);
	}
}
