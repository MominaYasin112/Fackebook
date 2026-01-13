#pragma once
struct FriendNode {
	int friendId;
	FriendNode* next;
	FriendNode(int id) : friendId(id), next(nullptr) {}
};
