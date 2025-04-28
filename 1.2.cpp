#include <iostream>
#include <deque>
#include <vector>
#include <list>
#include <string>
#include <stdexcept>

template <typename T, typename Container = std::deque<T> >
class Stack {

    Container container;

    public:

    bool is_empty() const {
        return container.empty();
    }

    int cont_size() const {
        return container.size();
    }

    T& top() {
        if (!is_empty()) {
            return container.back();
        } else {
            throw std::out_of_range("Empty stack!");
        }
    }

    const T& top() const {
        if (!is_empty()) {
            return container.back();
        } else {
            throw std::out_of_range("Empty stack!");
        }
    }

    void push(const T& value) {
        container.push_back(value);
    }

    T& pop() {
        if (!is_empty()) {
            T& temp = top();
            container.pop_back();
            return temp;
        } else {
            throw std::out_of_range("Empty stack!");
        }
    }
};


template <>
class Stack<char, std::string> {
    std::string container;

    public:

    bool is_empty() const {
        return container.empty();
    }

    int cont_size() const {
        return container.size();
    }

    char& top() {
        if (!is_empty()) {
            return container.back();
        } else {
            throw std::out_of_range("Empty stack!");
        }
    }

    const char& top() const {
        if (!is_empty()) {
            return container.back();
        } else {
            throw std::out_of_range("Empty stack!");
        }
    }

    void push(const char& value) {
        container.push_back(value);
    }

    char& pop() {
        if (!is_empty()) {
            char& temp = top();
            container.pop_back();
            return temp;
        } else {
            throw std::out_of_range("Empty stack!");
        }
    }

};


int main() {
    // Stack on vector
    Stack<int, std::vector<int> > vector_stack;
    vector_stack.push(1);
    vector_stack.push(2);
    vector_stack.push(3);
    std::cout << "Vector stack top: " << vector_stack.top() << std::endl;
    vector_stack.pop();
    std::cout << "After pop: " << vector_stack.top() << std::endl;

    std::cout << std::endl;

    // Stack on deque (default)
    Stack<float> deque_stack;
    deque_stack.push(4.2);
    deque_stack.push(2.8);
    std::cout << "Deque stack size: " << deque_stack.cont_size() << std::endl;

    std::cout << std::endl;

    // Stack on string
    Stack<char, std::string> char_stack;
    char_stack.push('a');
    char_stack.push('b');
    char_stack.push('c');
    std::cout << "Char stack top: " << char_stack.top() << std::endl;
    char_stack.pop();
    std::cout << "After pop: " << char_stack.top() << std::endl;

    return 0;
}