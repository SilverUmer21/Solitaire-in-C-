#include <iostream>
using namespace std;

template <class T>
class stack
{
private:
    class Node
    {
    public:
        T data;
        Node *next;

        Node(T d) : data(d), next(nullptr) {}
    };

    Node *top;
    int count;

public:
    stack() : top(nullptr), count(0) {}
    ~stack()
    {
        while (top != nullptr)
        {
            Node *temp = top;
            top = top->next;
            delete temp;
        }
    }

    void push(const T &d)
    {
        Node *new_node = new Node(d);
        new_node->next = top;
        top = new_node;
        ++count;
    }

    T pop()
    {
        if (top == nullptr)
        {
            cout << "Stack is empty" << endl;
            throw std::runtime_error("Stack is empty");
        }
        T temp = top->data;
        Node *temp_node = top;
        top = top->next;
        delete temp_node;
        --count;
        return temp;
    }

    T &peek()
    {
        if (top == nullptr)
        {
            cout << "Stack is empty" << endl;
            throw std::runtime_error("Stack is empty");
        }
        return top->data;
    }

    bool isEmpty()
    {
        return top == nullptr;
    }

    int size()
    {
        return count;
    }
};
