#include "../include/Profile.h"

// Constructor: Initializes member variables using an initialization list
Profile::Profile(std::string name, std::string b, std::string date) 
    : displayName(name), bio(b), joinDate(date) {}

std::string Profile::getDisplayName() const { 
    return displayName; 
}

std::string Profile::getBio() const { 
    return bio; 
}

std::string Profile::getJoinDate() const { 
    return joinDate; 
}

void Profile::setDisplayName(std::string name) { 
    displayName = name; 
}

void Profile::setBio(std::string b) { 
    bio = b; 
}