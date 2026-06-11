#ifndef USER_H
#define USER_H

#include "Profile.h"
#include <string>
#include <vector>

class User {
private:
    std::string id;
    std::string username;
    std::string passwordHash;
    Profile userProfile;
    std::vector<std::string> following;
    std::vector<std::string> followers;

    // Hashes password for security
    std::string hashPassword(const std::string& pass) const;

public:
    User(std::string uId, std::string uname, std::string pass, Profile prof, bool isHashed = false);

    // Getters
    std::string getId() const;
    std::string getUsername() const;
    std::string getPasswordHash() const;
    
    // Profile accessors
    Profile& getProfile();
    const Profile& getProfile() const;

    // Security
    bool verifyPassword(const std::string& pass) const;
    void changePassword(const std::string& newPass);

    // Social Network Management
    const std::vector<std::string>& getFollowing() const;
    const std::vector<std::string>& getFollowers() const;
    bool followUser(const std::string& targetUsername);
    bool unfollowUser(const std::string& targetUsername);
    void addFollower(const std::string& followerUsername);
    void removeFollower(const std::string& followerUsername);
};

#endif