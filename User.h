#pragma once
#include <string>
#include <ctime>
using namespace std;

struct User {
    int id;
    string username;
    string email;
    string password;
    string location;
    string gender;
    int age;
    bool isPublic;
    time_t createdAt;

    User() : id(0), age(0), isPublic(true), createdAt(0) {}
};