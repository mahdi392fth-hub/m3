#ifndef POST_H
#define POST_H

#include <string>
#include <vector>

class Post {
private:
    std::string id;
    std::string authorUsername;
    std::string content;
    std::string timestamp;
    std::vector<std::string> likedBy; // Stores usernames of users who liked the post
    bool isEdited; // Checks if the post content has changed

public:
    Post(std::string postId, std::string author, std::string text, std::string time, bool edited = false);
    
    std::string getId() const;
    std::string getAuthorUsername() const;
    std::string getContent() const;
    std::string getTimestamp() const;
    bool getIsEdited() const; // Returns edit status
    
    const std::vector<std::string>& getLikedBy() const;
    int getLikesCount() const;
    
    // Handles liking/unliking logic for a user
    bool toggleLike(const std::string& username);
    void editContent(const std::string& newText); // Changes post text

    void display(int commentCount) const;
};

#endif