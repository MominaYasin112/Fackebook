#define _CRT_SECURE_NO_WARNINGS
#include "App.h"
#include <iostream>

App::App() : currentUser(nullptr)
{
    std::cout << "Loading data...\n";
    userMgr.loadFromFile("allusers.txt");
    const std::vector<User>& users = userMgr.getAllUsers();
    int maxId = 0;
    for (const auto& u : users) {
        if (u.id > maxId) maxId = u.id;
    }
    friendMgr.initForUsers(maxId);
    friendMgr.loadFromFile("Friends.txt");

    postMgr.loadFromFile("posts.txt");
    reqMgr.loadFromFile("FriendRequests.txt");
    std::cout << "Data loaded successfully!\n\n";
}

void App::run()
{
    while (true)
    {
        if (currentUser == nullptr)
        {
            showMainMenu();
        }
        else
        {
            showUserMenu();
        }
    }
}

void App::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void App::pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore(10000, '\n');
    cin.get();
}

void App::showMainMenu()
{
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "     WELCOME TO FAKEBOOK\n";
    std::cout << "========================================\n";
    std::cout << "0. Generate Dummy Data\n";
    std::cout << "1. Sign Up\n";
    std::cout << "2. Login\n";
    std::cout << "3. Exit\n";
    std::cout << "========================================\n";
    std::cout << "Choose option: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(10000, '\n');

    switch (choice)
    {
    case 0:
        handleGenerateDummyData();
        break;
    case 1:
        handleSignUp();
        break;
    case 2:
        handleLogin();
        break;
    case 3:
        std::cout << "Goodbye!\n";
        exit(0);
    default:
        std::cout << "Invalid choice!\n";
        pause();
    }
}

void App::handleSignUp()
{
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "           SIGN UP\n";
    std::cout << "========================================\n";

    User newUser;

    std::cout << "Username: ";
    std::getline(std::cin, newUser.username);

    std::cout << "Email: ";
    std::getline(std::cin, newUser.email);

    std::cout << "Password: ";
    std::getline(std::cin, newUser.password);

    std::cout << "Location: ";
    std::getline(std::cin, newUser.location);

    std::cout << "Gender (Male/Female/Other): ";
    std::getline(std::cin, newUser.gender);

    std::cout << "Age: ";
    std::cin >> newUser.age;
    std::cin.ignore(10000, '\n');

    std::cout << "Public profile? (1 = Yes, 0 = No): ";
    int pub;
    std::cin >> pub;
    std::cin.ignore(10000, '\n');
    newUser.isPublic = (pub == 1);

    int userId = userMgr.registerUser(newUser);
    if (userId > 0) {
        std::cout << "\nAccount created successfully! Your User ID is: " << userId << "\n";
        friendMgr.initForUsers(userId);
    }
    else {
        std::cout << "\nError: Email already exists!\n";
    }

    pause();
}

void App::handleLogin()
{
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "           LOGIN\n";
    std::cout << "========================================\n";

    std::string email, password;

    std::cout << "Email: ";
    std::getline(std::cin, email);

    std::cout << "Password: ";
    std::getline(std::cin, password);

    currentUser = userMgr.login(email, password);

    if (currentUser != nullptr)
    {
        std::cout << "\nLogin successful! Welcome, " << currentUser->username << "!\n";
        pause();
    }
    else
    {
        std::cout << "\nInvalid email or password!\n";
        pause();
    }
}

void App::handleLogout()
{
    currentUser = nullptr;
    cout << "Logged out successfully!\n";
    pause();
}

void App::showUserMenu()
{
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "  Welcome, " << currentUser->username << " (ID: " << currentUser->id << ")\n";
    std::cout << "========================================\n";
    std::cout << "1. Create Post\n";
    std::cout << "2. View Feed\n";
    std::cout << "3. View My Profile\n";
    std::cout << "4. View Other User's Profile\n";
    std::cout << "5. Send Friend Request\n";
    std::cout << "6. View Friend Requests (Received)\n";
    std::cout << "7. View Sent Requests\n";
    std::cout << "8. View My Friends\n";
    std::cout << "9. Remove Friend\n";
    std::cout << "10. Logout\n";
    std::cout << "========================================\n";
    std::cout << "Choose option: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(10000, '\n');

    switch (choice)
    {
    case 1:
        handleCreatePost();
        break;
    case 2:
        handleViewFeed();
        break;
    case 3:
        handleViewProfile();
        break;
    case 4:
        handleViewOtherProfile();
        break;
    case 5:
        handleSendFriendRequest();
        break;
    case 6:
        handleViewFriendRequests();
        break;
    case 7:
        handleViewSentRequests();
        break;
    case 8:
        handleViewFriends();
        break;
    case 9:
        handleRemoveFriend();
        break;
    case 10:
        handleLogout();
        break;
    default:
        std::cout << "Invalid choice!\n";
        pause();
    }
}

