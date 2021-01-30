#include <initializer_list>
#include <iostream>

using namespace std;

class AbstractPair {
    public:
        virtual void imprime( std::ostream& o ) = 0;
};


template <typename A, typename B>
class ImplPair: public AbstractPair {
    public:
        ImplPair( A a, B b ) {
            this->a = a;
            this->b = b;
        }

        void imprime( std::ostream& o ) {
            o << this->a << " = " << this->b << std::endl;
        }

    private:
        A a;
        B b;
};

class Pair {
    public:
        template <typename A, typename B>
        Pair( A a, B b ) {
            ImplPair<A,B>* ip = new ImplPair<A,B>(a, b);
            p = ip;
        }

        void imprime( std::ostream& o ) {
            
            p->imprime(o);
        }
    private:
        AbstractPair *p;
};

void print( std::ostream& o, std::initializer_list<Pair> lista ) {
    for (auto i : lista)
        i.imprime(o);
}

int main() {
 
    Pair p( "1", 2 );

    // p.imprime( cout );

    print(cout, { p });
  
    print( cout, { { "jan", 1 }, { 2, "fev" }, { string( "pi" ), 3.14 } } );

  return 0;  
}