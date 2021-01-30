class X {
    public:

        double e( double v ) {
            return v;
        }

        double dx( double v ) {
            return 1;
        }
};

class Constant {
    public:
        Constant( double c ): c(c){}

        double e( double v ) {
            return c;
        }

        double dx( double v ) {
            return 0;
        }

    private:
        double c;
};

template<typename T1, typename T2>
class Values  {
    public:
        Values( T1 t1, T2 t2 ): t1(t1), t2(t2) {}

        virtual double e ( double v ) { return v; }

        virtual double dx( double v ) { return 1; }
    
    protected:
        T1 t1;
        T2 t2;
};

template<typename T1, typename T2>
class Soma : public Values<T1, T2> {
    public:
        Soma ( T1 t1, T2 t2 ): Values<T1, T2>( t1, t2 ) {}

        double e( double v ) {
            return this->t1.e( v ) + this->t2.e( v );
        }

        double dx( double v ) {
            return this->t1.dx( v ) + this->t2.dx( v );
        }
};

template<typename T1, typename T2>
class Subtrai : public Values<T1, T2> {
    public:
        Subtrai ( T1 t1, T2 t2 ): Values<T1, T2>( t1, t2 ) {}

        double e( double v ) {
            return this->t1.e( v ) - this->t2.e( v );
        }

        double dx( double v ) {
            return this->t1.dx( v ) - this->t2.dx( v );
        }
};

template<typename T1, typename T2>
class Multiplica : public Values<T1, T2> {
    public:
        Multiplica ( T1 t1, T2 t2 ): Values<T1, T2>( t1, t2 ) {}

        double e( double v ) {
            return this->t1.e( v ) * this->t2.e( v );
        }

        double dx( double v ) {
            return this->t1.e( v ) * this->t2.dx( v ) + this->t1.dx( v ) * this->t2.e( v );
        }
};

template<typename T1, typename T2>
class Divide : public Values<T1, T2> {
    public:
        Divide ( T1 t1, T2 t2 ): Values<T1, T2>( t1, t2 ) {}

        double e( double v ) {
            return this->t1.e( v ) / this->t2.e( v );
        }

        double dx( double v ) {
            return (this->t1.dx( v ) * this->t2.e( v ) - this->t1.e( v ) * this->t2.dx( v )) / ( pow( this->t2.e(v), 2 ) );
        }
};

template<typename T1, typename T2>
class Exponent : public Values<T1, T2> {
    public:
        Exponent ( T1 t1, T2 t2 ): Values<T1, T2>( t1, t2 ) {}

        double e( double v ) {
            return pow(this->t1.e( v ), this->t2.e( v ));
        }

        double dx( double v ) {
            return this->t2.e(v) * pow(this->t1.e(v), this->t2.e(v) - 1) * this->t1.dx(v);
        }
};

template<typename F>
class Valor {
    public:
        Valor( F f ): f( f ) {}

        virtual double e( double v ) { return v; }

        virtual double dx( double v ) { return 1; }
    
    protected:
        F f;
};

template<typename F>
class Seno : public Valor<F> {
    public:
        Seno( F f ): Valor<F>( f ) {}

        double e( double v ) {
            return sin( this->f.e(v) );
        }

        double dx( double v ) {
            return cos( this->f.e(v) ) * this->f.dx(v);
        }
};

template<typename F>
class Cosseno : public Valor<F> {
    public:
        Cosseno( F f ): Valor<F>( f ) {}

        double e( double v ) {
            return cos( this->f.e(v) );
        }

        double dx( double v ) {
            return -sin( this->f.e(v) ) * this->f.dx(v);
        }
};

template<typename F>
class Exponencial : public Valor<F> {
    public:
        Exponencial( F f ): Valor<F>( f ) {}

        double e( double v ) {
            return exp( this->f.e(v) );
        }

        double dx( double v ) {
            return exp( this->f.e(v) ) * this->f.dx(v);
        }
};

template<typename F>
class Logaritmica : public Valor<F> {
    public:
        Logaritmica( F f ): Valor<F>( f ) {}

        double e( double v ) {
            return log( this->f.e(v) );
        }

        double dx( double v ) {
            return 1/( this->f.e(v) ) * this->f.dx(v);
        }
};

template<typename T1, typename T2>
auto operator + ( T1 t1, T2 t2 ) {
    if constexpr(std::is_integral_v<T1> || std::is_floating_point_v<T1>)
        return Soma<Constant, T2>( t1, t2 );
    else if constexpr(std::is_integral_v<T2> || std::is_floating_point_v<T2>)
        return Soma<T1, Constant>( t1, t2 );
    else
        return Soma<T1, T2>( t1, t2 );
}

template<typename T1, typename T2>
auto operator - ( T1 t1, T2 t2 ) {
    if constexpr(std::is_integral_v<T1> || std::is_floating_point_v<T1>)
        return Subtrai<Constant, T2>( t1, t2 );
    else if constexpr(std::is_integral_v<T2> || std::is_floating_point_v<T2>)
        return Subtrai<T1, Constant>( t1, t2 );
    else
        return Subtrai<T1, T2>( t1, t2 );
}

template<typename T1, typename T2>
auto operator * ( T1 t1, T2 t2 ) {
    if constexpr(std::is_integral_v<T1> || std::is_floating_point_v<T1>)
        return Multiplica<Constant, T2>( t1, t2 );
    else if constexpr(std::is_integral_v<T2> || std::is_floating_point_v<T2>)
        return Multiplica<T1, Constant>( t1, t2 );
    else
        return Multiplica<T1, T2>( t1, t2 );
}

template<typename T1, typename T2>
auto operator / ( T1 t1, T2 t2 ) {
    if constexpr(std::is_integral_v<T1> || std::is_floating_point_v<T1>)
        return Divide<Constant, T2>( t1, t2 );
    else if constexpr(std::is_integral_v<T2> || std::is_floating_point_v<T2>)
        return Divide<T1, Constant>( t1, t2 );
    else
        return Divide<T1, T2>( t1, t2 );
}

template<typename T1, typename T2>
Exponent<T1, Constant> operator ->* ( T1 t1, T2 t2 ) {
    static_assert(!std::is_same_v<double, T2> , "Operador de potenciação definido apenas para inteiros");
    return Exponent<T1, Constant>( t1, t2 );
}

template<typename T1>
Seno< T1 > sin( T1 x ) {
    return Seno< T1 >( x );
}

template<typename T1>
Cosseno< T1 > cos( T1 x ) {
    return Cosseno< T1 >( x );
}

template<typename T1>
Exponencial< T1 > exp( T1 x ) {
    return Exponencial< T1 >( x );
}

template<typename T1>
Logaritmica< T1 > log( T1 x ) {
    return Logaritmica< T1 >( x );
}

X x;