void App::handleCreatePost()
{
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "         CREATE POST\n";
    std::cout << "========================================\n";

    std::string text;
    std::cout << "Enter post text (no # symbol): ";
    std::getline(std::cin, text);

    if (text.empty())
    {
        std::cout << "\nPost cannot be empty!\n";
        pause();
        return;
    }

    std::cout << "Post visibility (1 = Public, 0 = Friends Only): ";
    int vis;
    std::cin >> vis;
    std::cin.ignore(10000, '\n');

    bool isPublic = (vis == 1);

    int postId = postMgr.createPost(currentUser->id, text, isPublic);
    std::cout << "\nPost created successfully! Post ID: " << postId << "\n";

    pause();
}

void App::handleViewFeed()
{
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "         YOUR FEED\n";
    std::cout << "========================================\n";
    postMgr.buildFeedHeap(currentUser->id, friendMgr, userMgr);
    postMgr.resetFeedPosition();
    std::vector<Post> feed = postMgr.getNextPosts(50);

    if (feed.empty()) {
        std::cout << "No posts to show. Add friends to see their posts!\n";
    }
    else {
        std::cout << "Showing " << feed.size() << " posts:\n\n";

        for (size_t i = 0; i < feed.size(); i++) {
            const Post& p = feed[i];
            User* author = userMgr.getUserById(p.authorId);

            std::cout << "----------------------------\n";
            if (author) {
                std::cout << "Author: " << author->username << " (ID: " << p.authorId << ")\n";
            }
            else {
                std::cout << "Author ID: " << p.authorId << "\n";
            }
            std::cout << "Post: " << p.text << "\n";
            std::cout << "Visibility: " << (p.isPublic ? "Public" : "Friends Only") << "\n";
            std::time_t t = p.timestamp;
            std::cout << "Posted: " << ctime(&t);
            std::cout << "----------------------------\n\n";
        }
    }

    pause();
}

void App::handleViewProfile() {
    displayUserProfile(currentUser, true, true);
}

