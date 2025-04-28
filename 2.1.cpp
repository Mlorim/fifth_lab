#include <iostream>
#include <tuple>
#include <type_traits>

// Default case
template <size_t Index = 0, typename... TupleArgs>
typename std::enable_if<Index == sizeof...(TupleArgs), void>::type
print_tuple(const std::tuple<TupleArgs...>&) {}

// General case
template <size_t Index = 0, typename... TupleArgs>
typename std::enable_if<Index < sizeof...(TupleArgs), void>::type
print_tuple(const std::tuple<TupleArgs...>& tp) {
    
    std::cout << std::get<Index>(tp) << " ";
    
    print_tuple<Index + 1>(tp);
}

int main() {
    std::tuple<int, float, std::string, char> my_tuple(42, 4.2, "Fourty two", '4');

    print_tuple(my_tuple);
    return 0;
}