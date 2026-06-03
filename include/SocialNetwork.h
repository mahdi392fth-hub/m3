#ifndef SOCIALNETWORK_H
#define SOCIALNETWORK_H
#include <vector>
#include <string>
#include "User.h"
#include "Post.h"
#include "Comment.h"

class SocialNetwork {
private:
    std::vector<User> users;
    std::vector<Post> posts;
    std::vector<Comment> comments;
    int currentUserIndex; 
    
    int nextUserNum;
    int nextPostNum;

    bool isUsernameTaken(const std::string& uname) const;
    int findUserIndex(const std::string& uname);
    int findPostIndex(const std::string& postId);
    int getCommentCountForPost(const std::string& postId) const;
    
    void loadData(); 
    void saveData(); 

public:
    SocialNetwork(); // constructor
    ~SocialNetwork(); // destructor
    void start();
    
    void registerUser();
    void loginUser();
    void editProfile();
    void createPost();
    
    void viewProfile();
    void manageRelationships();
    void viewHomeFeed();
    void interactWithPost();
    void searchSystem();
};
#endif