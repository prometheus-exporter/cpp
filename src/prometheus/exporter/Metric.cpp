#include "Metric.h"
#include <memory>

using namespace std;

Metric::Metric(const MetricType &metricType)
	: metricType(metricType), registry(make_shared<Registry>()) {}

MetricType Metric::GetMetricType() const { return this->metricType; }

shared_ptr<Registry> Metric::GetRegistry() const { return this->registry; }
