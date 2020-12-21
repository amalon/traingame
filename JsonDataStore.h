#ifndef TRAINS_JSON_DATA_STORE_H
#define TRAINS_JSON_DATA_STORE_H

#include <string>
#include <fstream>

#include <nlohmann/json.hpp>

template <typename T>
class JsonDataStore
{
    typedef nlohmann::json json;
private:
    typedef std::map<std::string, T*> Database;

    const std::string path;
    Database database;

public:
    JsonDataStore(const std::string newPath)
    : path(newPath)
    {
    }

    const T *get(const std::string &name)
    {
        // Check for existing entry
        typename Database::iterator it = database.find(name);
        if (it != database.end())
            return (*it).second;

        // Load JSON file
        std::string fname = path + name + ".json";
        std::ifstream file(fname);
        json json;
        file >> json;

        // Create item from JSON and add to database
        T *item = load(json);
        database[name] = item;
        return item;
    }

protected:
    T *load(const json &data);
};

#endif // TRAINS_JSON_DATA_STORE_H
