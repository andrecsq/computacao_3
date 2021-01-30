#include <iostream>

#define MAX_PILHA 10

using namespace std;

class PilhaInt{
    public:
        PilhaInt(){
            topo_da_pilha = 0;
        }

        void empilha( int valor_a_ser_empilhado ) {
            tab[topo_da_pilha++] = valor_a_ser_empilhado;
        }

        int desempilha() {
            return tab[--topo_da_pilha];
        }

        void print( std::ostream& o ) const {
            o << "[ ";
            for( int i = 0; i < topo_da_pilha; i++ )
                o << tab[i] << ((i < (topo_da_pilha - 1)) ? ", " : " ]");
        }

        PilhaInt& operator << ( int valor_a_ser_empilhado ) {
            empilha( valor_a_ser_empilhado );
            return *this;
        }

        bool operator = ( const PilhaInt& p ) {
            while (this->topo_da_pilha != 0){
                this->desempilha();
            }
            for (int i = 0; i < p.topo_da_pilha; i++){
                this->empilha(p.tab[i]);
            }
            return true;
        }

    private:
        int tab[MAX_PILHA];
        int topo_da_pilha;
};

int main(){
    PilhaInt p, q;
    q << 2;
    p << 19 << 18 << 17 << 30;
    q = p;
    p.desempilha();
    q << 7;
    q.print(cout);
    cout << endl;
    p.print(cout);
}
