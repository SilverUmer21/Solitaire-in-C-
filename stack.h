#include<iostream>
template<typename T>
class STACK
{
private:
    class Node
    {
    public:
        T data;
        Node *next;

        Node(T d) : data(d), next(NULL) {}
    };

    Node *top;
    int count;

public:
    STACK() : top(NULL), count(0) {}
    ~STACK()
    {
        while (top != NULL)
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
        if (top == NULL)
        {
            cout << "STACK is empty" << endl;
            throw std::runtime_error("STACK is empty");
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
        if (top == NULL)
        {
            cout << "STACK is empty" << endl;
            throw std::runtime_error("STACK is empty");
        }
        return top->data;
    }
    bool isEmpty()
    {
        return top == NULL;
    }
    int size()
    {
        return count;
    }
};
