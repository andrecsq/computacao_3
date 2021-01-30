#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

using namespace std;

class Var {
    public:
        class Erro: public exception {
            public:
                Erro( string msg ): msg( msg ) {}

                string operator()() { return msg; }

                string what() { return msg; }
            
            private:
                string msg;
        };
        
        class Undefined {};

        class Object {
            public:
                virtual ~Object() {}
        
                virtual void print( ostream& o ) const { 
                    o << "object";
                }
        
                virtual Var func( const Var& xrg ) const {
                    throw Erro( "Essa variável não pode ser usada como função" ); 
                }  
        
                Var& lvalue( const string& st ) { return atr[st]; }
                Var rvalue( const string& st ) const {
                    if( auto x = atr.find( st ); x != atr.end() )
                        return x->second;
                    else
                        return Var(); 
                }
        
            private:
                map<string,Var> atr; 
        };
    
    private:
        enum { UNDEFINED, BOOL, CHAR, INT, DOUBLE, STRING, OBJECT };

        typedef variant<Undefined,bool,char,int,double,string,shared_ptr<Object>> Variant;

        template <class F>
        class Function: public Object {
            public:
                Function( F f ): f(f) {}

                void print( ostream& o ) const {
                    o << "function"; 
                }
            
                Var func( const Var& arg ) const {
                    return invoke( f, arg );
                }  
        
            private:
                F f;
        };
    
    public:
  
        Var(): v() {}
        Var( const char* st ): v( string(st) ) {}
        Var( bool v ): v(v) {}
        Var( char v ): v(v) {}
        Var( int v ): v(v) {}
        Var( double v ): v(v) {}
        Var( string v ): v(v) {}
        Var( Object* v ): v( shared_ptr<Object>( v ) ) {}

        template <typename T>
        Var( T func ): v( shared_ptr<Object>( new Function( func ) ) ) {}
  
        void print( ostream& o ) const {
            switch( v.index() ) {
                case UNDEFINED: o << "undefined"; break;
                case BOOL: o << (get<BOOL>( v ) ? "true" : "false"); break;
                case CHAR: o << get<CHAR>( v ); break;
                case INT: o << get<INT>( v ); break;
                case DOUBLE: o << get<DOUBLE>( v ); break;
                case STRING: o << get<STRING>( v ); break;
                case OBJECT: get<OBJECT>( v )->print( o ); break;
    
                default:
	                throw Erro( "Undefined type" );
            }
        }
  
        static constexpr int tipo( int x, int y ) { return (x << 4) | y; }

        Var operator()( Var arg ) const {
            if( v.index() != OBJECT )
                throw Erro( "Essa variável não pode ser usada como função" );

            return get<OBJECT>( v )->func( arg );
        }

        Var& operator[]( const string& st ) {
            if( v.index() != OBJECT )
                throw Erro( "Essa variável não é um objeto" );

            return get<OBJECT>( v )->lvalue( st );
        }

        Var operator[]( const string& st ) const {
            if( v.index() != OBJECT )
                throw Erro( "Essa variável não é um objeto" );

            return get<OBJECT>( v )->rvalue( st );
        }

        static Var var_not( const Var& x ) {
            switch( x.v.index() ) {
                case BOOL: return !get<BOOL>( x.v );

                default:
                    return Var();
            }
        }

        static Var var_and( const Var& x, const Var& y ) {
            switch( tipo( x.v.index(), y.v.index() ) ) {
                case tipo( BOOL, BOOL ): return get<BOOL>( x.v ) && get<BOOL>( y.v );

                default:
                    return Var();
            }
        }

        static Var var_or( const Var& x, const Var& y ) {
            switch( tipo( x.v.index(), y.v.index() ) ) {
                case tipo( BOOL, BOOL ): return get<BOOL>( x.v ) || get<BOOL>( y.v );

                default:
                    return Var();
            }
        }
  
