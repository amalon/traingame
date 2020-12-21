#ifndef TRAINS_RAIL_PROFILE_DATA_STORE_H
#define TRAINS_RAIL_PROFILE_DATA_STORE_H

#include "RailProfile.h"
#include "JsonDataStore.h"

typedef JsonDataStore<RailProfile> RailProfileDataStore;

extern RailProfileDataStore railProfileDataStore;

#endif // TRAINS_RAIL_PROFILE_DATA_STORE_H
