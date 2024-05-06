#include "InfoMetric.h"
#include <string>

using namespace std;

InfoMetric::InfoMetric(const string &name, const string &help,
					   const prometheus::Labels &constantLabels)
	: Metric(MetricType::Info),
	  family(
		  BuildInfo().Name(name).Help(help).Labels(constantLabels).Register(*this->GetRegistry())) {
	this->family.Add({});
}
