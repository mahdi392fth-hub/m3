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

bool SocialNetwork::isUsernameTaken(const std::string& uname) const {
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].getUsername() == uname) return true;
    }
    return false;
}

int SocialNetwork::findUserIndex(const std::string& uname) {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].getUsername() == uname) return (int)i;
    }
    return -1;
}

int SocialNetwork::findPostIndex(const std::string& postId) {
    for (size_t i = 0; i < posts.size(); ++i) {
        if (posts[i].getId() == postId) return (int)i;
    }
    return -1;
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
    std::ifstream uFile("users.txt");
    std::string line;
    
    // Read users
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
        
        users.push_back(User(id, uname, hash, Profile(dname, bio, date), true));
        
        // Update counter based on numeric part of ID (e.g. "U5" -> 5)
        int num = std::stoi(id.substr(1));
        if (num >= nextUserNum) nextUserNum = num + 1;
    }
    uFile.close();
    
    // Read posts
    std::ifstream pFile("posts.txt");
    while (std::getline(pFile, line)) {
        if(line.empty()) continue;
        std::stringstream ss(line);
        std::string id, author, content, time;
        
        std::getline(ss, id, '|');
        std::getline(ss, author, '|');
        std::getline(ss, content, '|');
        std::getline(ss, time, '|');
        
        posts.push_back(Post(id, author, content, time));
        
        int num = std::stoi(id.substr(1));
        if (num >= nextPostNum) nextPostNum = num + 1;
    }
    pFile.close();

    // Read relations (Using a simpler while direct extraction to break symmetry)
    std::ifstream fFile("follows.txt");
    std::string follower, followed;
    while (fFile >> follower) {
        // parsing manually with clean checks
        size_t pos = follower.find('|');
        if(pos != std::string::npos) {
            followed = follower.substr(pos + 1);
            follower = follower.substr(0, pos);
            
            int idx1 = findUserIndex(follower);
            int idx2 = findUserIndex(followed);
            if (idx1 != -1 && idx2 != -1) {
                users[idx1].followUser(followed);
                users[idx2].addFollower(follower);
            }
        }
    }
    fFile.close();

    // Read likes
    std::ifstream lFile("likes.txt");
    std::string pId, uName;
    while (std::getline(lFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::getline(ss, pId, '|');
        std::getline(ss, uName, '|');
        
        int pIdx = findPostIndex(pId);
        if (pIdx != -1) posts[pIdx].toggleLike(uName);
    }
    lFile.close();

    // Read comments
    std::ifstream cFile("comments.txt");
    std::string cPostId, cAuth, cText, cTime;
    while (std::getline(cFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::getline(ss, cPostId, '|');
        std::getline(ss, cAuth, '|');
        std::getline(ss, cText, '|');
        std::getline(ss, cTime, '|');
        comments.push_back(Comment(cPostId, cAuth, cText, cTime));
    }
    cFile.close();
}

void SocialNetwork::saveData() {
    std::ofstream uFile("users.txt");
    for (size_t i = 0; i < users.size(); i++) {
        User& u = users[i];
        uFile << u.getId() << "|" << u.getUsername() << "|" << u.getPasswordHash() << "|"
              << u.getProfile().getDisplayName() << "|" << u.getProfile().getBio() << "|"
              << u.getProfile().getJoinDate() << "\n";
    }
    uFile.close();
    
    std::ofstream pFile("posts.txt");
    for (auto& p : posts) {
        pFile << p.getId() << "|" << p.getAuthorUsername() << "|" 
              << p.getContent() << "|" << p.getTimestamp() << "\n";
    }
    pFile.close();

    std::ofstream fFile("follows.txt");
    for (size_t i = 0; i < users.size(); i++) {
        for (const auto& f : users[i].getFollowing()) {
            fFile << users[i].getUsername() << "|" << f << "\n";
        }
    }
    fFile.close();

    std::ofstream lFile("likes.txt");
    for (size_t i = 0; i < posts.size(); i++) {
        for (const auto& name : posts[i].getLikedBy()) {
            lFile << posts[i].getId() << "|" << name << "\n";
        }
    }
    lFile.close();

    std::ofstream cFile("comments.txt");
    for (const auto& c : comments) {
        cFile << c.getPostId() << "|" << c.getAuthor() << "|" 
              << c.getText() << "|" << c.getTimestamp() << "\n";
    }
    cFile.close();
}

void SocialNetwork::start() {
    int choice;
    while (true) {
        if (currentUserIndex == -1) { 
            std::cout << "\n* Welcome to MinSocial *\n1. Register Account\n2. Login\n3. Exit\nChoice: ";
            if (!(std::cin >> choice)) { // Input validation fix to prevent infinite loop bugs
                std::cin.clear();
                std::string junk; std::cin >> junk;
                std::cout << "Please enter a valid number.\n";
                continue;
            }

            if (choice == 1) registerUser();
            else if (choice == 2) loginUser();
            else if (choice == 3) break;
            else std::cout << "Option not found!\n";
        } 
        else {
            std::cout << "\n--- Dashboard [@" << users[currentUserIndex].getUsername() << "] ---\n";
            std::cout << "1. Write a Post\n2. View Feed\n3. Search Profiles\n4. Follow/Unfollow Menu\n5. Post Interactions\n6. Edit My Profile\n7. Security Settings\n8. Logout\nChoice: ";
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::string junk; std::cin >> junk;
                std::cout << "Invalid input format.\n";
                continue;
            }

            if (choice == 1) createPost();
            else if (choice == 2) viewHomeFeed();
            else if (choice == 3) viewProfile();
            else if (choice == 4) manageRelationships();
            else if (choice == 5) interactWithPost();
            else if (choice == 6) editProfile();
            else if (choice == 7) {
                std::string newPass;
                std::cout << "Enter new password: "; std::cin >> newPass;
                users[currentUserIndex].changePassword(newPass);
                saveData();
                std::cout << "Password modified.\n";
            }
            else if (choice == 8) currentUserIndex = -1;
            else std::cout << "Unknown command.\n";
        }
    }
}

