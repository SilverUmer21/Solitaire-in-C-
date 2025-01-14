#include <iostream>
using namespace std;

template <class T>
class List   
{
private:
    class Node
    {
    public:
        T card;
        Node *next;
        Node *prev;

        Node(T c) : card(c), next(NULL), prev(NULL) {}
    };
    Node *head;
    Node *tail;

public:
    List() : head(NULL), tail(NULL) {}
    ~List()
    {
        while (head != NULL)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
    }

    void add(T c)
    {
        Node *new_node = new Node(c);
        if (head == NULL)
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
        tail->next = NULL;
    }

    T remove()
    {
        if (head == NULL)
        {
            throw invalid_argument("Empty list");
        }

        Node *temp = head;
        if (head->next == NULL) // Only one element in the list
        {
            T c = head->card;
            delete head;
            head = NULL;
            return c;
        }

        // Traverse to the last node
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        T c = temp->card;
        temp->prev->next = NULL;
        tail = temp->prev;
        delete temp;
        return c;
    }

    int size()
    {
        Node *current = head;
        int count = 0;
        while (current != NULL)
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
        while (current != NULL)
        {
            if (count == index)
            {
                return &(current->card);
            }
            count++;
            current = current->next;
        }

        return NULL;
    }

    bool isEmpty()
    {
        return head == NULL;
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
    iterator end() { return iterator(NULL); }
};
