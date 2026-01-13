#pragma once
#include <string>
#include <ctime>
#include<iostream>
using namespace std;
struct Post {
    int postId;
    int authorId;
    string text;
    time_t timestamp;
    bool isPublic;

    Post() : postId(0), authorId(0), timestamp(0), isPublic(true) {}
    bool operator<(const Post& other) const 
    {
        return timestamp < other.timestamp;
    }

    bool operator>(const Post& other) const
    {
        return timestamp > other.timestamp;
    }
};