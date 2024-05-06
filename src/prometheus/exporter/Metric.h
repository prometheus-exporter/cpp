#pragma once

#include "prometheus/metric_type.h"
#include "prometheus/registry.h"
#include <memory>

using namespace std;
using namespace prometheus;

class Metric {
	private:
		MetricType metricType;
		shared_ptr<Registry> registry;

	protected:
	public:
		Metric(const MetricType &metricType);
		virtual ~Metric() = default;

		virtual MetricType GetMetricType() const final;
		virtual shared_ptr<Registry> GetRegistry() const final;
};
