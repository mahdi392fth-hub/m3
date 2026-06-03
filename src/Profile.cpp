#include "../include/Profile.h"

// Constructor: Initializes member variables using an initialization list
Profile::Profile(std::string name, std::string b, std::string date) 
    : displayName(name), bio(b), joinDate(date) {}

// Returns the user's display name
std::string Profile::getDisplayName() const { 
    return displayName; 
}

// Returns the user's biography
std::string Profile::getBio() const { 
    return bio; 
}

// Returns the registration date
std::string Profile::getJoinDate() const { 
    return joinDate; 
}

// Updates the user's display name
void Profile::setDisplayName(std::string name) { 
    displayName = name; 
}

// Updates the user's biography
void Profile::setBio(std::string b) { 
    bio = b; 
}