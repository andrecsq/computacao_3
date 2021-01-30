#include <iostream>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

void print(int x) {
	cout << x << " ";
}

template<typename T1, int sz, class Function>
auto operator | (const T1(& v)[sz], Function f) {
	vector<T1> temp;
  
    for_each( begin( v ), end( v ),
        [f, &temp]( auto x ) mutable { if( f(x) ){ temp.push_back(x); } } );

	return temp;
} 

template<typename T1, typename T2, template<typename, typename> class C, class Function>
auto operator | (const C<T1, T2> &v, Function f) {

    if constexpr ( is_same_v<invoke_result_t<decltype(f), decltype(*v.begin())> , bool>) {
        vector<T1> temp;

        for_each( v.begin(), v.end(), 
            [f, &temp]( auto x ) mutable { if( f(x) ) { temp.push_back(x); } } );

        return temp;
    } else if constexpr ( is_same_v< invoke_result_t<decltype(f), decltype(*v.begin())>, pair<string, double> > ) {
        vector< pair<string, double> > temp;
        
        for_each( begin( v ), end( v ),
            [f, &temp]( auto x ) mutable { temp.push_back( f(x) ); } );

        return temp;
    } else if constexpr ( is_same_v<invoke_result_t<decltype(f), decltype(*v.begin())>, string> ) {
        vector<string> temp;

        for_each( begin( v ), end( v ),
            [f, &temp]( auto x ) mutable { temp.push_back(f(x)); } );

        return temp;
    } else {
        for_each(v.begin(), v.end(), f);
        return;
    }
} 

template<
    typename T1, 
    typename T2, 
    typename T3, 
    typename T4, 
    template<typename, typename, typename, typename> class C, 
    class Function
>
auto operator | (const C<T1, T2, T3, T4> &v, Function f) {

    if constexpr ( is_same_v<invoke_result_t<decltype(f), decltype(*v.begin())> , bool>) {
        vector<pair<T1, T2>> temp;

        for_each( v.begin(), v.end(), 
            [f, &temp]( auto x ) mutable { if( f(x) ) { temp.push_back(x); } } );

        return temp;
    } else if constexpr ( is_same_v<invoke_result_t<decltype(f), decltype(*v.begin())> , pair<string, double>> ){
        vector<pair<string, double>> temp;
        
        for_each( begin( v ), end( v ),
            [f, &temp]( auto x ) mutable { temp.push_back( f(x) ); }
        );

        return temp;
    }
} 

auto operator | ( const vector<string> &v, decltype(&string::length) f ) {
  vector<int> temp;

  for_each( begin( v ), end( v ),
    [f, &temp]( auto x ) mutable { temp.push_back(x.length()); }
  );

  return temp;
}

int main(void) {
    vector<string> v1 = { "janeiro", "fevereiro", "março", "abril", "maio" };
    v1 | &string::length | [] ( int x ) { cout << x << " "; };

	return 0;
}

/*
    test3 :
    int v1[] = { 2, 9, 8, 8, 7, 4 };
        auto result = v1 | []( int x ) { return x % 2 == 0; };
        for( auto x : result )
            cout << x << " ";

    test6:
    map<string,string> v = { { "a", "1" }, { "b", "2" }, { "c", "3" }, { "d", "4" }, { "e", "5" }, { "f", "6" } };
    auto par = []( auto x ){ return stoi( x.second ) % 2 == 0; };
    auto upper_first = []( auto x ){ string st = x.first; transform(st.begin(), st.end(), st.begin(), ::toupper); return st; };
    v | par | upper_first | []( auto st ) { cout << st + "! "; };

    test8:
    vector<string> v1 = { "janeiro", "fevereiro", "março", "abril", "maio" };
    v1 | &string::length | [] ( int x ) { cout << x << " "; };
*/