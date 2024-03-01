#include "MyPriorityQueue.h"
#include <algorithm>  // for std::make_heap, std::push_heap, std::pop_heap

template <typename T>
void MyPriorityQueue<T>::push(const T& item) {
    data.push_back(item);
    std::push_heap(data.begin(), data.end());
}

template <typename T>
void MyPriorityQueue<T>::pop() {
    if (!empty()) {
        std::pop_heap(data.begin(), data.end());
        data.pop_back();
    }
}

template <typename T>
const T& MyPriorityQueue<T>::top() const {
    return data.front();
}

template <typename T>
bool MyPriorityQueue<T>::empty() const {
    return data.empty();
}

template <typename T>
size_t MyPriorityQueue<T>::size() const {
    return data.size();
}