        static Var var_sum( const Var& x, const Var& y ) {
            switch( tipo( x.v.index(), y.v.index() ) ) {
                case tipo( INT, INT ): return get<INT>( x.v ) + get<INT>( y.v );
                case tipo( CHAR, CHAR ): return string("") + get<CHAR>( x.v ) + get<CHAR>( y.v );
                case tipo( DOUBLE, DOUBLE ): return get<DOUBLE>( x.v ) + get<DOUBLE>( y.v );
                case tipo( STRING, STRING ): return get<STRING>( x.v ) + get<STRING>( y.v );

                case tipo( DOUBLE, INT ): return get<DOUBLE>( x.v ) + get<INT>( y.v );
                case tipo( INT, DOUBLE ): return get<INT>( x.v ) + get<DOUBLE>( y.v );

                case tipo( INT, CHAR ): return get<INT>( x.v ) + get<CHAR>( y.v );
                case tipo( CHAR, INT ): return get<CHAR>( x.v ) + get<INT>( y.v );

                case tipo( STRING, CHAR ): return get<STRING>( x.v ) + get<CHAR>( y.v );
                case tipo( CHAR, STRING ): return get<CHAR>( x.v ) + get<STRING>( y.v );
      
                default:
                    return Var();
            }
        }

        static Var var_mult( const Var& x, const Var& y ) {
            switch( tipo( x.v.index(), y.v.index() ) ) {
                case tipo( INT, INT ): return get<INT>( x.v ) * get<INT>( y.v );
                case tipo( DOUBLE, DOUBLE ): return get<DOUBLE>( x.v ) * get<DOUBLE>( y.v );

                case tipo( DOUBLE, INT ): return get<DOUBLE>( x.v ) * get<INT>( y.v );
                case tipo( INT, DOUBLE ): return get<INT>( x.v ) * get<DOUBLE>( y.v );
      
                default:
                    return Var();
            }
        }

        static Var var_div( const Var& x, const Var& y ) {
            switch( tipo( x.v.index(), y.v.index() ) ) {
                case tipo( INT, INT ): return get<INT>( x.v ) / get<INT>( y.v );
                case tipo( DOUBLE, DOUBLE ): return get<DOUBLE>( x.v ) / get<DOUBLE>( y.v );

                case tipo( DOUBLE, INT ): return get<DOUBLE>( x.v ) / get<INT>( y.v );
                case tipo( INT, DOUBLE ): return get<INT>( x.v ) / get<DOUBLE>( y.v );
      
                default:
                    return Var();
            }
        }

        static Var var_gt( const Var& x, const Var& y ) {
            string t1, t2;
            switch( tipo( x.v.index(), y.v.index() ) ) {
                case tipo( BOOL, BOOL ): return get<BOOL>( x.v ) > get<BOOL>( y.v );
                case tipo( INT, INT ): return get<INT>( x.v ) > get<INT>( y.v );
                case tipo( CHAR, CHAR ): return get<CHAR>( x.v ) > get<CHAR>( y.v );
                case tipo( DOUBLE, DOUBLE ): return get<DOUBLE>( x.v ) > get<DOUBLE>( y.v );
                case tipo( STRING, STRING ):
                    t1 = get<STRING>( x.v );
                    t2 = get<STRING>( y.v );
                    return t1.compare(t2) > 0 ? true : false;

                case tipo( DOUBLE, INT ): return get<DOUBLE>( x.v ) > get<INT>( y.v );
                case tipo( INT, DOUBLE ): return get<INT>( x.v ) > get<DOUBLE>( y.v );

                case tipo( INT, CHAR ): return get<INT>( x.v ) > get<CHAR>( y.v );
                case tipo( CHAR, INT ): return get<CHAR>( x.v ) > get<INT>( y.v );

                case tipo( STRING, CHAR ):
                    t1 = get<STRING>( x.v );
                    t2 = to_string(get<CHAR>( y.v ));
                    return t1.compare(t2) > 0 ? true : false;

                case tipo( CHAR, STRING ):
                    t1 = to_string(get<CHAR>( x.v ));
                    t2 = get<STRING>( y.v );
                    return t1.compare(t2) > 0 ? true : false;


      
                default:
                    return Var();
            }
        }

