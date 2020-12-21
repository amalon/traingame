#include "RailProfileDataStore.h"
#include "RailProfileSimple.h"

typedef nlohmann::json json;

RailProfileDataStore railProfileDataStore("data/rail_profile/");

template <>
RailProfile *RailProfileDataStore::load(const json &data)
{
    std::string type = data["type"];
    RailProfile *profile = nullptr;
    if (type == "simple") {
        RailProfileSimple *simple = new RailProfileSimple();

        json blocks = data["blocks"];
        json foot = blocks["foot"];
        json web = blocks["web"];
        json head = blocks["head"];
        simple->setBlockSize(RailProfileSimple::FOOT, foot[0], foot[1]);
        simple->setBlockSize(RailProfileSimple::WEB, web[0], web[1]);
        simple->setBlockSize(RailProfileSimple::HEAD, head[0], head[1]);

        profile = simple;
    } else {
        profile = new RailProfile();
    }

    profile->setName(data["name"]);

    json refs = data["references"];
    for (json::iterator it = refs.begin(); it != refs.end(); ++it) {
        json ref = it.value();
        profile->addRef(it.key(), ref[0], ref[1]);
    }

    return profile;
}
