#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <chrono>
#include <functional>
#include <random>
#include <type_traits>

template<typename T, typename = void>
struct is_random_access_iterator : std::false_type {};

// Only Random Access Iterators have operation '-'
template<typename T> 
struct is_random_access_iterator<T,
    std::void_t<decltype(std::declval<T>() - 1)> >: 
        std::true_type {}; 


// q-sort
template<typename RandomAccessIterator, typename Compare>
void q_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    // Check for valid iterator
    static_assert(is_random_access_iterator<RandomAccessIterator>::value,
        "q_sort requires random access iterators"
    );

    if (first == last || first + 1 == last) return;

    auto pivot = *(first + (last - first) / 2);
    RandomAccessIterator left = first;
    RandomAccessIterator right = last - 1;

    while (left <= right) {
        while (comp(*left, pivot)) ++left;
        while (comp(pivot, *right)) --right;
        if (left <= right) {
            std::iter_swap(left, right);
            ++left;
            --right;
        }
    }

    sort(first, right + 1, comp);
    sort(left, last, comp);
}

template<typename T, typename Compare>
void check(T cont, Compare comp) {
    std::cout << "Before sort: ";
    for (int num : cont) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    q_sort(cont.begin(), cont.end(), comp);
    std::cout << "After sort: ";
    for (int num : cont) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}


template<typename T>
const bool def_comp(T a, T b) {
    return a < b;
}

struct Comparator {
    template<typename T>
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

// Time maesure
template<typename Container>
void measure_sort(Container& container, const std::string& container_name) {
    auto start = std::chrono::high_resolution_clock::now();
    q_sort(container.begin(), container.end(), std::less<typename Container::value_type>());
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Sorting " << container_name << " took " << duration.count() << " microseconds\n";
}

int main() {
    const int size = 10;

    // Different containers
    
    // Check for vector
    std::cout << "Vector:" << std::endl;
    std::vector<int> vect(size);
    for (int i = 0; i < size; ++i) {
        vect[i] = size - i; 
    }
    check(vect, def_comp<int>);

    std::cout << std::endl;

    // Check for deque
    std::cout << "Deque:" << std::endl;
    std::deque<int> deq(size);
    for (int i = 0; i < size; ++i) {
        deq[i] = size - i; 
    }
    check(deq, def_comp<int>);

    std::cout << std::endl;

    // List => error

    # if 0
    std::cout << "List:" << std::endl;
    std::list<int> lst;
    for (int i = 0; i < size; ++i) {
        lst.push_back(i);
    }
    check(lst, def_comp<int>);
    #endif
    
    // Different comparators

    std::cout << "Lambda:" << std::endl;

    std::vector<int> vect_1(size);
    for (int i = 0; i < size; ++i) {
        vect_1[i] = size - i; 
    }

    check(vect_1,  [](int a, int b) { return a < b; });

    std::cout << std::endl;

    std::cout << std::endl;

    std::cout << "Object():" << std::endl;

    std::vector<int> vect_2(size);
    for (int i = 0; i < size; ++i) {
        vect_2[i] = size - i; 
    }

    check(vect_2, Comparator());

    std::cout << std::endl;

    // Sort time comparasion: vector vs deque

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 10); 

    // Random generation
    std::vector<int> r_vect(size);
    for (int i = 0; i < size; ++i) {
        r_vect[i] = distr(gen);
    }

    std::deque<int> r_deq(size);
    for (int i = 0; i < size; ++i) {
        r_deq[i] = distr(gen);
    }

    measure_sort(r_vect, "vector"); //  2 microseconds
    measure_sort(r_deq, "deque"); // 5 microseconds
     
    return 0;
}
