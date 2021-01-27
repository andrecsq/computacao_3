#include <initializer_list>
#include <iostream>
#include <memory>

using namespace std;

int x = 1;

class Leak {
    public:
        Leak(){
            val = x++;
            cout << "Construiu " << val << endl;
        }
        ~Leak(){
            cout << "Destruiu " << val << endl;
        }

        int val;
};

std::ostream &operator<<(std::ostream &os, Leak const &m) { 
    return os << m.val;
}

class AbstractPair {
    public:
        virtual void imprime( std::ostream& o ) = 0;
        virtual ~AbstractPair() {};
};


template <typename A, typename B>
class ImplPair: public AbstractPair {
    public:
        ImplPair( const A& a, const B& b ) : p(a, b) { }

        ~ImplPair() { }

        void imprime( std::ostream& o ) {
            o << this->p.first << " = " << this->p.second << std::endl;
        }

    private:
        std::pair<A, B> p;
};

class Pair {
    public:
        template <typename A, typename B>
        Pair( A a, B b ){ 
            std::shared_ptr<ImplPair<A,B>> ip( new ImplPair<A,B>(a, b) ); 
            p = ip;
        }

        void imprime( std::ostream& o ) {            
            p->imprime(o);
        }

    private:
        std::shared_ptr<AbstractPair> p;
};

void print( std::ostream& o, const std::initializer_list<Pair>& lista ) {
    for (auto i : lista)
        i.imprime(o);
}

int main() {
  
    print( cout, { { "jan", 1 }, { 2, "fev" }} );

    // print( cout, { { Leak(), 1 } } );

  return 0;  
}