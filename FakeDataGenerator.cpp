#include "FakeDataGenerator.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>

FakeDataGenerator::FakeDataGenerator() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    initializeData();
}

void FakeDataGenerator::initializeData() {
    firstNames = {
        "Ahmed", "Fatima", "Ali", "Ayesha", "Hassan", "Zainab",
        "Muhammad", "Maryam", "Usman", "Aisha", "Omar", "Hira",
        "Bilal", "Sana", "Hamza", "Noor", "Imran", "Zara",
        "Saad", "Amna", "Faisal", "Mehwish", "Arslan", "Rabia",
        "Kamran", "Sidra", "Talha", "Mahnoor", "Kashif", "Anum"
    };
    lastNames = {
        "Khan", "Ahmed", "Ali", "Hussain", "Shah", "Malik",
        "Iqbal", "Siddiqui", "Haider", "Raza", "Butt", "Chaudhry",
        "Sheikh", "Mirza", "Aziz", "Noor", "Jamil", "Farooq", "Aslam", "Nawaz"
    };

    locations = {
        "Karachi", "Lahore", "Islamabad", "Rawalpindi", "Faisalabad",
        "Multan", "Peshawar", "Quetta", "Sialkot", "Gujranwala",
        "Hyderabad", "Bahawalpur", "Sargodha", "Sukkur", "Larkana",
        "Gujrat", "Mardan", "Kasur", "Rahim Yar Khan", "Sahiwal",
        "Okara", "Wah Cantt", "Dera Ghazi Khan", "Mirpur Khas", "Nawabshah"
    };
    genders = { "Male", "Female", "Other" };

    postTexts = {
    "Alhamdulillah for another beautiful day!",
    "Chai aur pakore in this amazing weather :)",
    "Just had the best biryani ever!!!",
    "Traffic in Lahore is crazy today...",
    "Missing home-cooked daal chawal <3",
    "Friday prayers at the mosque were peaceful.",
    "Watching PSL match tonight! Go Pakistan!",
    "Eid Mubarak to everyone!!!",
    "Ramadan kareem! First roza done :)",
    "Just finished my shift at the hospital.",
    "University exams are finally over! Yay!",
    "Planning a trip to Murree this weekend.",
    "Mango season is the best season!",
    "Late night chai with friends hits different.",
    "Alhamdulillah got the job! <3",
    "Pakistan Zindabad! Green and white pride!",
    "Missing northern areas so much :(",
    "Karachi heat is unbearable today...",
    "Just watched a great Pakistani drama.",
    "Trying to improve my Urdu calligraphy.",
    "Family dinner at Bundu Khan tonight!",
    "Rickshaw ride adventures haha!",
    "Street food in old Lahore is unbeatable.",
    "Completed my Quran recitation today.",
    "Cricket match with cousins was epic!",
    "Monsoon season finally here!",
    "Working from home today.",
    "Just had the best nihari for breakfast.",
    "Visiting Badshahi Mosque today.",
    "Startup life in Pakistan is challenging but exciting!",
    "Samosa and chai = perfect combination.",
    "Attended an amazing qawwali night.",
    "Fawad Khan's new drama is amazing!",
    "Planning to visit Hunza Valley soon.",
    "Study session at Gloria Jeans.",
    "Just got my CNIC renewed finally!",
    "Load shedding is back again ugh!",
    "Desi weddings are the best celebrations!",
    "Trying to wake up for Fajr on time.",
    "Just had the best lassi in town!",
    "Missing university days with friends.",
    "Karachi ke samose > everything else",
    "Started learning Pashto language.",
    "Throwback to northern areas trip!",
    "Islamabad's weather is perfect today!",
    "Just finished watching Coke Studio.",
    "Iftari shopping at Anarkali bazaar.",
    "May Allah bless everyone! Ameen",
    "Pakistan has the most beautiful mountains!",
    "Dinner at Kolachi was amazing!"
    };
}

std::string FakeDataGenerator::getRandomElement(const std::vector<std::string>& vec) {
    if (vec.empty()) return "";
    return vec[std::rand() % vec.size()];
}

int FakeDataGenerator::getRandomNumber(int min, int max) {
    return min + (std::rand() % (max - min + 1));
}

bool FakeDataGenerator::coinFlip() {
    return (std::rand() % 2) == 0;
}

void FakeDataGenerator::clearAllFiles() {
    std::cout << "Clearing all data files...\n";

    std::ofstream file1("allusers.txt", std::ofstream::trunc);
    file1.close();

    std::ofstream file2("Friends.txt", std::ofstream::trunc);
    file2.close();

    std::ofstream file3("posts.txt", std::ofstream::trunc);
    file3.close();

    std::ofstream file4("FriendRequests.txt", std::ofstream::trunc);
    file4.close();

    std::cout << "All files cleared.\n\n";
}


