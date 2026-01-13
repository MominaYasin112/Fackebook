#pragma once
#include <vector>
#include <string>
#include "User.h"
using namespace std;
class UserManager {
public:
    UserManager();
    int registerUser(const User& u);
    User* login(const string& email, const string& password);
    User* getUserById(int id);
    User* getUserByEmail(const string& email);
    User* getUserByUsername(const string& username);
    const vector<User>& getAllUsers() const { return users; }
    void loadFromFile(const string& filename);
    void saveToFile(const string& filename);
    int getNextUserId() const { return maxId + 1; }

private:
    vector<User> users;
    int maxId;
    int findUserIndexById(int id) const;
};