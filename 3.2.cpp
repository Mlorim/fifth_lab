#include <array>
#include <iostream>
#include <type_traits>
#include <utility>


template <
    typename T, 
    size_t K, 
    const std::array<T, K>& Init, 
    const std::array<T, K>& Coeff, 
    size_t N, 
    typename = void
>
struct LinearRecurrence;

// N <= n-1
template <typename T, size_t K, const std::array<T, K>& Init, const std::array<T, K>& Coeff, size_t N>
struct LinearRecurrence<T, K, Init, Coeff, N, std::enable_if_t<(N < K)>> {
    static constexpr T value = Init[N];
};

// N >= n
template <typename T, size_t K, const std::array<T, K>& Init, const std::array<T, K>& Coeff, size_t N>
struct LinearRecurrence<T, K, Init, Coeff, N, std::enable_if_t<(N >= K)>> {
    template <size_t... Is>
    static constexpr T sum(std::index_sequence<Is...>) {
        return ((Coeff[Is] * LinearRecurrence<T, K, Init, Coeff, N - Is - 1>::value) + ...);
    }

    static constexpr T value = sum(std::make_index_sequence<K>{});
};

template <typename T, size_t K, const std::array<T, K>& Init, const std::array<T, K>& Coeff, size_t N>
constexpr T computeElement() {
    return LinearRecurrence<T, K, Init, Coeff, N>::value;
}


constexpr std::array<int, 3> Init{1, 2, 3};
constexpr std::array<int, 3> Coeff{1, 1, 1};


int main() {
    constexpr int N = 4;
    constexpr int result = computeElement<int, 3, Init, Coeff, N>();
    std::cout << result << std::endl; //11
}