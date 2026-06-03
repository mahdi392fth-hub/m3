#include "../include/User.h"
#include <functional>
#include <algorithm>

std::string User::hashPassword(const std::string& pass) const {
    std::hash<std::string> hasher;
    return std::to_string(hasher(pass));
}

// Keeping the original initialization list format from phase 1
User::User(std::string uId, std::string uname, std::string pass, Profile prof, bool isHashed) 
    : id(uId), username(uname), userProfile(prof) {
    passwordHash = isHashed ? pass : hashPassword(pass);
}

std::string User::getId() const { 
    return id; 
}

std::string User::getUsername() const { 
    return username; 
}

std::string User::getPasswordHash() const { 
    return passwordHash; 
}

Profile& User::getProfile() { 
    return userProfile; 
}

const Profile& User::getProfile() const { 
    return userProfile; 
}

bool User::verifyPassword(const std::string& pass) const {
    return passwordHash == hashPassword(pass);
}

void User::changePassword(const std::string& newPass) {
    passwordHash = hashPassword(newPass);
}

const std::vector<std::string>& User::getFollowing() const { 
    return following; 
}

const std::vector<std::string>& User::getFollowers() const { 
    return followers; 
}

bool User::followUser(const std::string& targetUsername) {
    for (size_t i = 0; i < following.size(); i++) {
        if (following[i] == targetUsername) return false; 
    }
    following.push_back(targetUsername);
    return true;
}

bool User::unfollowUser(const std::string& targetUsername) {
    for (auto it = following.begin(); it != following.end(); ++it) {
        if (*it == targetUsername) {
            following.erase(it);
            return true;
        }
    }
    return false;
}

void User::addFollower(const std::string& followerUsername) {
    bool exists = false;
    for (size_t i = 0; i < followers.size(); i++) {
        if (followers[i] == followerUsername) exists = true;
    }
    if (!exists) {
        followers.push_back(followerUsername);
    }
}

void User::removeFollower(const std::string& followerUsername) {
    auto it = std::find(followers.begin(), followers.end(), followerUsername);
    if (it != followers.end()) {
        followers.erase(it);
    }
}