#include "FriendManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

FriendManager::FriendManager() : maxUserId(0) {}

FriendManager::~FriendManager() {
    for (auto i = 0; i < heads.size(); i++) {
        clearList(i);
    }
}

void FriendManager::initForUsers(int maxId)
{
    maxUserId = maxId;
    heads.resize(maxUserId + 1, nullptr); // Index 0 is unused, 1 to maxUserId is used
}

bool FriendManager::areFriends(int userId, int friendId) const {
    if (userId <= 0 || userId >= (int)heads.size()) return false;
    if (friendId <= 0 || friendId >= (int)heads.size()) return false;
    return isInList(userId, friendId);
}

bool FriendManager::addFriendMutual(int userA, int userB) {
    if (userA < 1 || userA > maxUserId || userB < 1 || userB > maxUserId) {
        return false;
    }
    if (userA == userB) return false;
    if (areFriends(userA, userB)) return false;

    addToList(userA, userB);
    addToList(userB, userA);
    saveToFile("Friends.txt");
    return true;
}

bool FriendManager::removeFriendMutual(int userA, int userB) {
    if (userA < 1 || userA > maxUserId || userB < 1 || userB > maxUserId) {
        return false;
    }

    bool removed1 = removeFromList(userA, userB);
    bool removed2 = removeFromList(userB, userA);
    if (removed1 && removed2) saveToFile("Friends.txt");
    return removed1 && removed2;
}

vector<int> FriendManager::getFriendsList(int userId) const {
    vector<int> friends;
    if (userId < 1 || userId > maxUserId) return friends;

    FriendNode* current = heads[userId];
    while (current != nullptr) {
        friends.push_back(current->friendId);
        current = current->next;
    }
    return friends;
}

void FriendManager::addToList(int userId, int friendId) {
    if (userId < 1 || userId > maxUserId) return;

    if (isInList(userId, friendId)) return;

    FriendNode* newNode = new FriendNode(friendId);
    newNode->next = heads[userId];
    heads[userId] = newNode;
}

bool FriendManager::removeFromList(int userId, int friendId) {
    if (userId < 1 || userId > maxUserId) return false;

    FriendNode* current = heads[userId];
    FriendNode* previous = nullptr;

    while (current != nullptr) {
        if (current->friendId == friendId) {
            if (previous == nullptr) {
                heads[userId] = current->next;
            }
            else {
                previous->next = current->next;
            }
            delete current;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

void FriendManager::clearList(int userId) {
    if (userId < 0 || userId >= (int)heads.size()) return;

    FriendNode* current = heads[userId];
    while (current != nullptr) {
        FriendNode* temp = current;
        current = current->next;
        delete temp;
    }
    heads[userId] = nullptr;
}

bool FriendManager::isInList(int userId, int friendId) const {
    if (userId < 1 || userId > maxUserId) return false;
    if (friendId < 1 || friendId > maxUserId) return false;

    FriendNode* current = heads[userId];
    while (current != nullptr) {
        if (current->friendId == friendId) return true;
        current = current->next;
    }
    return false;
}

void FriendManager::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open friends file.\n";
        return;
    }

    string line;
    int friendshipsLoaded = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;

        // Format: userId:friendId1,friendId2,...
        auto colonPos = line.find(':');
        if (colonPos == string::npos) continue;

        int userId = stoi(line.substr(0, colonPos));
        string friendsStr = line.substr(colonPos + 1);

        if (friendsStr.empty()) continue;

        stringstream ss(friendsStr);
        string friendIdStr;

        while (getline(ss, friendIdStr, ',')) {
            if (!friendIdStr.empty()) {
                int friendId = stoi(friendIdStr);
                addToList(userId, friendId);
                friendshipsLoaded++;
            }
        }
    }
    file.close();
    cout << "Loaded " << friendshipsLoaded << " friendships.\n";
}

void FriendManager::saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot save friends file.\n";
        return;
    }

    for (int userId = 1; userId <= maxUserId; userId++) {
        file << userId << ":";

        FriendNode* current = heads[userId];
        bool first = true;
        while (current != nullptr) {
            if (!first) file << ",";
            file << current->friendId;
            first = false;
            current = current->next;
        }
        file << endl;
    }
    file.close();
}