void App::handleViewOtherProfile()
{
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "      VIEW USER PROFILE\n";
    std::cout << "========================================\n";

    std::cout << "Search by:\n";
    std::cout << "1. Username\n";
    std::cout << "2. User ID\n";
    std::cout << "Choose option: ";

    int searchChoice;
    std::cin >> searchChoice;
    std::cin.ignore(10000, '\n');

    User* targetUser = nullptr;

    if (searchChoice == 1)
    {
        std::cout << "Enter username: ";
        std::string username;
        std::getline(std::cin, username);

        targetUser = userMgr.getUserByUsername(username);

        if (targetUser == nullptr)
        {
            std::cout << "\nUser with username '" << username << "' not found!\n";
            pause();
            return;
        }
    }
    else if (searchChoice == 2)
    {
        std::cout << "Enter User ID: ";
        int userId;
        std::cin >> userId;
        std::cin.ignore(10000, '\n');

        targetUser = userMgr.getUserById(userId);

        if (targetUser == nullptr)
        {
            std::cout << "\nUser with ID " << userId << " not found!\n";
            pause();
            return;
        }
    }
    else
    {
        std::cout << "Invalid choice!\n";
        pause();
        return;
    }

    if (targetUser->id == currentUser->id)
    {
        std::cout << "\nThis is your own profile. Use 'View My Profile' option instead.\n";
        pause();
        return;
    }

    bool isFriend = friendMgr.areFriends(currentUser->id, targetUser->id);

    displayUserProfile(targetUser, isFriend, false);
}
void App::handleGenerateDummyData()
{
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "  GENERATE DUMMY DATA\n";
    std::cout << "========================================\n";
    std::cout << "WARNING: This will ERASE all existing data!\n";
    std::cout << "Are you sure you want to continue? (1=Yes, 0=No): ";

    int confirm;
    std::cin >> confirm;
    std::cin.ignore(10000, '\n');

    if (confirm != 1) {
        std::cout << "Operation cancelled.\n";
        pause();
        return;
    }

    std::cout << "\nHow many users to generate? (Recommended: 20-30): ";
    int numUsers;
    std::cin >> numUsers;
    std::cin.ignore(10000, '\n');

    if (numUsers < 5) {
        std::cout << "Minimum 5 users required. Setting to 5.\n";
        numUsers = 5;
    }
    if (numUsers > 100) {
        std::cout << "Maximum 100 users allowed. Setting to 100.\n";
        numUsers = 100;
    }

    std::cout << "How many posts to generate? (Recommended: 50-100): ";
    int numPosts;
    std::cin >> numPosts;
    std::cin.ignore(10000, '\n');

    if (numPosts < 10) {
        std::cout << "Minimum 10 posts required. Setting to 10.\n";
        numPosts = 10;
    }
    if (numPosts > 500) {
        std::cout << "Maximum 500 posts allowed. Setting to 500.\n";
        numPosts = 500;
    }

    std::cout << "\nGenerating data...\n";

    FakeDataGenerator generator;
    generator.generateAll(numUsers, numPosts);

    std::cout << "Reloading application data...\n";
    reloadAllData();
    std::cout << "\n✓ Dummy data generation complete!\n";
    std::cout << "\nSample Login Credentials:\n";
    std::cout << "Email: AhmedKhan1@fakebook.pk\n";
    std::cout << "Password: pakistan123\n";
    std::cout << "\n(All users have password: pakistan123)\n";

    pause();
}

void App::reloadAllData()
{
    userMgr.loadFromFile("allusers.txt");

    const std::vector<User>& users = userMgr.getAllUsers();
    int maxId = 0;
    for (const auto& u : users) {
        if (u.id > maxId) maxId = u.id;
    }

    friendMgr.initForUsers(maxId);
    friendMgr.loadFromFile("Friends.txt");

    postMgr.loadFromFile("posts.txt");
    reqMgr.loadFromFile("FriendRequests.txt");

    std::cout << "Data reloaded successfully!\n";
}
void App::handleSendFriendRequest()
{
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "      SEND FRIEND REQUEST\n";
    std::cout << "========================================\n";

    std::cout << "Enter User ID to send request: ";
    int targetId;
    std::cin >> targetId;
    std::cin.ignore(10000, '\n');

    if (targetId == currentUser->id)
    {
        std::cout << "You cannot send a friend request to yourself!\n";
        pause();
        return;
    }

    User* targetUser = userMgr.getUserById(targetId);
    if (targetUser == nullptr)
    {
        std::cout << "User not found!\n";
        pause();
        return;
    }
    if (friendMgr.areFriends(currentUser->id, targetId))
    {
        std::cout << "You are already friends with " << targetUser->username << "!\n";
        pause();
        return;
    }

    reqMgr.sendRequest(currentUser->id, targetId);
    std::cout << "Friend request sent to " << targetUser->username << "!\n";
    pause();
}

void App::handleViewFriendRequests()
{
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "      FRIEND REQUESTS (RECEIVED)\n";
    std::cout << "========================================\n";

    std::vector<Request> pending = reqMgr.getPendingForUser(currentUser->id);

    if (pending.empty())
    {
        std::cout << "No pending friend requests.\n";
        pause();
        return;
    }

    std::cout << "You have " << pending.size() << " pending request(s):\n\n";

    for (size_t i = 0; i < pending.size(); i++)
    {
        User* sender = userMgr.getUserById(pending[i].fromUserId);
        cout << (i + 1) << ". From: ";
        if (sender)
        {
            std::cout << sender->username << " (ID: " << sender->id << ")";
            std::cout << " - " << sender->location << ", Age " << sender->age << "\n";
        }
        else
        {
            std::cout << "User ID " << pending[i].fromUserId << "\n";
        }
    }

    std::cout << "\nEnter request number to respond (0 to cancel): ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(10000, '\n');

    if (choice < 1 || choice >(size_t)pending.size())
    {
        return;
    }

    Request& selectedReq = pending[choice - 1];

    std::cout << "Accept request? (1 = Yes, 0 = No): ";
    int accept;
    std::cin >> accept;
    std::cin.ignore(10000, '\n');

    reqMgr.respondToRequest(selectedReq.fromUserId, selectedReq.toUserId, (accept == 1), friendMgr);

    pause();
}

