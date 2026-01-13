#pragma once
#include <string>
#include <vector>
#include "UserManager.h"
#include "FriendManager.h"
#include "PostManager.h"
#include "RequestManager.h"

class FakeDataGenerator {
public:
    FakeDataGenerator();

    void generateAll(int numUsers = 25, int numPosts = 80);

    void generateUsers(int count, UserManager& um);
    void generateFriendships(UserManager& um, FriendManager& fm, int avgFriendsPerUser = 6);
    void generatePosts(UserManager& um, PostManager& pm, int count);
    void generateRequests(UserManager& um, RequestManager& rm, FriendManager& fm, int count);

    void clearAllFiles();

private:
    std::vector<std::string> firstNames;
    std::vector<std::string> lastNames;
    std::vector<std::string> locations;
    std::vector<std::string> postTexts;
    std::vector<std::string> genders;

    void initializeData();
    std::string getRandomElement(const std::vector<std::string>& vec);
    int getRandomNumber(int min, int max);
    bool coinFlip();
};