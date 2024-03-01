#ifndef MYQUEUE_H_INCLUDED
#define MYQUEUE_H_INCLUDED

#include <vector>

template <typename T>
class MyQueue {
private:
    std::vector<T> data;

public:
    void enqueue(const T& item);
    void dequeue();
    const T& front() const;
    bool empty() const;
    size_t size() const;
};

#include "MyQueue.cpp"  // Include the implementation file



#endif // MYQUEUE_H_INCLUDED
