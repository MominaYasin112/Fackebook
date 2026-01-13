#pragma once
#include <string>
#include <ctime>
using namespace std;
struct Request 
{
    int fromUserId;
    int toUserId;
    time_t timestamp;
    string status; // "PENDING", "ACCEPTED", "DECLINED"

    Request() : fromUserId(0), toUserId(0), timestamp(0), status("PENDING") {}
};