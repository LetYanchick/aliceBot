#include "Room.h"
#include <stdexcept>

std::string resourceName(Resource r) {
    switch (r) {
        case Resource::IRON: 
            return "iron";
        case Resource::GOLD: 
            return "gold";
        case Resource::GEMS: 
            return "gems";
        case Resource::EXP: 
            return "exp";
        default: 
            return "_";
    }
}

Resource resourceFromString(const std::string& s) {
    if (s == "iron") 
        return Resource::IRON;
    if (s == "gold") 
        return Resource::GOLD;
    if (s == "gems") 
        return Resource::GEMS;
    if (s == "exp") 
        return Resource::EXP;
    return 
        Resource::NONE;
}

int resourceBaseValue(Resource r) {
    switch (r) {
        case Resource::IRON: 
            return 7;
        case Resource::GOLD: 
            return 11;
        case Resource::GEMS: 
            return 23;
        case Resource::EXP: 
            return 1;
        default: 
            return 0;
    }
}

int& Room::resourceRef(Resource r) {
    switch (r) {
        case Resource::IRON: 
            return iron;
        case Resource::GOLD: 
            return gold;
        case Resource::GEMS: 
            return gems;
        case Resource::EXP:  
            return exp;
        default: 
            throw std::runtime_error("whats that resource");
    }
}

int Room::resourceCount(Resource r) const {
    switch (r) {
        case Resource::IRON: 
            return iron;
        case Resource::GOLD: 
            return gold;
        case Resource::GEMS: 
            return gems;
        case Resource::EXP: 
            return exp;
        default: 
            return 0;
    }
}
