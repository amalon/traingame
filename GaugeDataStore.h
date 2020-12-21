#ifndef TRAINS_GAUGE_DATA_STORE_H
#define TRAINS_GAUGE_DATA_STORE_H

#include "Gauge.h"
#include "JsonDataStore.h"

typedef JsonDataStore<Gauge> GaugeDataStore;

extern GaugeDataStore gaugeDataStore;

#endif // TRAINS_GAUGE_DATA_STORE_H
