#include <iostream>

using namespace std;

class PilhaInt{
  public:
    PilhaInt(int capacidade = 10){
      this->topo = 0;
      this->cap = capacidade;
    	this->pilha = (int*) malloc(capacidade * sizeof(int));
    }
    PilhaInt(const PilhaInt& p){
      this->topo = 0;
      this->cap = p.cap;
      this->pilha = (int*) malloc(p.cap * sizeof(int));

      for (int i = 0; i < p.topo; i++)
        this->empilha(p.pilha[i]);
    }
    ~PilhaInt(){
      free(this->pilha);
    }

    void empilha( int valor_a_ser_empilhado ) {
      if (this->topo == this->cap)
        redimensiona(2 * this->cap);
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
      if (n < this->topo)
        this->topo = n;
      this->cap = n;
      this->pilha = (int *) realloc(this->pilha, n * sizeof(int));      
    }

    int desempilha() {
    	return pilha[--topo];
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
      int* nova_pilha = (int*) malloc(p.cap * sizeof(int));
      for (int i = 0; i < p.topo; i++)
        nova_pilha[i] = p.pilha[i];

      if (this->pilha != NULL)
        free(this->pilha);

      this->pilha = nova_pilha;
      this->cap = p.cap;
      this->topo = p.topo;

      return true;
    }

  private:
    int* pilha;
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
