#pragma once
#include <vector>
#include <string>
#include <queue>
#include "Post.h"
#include "User.h"
#include<iostream>
using namespace std;
class FriendManager;
class UserManager;

class PostManager 
{
    vector<Post> posts;
    int maxPostId;
    vector<Post> feedCache;
    int currentFeedIndex;
    bool contains(const vector<int>& vec, int value) const;
public:
    PostManager();
    int createPost(int authorId, const string& text, bool isPublic);
    vector<Post> getPostsByAuthor(int authorId) const;
    void buildFeedHeap(int userId, const FriendManager& fm, const UserManager& um);
    vector<Post> getNextPosts(int count);
    vector<Post> getPreviousPosts(int count);
    bool hasMorePosts() const;
    bool hasPreviousPosts() const;
    void resetFeedPosition();
    void loadFromFile(const string& filename);
    void saveToFile(const string& filename);
    const vector<Post>& getAllPosts() const
    {
        return posts; 
    }
};