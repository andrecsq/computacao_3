#include <vector>
#include <iostream>
#include <map>
#include <string>

template <typename Type, typename Function>
auto apply(const Type& v, Function applied_function){

    std::vector<decltype(applied_function(v[0]))> new_vector;
    for (auto i : v)
        new_vector.push_back(applied_function(i));

    return new_vector;
}

template <typename Type, typename Function>
auto apply(const std::initializer_list<Type>& v, Function applied_function){

    std::vector<decltype(applied_function(v.begin()[0]))> new_vector;
    for (auto i : v)
        new_vector.push_back(applied_function(i));

    return new_vector;
}

using namespace std;

string roman( int n ) {
    map<int,string> rom = { { 1, "I" }, { 2, "II" }, { 3, "III" }, { 4, "IV" }, { 5, "V" }, { 6, "VI" }, { 7, "VII" }, { 8, "VIII" } } ; 

    return rom[n]; 
}

int main(){

    cout << ::apply( { 1, 2, 3, 4, 5 }, roman );

}