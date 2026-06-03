#ifndef USER_H
#define USER_H

#include "Profile.h"
#include <string>
#include <vector>

// Represents a user system account and credentials
class User {
private:
    std::string id;              // Unique identifier for the user
    std::string username;        // Account username used for login
    std::string passwordHash;    // Secured (hashed) password string
    Profile userProfile;         // Associated profile details for the user
    std::vector<std::string> following; 
    std::vector<std::string> followers; 

    // Internal helper method to secure passwords
    std::string hashPassword(const std::string& pass) const;

public:
    // Constructor: Sets up account details and handles password preparation
    User(std::string uId, std::string uname, std::string pass, Profile prof, bool isHashed = false);

    // Identity and credential accessors (Getters)
    std::string getId() const;
    std::string getUsername() const;
    std::string getPasswordHash() const;
    
    // Returns a modifiable reference to the internal Profile object
    Profile& getProfile();
    const Profile& getProfile() const;

    // Security check and account update operations
    bool verifyPassword(const std::string& pass) const;
    void changePassword(const std::string& newPass);

    const std::vector<std::string>& getFollowing() const;
    const std::vector<std::string>& getFollowers() const;
    bool followUser(const std::string& targetUsername);
    bool unfollowUser(const std::string& targetUsername);
    void addFollower(const std::string& followerUsername);
    void removeFollower(const std::string& followerUsername);
};

#endif