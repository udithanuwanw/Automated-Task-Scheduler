#include "MyStack.h"

template <typename T>
void MyStack<T>::push(const T& item) {
    data.push_back(item);
}

template <typename T>
void MyStack<T>::pop() {
    if (!empty()) {
        data.pop_back();
    }
}

template <typename T>
const T& MyStack<T>::top() const {
    return data.back();
}

template <typename T>
bool MyStack<T>::empty() const {
    return data.empty();
}

template <typename T>
size_t MyStack<T>::size() const {
    return data.size();
}
