#ifndef MYSTACK_H_INCLUDED
#define MYSTACK_H_INCLUDED

#include <vector>

template <typename T>
class MyStack {
private:
    std::vector<T> data;

public:
    void push(const T& item);
    void pop();
    const T& top() const;
    bool empty() const;
    size_t size() const;
};

#include "MyStack.cpp"  // Include the implementation file



#endif // MYSTACK_H_INCLUDED