void App::handleViewSentRequests()
{
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "      MY SENT REQUESTS\n";
    std::cout << "========================================\n";

    std::vector<Request> sent = reqMgr.getSentByUser(currentUser->id);

    if (sent.empty())
    {
        std::cout << "You haven't sent any friend requests.\n";
        pause();
        return;
    }

    std::cout << "You have sent " << sent.size() << " request(s):\n\n";

    int pendingCount = 0, acceptedCount = 0, declinedCount = 0;

    for (size_t i = 0; i < sent.size(); i++)
    {
        User* recipient = userMgr.getUserById(sent[i].toUserId);

        std::cout << (i + 1) << ". To: ";
        if (recipient)
        {
            std::cout << recipient->username << " (ID: " << recipient->id << ")";
        }
        else
        {
            std::cout << "User ID " << sent[i].toUserId;
        }

        std::cout << " - Status: " << sent[i].status;
        std::time_t t = sent[i].timestamp;
        std::cout << " - Sent: " << ctime(&t);
        if (sent[i].status == "PENDING") pendingCount++;
        else if (sent[i].status == "ACCEPTED") acceptedCount++;
        else if (sent[i].status == "DECLINED") declinedCount++;
    }

    std::cout << "\n--- Summary ---\n";
    std::cout << "Pending: " << pendingCount << "\n";
    std::cout << "Accepted: " << acceptedCount << "\n";
    pause();
}

void App::handleViewFriends()
{
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "         MY FRIENDS\n";
    std::cout << "========================================\n";

    std::vector<int> friends = friendMgr.getFriendsList(currentUser->id);

    if (friends.empty())
    {
        std::cout << "You have no friends yet. Send friend requests to connect!\n";
    }
    else
    {
        std::cout << "You have " << friends.size() << " friend(s):\n\n";

        for (size_t i = 0; i < friends.size(); i++)
        {
            User* friendUser = userMgr.getUserById(friends[i]);
            std::cout << (i + 1) << ". ";
            if (friendUser)
            {
                std::cout << friendUser->username
                    << " (ID: " << friendUser->id << ") - "
                    << friendUser->location << ", Age: " << friendUser->age
                    << ", " << friendUser->gender << "\n";
            }
            else
            {
                std::cout << "User ID " << friends[i] << "\n";
            }
        }
    }

    pause();
}

