#ifndef MYPRIORITYQUEUE_H_INCLUDED
#define MYPRIORITYQUEUE_H_INCLUDED

#include <vector>

template <typename T>
class MyPriorityQueue {
private:
    std::vector<T> data;

public:
    void push(const T& item);
    void pop();
    const T& top() const;
    bool empty() const;
    size_t size() const;
};

#include "MyPriorityQueue.cpp"  // Include the implementation fil



#endif // MYPRIORITYQUEUE_H_INCLUDED
