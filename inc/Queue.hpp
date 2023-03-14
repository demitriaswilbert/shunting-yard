#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "linkedlist.hpp"
#include <bits/stdc++.h>
template <typename T> class Queue {
    SingleNode<T> *m_front, *m_back;
    size_t m_size;

    void copyFrom(Queue<T>& other) {
        while (size())
            pop();
        for (SingleNode<T>* i = other.begin(); i != NULL; i = i->next())
            push(i->getElement());
    }

    void copyFrom(Queue<T>&& other) {
        while (size())
            pop();
        m_front = other.m_front;
        m_back = other.m_back;
        m_size = other.m_size;
        other.m_front = other.m_back = NULL;
        other.m_size = 0;
    }
    
  public:
    Queue<T>() : m_front(NULL), m_back(NULL), m_size(0) {}
    Queue(Queue<T>& other) : Queue() { copyFrom(other); }
    Queue(Queue<T>&& other) : Queue() { copyFrom(std::move(other)); }

    SingleNode<T>* begin() { return m_front; }
    SingleNode<T>* end() { return m_back; }

    void pop() {
        // empty Queue
        if (!m_front)
            return;

        // 1 left
        if (m_front == m_back)
            m_back = NULL;

        SingleNode<T>* tmp = m_front->next();
        delete m_front;
        m_front = tmp;

        // decrement size
        m_size--;
    }

    void push(T element) {
        // new element
        SingleNode<T>* elem = new SingleNode<T>(element);

        // empty queue
        if (m_back == NULL) {
            m_back = m_front = elem;
            m_size++;
            return;
        }

        m_back->next(elem);
        m_back = elem;
        m_size++;
    }

    T& front() const { return m_front->getElement(); }
    T& back() const { return m_back->getElement(); }

    std::string print() {
        std::stringstream ss;
        ss << "Size: " << m_size << std::endl;
        for (SingleNode<T>* elem = m_front; elem != NULL; elem = elem->next()) {
            ss << ((elem == m_front) ? "" : ", ") << elem->getElement();
        }
        return ss.str();
    }
    size_t size() const { return m_size; }
    void operator=(Queue<T>& other) { copyFrom(other); }
    void operator=(Queue<T>&& other) { copyFrom(std::move(other)); }
    ~Queue() {
        while (size())
            pop();
    }
};
#endif // QUEUE_HPP