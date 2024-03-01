#ifndef SIMPLEUNORDEREDSET_H_INCLUDED
#define SIMPLEUNORDEREDSET_H_INCLUDED


class SimpleUnorderedSet {
private:
    static const int MAX_SIZE = 1024;
    std::string elements[MAX_SIZE];
    int numElements;

public:
    SimpleUnorderedSet() : numElements(0) {}

    bool contains(const std::string& element) const {
        for (int i = 0; i < numElements; ++i) {
            if (elements[i] == element) {
                return true;
            }
        }
        return false;
    }

    void insert(const std::string& element) {
        if (!contains(element) && numElements < MAX_SIZE) {
            elements[numElements++] = element;
        }
    }
};

#endif // SIMPLEUNORDEREDSET_H_INCLUDED
