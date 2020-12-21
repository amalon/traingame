#ifndef TRAINS_RAIL_PROFILE_H
#define TRAINS_RAIL_PROFILE_H

#include <string>
#include <map>
#include <ostream>

// Generic rail profile
class RailProfile
{
private:
    struct xy {
        float x, y;
    };
    typedef std::map<std::string, struct xy> ReferenceMap;

    std::string name;

    // Reference positions
    ReferenceMap refs;

public:

    void setName(const std::string &newName)
    {
        name = newName;
    }

    void addRef(const std::string &name, float x, float y)
    {
        struct xy xy = { x, y };
        refs[name] = xy;
    }

    bool lookupRef(const std::string &name, float &x_out, float &y_out) const
    {
        ReferenceMap::const_iterator it = refs.find(name);
        if (it == refs.end())
            return false;
        x_out = (*it).second.x;
        y_out = (*it).second.y;
        return true;
    }

    friend std::ostream &operator << (std::ostream &s, const RailProfile &profile) {
        return s << "{ name: \"" << profile.name << "\" }";
    }

protected:
};

#endif // TRAINS_RAIL_PROFILE_H
