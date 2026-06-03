#include "../include/Post.h"
#include <iostream>
#include <algorithm>

Post::Post(std::string postId, std::string author, std::string text, std::string time) {
    id = postId;
    authorUsername = author;
    content = text;
    timestamp = time;
}

std::string Post::getId() const { return id; }
std::string Post::getAuthorUsername() const { return authorUsername; }
std::string Post::getContent() const { return content; }
std::string Post::getTimestamp() const { return timestamp; }

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

void Post::display(int commentCount) const {
    std::cout << "\n----------------------------------------\n";
    std::cout << "@" << authorUsername << " | Post ID: " << id << " | " << timestamp << "\n";
    std::cout << "Content: " << content << "\n";
    std::cout << "Likes: " << likedBy.size() << "  Comments: " << commentCount << "\n";
    std::cout << "----------------------------------------\n";
}