void FakeDataGenerator::generateUsers(int count, UserManager& um) {
    std::cout << "Generating " << count << " users...\n";

    for (int i = 0; i < count; i++) {
        User u;

        std::string firstName = getRandomElement(firstNames);
        std::string lastName = getRandomElement(lastNames);
        u.username = firstName + lastName + std::to_string(i + 1);
        u.email = u.username + "@fakebook.pk";
        u.password = "pakistan123";
        u.location = getRandomElement(locations);
        u.gender = getRandomElement(genders);
        u.age = getRandomNumber(18, 65);
        u.isPublic = coinFlip();

        int userId = um.registerUser(u);
        if (userId > 0) {
            std::cout << "  Created: " << u.username
                << " (ID: " << userId << ", "
                << (u.isPublic ? "Public" : "Private") << ")\n";
        }
    }
    std::cout << "Users created successfully!\n\n";
}
void FakeDataGenerator::generateFriendships(UserManager& um, FriendManager& fm, int avgFriendsPerUser) {
    std::cout << "Generating friendships...\n";

    const std::vector<User>& users = um.getAllUsers();
    if (users.size() < 2) {
        std::cout << "Not enough users to create friendships.\n\n";
        return;
    }

    int friendshipsCreated = 0;

    for (size_t i = 0; i < users.size(); i++) {
        // Each user gets a random number of friends (between 1 and avgFriendsPerUser*2)
        int targetFriends = getRandomNumber(1, avgFriendsPerUser * 2);
        int attemptsMade = 0;
        int maxAttempts = users.size() * 2;  // Prevent infinite loops

        while (attemptsMade < maxAttempts && targetFriends > 0) {
            // Pick a random user to befriend
            int randomIndex = getRandomNumber(0, users.size() - 1);

            if (randomIndex != (int)i) {
                int userA = users[i].id;
                int userB = users[randomIndex].id;

                if (!fm.areFriends(userA, userB)) {
                    if (fm.addFriendMutual(userA, userB)) {
                        friendshipsCreated++;
                        targetFriends--;
                    }
                }
            }
            attemptsMade++;
        }
    }

    std::cout << "  ✓ Created " << friendshipsCreated << " mutual friendships.\n";
    std::cout << "Friendships created successfully!\n\n";
}

void FakeDataGenerator::generatePosts(UserManager& um, PostManager& pm, int count) {
    std::cout << "Generating " << count << " posts...\n";

    const std::vector<User>& users = um.getAllUsers();
    if (users.empty()) {
        std::cout << "No users available to create posts.\n\n";
        return;
    }

    int publicPosts = 0;
    int privatePosts = 0;

    for (int i = 0; i < count; i++) {
        // Pick random author
        int randomIndex = getRandomNumber(0, users.size() - 1);
        int authorId = users[randomIndex].id;

        // Pick random post text
        std::string text = getRandomElement(postTexts);

        // Random visibility (60% public, 40% friends-only for variety)
        bool isPublic = (getRandomNumber(1, 10) <= 6);

        if (isPublic) publicPosts++;
        else privatePosts++;

        pm.createPost(authorId, text, isPublic);
    }

    std::cout << "  ✓ Created " << publicPosts << " public posts.\n";
    std::cout << "  ✓ Created " << privatePosts << " private posts.\n";
    std::cout << "Posts created successfully!\n\n";
}

void FakeDataGenerator::generateRequests(UserManager& um, RequestManager& rm, FriendManager& fm, int count) {
    std::cout << "Generating " << count << " pending friend requests...\n";

    const std::vector<User>& users = um.getAllUsers();
    if (users.size() < 2) {
        std::cout << "Not enough users to create requests.\n\n";
        return;
    }

    int requestsCreated = 0;
    int attempts = 0;
    int maxAttempts = count * 5;  // Prevent infinite loops

    while (requestsCreated < count && attempts < maxAttempts) {
        int fromIndex = getRandomNumber(0, users.size() - 1);
        int toIndex = getRandomNumber(0, users.size() - 1);

        if (fromIndex != toIndex) {
            int fromId = users[fromIndex].id;
            int toId = users[toIndex].id;

            // Don't send request if already friends
            if (!fm.areFriends(fromId, toId)) {
                rm.sendRequest(fromId, toId);
                requestsCreated++;
            }
        }
        attempts++;
    }

    std::cout << "  ✓ Created " << requestsCreated << " pending friend requests.\n";
    std::cout << "Friend requests created successfully!\n\n";
}

void FakeDataGenerator::generateAll(int numUsers, int numPosts) {
    std::cout << "\n========================================\n";
    std::cout << "  FAKEBOOK DUMMY DATA GENERATOR\n";
    std::cout << "========================================\n\n";
    clearAllFiles();
    UserManager um;
    FriendManager fm;
    PostManager pm;
    RequestManager rm;
    generateUsers(numUsers, um);
    const std::vector<User>& users = um.getAllUsers();
    int maxId = 0;
    for (const auto& u : users) {
        if (u.id > maxId) maxId = u.id;
    }
    fm.initForUsers(maxId);
    generateFriendships(um, fm, 6);
    generatePosts(um, pm, numPosts);
    int numRequests = numUsers / 5;
    if (numRequests < 3) numRequests = 3;
    generateRequests(um, rm, fm, numRequests);

    std::cout << "========================================\n";
    std::cout << "  DATA GENERATION COMPLETE!\n";
    std::cout << "========================================\n";
    std::cout << "Summary:\n";
    std::cout << "   Users: " << numUsers << "\n";
    std::cout << "   Posts: " << numPosts << "\n";
    std::cout << "   Pending Requests: " << numRequests << "\n";
    std::cout << "\nAll files have been populated.\n";
    std::cout << "You can now use the system normally.\n";
    std::cout << "========================================\n\n";
}