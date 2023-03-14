#ifndef STACK_HPP
#define STACK_HPP
#include "linkedlist.hpp"
#include <bits/stdc++.h>

template <typename T> class Stack {
    SingleNode<T>* m_head;
    size_t m_size;

    void copyFrom(Stack<T>& other) {
        while (size())
            pop();
        for (SingleNode<T>* i = other.begin(); i != NULL; i = i->next())
            push(i->getElement());
    }

    void copyFrom(Stack<T>&& other) {
        while (size())
            pop();
        m_head = other.m_head;
        m_size = other.m_size;
        other.m_head = NULL;
        other.m_size = 0;
    }

  public:
    Stack<T>() : m_head(NULL), m_size(0) {}
    Stack<T>(Stack<T>& other) : Stack() { copyFrom(other); }
    Stack<T>(Stack<T>&& other) : Stack() { copyFrom(std::move(other)); }

    SingleNode<T>* begin() { return m_head; }

    T& top() { return m_head->getElement(); }

    bool empty() { return m_size == 0; }

    void pop() {
        // empty stack
        if (!m_head)
            return;

        SingleNode<T>* tmp = m_head->next();
        delete m_head;
        m_head = tmp;

        // decrement size
        m_size--;
    }

    void push(T element) {
        // new element
        SingleNode<T>* elem = new SingleNode<T>(element);

        if (m_head != NULL)
            elem->next(m_head);
        m_head = elem;
        m_size++;
    }

    std::string print() {
        std::stringstream ss;
        ss << "Size: " << m_size << std::endl;
        for (SingleNode<T>* elem = m_head; elem != NULL; elem = elem->next()) {
            ss << ((elem == m_head) ? "" : ", ") << elem->getElement();
        }
        return ss.str();
    }
    size_t size() const { return m_size; }
    void operator=(Stack<T>& other) { copyFrom(other); }
    void operator=(Stack<T>&& other) { copyFrom(std::move(other)); }
};
#endif // STACK_HPP