#include <bits/stdc++.h>
#include "Queue.hpp"

int main() {
    Queue<std::string> q;
    for (auto i : {"Hello", "World", "This", "Is", "Dewe"}) 
        q.push(i);
    std::cout << q.print() << std::endl;
    Queue<std::string> tmp;
    tmp = q;
    std::cout << tmp.print() << std::endl;
}