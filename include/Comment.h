#ifndef COMMENT_H
#define COMMENT_H

#include <string>

class Comment {
private:
    std::string postId;
    std::string author;
    std::string text;
    std::string timestamp;

public:
    Comment(std::string pId, std::string auth, std::string txt, std::string time);

    // Getters
    std::string getPostId() const;
    std::string getAuthor() const;
    std::string getText() const;
    std::string getTimestamp() const;
};

#endif