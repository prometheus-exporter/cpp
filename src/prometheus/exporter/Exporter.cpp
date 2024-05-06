#include "Exporter.h"
#include <string>
#include <vector>

using namespace std;

Exporter::Exporter(const string &address, const size_t &threadSize)
	: exposer{address, threadSize} {}

void Exporter::RegisterAuth(function<bool(const string &, const string &)> authCB,
							const string &realm, const string &uri) {
	this->exposer.RegisterAuth(authCB, realm, uri);
}

void Exporter::RegisterMetric(const Metric &metric, const string &uri) {
	this->exposer.RegisterCollectable(metric.GetRegistry(), uri);
}

void Exporter::RemoveMetric(const Metric &metric, const string &uri) {
	this->exposer.RemoveCollectable(metric.GetRegistry(), uri);
}

vector<int> Exporter::GetListeningPorts() const { return this->exposer.GetListeningPorts(); }
