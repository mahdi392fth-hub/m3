#include "../include/User.h"
#include <functional>
#include <algorithm>

// Hashes password for security
std::string User::hashPassword(const std::string& pass) const {
    std::hash<std::string> hasher;
    return std::to_string(hasher(pass));
}

// Constructor: Initializes basic info and hashes password if needed
User::User(std::string uId, std::string uname, std::string pass, Profile prof, bool isHashed) 
    : id(uId), username(uname), userProfile(prof) {
    passwordHash = isHashed ? pass : hashPassword(pass);
}

// Identity Getters
std::string User::getId() const { return id; }
std::string User::getUsername() const { return username; }
std::string User::getPasswordHash() const { return passwordHash; }

// Profile accessors
Profile& User::getProfile() { return userProfile; }
const Profile& User::getProfile() const { return userProfile; }

bool User::verifyPassword(const std::string& pass) const {
    return passwordHash == hashPassword(pass);
}

// Updates password with a new hash
void User::changePassword(const std::string& newPass) {
    passwordHash = hashPassword(newPass);
}

// Social Networking Getters
const std::vector<std::string>& User::getFollowing() const { return following; }
const std::vector<std::string>& User::getFollowers() const { return followers; }

// Adds a user to the following list if not already present
bool User::followUser(const std::string& targetUsername) {
    for (size_t i = 0; i < following.size(); i++) {
        if (following[i] == targetUsername) return false; 
    }
    following.push_back(targetUsername);
    return true;
}

// Removes a user from following list
bool User::unfollowUser(const std::string& targetUsername) {
    for (auto it = following.begin(); it != following.end(); ++it) {
        if (*it == targetUsername) {
            following.erase(it);
            return true;
        }
    }
    return false;
}

// Management of followers list
void User::addFollower(const std::string& followerUsername) {
    bool exists = false;
    for (size_t i = 0; i < followers.size(); i++) {
        if (followers[i] == followerUsername) exists = true;
    }
    if (!exists) followers.push_back(followerUsername);
}

// Removes a follower from the list
void User::removeFollower(const std::string& followerUsername) {
    auto it = std::find(followers.begin(), followers.end(), followerUsername);
    if (it != followers.end()) {
        followers.erase(it);
    }
}

// Manage user bookmarks
const std::vector<std::string>& User::getSavedPostIds() const {
    return savedPostIds;
}

bool User::savePost(const std::string& postId) {
    for (size_t i = 0; i < savedPostIds.size(); i++) {
        if (savedPostIds[i] == postId) return false; 
    }
    savedPostIds.push_back(postId);
    return true;
}

bool User::unsavePost(const std::string& postId) {
    auto it = std::find(savedPostIds.begin(), savedPostIds.end(), postId);
    if (it != savedPostIds.end()) {
        savedPostIds.erase(it);
        return true;
    }
    return false;
}