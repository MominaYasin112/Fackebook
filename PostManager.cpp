#include "PostManager.h"
#include "FriendManager.h"
#include "UserManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <queue>
using namespace std;

PostManager::PostManager() : maxPostId(0), currentFeedIndex(0) {}

int PostManager::createPost(int authorId, const string& text, bool isPublic) {
    Post p;
    p.postId = ++maxPostId;
    p.authorId = authorId;
    p.text = text;
    p.timestamp = time(nullptr);
    p.isPublic = isPublic;

    posts.push_back(p);
    ofstream file("posts.txt", ios::app);
    if (file.is_open()) 
    {
        string safeText = text;
        for (size_t i = 0; i < safeText.length(); i++) 
        {
            if (safeText[i] == '#') 
                safeText[i] = '|';
        }

        file << p.postId << "#" << p.authorId << "#" << safeText << "#"
            << p.timestamp << "#" << (p.isPublic ? "Public" : "Friends") << "\n";
        file.close();
    }

    return p.postId;
}

vector<Post> PostManager::getPostsByAuthor(int authorId) const
{
    vector<Post> result;
    for (const auto& p : posts) {
        if (p.authorId == authorId) {
            result.push_back(p);
        }
    }
    return result;
}

bool PostManager::contains(const std::vector<int>& vec, int value) const 
{
    for (size_t i = 0; i < vec.size(); i++)
    {
        if (vec[i] == value) 
            return true;
    }
    return false;
}

void PostManager::buildFeedHeap(int userId, const FriendManager& fm, const UserManager& um) 
{
    feedCache.clear();
    currentFeedIndex = 0;
    priority_queue<Post> maxHeap;
    vector<int> seenPostIds;
    vector<int> directFriends = fm.getFriendsList(userId);
    for (size_t i = 0; i < directFriends.size(); i++) 
    {
        int friendId = directFriends[i];
        vector<Post> friendPosts = getPostsByAuthor(friendId);
        for (size_t j = 0; j < friendPosts.size(); j++) 
        {
            if (!contains(seenPostIds, friendPosts[j].postId))
            {
                maxHeap.push(friendPosts[j]);
                seenPostIds.push_back(friendPosts[j].postId);
            }
        }
    }
    vector<int> fofIds;
    for (size_t i = 0; i < directFriends.size(); i++)
    {
        vector<int> friendsOfFriend = fm.getFriendsList(directFriends[i]);
        for (size_t j = 0; j < friendsOfFriend.size(); j++) 
        {
            int fofId = friendsOfFriend[j];
            if (fofId == userId) 
                continue;
            if (contains(directFriends, fofId)) 
                continue;
            if (contains(fofIds, fofId)) 
                continue;

            fofIds.push_back(fofId);
        }
    }
    for (size_t i = 0; i < fofIds.size(); i++)
    {
        vector<Post> postsOfFOF = getPostsByAuthor(fofIds[i]);

        for (size_t j = 0; j < postsOfFOF.size(); j++) 
        {
            if (postsOfFOF[j].isPublic && !contains(seenPostIds, postsOfFOF[j].postId)) 
            {
                maxHeap.push(postsOfFOF[j]);
                seenPostIds.push_back(postsOfFOF[j].postId);
            }
        }
    }
    while (!maxHeap.empty()) 
    {
        feedCache.push_back(maxHeap.top());
        maxHeap.pop();
    }
}

vector<Post> PostManager::getNextPosts(int count) 
{
    vector<Post> batch;
    int endIndex = currentFeedIndex + count;
    if (endIndex > (int)feedCache.size()) 
    {
        endIndex = feedCache.size();
    }

    for (int i = currentFeedIndex; i < endIndex; i++)
    {
        batch.push_back(feedCache[i]);
    }

    currentFeedIndex = endIndex;

    return batch;
}

vector<Post> PostManager::getPreviousPosts(int count)
{
    vector<Post> batch;
    int startIndex = currentFeedIndex - count;
    if (startIndex < 0) 
    {
        startIndex = 0;
    }
    for (int i = startIndex; i < currentFeedIndex; i++) 
    {
        batch.push_back(feedCache[i]);
    }

    currentFeedIndex = startIndex;

    return batch;
}

bool PostManager::hasMorePosts() const
{
    return currentFeedIndex < (int)feedCache.size();
}

bool PostManager::hasPreviousPosts() const 
{
    return currentFeedIndex > 0;
}

void PostManager::resetFeedPosition()
{
    currentFeedIndex = 0;
}

void PostManager::loadFromFile(const string& filename) 
{
    posts.clear();
    maxPostId = 0;
    ifstream file(filename);
    if (!file.is_open()) 
    {
        cout << "No existing posts file found. Starting fresh."<<endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty()) continue;

        stringstream ss(line);
        Post p;
        string temp;
        getline(ss, temp, '#'); 
        p.postId = stoi(temp);
        getline(ss, temp, '#'); 
        p.authorId = stoi(temp);
        getline(ss, p.text, '#');
        getline(ss, temp, '#'); 
        p.timestamp = stoll(temp);
        getline(ss, temp, '#'); 
        p.isPublic = (temp == "Public");

        posts.push_back(p);
        if (p.postId > maxPostId)
            maxPostId = p.postId;
    }
    file.close();
    cout << "Loaded " << posts.size() << " posts."<<endl;
}

void PostManager::saveToFile(const string& filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot save posts file.\n";
        return;
    }

    for (const auto& p : posts)
    {
        string safeText = p.text;
        for (size_t i = 0; i < safeText.length(); i++) 
        {
            if (safeText[i] == '#') 
                safeText[i] = '|';
        }

        file << p.postId << "#" << p.authorId << "#" << safeText << "#"
            << p.timestamp << "#" << (p.isPublic ? "Public" : "Friends") << "\n";
    }
    file.close();
}