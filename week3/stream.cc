#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print(int x) {
	cout << x << endl;
}

template<typename T1, int sz, class Function>
void operator | (const T1(& v)[sz], Function f) {
	vector<T1> temp (v, v + sz);
	for_each(temp.begin(), temp.end(), f);
} 

template<typename T1, typename T2, template<typename, typename> class C, class Function>
void operator | (C<T1, T2> v, Function f) {
	for_each(v.begin(), v.end(), f);
} 

int main(void) {
    int tab[10] =  {1, 2, 3, 2, 3, 4, 6, 0, 1, 8};
    vector<int> v{2 , 6, 8};

    tab | [](int x) {cout << x*x << " ";};    
    cout << endl;

    tab | [ &v ](int x) {v.push_back(x);};
    cout << endl;

    v | [](int x) {cout << x*x << " ";};
    v | &print;
	return 0;
}