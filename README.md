# A Social Network by Team MMA

This is a simple text-based social network we built in C++ for our university project. Users can make accounts, log in, and share thoughts on a public feed.

## What it does
* **Accounts:** You can register a new username and log in.
* **The Feed:** Once you're in, you can write your own posts and view everything other users have posted.
* **Save:** Every time someone posts or makes an account, it gets saved directly to `users.txt` and `posts.txt`. When you run the app again, all your history loads right back up.

## How we built it
We focused on keeping the code organized:
* **Object-Oriented:** Everything is divided into clean classes (`User`, `Profile`, `Post`, `SocialNetwork`).
* **Memory Safe:** We used standard C++ tools like `std::vector` and `std::string` to handle our data safely.
* **File Structure:** The headers are in the `include/` folder, and the main logic is in the `src/` folder.

## How to run it
Open your terminal in the main project folder:
1. Type `make` or `mingw32-make` to compile the code.
2. Run the generated file `./SocialNetwork`.

## The Team
This was built as a three student project.
* Mahdi Fathi - Posts and Data Saving
* Mahdi Eslami - User and Profile Management
* Amirali Honarpisheh - Core System Logic