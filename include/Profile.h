#ifndef PROFILE_H
#define PROFILE_H

#include <string>

class Profile {
private:
    std::string displayName;
    std::string bio;
    std::string joinDate;

public:
    Profile(std::string name = "", std::string b = "", std::string date = "");

    std::string getDisplayName() const;
    std::string getBio() const;
    std::string getJoinDate() const;

    void setDisplayName(std::string name);
    void setBio(std::string b);
};

#endif