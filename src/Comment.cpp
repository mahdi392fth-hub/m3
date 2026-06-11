#include "../include/Comment.h"

// Constructor: Initializes all comment data
Comment::Comment(std::string pId, std::string auth, std::string txt, std::string time)
    : postId(pId), author(auth), text(txt), timestamp(time) {}

// Data retrieval methods
std::string Comment::getPostId() const { return postId; }
std::string Comment::getAuthor() const { return author; }
std::string Comment::getText() const { return text; }
std::string Comment::getTimestamp() const { return timestamp; }