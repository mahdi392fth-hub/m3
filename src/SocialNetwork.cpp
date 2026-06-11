#include "../include/SocialNetwork.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>

SocialNetwork::SocialNetwork() {
    currentUserIndex = -1;
    nextUserNum = 1;
    nextPostNum = 1;
    loadData();
}

SocialNetwork::~SocialNetwork() {
    saveData();
}

bool SocialNetwork::isUsernameTaken(const std::string& uname) const {  // const method only reads the input and doesn't change anything
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].getUsername() == uname) return true;
    }
    return false;
}

int SocialNetwork::findUserIndex(const std::string& uname) {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].getUsername() == uname) return (int)i;
    }
    return -1;  // user not found
}

int SocialNetwork::findPostIndex(const std::string& postId) {
    for (size_t i = 0; i < posts.size(); ++i) {
        if (posts[i].getId() == postId) return (int)i;
    }
    return -1;  // post not found
}

int SocialNetwork::getCommentCountForPost(const std::string& postId) const {
    int total = 0;
    for (const auto& c : comments) {
        if (c.getPostId() == postId) {
            total++;
        }
    }
    return total;
}

void SocialNetwork::loadData() {
    // Reset data to read from files again
    users.clear();
    posts.clear();
    comments.clear();
    nextUserNum = 1;
    nextPostNum = 1;

    // Load user profiles
    std::ifstream uFile("users.txt");
    std::string line;
    
    while (std::getline(uFile, line)) {
        if(line.empty()) continue;
        std::stringstream ss(line);
        std::string id, uname, hash, dname, bio, date;
        
        std::getline(ss, id, '|');
        std::getline(ss, uname, '|');
        std::getline(ss, hash, '|');
        std::getline(ss, dname, '|');
        std::getline(ss, bio, '|');
        std::getline(ss, date, '|');
        
        // Construct and store the User object
        users.push_back(User(id, uname, hash, Profile(dname, bio, date), true));
        
        // Update next available user ID
        int num = std::stoi(id.substr(1));
        if (num >= nextUserNum) nextUserNum = num + 1;
    }
    uFile.close();
    
    // Load social media posts
    std::ifstream pFile("posts.txt");
    while (std::getline(pFile, line)) {
        if(line.empty()) continue;
        std::stringstream ss(line);
        std::string id, author, content, time, statusEdited;
        
        std::getline(ss, id, '|');
        std::getline(ss, author, '|');
        std::getline(ss, content, '|');
        std::getline(ss, time, '|');
        std::getline(ss, statusEdited, '|');
        
        bool isEd = (statusEdited == "1");
        posts.push_back(Post(id, author, content, time, isEd));
        
        // Update next available post ID
        int num = std::stoi(id.substr(1));
        if (num >= nextPostNum) nextPostNum = num + 1;
    }
    pFile.close();

    // Load follower or following relationships
    std::ifstream fFile("follows.txt");
    std::string follower, followed;
    while (fFile >> follower) {
        size_t pos = follower.find('|');
        if(pos != std::string::npos) {
            followed = follower.substr(pos + 1);
            follower = follower.substr(0, pos);
            
            // Connect the two users if both exist in the system
            int idx1 = findUserIndex(follower);
            int idx2 = findUserIndex(followed);
            if (idx1 != -1 && idx2 != -1) {
                users[idx1].followUser(followed);
                users[idx2].addFollower(follower);
            }
        }
    }
    fFile.close();

    // Load post likes
    std::ifstream lFile("likes.txt");
    while (std::getline(lFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string pId, uName;
        std::getline(ss, pId, '|');
        std::getline(ss, uName, '|');
        
        // Add the user's like to this post
        int pIdx = findPostIndex(pId);
        if (pIdx != -1) posts[pIdx].toggleLike(uName);
    }
    lFile.close();

    // Load post comments
    std::ifstream cFile("comments.txt");
    while (std::getline(cFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string cPostId, cAuth, cText, cTime;
        std::getline(ss, cPostId, '|');
        std::getline(ss, cAuth, '|');
        std::getline(ss, cText, '|');
        std::getline(ss, cTime, '|');
        
        comments.push_back(Comment(cPostId, cAuth, cText, cTime));
    }
    cFile.close();

    // Load all user bookmarks
    std::ifstream sFile("saved.txt");
    while (std::getline(sFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string uName, pId;
        std::getline(ss, uName, '|');
        std::getline(ss, pId, '|');
        
        int uIdx = findUserIndex(uName);
        if (uIdx != -1) {
            users[uIdx].savePost(pId);
        }
    }
    sFile.close();
}

void SocialNetwork::saveData() {
    // Save user profiles
    std::ofstream uFile("users.txt");
    for (size_t i = 0; i < users.size(); i++) {
        User& u = users[i];
        uFile << u.getId() << "|" << u.getUsername() << "|" << u.getPasswordHash() << "|"
              << u.getProfile().getDisplayName() << "|" << u.getProfile().getBio() << "|"
              << u.getProfile().getJoinDate() << "\n";
    }
    uFile.close();
    
    // Save social media posts
    std::ofstream pFile("posts.txt");
    for (auto& p : posts) {
        pFile << p.getId() << "|" << p.getAuthorUsername() << "|" 
              << p.getContent() << "|" << p.getTimestamp() << "|" 
              << (p.getIsEdited() ? "1" : "0") << "\n";
    }
    pFile.close();

    // Save follower or following relationships
    std::ofstream fFile("follows.txt");
    for (size_t i = 0; i < users.size(); i++) {
        for (const auto& f : users[i].getFollowing()) {
            fFile << users[i].getUsername() << "|" << f << "\n";
        }
    }
    fFile.close();

    // Save post likes
    std::ofstream lFile("likes.txt");
    for (size_t i = 0; i < posts.size(); i++) {
        for (const auto& name : posts[i].getLikedBy()) {
            lFile << posts[i].getId() << "|" << name << "\n";
        }
    }
    lFile.close();

    // Save post comments
    std::ofstream cFile("comments.txt");
    for (const auto& c : comments) {
        cFile << c.getPostId() << "|" << c.getAuthor() << "|" 
              << c.getText() << "|" << c.getTimestamp() << "\n";
    }
    cFile.close();

    // Save all user bookmarks
    std::ofstream sFile("saved.txt");
    for (size_t i = 0; i < users.size(); i++) {
        for (const auto& pId : users[i].getSavedPostIds()) {
            sFile << users[i].getUsername() << "|" << pId << "\n";
        }
    }
    sFile.close();
}

void SocialNetwork::refreshSystemData() {
    // Reloads all files to refresh the database
    std::string currentUsername = (currentUserIndex != -1) ? users[currentUserIndex].getUsername() : "";
    loadData();
    if (!currentUsername.empty()) {
        currentUserIndex = findUserIndex(currentUsername);
    }
    std::cout << "Data reloaded successfully.\n";
}

void SocialNetwork::start() {
    int choice; 
    while (true) {
        // user not logged in yet
        if (currentUserIndex == -1) { 
            std::cout << "\n Welcome to MMA Mini Social Application!\n1. Register\n2. Login\n3. Refresh Data\n4. Exit\nChoice: ";
            if (!(std::cin >> choice)) { // input validation
                std::cin.clear();
                std::string temp; std::cin >> temp;
                std::cout << "Please enter a valid number.\n";
                continue;
            }

            if (choice == 1) registerUser();
            else if (choice == 2) loginUser();
            else if (choice == 3) refreshSystemData();
            else if (choice == 4) break;
            else std::cout << "Invalid input!\n";
        } 
        // user logged in successfully
        else {
            std::cout << "\n Welcome, [@" << users[currentUserIndex].getUsername() << "]\n";
            std::cout << "1. Create a Post\n";
            std::cout << "2. View Feed\n";
            std::cout << "3. View Profile\n";
            std::cout << "4. Follow/Unfollow (Manage Relationships)\n";
            std::cout << "5. Like/Comment on a Post\n";
            std::cout << "6. Search Post/User\n";
            std::cout << "7. Edit Profile\n";
            std::cout << "8. Change Password\n";
            std::cout << "9. Manage My Posts (Edit/Delete)\n";
            std::cout << "10. Saved Posts (Bookmarks)\n";
            std::cout << "11. Delete My Account\n";
            std::cout << "12. Refresh / Sync Data\n";
            std::cout << "13. Logout\nChoice: ";
            
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::string temp; std::cin >> temp;
                std::cout << "Please enter a valid number.\n";
                continue;
            }

            if (choice == 1) createPost();
            else if (choice == 2) viewHomeFeed();
            else if (choice == 3) viewProfile();
            else if (choice == 4) manageRelationships();
            else if (choice == 5) interactWithPost();
            else if (choice == 6) searchSystem();
            else if (choice == 7) editProfile();
            else if (choice == 8) {
                std::string newPass;
                std::cout << "New Password: "; std::cin >> newPass;
                users[currentUserIndex].changePassword(newPass);
                saveData();
                std::cout << "Password updated.\n";
            }
            else if (choice == 9) manageMyPosts();
            else if (choice == 10) manageSavedPosts();
            else if (choice == 11) deleteCurrentUser();
            else if (choice == 12) refreshSystemData();
            else if (choice == 13) currentUserIndex = -1; // user logging out
            else std::cout << "Invalid input!\n";
        }
    }
}

void SocialNetwork::registerUser() {
    std::string uname, pass, dname;
    std::cout << "Username: "; std::cin >> uname;
    

    // controlling input usernames
    if (uname.empty() || isUsernameTaken(uname)) {
        std::cout << "Error: Invalid or duplicate username.\n";
        return;
    }
    
    std::cout << "Password: "; std::cin >> pass;
    std::cin.ignore();  // clears the \n, allowing getline to work properly
    std::cout << "Display Name: "; std::getline(std::cin, dname);
    
    // fetch the current system time
    std::time_t now = std::time(nullptr);
    char timeBuf[80];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    
    // generating ID for each user
    std::string id = "U" + std::to_string(nextUserNum++);
    users.push_back(User(id, uname, pass, Profile(dname, "No bio", std::string(timeBuf))));
    
    saveData();
    std::cout << "Registration successful!\n";
}

void SocialNetwork::loginUser() {
    std::string uname, pass;
    std::cout << "Username: "; std::cin >> uname;
    std::cout << "Password: "; std::cin >> pass;

    int idx = findUserIndex(uname);
    if (idx != -1 && users[idx].verifyPassword(pass)) {
        currentUserIndex = idx; 
        std::cout << "Login success!\n";
        return;
    }
    std::cout << "Invalid username or password.\n";
}

void SocialNetwork::createPost() {
    std::string content;
    std::cin.ignore();
    std::cout << "Enter post content: ";
    std::getline(std::cin, content);
    
    if (content.empty()) {
        std::cout << "Error: Post content cannot be empty.\n";
        return;
    }
    
    std::time_t now = std::time(nullptr);
    char timeBuf[80];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    
    std::string id = "P" + std::to_string(nextPostNum++);
    posts.push_back(Post(id, users[currentUserIndex].getUsername(), content, std::string(timeBuf)));
    
    saveData();
    std::cout << "Post created!\n";
}

void SocialNetwork::viewProfile() {
    std::string uname;
    std::cout << "Search username (Leave blank for yours): ";
    std::cin.ignore();
    std::getline(std::cin, uname);

    if (uname.empty()) { // viewing the current user's profile
        uname = users[currentUserIndex].getUsername();
    }
    
    // index validation
    int idx = findUserIndex(uname);
    if (idx == -1) {
        std::cout << "This user does not exist.\n";
        return;
    }

    User& u = users[idx];
    std::cout << "\n--------------------------------------\n";
    std::cout << "User: @" << u.getUsername() << " [" << u.getProfile().getDisplayName() << "]\n";
    std::cout << "Biography: " << u.getProfile().getBio() << "\n";
    std::cout << "Member since: " << u.getProfile().getJoinDate() << "\n";
    std::cout << "Stats -> Followers: " << u.getFollowers().size() << " | Following: " << u.getFollowing().size() << "\n";
    std::cout << "--------------------------------------\n";

    // loops through all posts and displays the user's posts
    int counter = 0;
    for (size_t i = 0; i < posts.size(); i++) {
        if (posts[i].getAuthorUsername() == u.getUsername()) {
            posts[i].display(getCommentCountForPost(posts[i].getId()));
            counter++;
        }
    }
    std::cout << "Total Feed Count: " << counter << "\n";
}

void SocialNetwork::manageRelationships() {
    std::cout << "1. Follow someone\n2. Unfollow someone\n3. Show connections\nChoice: ";
    int choice; std::cin >> choice;
    
    // select a user to follow or unfollow
    if (choice == 1 || choice == 2) {
        std::string target;
        std::cout << "Username: "; std::cin >> target;
        
        // users can't follow or unfollow themselves
        if (target == users[currentUserIndex].getUsername()) {
            std::cout << "Operation rejected: Cannot target yourself.\n";
            return;
        }

        // index validation
        int tIdx = findUserIndex(target);
        if (tIdx == -1) {
            std::cout << "Target account not found.\n";
            return;
        }

        // follow the selected user
        if (choice == 1) {
            if (users[currentUserIndex].followUser(target)) { // follows the user and returns true
                users[tIdx].addFollower(users[currentUserIndex].getUsername());
                std::cout << "Following @" << target << "\n";
            } else { // returns false if user already follows target
                std::cout << "You already follow them.\n";
            }
        } 
        // unfollow the selected user
        else {
            if (users[currentUserIndex].unfollowUser(target)) { // same logic for unfollowing
                users[tIdx].removeFollower(users[currentUserIndex].getUsername());
                std::cout << "Unfollowed @" << target << "\n";
            } else {
                std::cout << "You don't follow them.\n";
            }
        }
        saveData();
    } 
    // displays the lists of followers and followings
    else if (choice == 3) {
        std::cout << "\n[Following List]\n";
        for (const auto& n : users[currentUserIndex].getFollowing()) std::cout << " - @" << n << "\n";
        std::cout << "\n[Followers List]\n";
        for (const auto& n : users[currentUserIndex].getFollowers()) std::cout << " - @" << n << "\n";
    }
}

void SocialNetwork::viewHomeFeed() {
    std::cout << "\n--- Feed Display Modes ---\n";
    std::cout << "1. Sort by Timestamp (Newest First)\n2. Sort by Popularity (Most Liked First)\nChoice: ";
    int feedChoice; std::cin >> feedChoice;

    std::vector<Post> feedPosts;
    const auto& myFollowings = users[currentUserIndex].getFollowing();

    // collects all relevant timeline posts
    for (size_t i = 0; i < posts.size(); i++) {
        std::string author = posts[i].getAuthorUsername();
        if (std::find(myFollowings.begin(), myFollowings.end(), author) != myFollowings.end() ||
            author == users[currentUserIndex].getUsername()) {
            feedPosts.push_back(posts[i]);
        }
    }

    if (feedPosts.empty()) {
        std::cout << "No updates available.\n";
        return;
    }

    // handles feed sorting
    if (feedChoice == 1) {
        std::reverse(feedPosts.begin(), feedPosts.end());
    } else if (feedChoice == 2) {
        std::sort(feedPosts.begin(), feedPosts.end(), [](const Post& a, const Post& b) {
            return a.getLikesCount() > b.getLikesCount();
        });
    }

    std::cout << "\n--- Home Feed ---\n";
    for (const auto& p : feedPosts) {
        p.display(getCommentCountForPost(p.getId()));
    }
}

void SocialNetwork::interactWithPost() {
    std::string pId;
    std::cout << "Target Post ID: "; std::cin >> pId;
    
    // index validation
    int pIdx = findPostIndex(pId);
    if (pIdx == -1) {
        std::cout << "No posts were found.\n";
        return;
    }

    std::cout << "1. Toggle Like\n2. Add a Comment\n3. Show Details & Comments Thread\n4. Save Post to Bookmarks\nChoice: ";
    int choice; std::cin >> choice;

    // toggles like on the selected post
    if (choice == 1) {
        // returns true if user have not liked this post. if user liked this post before, removes the like and returns false
        bool res = posts[pIdx].toggleLike(users[currentUserIndex].getUsername());  
        if (res) std::cout << "Liked!\n";
        else std::cout << "Like removed.\n";
        saveData();
    } 
    // leave a comment on the selected post
    else if (choice == 2) {
        std::string txt;
        std::cin.ignore();
        std::cout << "Write your text: "; std::getline(std::cin, txt);
        
        if (txt.empty()) return;

        std::time_t now = std::time(nullptr);
        char timeBuf[80];
        std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

        comments.push_back(Comment(pId, users[currentUserIndex].getUsername(), txt, std::string(timeBuf)));
        saveData();
        std::cout << "Comment posted.\n";
    } 
    // displays the comments of the selected post
    else if (choice == 3) {
        posts[pIdx].display(getCommentCountForPost(pId));
        std::cout << "\n--- Liked By Users ---\n";
        for(const auto& name : posts[pIdx].getLikedBy()) std::cout << " - @" << name << "\n";

        std::cout << "\n--- Thread [" << pId << "] ---\n";
        for (size_t i = 0; i < comments.size(); i++) {
            if (comments[i].getPostId() == pId) {
                std::cout << "@" << comments[i].getAuthor() << ": " << comments[i].getText() << " (" << comments[i].getTimestamp() << ")\n";
            }
        }
    }
    // handles bookmarked saved posts
    else if (choice == 4) {
        if (users[currentUserIndex].savePost(pId)) {
            std::cout << "Post bookmarked successfully.\n";
            saveData();
        } else {
            std::cout << "This post is already in your saved list.\n";
        }
    }
}

void SocialNetwork::searchSystem() {
    std::cout << "1. Find Users by Username\n2. Find Users by Display Name\n3. Filter Posts by Keyword\n4. Filter Posts by Author Username\nChoice: ";
    int choice; std::cin >> choice;
    std::cin.ignore();

    std::string term;
    std::cout << "Search term: "; std::getline(std::cin, term);
    std::cout << "\nMatches:\n";

    if (choice == 1) {
        for (const auto& u : users) {
            if (u.getUsername().find(term) != std::string::npos) { // .find() method returns npos if it fails
                std::cout << " -> @" << u.getUsername() << " [" << u.getProfile().getDisplayName() << "]\n";
            }
        }
    } else if (choice == 2) {
        for (const auto& u : users) {
            if (u.getProfile().getDisplayName().find(term) != std::string::npos) {
                std::cout << " -> @" << u.getUsername() << " [" << u.getProfile().getDisplayName() << "]\n";
            }
        }
    } else if (choice == 3) {
        for (size_t i = 0; i < posts.size(); i++) {
            if (posts[i].getContent().find(term) != std::string::npos) {
                posts[i].display(getCommentCountForPost(posts[i].getId()));
            }
        }
    } else if (choice == 4) {
        for (size_t i = 0; i < posts.size(); i++) {
            if (posts[i].getAuthorUsername() == term) {
                posts[i].display(getCommentCountForPost(posts[i].getId()));
            }
        }
    }
}

void SocialNetwork::editProfile() {
    std::string newDName, newBio;
    std::cin.ignore();
    std::cout << "New Display Name: "; std::getline(std::cin, newDName);
    std::cout << "New Bio: "; std::getline(std::cin, newBio);
    
    // updating profile info
    users[currentUserIndex].getProfile().setDisplayName(newDName);
    users[currentUserIndex].getProfile().setBio(newBio);
    
    saveData();
    std::cout << "Profile updated.\n";
}

// ======================== Phase 3 Member Function Implementations ========================

void SocialNetwork::manageMyPosts() {
    std::cout << "\n--- Your Published Posts ---\n";
    std::string currentUname = users[currentUserIndex].getUsername();
    std::vector<std::string> myPostIds;

    for (size_t i = 0; i < posts.size(); i++) {
        if (posts[i].getAuthorUsername() == currentUname) {
            posts[i].display(getCommentCountForPost(posts[i].getId()));
            myPostIds.push_back(posts[i].getId());
        }
    }

    if (myPostIds.empty()) {
        std::cout << "You haven't posted anything yet.\n";
        return;
    }

    std::cout << "1. Edit Content of a Post\n2. Permanently Delete a Post\n3. Back\nChoice: ";
    int choice; std::cin >> choice;

    if (choice == 1 || choice == 2) {
        std::string targetId;
        std::cout << "Enter Post ID: "; std::cin >> targetId;

        // check if the post belongs to the user
        if (std::find(myPostIds.begin(), myPostIds.end(), targetId) == myPostIds.end()) {
            std::cout << "Access Denied: Invalid Post ID or unauthorized action.\n";
            return;
        }

        int pIdx = findPostIndex(targetId);
        if (choice == 1) {
            std::string newText;
            std::cin.ignore();
            std::cout << "Enter new content: "; std::getline(std::cin, newText);
            if (!newText.empty()) {
                posts[pIdx].editContent(newText);
                std::cout << "Post modified successfully.\n";
            }
        } 
        else {
            // delete post and its comments
            posts.erase(posts.begin() + pIdx);

            // delete comments of this post
            for (auto it = comments.begin(); it != comments.end(); ) {
                if (it->getPostId() == targetId) {
                    it = comments.erase(it);
                } else {
                    ++it;
                }
            }

            // remove post from all users bookmarks
            for (size_t i = 0; i < users.size(); i++) {
                users[i].unsavePost(targetId);
            }

            std::cout << "Post and its dependencies cleaned up securely.\n";
        }
        saveData();
    }
}

void SocialNetwork::manageSavedPosts() {
    const auto& savedIds = users[currentUserIndex].getSavedPostIds();
    std::cout << "\n--- Bookmarked Posts ---\n";
    
    if (savedIds.empty()) {
        std::cout << "Your saved bookmarks list is empty.\n";
        return;
    }

    for (const auto& id : savedIds) {
        int pIdx = findPostIndex(id);
        if (pIdx != -1) {
            posts[pIdx].display(getCommentCountForPost(id));
        }
    }

    std::cout << "1. Remove a Post from Saved Bookmarks\n2. Back\nChoice: ";
    int choice; std::cin >> choice;
    if (choice == 1) {
        std::string targetId;
        std::cout << "Enter Post ID to unsave: "; std::cin >> targetId;
        if (users[currentUserIndex].unsavePost(targetId)) {
            std::cout << "Removed from bookmarks.\n";
            saveData();
        } else {
            std::cout << "Post ID not found in saved list.\n";
        }
    }
}

void SocialNetwork::deleteCurrentUser() {
    std::cout << "WARNING: Are you sure you want to permanently delete your account? (1 = Yes, 0 = No): ";
    int confirm; std::cin >> confirm;
    if (confirm != 1) return;

    std::string targetUname = users[currentUserIndex].getUsername();

    // delete all posts of this user
    for (auto it = posts.begin(); it != posts.end(); ) {
        if (it->getAuthorUsername() == targetUname) {
            std::string pId = it->getId();
            it = posts.erase(it);
            
            // delete comments of these posts
            for (auto cit = comments.begin(); cit != comments.end(); ) {
                if (cit->getPostId() == pId) cit = comments.erase(cit);
                else ++cit;
            }
        } else {
            // remove user's likes from other posts
            it->toggleLike(targetUname);
            ++it;
        }
    }

    // delete comments made by this user
    for (auto it = comments.begin(); it != comments.end(); ) {
        if (it->getAuthor() == targetUname) {
            it = comments.erase(it);
        } else {
            ++it;
        }
    }

    // clean followers, followings, and bookmarks
    for (size_t i = 0; i < users.size(); i++) {
        users[i].unfollowUser(targetUname);
        users[i].removeFollower(targetUname);
        users[i].unsavePost(targetUname);
    }

   // remove user from users vector
    users.erase(users.begin() + currentUserIndex);
    currentUserIndex = -1; // log out current user
    saveData();
    std::cout << "Account and records wiped out completely.\n";
}