void SocialNetwork::registerUser() {
    std::string uname, pass, dname;
    std::cout << "Choose Username: "; std::cin >> uname;
    
    if (uname.empty() || isUsernameTaken(uname)) {
        std::cout << "Username is taken or empty!\n";
        return;
    }
    
    std::cout << "Choose Password: "; std::cin >> pass;
    std::cin.ignore(); 
    std::cout << "Enter Display Name: "; std::getline(std::cin, dname);
    
    std::time_t now = std::time(nullptr);
    char timeBuf[80];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    
    // Using independent counter variable
    std::string id = "U" + std::to_string(nextUserNum++);
    users.push_back(User(id, uname, pass, Profile(dname, "No bio", std::string(timeBuf))));
    
    saveData();
    std::cout << "Account created successfully.\n";
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
    std::cout << "Wrong username or password.\n";
}

void SocialNetwork::createPost() {
    std::string content;
    std::cin.ignore();
    std::cout << "What's on your mind? ";
    std::getline(std::cin, content);
    
    if (content.empty()) {
        std::cout << "Content cannot be blank.\n";
        return;
    }
    
    std::time_t now = std::time(nullptr);
    char timeBuf[80];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    
    // Using independent counter variable
    std::string id = "P" + std::to_string(nextPostNum++);
    posts.push_back(Post(id, users[currentUserIndex].getUsername(), content, std::string(timeBuf)));
    
    saveData();
    std::cout << "Post shared!\n";
}

void SocialNetwork::viewProfile() {
    std::string uname;
    std::cout << "Search username (Leave blank for yours): ";
    std::cin.ignore();
    std::getline(std::cin, uname);

    if (uname.empty()) {
        uname = users[currentUserIndex].getUsername();
    }

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
    
    if (choice == 1 || choice == 2) {
        std::string target;
        std::cout << "Username: "; std::cin >> target;
        
        if (target == users[currentUserIndex].getUsername()) {
            std::cout << "Operation rejected: Cannot target yourself.\n";
            return;
        }

        int tIdx = findUserIndex(target);
        if (tIdx == -1) {
            std::cout << "Target account not found.\n";
            return;
        }

        if (choice == 1) {
            if (users[currentUserIndex].followUser(target)) {
                users[tIdx].addFollower(users[currentUserIndex].getUsername());
                std::cout << "Following @" << target << "\n";
            } else {
                std::cout << "You already follow them.\n";
            }
        } else {
            if (users[currentUserIndex].unfollowUser(target)) {
                users[tIdx].removeFollower(users[currentUserIndex].getUsername());
                std::cout << "Unfollowed @" << target << "\n";
            } else {
                std::cout << "You don't follow them.\n";
            }
        }
        saveData();
    } else if (choice == 3) {
        std::cout << "\n[Following List]\n";
        for (const auto& n : users[currentUserIndex].getFollowing()) std::cout << " - @" << n << "\n";
        std::cout << "\n[Followers List]\n";
        for (const auto& n : users[currentUserIndex].getFollowers()) std::cout << " - @" << n << "\n";
    }
}

