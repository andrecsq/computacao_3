#include <iostream>
#include <vector>

using namespace std;

template<int N = 2, typename T = double>
class Vetor {
    public:

        Vetor() {
            for(int i = 0; i < N; i++)
                tab.push_back(0);
            tab.resize(N);
        }

        class AtribuicaoVirgula {
            public:
                AtribuicaoVirgula operator ,(T n) {
                    *vetor = n;
                    return *this;
                }

            private:
                Vetor<N, T> *vetor;

                AtribuicaoVirgula(Vetor<N, T>* v): vetor(v) {}

                friend class Vetor;
        };

        class middleCrossProduct {
            public:

                Vetor<N, T> getVetor() const {
                    return *vetor;
                }

            private:
                Vetor<N, T> *vetor;

                middleCrossProduct(Vetor<N, T>* v): vetor(v) {}

                friend class Vetor;
        };

        void insertValue(T x) {
            if(curr < N) {
                tab[ curr++ ] = x;
            } 
        }

        AtribuicaoVirgula operator = (T n) {
            insertValue(n);
            return AtribuicaoVirgula(this);
        }

        middleCrossProduct operator * () {
            return middleCrossProduct(this);
        }

        T getElement(unsigned int i) const {
            return tab.at(i);
        }

        void custom_print(ostream& o) const {
            for(auto x : tab) {
                o << x << " ";
            }
        }

    private:
        vector<T> tab;
        unsigned curr = 0;
};

template<int N = 2, typename T1 = double, typename T2 = double>
Vetor<N, double> addSubtract(const Vetor<N, T1> a, const Vetor<N, T2> b, bool soma = true) {
    Vetor<N, double> result;

    for(unsigned int i = 0; i < N; i++) {
        result.insertValue(a.getElement(i) + (soma ? b.getElement(i) : -b.getElement(i)));
    }

    return result;
}

template<int N = 2, typename T1 = double, typename T2 = double>
Vetor<N, double> scalarDivision(double scalar, const Vetor<N, T2> b, bool multi = true) {
    Vetor<N, double> result;

    if(multi) {
        for(unsigned int i = 0; i < N; i++) {
            result.insertValue(scalar * b.getElement(i));
        }
    } else {
        for(unsigned int i = 0; i < N; i++) {
            result.insertValue(scalar / b.getElement(i));
        }
    }

    return result;
}

template<int N = 2, typename T1 = double, typename T2 = double>
double innerProduct(const Vetor<N, T1> a, const Vetor<N, T2> b) {
    double produto = 0;

    for(unsigned int i = 0; i < N; i++) {
        produto += (a.getElement(i) * b.getElement(i));
    }

    return produto;
}

template<int N = 2, typename T1 = double, typename T2 = double>
Vetor<N, double> crossProduct(const Vetor<N, T1> a, const Vetor<N, T2> b) {
    Vetor<N, double> produto;
    double temp;

    // Eixo x
    temp = (a.getElement(1) * b.getElement(2)) - (a.getElement(2) * b.getElement(1));
    produto.insertValue(temp);
    // Eixo Y
    temp = (a.getElement(2) * b.getElement(0)) - (a.getElement(0) * b.getElement(2));
    produto.insertValue(temp);
    // Eixo z
    temp = (a.getElement(0) * b.getElement(1)) - (a.getElement(1) * b.getElement(0));
    produto.insertValue(temp);

    return produto;
}

template<int N = 2, typename T1 = double, typename T2 = double>
Vetor<N, double> operator + (const Vetor<N, T1> a, const Vetor<N, T2> b) {
    return addSubtract(a, b);
}

template<int N = 2, typename T1 = double, typename T2 = double>
Vetor<N, double> operator - (const Vetor<N, T1> a, const Vetor<N, T2> b) {
    return addSubtract(a, b, false);
}

template<int N = 2, typename T1 = double, typename T2 = double>
Vetor<N, double> operator * (const double a, const Vetor<N, T2> b) {
    return scalarDivision(a, b);
}

template<int N = 2, typename T1 = double, typename T2 = double>
Vetor<N, double> operator * (const Vetor<N, T2> b, const double a) {
    return scalarDivision(a, b);
}

template<int N = 2, typename T1 = double, typename T2 = double>
double operator * (const Vetor<N, T1> a, const Vetor<N, T2> b) {
    return innerProduct(a, b);
}

template<int N = 2, typename T1 = double, typename T2=double>
Vetor<N, double> operator * (const Vetor<N, T1> a, const typename Vetor<N, T2>::middleCrossProduct b) {
    return crossProduct(a, b.getVetor());
}

template<int N = 2, typename T = double>
ostream& operator << (ostream& o, const Vetor<N, T>& v) {
    v.custom_print(o);
    return o;
}

int main(int argc, char *argv[]) {
    Vetor<3, double> v1, v2;

    v1 = 2.1, 4.2, 7.2;
    v2 = 87, 59, 34;

    cout << v1 + v2 << endl;

    cout << v1 - v2 << endl;

    cout << 3 * v1 << endl;

    cout << v1 * 3 << endl;

    cout << v1 * v2 << endl;

    cout << v1 ** v2 << endl;

    return 0;
}