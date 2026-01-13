#pragma once
#include <vector>
#include <string>
#include "FriendNode.h"
#include "User.h"
using namespace std;
class FriendManager {
public:
    FriendManager();
    ~FriendManager();

    void initForUsers(int maxUserId);

    bool areFriends(int userId, int friendId) const;

    bool addFriendMutual(int userA, int userB);     // called when request accepted


    bool removeFriendMutual(int userA, int userB);

    vector<int> getFriendsList(int userId) const;

    void loadFromFile(const string& filename);
    void saveToFile(const string& filename);

private:
    vector<FriendNode*> heads;
    int maxUserId;

    void addToList(int userId, int friendId);
    bool removeFromList(int userId, int friendId);
    void clearList(int userId);
    bool isInList(int userId, int friendId) const;
};