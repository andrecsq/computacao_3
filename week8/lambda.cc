#include <algorithm>
#include <functional>
#include <type_traits>
#include <vector>
#include <iostream>

using namespace std;

class X {
    public:
        template <typename T1>
        T1 operator() (const T1 value) const { return value; }
};

template <typename T1>
class Constant {
    public:
        Constant(const T1& value) : value(value) {}

        template<typename T2>
        T1 operator() (T2 _) const { return value; } 

    private:
        T1 value;
};

template <typename T1, typename T2> 
class Add {
    public:
        Add(T1 x, T2 y) : x(x), y(y) {}

        template<typename K>
        K operator() (K value) { return x(value) + y(value); }

    private:
        T1 x;
        T2 y;
};

template <typename T1, typename T2> 
class Multiply {
    public:
        Multiply(T1 x, T2 y) : x(x), y(y) {}

        template<typename K>
        K operator() (K value) { return x(value) * y(value); }

    private:
        T1 x;
        T2 y;
};

template <typename T1, typename T2> 
class Modulo {
    public:
        Modulo(T1 x, T2 y) : x(x), y(y) {}

        template<typename K>
        K operator() (K value) { return x(value) % y(value); }

    private:
        T1 x;
        T2 y;
};

template <typename T1, typename T2> 
class IsEqual {
    public:
        IsEqual(T1 x, T2 y) : x(x), y(y) {}

        template<typename K>
        bool operator() (K value) { return x(value) == y(value); }

    private:
        T1 x;
        T2 y;
};

template <typename T1, typename T2>
class Ostream {
    public:
        Ostream(T1 &t1, T2 t2) : t1(t1), t2(t2) {}

        template<typename K>
        void operator() (K value) { t1 <<t2(value) <<buffer; }

        void include(const char c) { buffer += c; }

    private:
        T1 &t1;
        T2 t2;
        std::string buffer = "";
};

// *

template <typename T1, typename T2,
typename std::enable_if_t<std::is_arithmetic_v<T1>, int> = 0>
auto operator * (T1 x, T2 y) { return Multiply<Constant<T1>, T2>(x, y); }

template <typename T1, typename T2,
typename std::enable_if_t<std::is_arithmetic_v<T2>, int> = 0>
auto operator * (T1 x, T2 y) { return Multiply<T1, Constant<T2> >(x, y); }

template <typename T1, typename T2,
typename std::enable_if_t<!std::is_arithmetic_v<T1> && !std::is_arithmetic_v<T2>, int> = 0>
auto operator * (T1 x, T2 y) { return Multiply<T1, T2 >(x, y); }

// +

template <
    typename T1, 
    typename T2,
    typename std::enable_if_t<std::is_arithmetic_v<T1>, int> = 0
>
auto operator + (T1 x, T2 y) { return Add<Constant<T1>, T2 >(x, y); }

template <
    typename T1, 
    typename T2,
    typename std::enable_if_t<std::is_arithmetic_v<T2>, int> = 0
>
auto operator + (T1 x, T2 y) { return Add<T1, Constant<T2> >(x, y); }

template <
    typename T1, 
    typename T2,
    typename std::enable_if_t<!std::is_arithmetic_v<T2> && !std::is_arithmetic_v<T2>, int> = 0
>
auto operator + (T1 x, T2 y) { return Add<T1, T2>(x, y); }

// %

template <typename T1, typename T2,
typename std::enable_if_t<std::is_integral_v<T1>, int> = 0>
auto operator % (T1 x, T2 y) { return Modulo<Constant<int>, T2>(x, y); }

template <typename T1, typename T2,
typename std::enable_if_t<std::is_integral_v<T2>, int> = 0>
auto operator % (T1 x, T2 y) { return Modulo<T1, Constant<int> >(x, y); }

template <typename T1, typename T2,
typename std::enable_if_t<!std::is_integral_v<T1> && !std::is_integral_v<T2>, int> = 0>
auto operator % (T1 x, T2 y) { return Modulo<T1, T2>(x, y); }

// ==

template <typename T1, typename T2,
typename std::enable_if_t<std::is_arithmetic_v<T1>, int> = 0>
auto operator == (T1 x, T2 y) { return IsEqual<Constant<T1>, T2>(x, y); }

template <typename T1, typename T2,
typename std::enable_if_t<std::is_arithmetic_v<T2>, int> = 0>
auto operator == (T1 x, T2 y) { return IsEqual<T1, Constant<T2> >(x, y); }

template <typename T1, typename T2,
typename std::enable_if_t<!std::is_arithmetic_v<T1> && !std::is_arithmetic_v<T2>, int> = 0>
auto operator == (T1 x, T2 y) { return IsEqual<T1, T2>(x, y); }

// <<

template <typename T1, typename T2>
auto operator <<(ostream& x, Add<T1, T2> y) { return Ostream<ostream, Add<T1, T2> >(x, y); }

template <typename T1, typename T2>
auto operator <<(ostream& x, Multiply<T1, T2> y) { return Ostream<ostream, Multiply<T1, T2> >(x, y); }

template <typename T1, typename T2>
auto operator <<(ostream& x, Modulo<T1, T2> y) { return Ostream<ostream, Modulo<T1, T2> >(x, y); }

template <typename T1, typename T2>
auto operator <<(ostream& x, IsEqual<T1, T2> y) { return Ostream<ostream, IsEqual<T1, T2> >(x, y); }

auto operator <<(ostream& o, X x) { return Ostream<ostream, X>(o, x); }

// Cannot change std::
template <typename T1,
typename std::enable_if_t<std::is_member_function_pointer_v<decltype(&T1::include)>, int> = 0>
auto operator <<(T1 t1, const char c) {
    t1.include(c);
    return t1;
}

// |

template <typename Vector, typename Func>
auto operator | (const Vector& v , Func f) {
    if constexpr (is_void_v<invoke_result_t<Func, decltype(*begin(v))> >) {
        for_each(begin(v), end(v), [&f](auto x){ invoke(f, x); });
    } else if constexpr (is_same_v<invoke_result_t<Func, decltype(*begin(v))>, bool >) {
        vector<decay_t<decltype(*begin(v))> > result;
        int count = 0;
        for_each(begin(v), end(v),
            [&f, &result, &count](auto x){ if(invoke(f, x)) { result.push_back(x); count++; } });
        result.resize(count);
        return result;
    } else {
        vector<decay_t<invoke_result_t<Func, decltype(*begin(v))> > > result;
        int count = 0;
        for_each(begin(v), end(v),
            [&f, &result, &count](auto x){ result.push_back(invoke(f, x)); count++; });
        result.resize(count);
        return result;
    }
}

X x;

int main() {

    // vector<int> v1 = { 1, 2, 3, 4, 1, 1, 0, 8 };

    // v1 | (x % 2 == 0) | cout <<x*x + 1 <<' ';



    string v1[] = { "a", "b2", "cc3", "saci" };

    v1 | [](string n) { return n.length() % 2 == 0; } | (x+x) | cout <<x <<' ';


    // int tab[10] =  { 1, 2, 3, 2, 3, 4, 6, 0, 1, 8 };
    // vector<int> v;
    // tab | cout <<x*x <<'\n';
    // tab | [ &v ](int x) { v.push_back(x); };
    // v | x % 2 == 0 | cout <<x*x + 1 <<'\n';
    // v | x % 2 == 1 | x*x | cout <<x <<'\n';
}