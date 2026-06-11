#include "../include/Post.h"
#include <iostream>
#include <algorithm>

Post::Post(std::string postId, std::string author, std::string text, std::string time, bool edited) {
    id = postId;
    authorUsername = author;
    content = text;
    timestamp = time;
    isEdited = edited;
}

std::string Post::getId() const { return id; }
std::string Post::getAuthorUsername() const { return authorUsername; }
std::string Post::getContent() const { return content; }
std::string Post::getTimestamp() const { return timestamp; }
bool Post::getIsEdited() const { return isEdited; }

const std::vector<std::string>& Post::getLikedBy() const { return likedBy; }
int Post::getLikesCount() const { return (int)likedBy.size(); }

bool Post::toggleLike(const std::string& username) {
    // If already liked, remove the like
    for (size_t i = 0; i < likedBy.size(); i++) {
        if (likedBy[i] == username) {
            likedBy.erase(likedBy.begin() + i);
            return false; 
        }
    }
    // Otherwise, add a new like
    likedBy.push_back(username);
    return true;
}

void Post::editContent(const std::string& newText) {
    content = newText;
    isEdited = true; // Mark as edited
}

void Post::display(int commentCount) const {
    std::cout << "\n----------------------------------------\n";
    std::cout << "@" << authorUsername << " | Post ID: " << id << " | " << timestamp;
    if (isEdited) {
        std::cout << " (Edited)";
    }
    std::cout << "\nContent: " << content << "\n";
    std::cout << "Likes: " << likedBy.size() << "  Comments: " << commentCount << "\n";
    std::cout << "----------------------------------------\n";
}