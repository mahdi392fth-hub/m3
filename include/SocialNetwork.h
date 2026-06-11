#ifndef SOCIALNETWORK_H
#define SOCIALNETWORK_H
#include <vector>
#include <string>
#include "User.h"
#include "Post.h"
#include "Comment.h"

class SocialNetwork {
private:
    std::vector<User> users;        // registered users database
    std::vector<Post> posts;        // published posts database
    std::vector<Comment> comments;  // posted comments database

    int currentUserIndex;  
    int nextUserNum;  // counter for generating user IDs
    int nextPostNum;  // counter for generating post IDs

    bool isUsernameTaken(const std::string& uname) const;         // checks and makes sure that the username was not taken before
    int findUserIndex(const std::string& uname);                  // finds the index of the user in the vector
    int findPostIndex(const std::string& postId);                 // finds the index of the post in the vector
    int getCommentCountForPost(const std::string& postId) const;  // counts the comments of a post
    
    void loadData();  // loads data from txt files at app startup
    void saveData();  // saves data to txt files on changes/app shutdown

public:
    SocialNetwork();  // constructor
    ~SocialNetwork(); // destructor
    void start();     // starts the main app menu loop
    
    void registerUser();
    void loginUser();
    void editProfile();
    void createPost();

    void viewProfile();
    void manageRelationships();  // handles follow/unfollow logic
    void viewHomeFeed();         // displays current user's and followed users posts
    void interactWithPost();     // handles likes and comments on a post
    void searchSystem();         // handles searching for users and post contents

    void manageMyPosts();       // allows user to edit or delete their own posts
    void manageSavedPosts();    // shows and removes bookmarked posts
    void deleteCurrentUser();   // completely removes user and all their related data
    void refreshSystemData();   // reads text files again to update data
};
#endif