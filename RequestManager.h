#pragma once
#include <vector>
#include <string>
#include "Request.h"
using namespace std;
class FriendManager;

class RequestManager {
public:
    RequestManager();

    void sendRequest(int fromUserId, int toUserId);

    vector<Request> getPendingForUser(int userId) const;

    vector<Request> getSentByUser(int userId) const;

    void respondToRequest(int fromUserId, int toUserId, bool accept, FriendManager& fm);

    void loadFromFile(const string& filename);
    void saveToFile(const string& filename);

private:
    vector<Request> requests;

    int findRequestIndex(int fromUserId, int toUserId) const;
};