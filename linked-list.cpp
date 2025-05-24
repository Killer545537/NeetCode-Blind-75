#include <bits/stdc++.h>
#include <catch2/catch_test_macros.hpp>

struct ListNode {
    int val;
    ListNode* next;

    ListNode(): val(0), next(nullptr) {}
    explicit ListNode(const int x): val(x), next(nullptr) {}
    ListNode(const int x, ListNode* next): val(x), next(next) {}
};

ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* current = head;

    while (current) {
        ListNode* next_temp = current->next;
        current->next = prev;
        prev = current;
        current = next_temp;
    }

    return prev;
}

TEST_CASE("Reverse Linked List") {
    SECTION("Empty list") {
        ListNode* head = nullptr;
        REQUIRE(reverseList(head) == nullptr);
    }

    SECTION("Single node") {
        auto* head = new ListNode(5);
        ListNode* reversed = reverseList(head);
        REQUIRE(reversed->val == 5);
        REQUIRE(reversed->next == nullptr);
        delete reversed; // Clean up
    }

    SECTION("Multiple nodes") {
        // Create list: 1->2->3->4->5
        auto* head = new ListNode(1);
        head->next = new ListNode(2);
        head->next->next = new ListNode(3);
        head->next->next->next = new ListNode(4);
        head->next->next->next->next = new ListNode(5);

        // Reverse list
        ListNode* reversed = reverseList(head);

        // Check reversed: 5->4->3->2->1
        REQUIRE(reversed->val == 5);
        REQUIRE(reversed->next->val == 4);
        REQUIRE(reversed->next->next->val == 3);
        REQUIRE(reversed->next->next->next->val == 2);
        REQUIRE(reversed->next->next->next->next->val == 1);
        REQUIRE(reversed->next->next->next->next->next == nullptr);

        // Clean up
        ListNode* current = reversed;
        while (current) {
            ListNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    if (!list1) return list2;
    if (!list2) return list1;

    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (list1 and list2) {
        if (list1->val <= list2->val) {
            tail->next = list1;
            list1 = list1->next;
        }
        else {
            tail->next = list2;
            list2 = list2->next;
        }

        tail = tail->next;
    }

    tail->next = list1 ? list1 : list2;

    return dummy.next;
}

TEST_CASE("Merge Two Sorted Lists") {
    SECTION("Regular case: [1,2,4] and [1,3,4]") {
        // Create list1: 1->2->4
        auto* list1 = new ListNode(1);
        list1->next = new ListNode(2);
        list1->next->next = new ListNode(4);

        // Create list2: 1->3->4
        auto* list2 = new ListNode(1);
        list2->next = new ListNode(3);
        list2->next->next = new ListNode(4);

        // Merge lists
        ListNode* merged = mergeTwoLists(list1, list2);

        // Check merged: 1->1->2->3->4->4
        REQUIRE(merged->val == 1);
        REQUIRE(merged->next->val == 1);
        REQUIRE(merged->next->next->val == 2);
        REQUIRE(merged->next->next->next->val == 3);
        REQUIRE(merged->next->next->next->next->val == 4);
        REQUIRE(merged->next->next->next->next->next->val == 4);
        REQUIRE(merged->next->next->next->next->next->next == nullptr);

        // Clean up
        ListNode* current = merged;
        while (current) {
            ListNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    SECTION("Both lists empty: [] and []") {
        ListNode* list1 = nullptr;
        ListNode* list2 = nullptr;
        REQUIRE(mergeTwoLists(list1, list2) == nullptr);
    }

    SECTION("One list empty: [] and [0]") {
        ListNode* list1 = nullptr;
        auto* list2 = new ListNode(0);

        ListNode* merged = mergeTwoLists(list1, list2);
        REQUIRE(merged->val == 0);
        REQUIRE(merged->next == nullptr);

        delete merged;
    }
}

bool hasCycle(const ListNode* head) {
    const ListNode *tortoise = head, *hare = head;

    while (hare and hare->next) {
        tortoise = tortoise->next;
        hare = hare->next->next;

        if (tortoise == hare) return true;
    }

    return false;
}

TEST_CASE("Linked List Cycle Detection") {
    SECTION("List with a cycle: [3,2,0,-4], pos = 1") {
        // Create list: 3->2->0->-4->2...
        auto* head = new ListNode(3);
        auto* node1 = new ListNode(2);
        auto* node2 = new ListNode(0);
        auto* node3 = new ListNode(-4);

        head->next = node1;
        node1->next = node2;
        node2->next = node3;
        node3->next = node1; // Create cycle to position 1

        REQUIRE(hasCycle(head) == true);

        // Break the cycle for cleanup
        node3->next = nullptr;
        delete head;
        delete node1;
        delete node2;
        delete node3;
    }

    SECTION("List with a cycle at head: [1,2], pos = 0") {
        auto* head = new ListNode(1);
        auto* node1 = new ListNode(2);

        head->next = node1;
        node1->next = head; // Create cycle to position 0

        REQUIRE(hasCycle(head) == true);

        // Break the cycle for cleanup
        node1->next = nullptr;
        delete head;
        delete node1;
    }

    SECTION("List without a cycle: [1], pos = -1") {
        auto* head = new ListNode(1);

        REQUIRE(hasCycle(head) == false);

        delete head;
    }
}

void reorderList(ListNode* head) {
    if (!head or !head->next) return;

    // Find the middle of the list
    ListNode *slow = head, *fast = head;
    while (fast and fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    ListNode* second_half = slow->next;
    slow->next = nullptr;

    // Reverse the second half
    ListNode* prev = nullptr;
    while (second_half) {
        ListNode* next_temp = second_half->next;
        second_half->next = prev;
        prev = second_half;
        second_half = next_temp;
    }
    second_half = prev; // Head of reversed second half

    //Merge the two halves
    ListNode* first_half = head;
    while (second_half) {
        ListNode* first_next = first_half->next;
        ListNode* second_next = second_half->next;

        first_half->next = second_half;
        second_half->next = first_next;

        first_half = first_next;
        second_half = second_next;
    }
}

ListNode* removeNthFromEnd(ListNode* head, const int n) {
    ListNode dummy(0, head);
    ListNode *left = &dummy, *right = head;

    for (int i = 0; i < n and right; ++i) right = right->next;

    while (right) {
        left = left->next;
        right = right->next;
    }

    const auto* to_delete = left->next;
    left->next = left->next->next;

    delete to_delete;
    return dummy.next;
}

ListNode* mergeKLists(std::vector<ListNode*>& lists) {
    const auto merge_two_lists = [](ListNode* list1, ListNode* list2) {
        if (!list1) return list2;
        if (!list2) return list1;

        ListNode dummy(0);
        ListNode* tail = &dummy;

        while (list1 and list2) {
            if (list1->val < list2->val) {
                tail->next = list1;
                list1 = list1->next;
            }
            else {
                tail->next = list2;
                list2 = list2->next;
            }
            tail = tail->next;
        }

        tail->next = list1 ? list1 : list2;

        return dummy.next;
    };

    if (lists.empty()) return nullptr;

    while (lists.size() > 1) {
        lists.emplace_back(merge_two_lists(lists[0], lists[1]));
        lists.erase(lists.begin());
        lists.erase(lists.begin());
    }

    return lists.front();
}
