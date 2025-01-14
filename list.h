#include <iostream>
using namespace std;

template <class T>
class DLL
{
private:
    class Node
    {
    public:
        T card;
        Node *next;
        Node *prev;

        Node(T c) : card(c), next(nullptr), prev(nullptr) {}
    };
    Node *head;
    Node *tail;

public:
    DLL() : head(nullptr), tail(nullptr) {}
    ~DLL()
    {
        while (head != nullptr)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
    }

    void add(T c)
    {
        Node *new_node = new Node(c);
        if (head == nullptr)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
        tail->next = nullptr;
    }

    T remove()
    {
        if (head == nullptr)
        {
            throw invalid_argument("Empty list");
        }

        Node *temp = head;
        if (head->next == nullptr) // Only one element in the list
        {
            T c = head->card;
            delete head;
            head = nullptr;
            return c;
        }

        // Traverse to the last node
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }

        T c = temp->card;
        temp->prev->next = nullptr;
        tail = temp->prev;
        delete temp;
        return c;
    }

    int size()
    {
        Node *current = head;
        int count = 0;
        while (current != nullptr)
        {
            count++;
            current = current->next;
        }
        return count;
    }

    T *get(int index)
    {
        Node *current = head;
        int count = 0;
        while (current != nullptr)
        {
            if (count == index)
            {
                return &(current->card);
            }
            count++;
            current = current->next;
        }

        return nullptr;
    }

    bool isEmpty()
    {
        return head == nullptr;
    }

    class iterator
    {
    private:
        Node *current;

    public:
        iterator(Node *current) : current(current) {}
        iterator &operator++()
        {
            current = current->next;
            return *this;
        }
        iterator &operator--()
        {
            current = current->prev;
            return *this;
        }
        T operator*() { return current->card; }
        bool operator!=(const iterator &other) { return current != other.current; }
    };

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
};