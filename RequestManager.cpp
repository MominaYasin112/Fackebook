#include "RequestManager.h"
#include "FriendManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

RequestManager::RequestManager() {}

void RequestManager::sendRequest(int fromUserId, int toUserId) {
    int existing = findRequestIndex(fromUserId, toUserId);
    if (existing >= 0) {
        cout << "Request already exists!\n";
        return;
    }

    Request req;
    req.fromUserId = fromUserId;
    req.toUserId = toUserId;
    req.timestamp = time(nullptr);
    req.status = "PENDING";

    requests.push_back(req);

    ofstream file("FriendRequests.txt", ios::app);
    if (file.is_open()) {
        file << req.fromUserId << "#" << req.toUserId << "#"
            << req.timestamp << "#" << req.status << "\n";
        file.close();
    }
}

vector<Request> RequestManager::getPendingForUser(int userId) const {
    vector<Request> pending;
    for (const auto& req : requests) {
        if (req.toUserId == userId && req.status == "PENDING") {
            pending.push_back(req);
        }
    }
    return pending;
}

vector<Request> RequestManager::getSentByUser(int userId) const {
    vector<Request> sent;
    for (const auto& req : requests) {
        if (req.fromUserId == userId && req.status == "PENDING") {
            sent.push_back(req);
        }
    }
    return sent;
}

void RequestManager::respondToRequest(int fromUserId, int toUserId, bool accept, FriendManager& fm) {
    int index = findRequestIndex(fromUserId, toUserId);
    if (index < 0) {
        cout << "Request not found!\n";
        return;
    }

    if (requests[index].status != "PENDING") {
        cout << "Request already processed!\n";
        return;
    }

    if (accept) {
        requests[index].status = "ACCEPTED";
        fm.addFriendMutual(fromUserId, toUserId);
        cout << "Friend request accepted!\n";
    }
    else {
        cout << "Friend request declined.\n";
        requests.erase(requests.begin() + index);
    }
    saveToFile("FriendRequests.txt");

}

int RequestManager::findRequestIndex(int fromUserId, int toUserId) const {
    for (auto i = 0; i < requests.size(); i++) {
        if (requests[i].fromUserId == fromUserId && requests[i].toUserId == toUserId) {
            return i;
        }
    }
    return -1;
}

void RequestManager::loadFromFile(const string& filename) {
    requests.clear();

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open requests file.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        Request req;
        string temp;

        // Format: fromUserId#toUserId#timestamp#status
        getline(ss, temp, '#'); req.fromUserId = stoi(temp);
        getline(ss, temp, '#'); req.toUserId = stoi(temp);
        getline(ss, temp, '#'); req.timestamp = stoll(temp);
        getline(ss, req.status);

        requests.push_back(req);
    }
    file.close();
    cout << "Loaded " << requests.size() << " friend requests.\n";
}

void RequestManager::saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot save requests file.\n";
        return;
    }

    for (const auto& req : requests) {
        file << req.fromUserId << "#" << req.toUserId << "#"
            << req.timestamp << "#" << req.status << "\n";
    }
    file.close();
}