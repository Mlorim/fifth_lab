#include <array>
#include <tuple>
#include <utility>
#include <cassert>
#include <iostream>

// Size calculation
template<typename... Arrays>
struct product_size;

// Default case
template<>
struct product_size<> {
    static constexpr size_t value = 1;
};

// General case
template<typename First, typename... Rest>
struct product_size<First, Rest...> {
    static constexpr size_t value = 
        std::tuple_size_v<First> * product_size<Rest...>::value;
};

// Print tuple
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


namespace detail {
    // Default case: one arr
    template<size_t I, typename Array>
    constexpr auto build_tuple(const Array& arr) {
        return std::make_tuple(arr[I]);
    }

    // General case
    template<size_t I, typename FirstArray, typename... RestArrays>
    constexpr auto build_tuple(const FirstArray& first, const RestArrays&... rest) {
        constexpr size_t rest_product = product_size<RestArrays...>::value;
        constexpr size_t first_size = std::tuple_size_v<FirstArray>;
        constexpr size_t current_dim = rest_product;
        
        return std::tuple_cat(
            std::make_tuple(first[I / current_dim]),
            build_tuple<I % current_dim>(rest...)
        );
    }

    // Array generation
    template<typename Tuple, size_t... Is, typename... Arrays>
    constexpr auto generate_array(std::index_sequence<Is...>, const Arrays&... arrays) {
        return std::array<Tuple, sizeof...(Is)>{ build_tuple<Is>(arrays...)... };
    }
}

// Cartesian product calculation
template<typename... Arrays>
constexpr auto cartesian_product(const Arrays&... arrays) {
    using TupleType = decltype(detail::build_tuple<0>(arrays...));
    return detail::generate_array<TupleType>(
        std::make_index_sequence<product_size<Arrays...>::value>{},
        arrays...
    );
}

// Test
int main() {
    constexpr std::array<int, 3> arr1{1, 2, 3};
    constexpr std::array<char, 2> arr2{'a', 'b'};
    constexpr std::array<char, 2> arr3{'o', 't'};

    constexpr auto result = cartesian_product(arr1, arr2, arr3);

    std::cout << "Декартово произведение:\n";
    for (const auto& tuple : result) {
        print_tuple(tuple);
        std::cout << "\n";
    }
    return 0;
}