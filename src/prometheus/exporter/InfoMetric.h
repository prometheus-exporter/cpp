#pragma once

#include "Metric.h"
#include "prometheus/info.h"
#include <string>

using namespace std;
using namespace prometheus;

class InfoMetric : public Metric {
	private:
		Family<Info> &family;

	public:
		InfoMetric(const string &name, const string &help,
				   const prometheus::Labels &constantLabels);
		virtual ~InfoMetric() = default;
};
