#include <iostream>
#include <vector>

using namespace std;

class PilhaInt{
  public:
    PilhaInt(int capacidade = 10){
        this->topo = 0;
        this->cap = capacidade;
        this->pilha.resize(capacidade);
    }

    PilhaInt(const PilhaInt& p){
        *this = p;
    }

    void empilha( int valor_a_ser_empilhado ) {
        if (this->topo == this->cap)
            redimensiona(2*this->cap);
        pilha[this->topo] = valor_a_ser_empilhado;
        this->topo = this->topo + 1;
    }

    int capacidade(){
        return this->cap;
    }

    int get_topo(){
        return this->topo;
    }

    void redimensiona(int n){
        this->pilha.resize(n);
        this->cap = n;
        if (n < this->topo)
            this->topo = n;
    }

    int desempilha() {
    	return this->pilha[--this->topo];
    }

    void print( std::ostream& o ) const {
        o << "[ ";
        for( int i = 0; i < this->topo; i++ )
            o << this->pilha[i] << ((i < (this->topo - 1)) ? ", " : " ]");
    }

    PilhaInt& operator << ( int valor_a_ser_empilhado ) {
        empilha( valor_a_ser_empilhado );
        return *this;
    }

    bool operator = ( const PilhaInt& p ) {
        vector<int> nova_pilha = p.pilha;

        this->pilha = nova_pilha;
        this->cap = p.cap;
        this->topo = p.topo;

        return true;
    }

    private:
        std::vector<int> pilha;
        int topo;
        int cap;
};

PilhaInt embaralha( PilhaInt q ) {
    int aux = q.desempilha();
    q << 32 << 9 << aux;
    
    return q;
}

int main(){
    PilhaInt a{3};
    for( int i = 0; i < 20; i++ ) {
        a << i;
        cout << a.capacidade() << " ";
    }
    cout << endl;
    a.print( cout ); cout << endl;
}
