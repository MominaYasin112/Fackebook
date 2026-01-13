#include "UserManager.h"
#include <fstream>
#include <iostream>

using namespace std;

UserManager::UserManager() : maxId(0) {}

int UserManager::registerUser(const User& u) {
    if (getUserByEmail(u.email) != nullptr) {
        return 0;
    }

    User newUser = u;
    newUser.id = getNextUserId();
    newUser.createdAt = time(nullptr);

    users.push_back(newUser);
    maxId = newUser.id;
    ofstream file("allusers.txt", ios::app);
    if (file.is_open()) {
        file << newUser.id << "#" << newUser.username << "#"
            << newUser.email << "#" << newUser.password << "#"
            << newUser.location << "#" << newUser.gender << "#"
            << newUser.age << "#" << (newUser.isPublic ? 1 : 0) << "#"
            << newUser.createdAt << "\n";
        file.close();
    }

    return newUser.id;
}

User* UserManager::login(const string& email, const string& password) {
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].email == email && users[i].password == password) {
            return &users[i];
        }
    }
    return nullptr;
}

User* UserManager::getUserById(int id) {
    int index = findUserIndexById(id);
    if (index >= 0) {
        return &users[index];
    }
    return nullptr;
}

User* UserManager::getUserByEmail(const string& email) {
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].email == email) {
            return &users[i];
        }
    }
    return nullptr;
}
User* UserManager::getUserByUsername(const string& username) {
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].username == username) {
            return &users[i];
        }
    }
    return nullptr;
}
int UserManager::findUserIndexById(int id) const {
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].id == id) {
            return i;
        }
    }
    return -1;
}

void UserManager::loadFromFile(const string& filename) {
    users.clear();
    maxId = 0;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "No existing users file found.\n";
        return;
    }
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        User u;
        int pos = 0;
        int prev = 0;
        string fields[9];
        int fieldIndex = 0;
        while ((pos = line.find('#', prev)) != string::npos && fieldIndex < 8) {
            fields[fieldIndex++] = line.substr(prev, pos - prev);
            prev = pos + 1;
        }
        if (fieldIndex < 9) {
            fields[fieldIndex++] = line.substr(prev);
        }
        if (fieldIndex == 9) {
            u.id = stoi(fields[0]);
            u.username = fields[1];
            u.email = fields[2];
            u.password = fields[3];
            u.location = fields[4];
            u.gender = fields[5];
            u.age = stoi(fields[6]);
            if (fields[7] == "1")
                u.isPublic = true;
            else
                u.isPublic = false;
            u.createdAt = stoll(fields[8]);
            users.push_back(u);
            if (u.id > maxId) maxId = u.id;
        }
    }

    file.close();
    cout << "Loaded " << users.size() << " users.\n";
}


void UserManager::saveToFile(const std::string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot save users file.\n";
        return;
    }

    for (const auto& u : users) {
        file << u.id << "#" << u.username << "#"
            << u.email << "#" << u.password << "#"
            << u.location << "#" << u.gender << "#"
            << u.age << "#" << (u.isPublic ? 1 : 0) << "#"
            << u.createdAt << "\n";
    }
    file.close();
}