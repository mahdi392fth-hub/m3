#ifndef PROFILE_H
#define PROFILE_H

#include <string>

// Represents a user profile in the system
class Profile {
private:
    std::string displayName; // User's visible profile name
    std::string bio;         // User's short biography
    std::string joinDate;    // Date when the user registered

public:
    // Initializes a profile with optional default values
    Profile(std::string name = "", std::string b = "", std::string date = "");

    // Value retrieval methods (Getters)
    std::string getDisplayName() const;
    std::string getBio() const;
    std::string getJoinDate() const;

    // Value modification methods (Setters)
    void setDisplayName(std::string name);
    void setBio(std::string b);
};

#endif