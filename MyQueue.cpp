#include "MyQueue.h"

template <typename T>
void MyQueue<T>::enqueue(const T& item) {
    data.push_back(item);
}

template <typename T>
void MyQueue<T>::dequeue() {
    if (!empty()) {
        data.erase(data.begin());
    }
}

template <typename T>
const T& MyQueue<T>::front() const {
    return data.front();
}

template <typename T>
bool MyQueue<T>::empty() const {
    return data.empty();
}

template <typename T>
size_t MyQueue<T>::size() const {
    return data.size();
}