void App::handleRemoveFriend()
{
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "         REMOVE FRIEND\n";
    std::cout << "========================================\n";

    std::vector<int> friends = friendMgr.getFriendsList(currentUser->id);

    if (friends.empty())
    {
        std::cout << "You have no friends to remove.\n";
        pause();
        return;
    }

    std::cout << "Your friends:\n\n";
    for (size_t i = 0; i < friends.size(); i++)
    {
        User* friendUser = userMgr.getUserById(friends[i]);
        std::cout << (i + 1) << ". ";
        if (friendUser)
        {
            cout << friendUser->username << " (ID: " << friendUser->id << ")\n";
        }
        else
        {
            cout << "User ID " << friends[i] << "\n";
        }
    }

    cout << "\nEnter friend number to remove (0 to cancel): ";
    int choice;
    cin >> choice;
    cin.ignore(10000, '\n');

    if (choice < 1 || choice >(size_t)friends.size())
    {
        std::cout << "Cancelled.\n";
        pause();
        return;
    }

    int friendToRemove = friends[choice - 1];
    User* friendUser = userMgr.getUserById(friendToRemove);

    std::cout << "Are you sure you want to remove ";
    if (friendUser)
    {
        std::cout << friendUser->username;
    }
    else
    {
        std::cout << "User ID " << friendToRemove;
    }
    std::cout << " from your friends? (1 = Yes, 0 = No): ";

    int confirm;
    std::cin >> confirm;
    std::cin.ignore(10000, '\n');

    if (confirm == 1)
    {
        if (friendMgr.removeFriendMutual(currentUser->id, friendToRemove))
        {
            std::cout << "\nFriend removed successfully!\n";
        }
        else
        {
            std::cout << "\nError removing friend.\n";
        }
    }
    else
    {
        std::cout << "\nCancelled.\n";
    }

    pause();
}
void App::displayUserProfile(User* targetUser, bool isFriend, bool isOwnProfile)
{
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "         USER PROFILE\n";
    std::cout << "========================================\n";

    std::cout << "Username: " << targetUser->username << "\n";
    std::cout << "User ID: " << targetUser->id << "\n";
    std::cout << "Location: " << targetUser->location << "\n";
    std::cout << "Age: " << targetUser->age << "\n";

    bool showFullProfile = isOwnProfile || targetUser->isPublic || isFriend;

    if (showFullProfile)
    {
        std::cout << "Gender: " << targetUser->gender << "\n";
        std::cout << "Email: " << targetUser->email << "\n";
        std::cout << "Profile Type: " << (targetUser->isPublic ? "Public" : "Private") << "\n";
        std::vector<int> friends = friendMgr.getFriendsList(targetUser->id);
        std::cout << "Friends: " << friends.size() << "\n";
    }
    else
    {
        std::cout << "Profile Type: Private\n";
        std::cout << "\n[This is a private profile. Send a friend request to see more details.]\n";
    }
    std::vector<Post> userPosts = postMgr.getPostsByAuthor(targetUser->id);
    std::vector<Post> visiblePosts;

    for (size_t i = 0; i < userPosts.size(); i++)
    {
        const Post& post = userPosts[i];

        if (isOwnProfile || targetUser->isPublic || isFriend)
        {
            visiblePosts.push_back(post);
        }
        else
        {
            if (post.isPublic)
            {
                visiblePosts.push_back(post);
            }
        }
    }

    std::cout << "\n--- Posts (" << visiblePosts.size() << " visible) ---\n";

    if (visiblePosts.empty())
    {
        if (userPosts.empty())
        {
            std::cout << "No posts yet.\n";
        }
        else
        {
            std::cout << "No public posts available.\n";
            std::cout << "(This user has " << userPosts.size()
                << " post(s), but they are private.)\n";
        }
    }
    else
    {
        std::cout << "\n";
        for (size_t i = 0; i < visiblePosts.size(); i++)
        {
            const Post& p = visiblePosts[i];
            std::cout << "----------------------------\n";
            std::cout << "Post #" << (i + 1) << "\n";
            std::cout << "Text: " << p.text << "\n";
            std::cout << "Visibility: " << (p.isPublic ? "Public" : "Friends Only") << "\n";

            std::time_t t = p.timestamp;
            std::cout << "Posted: " << std::ctime(&t);
            std::cout << "----------------------------\n";
        }
    }

    std::cout << "\n--- Relationship Status ---\n";
    if (isOwnProfile)
    {
        std::cout << "This is your profile.\n";
    }
    else if (isFriend)
    {
        std::cout << "You are friends with " << targetUser->username << ".\n";
    }
    else
    {
        std::cout << "You are not friends with " << targetUser->username << ".\n";
        std::cout << "Send a friend request to connect!\n";
    }
    if (!isOwnProfile)
    {
        std::cout << "\n--- Quick Actions ---\n";
        if (!isFriend)
        {
            std::cout << "1. Send Friend Request\n";
        }
        else
        {
            std::cout << "1. Remove Friend\n";
        }
        std::cout << "2. Return to Menu\n";
        std::cout << "Choose: ";

        int action;
        std::cin >> action;
        std::cin.ignore(10000, '\n');

        if (action == 1 && !isFriend)
        {
            reqMgr.sendRequest(currentUser->id, targetUser->id);
            std::cout << "\nFriend request sent to " << targetUser->username << "!\n";
            pause();
        }
        else if (action == 1 && isFriend)
        {
            std::cout << "Remove " << targetUser->username << " from friends? (1=Yes, 0=No): ";
            int confirm;
            std::cin >> confirm;
            std::cin.ignore(10000, '\n');

            if (confirm == 1)
            {
                if (friendMgr.removeFriendMutual(currentUser->id, targetUser->id))
                {
                    std::cout << "\nFriend removed successfully!\n";
                }
            }
            pause();
        }
    }
    else
    {
        pause();
    }
}