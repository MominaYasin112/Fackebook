#pragma once
#include "UserManager.h"
#include "FriendManager.h"
#include "PostManager.h"
#include "RequestManager.h"
#include "FakeDataGenerator.h"  
class App {
public:
    App();
    void run();

private:
    UserManager userMgr;
    FriendManager friendMgr;
    PostManager postMgr;
    RequestManager reqMgr;
    User* currentUser;
    void showMainMenu();
    void handleSignUp();
    void handleLogin();
    void handleLogout();
    void handleGenerateDummyData();
    void showUserMenu();
    void handleCreatePost();
    void handleViewFeed();
    void handleViewProfile();
    void handleViewOtherProfile();
    void handleSendFriendRequest();
    void handleViewFriendRequests();
    void handleViewSentRequests();
    void handleViewFriends();
    void handleRemoveFriend();
    void clearScreen();
    void pause();
    void displayUserProfile(User* targetUser, bool isFriend, bool isOwnProfile);
    void reloadAllData();
};