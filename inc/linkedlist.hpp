
#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP
#include <bits/stdc++.h>

template <typename T>
class SingleNode {
    T m_element;
    SingleNode *m_next;

    public:
    SingleNode(T _element) : m_element(_element), m_next(NULL) {}
    
    SingleNode* next() { return m_next; }

    void next(SingleNode* _next) { m_next = _next; }

    T& getElement() { return m_element; }
    void setElement(T _element) { m_element = _element; }
};

#endif// LINKEDLIST_HPP