void SocialNetwork::viewHomeFeed() {
    std::cout << "\n--- HOME FEED ---\n";
    const auto& myFollowings = users[currentUserIndex].getFollowing();
    bool hasData = false;

    // Standard human loop iteration instead of complex reverse iterators everywhere
    for (int i = (int)posts.size() - 1; i >= 0; i--) {
        std::string author = posts[i].getAuthorUsername();
        if (std::find(myFollowings.begin(), myFollowings.end(), author) != myFollowings.end() ||
            author == users[currentUserIndex].getUsername()) {
            posts[i].display(getCommentCountForPost(posts[i].getId()));
            hasData = true;
        }
    }
    if (!hasData) {
        std::cout << "No updates available.\n";
    }
}

void SocialNetwork::interactWithPost() {
    std::string pId;
    std::cout << "Target Post ID: "; std::cin >> pId;
    
    int pIdx = findPostIndex(pId);
    if (pIdx == -1) {
        std::cout << "Post lookup failed.\n";
        return;
    }

    std::cout << "1. Toggle Like\n2. Add a Comment\n3. Show Comments\nChoice: ";
    int choice; std::cin >> choice;

    if (choice == 1) {
        bool res = posts[pIdx].toggleLike(users[currentUserIndex].getUsername());
        if (res) std::cout << "Liked!\n";
        else std::cout << "Like removed.\n";
        saveData();
    } else if (choice == 2) {
        std::string txt;
        std::cin.ignore();
        std::cout << "Write your text: "; std::getline(std::cin, txt);
        
        if (txt.empty()) return;

        std::time_t now = std::time(nullptr);
        char timeBuf[80];
        std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

        comments.push_back(Comment(pId, users[currentUserIndex].getUsername(), txt, std::string(timeBuf)));
        saveData();
        std::cout << "Comment registered.\n";
    } else if (choice == 3) {
        std::cout << "\n--- Thread [" << pId << "] ---\n";
        for (size_t i = 0; i < comments.size(); i++) {
            if (comments[i].getPostId() == pId) {
                std::cout << "@" << comments[i].getAuthor() << ": " << comments[i].getText() << " (" << comments[i].getTimestamp() << ")\n";
            }
        }
    }
}

void SocialNetwork::searchSystem() {
    std::cout << "1. Find Users\n2. Filter Posts\nChoice: ";
    int choice; std::cin >> choice;
    std::cin.ignore();

    if (choice == 1) {
        std::string term;
        std::cout << "Search term: "; std::getline(std::cin, term);
        std::cout << "\nMatches:\n";
        for (const auto& u : users) {
            if (u.getUsername().find(term) != std::string::npos) {
                std::cout << " -> @" << u.getUsername() << " [" << u.getProfile().getDisplayName() << "]\n";
            }
        }
    } else if (choice == 2) {
        std::string term;
        std::cout << "Keyword: "; std::getline(std::cin, term);
        std::cout << "\nMatches:\n";
        for (size_t i = 0; i < posts.size(); i++) {
            if (posts[i].getContent().find(term) != std::string::npos) {
                posts[i].display(getCommentCountForPost(posts[i].getId()));
            }
        }
    }
}

void SocialNetwork::editProfile() {
    std::string name, bio;
    std::cin.ignore();
    std::cout << "Modify Display Name: "; std::getline(std::cin, name);
    std::cout << "Modify Bio: "; std::getline(std::cin, bio);
    
    users[currentUserIndex].getProfile().setDisplayName(name);
    users[currentUserIndex].getProfile().setBio(bio);
    
    saveData();
    std::cout << "Changes updated.\n";
}