        static Var var_equals( const Var& x, const Var& y ) {
            string t1, t2;
            switch( tipo( x.v.index(), y.v.index() ) ) {
                case tipo( BOOL, BOOL ): return get<BOOL>( x.v ) == get<BOOL>( y.v );
                case tipo( INT, INT ): return get<INT>( x.v ) == get<INT>( y.v );
                case tipo( DOUBLE, DOUBLE ): return get<DOUBLE>( x.v ) == get<DOUBLE>( y.v );
                case tipo( STRING, STRING ):
                    t1 = get<STRING>( x.v );
                    t2 = get<STRING>( y.v );
                    return t1.compare(t2) == 0 ? true : false;

                case tipo( DOUBLE, INT ): return get<DOUBLE>( x.v ) == get<INT>( y.v );
                case tipo( INT, DOUBLE ): return get<INT>( x.v ) == get<DOUBLE>( y.v );

                case tipo( INT, CHAR ): return get<INT>( x.v ) == get<CHAR>( y.v );
                case tipo( CHAR, INT ): return get<CHAR>( x.v ) == get<INT>( y.v );

                case tipo( STRING, CHAR ):
                    t1 = get<STRING>( x.v );
                    t2 = to_string(get<CHAR>( y.v ));
                    return t1.compare(t2) == 0 ? true : false;

                case tipo( CHAR, STRING ):
                    t1 = to_string(get<CHAR>( x.v ));
                    t2 = get<STRING>( y.v );
                    return t1.compare(t2) == 0 ? true : false;
      
                default:
                    return Var();
            }
        }
  
    private:
        Variant v;
};

ostream& operator << ( ostream& o, const Var& v ) {
    v.print( o );
    return o;
}

inline Var operator + ( const Var& x, const Var& y ) {
    return Var::var_sum( x, y );
}

inline Var operator * ( const Var& x, const Var& y ) {
    return Var::var_mult( x, y );
}

inline Var operator - ( const Var& x, const Var& y ) {
    return x + (-1)*y;
}

inline Var operator / ( const Var& x, const Var& y ) {
    return Var::var_div( x, y );
}

inline Var operator && ( const Var& x, const Var& y ) {
    return Var::var_and( x, y );
}

inline Var operator || ( const Var& x, const Var& y ) {
    return Var::var_or( x, y );
}

inline Var operator ! ( const Var& x ) {
    return Var::var_not( x );
}

inline Var operator > ( const Var& x, const Var& y ) {
    return Var::var_gt( x, y );
}

inline Var operator < ( const Var& x, const Var& y ) { return y>x; }

inline Var operator != ( const Var& x, const Var& y ) { return (x<y) || (y<x); }

inline Var operator == ( const Var& x, const Var& y ) { return !(x!=y); }

inline Var operator >= ( const Var& x, const Var& y ) { return !(x<y); }

inline Var operator <= ( const Var& x, const Var& y ) { return !(y<x); }

Var::Object* newObject() {
    return new Var::Object();
}

Var print( Var a ) {
    cout <<
    "{ nome: " << a["nome"] << ", " <<
    "idade: " << a["idade"](a) << ", " <<
    "nascimento: " << a["nascimento"] << ", " <<
    "print: " << a["print"] <<
    "}" << endl;

    return Var();
}

int main() {
    Var a, b;
    a = newObject();
    b = "José Maria";
    a["nome"] = b;
    a["nascimento"] = 1998;
    b = "Maria José";
    try {
    print( a );
    } catch( Var::Erro e ) {
    cout << "Erro fatal: " << e() << endl;
    }
    cout << a << " " << a["nome"] << " " << a["nascimento"] << endl;


    // Var a, b;

    // cout << a << endl; // saída: "undefined"
    // a = 1;
    // b = 2;
    // cout << a - b << endl;

    // cout << a << endl; // saída: 1
    // a = "hello";
    // cout << a << endl; // saída: hello

    // a = newObject();
    // a["atr"] = 9;
    // a["metodo"] = []( auto x ){ return x*x; };
    // b = 4;
    // cout << a["metodo"]( b ) << endl; // Saída: 16;
}