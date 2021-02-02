#include <type_traits>
#include <iostream>
#include <cmath>

using namespace std;

class X {
    public:
        double e(double k) { return k; }
        double dx(double k) { return 1; }
};

template<typename T1>
class Value {
    public:
        Value(T1 t1): t1(t1) {}

        virtual double e(double k) { return k; }

        virtual double dx(double k) { return 1; }
    
    protected:
        T1 t1;
};

template<typename T1, typename T2>
class Values {
    public:
        Values(T1 t1, T2 t2): t1(t1), t2(t2) {}

        virtual double e(double k) { return k; }
        virtual double dx(double k) { return 1; }
    
    protected:
        T1 t1;
        T2 t2;
};

class Constant {
    public:
        Constant(double c): c(c){}
        double e(double k) { return c; }
        double dx(double k) { return 0; }

    private:
        double c;
};

template<typename T1, typename T2>
class Sum: public Values<T1, T2> {
    public:
        Sum(T1 t1, T2 t2): Values<T1, T2>(t1, t2) {}

        double e(double k) {
            return this->t1.e(k) + this->t2.e(k);
        }

        double dx(double k) {
            return this->t1.dx(k) + this->t2.dx(k);
        }
};

template<typename T1, typename T2>
class Subtract: public Values<T1, T2> {
    public:
        Subtract(T1 t1, T2 t2): Values<T1, T2>(t1, t2) {}

        double e(double v) {
            return this->t1.e(v) - this->t2.e(v);
        }

        double dx(double v) {
            return this->t1.dx(v) - this->t2.dx(v);
        }
};

template<typename T1, typename T2>
class Multiply: public Values<T1, T2> {
    public:
        Multiply(T1 t1, T2 t2): Values<T1, T2>(t1, t2) {}

        double e(double k) {
            return this->t1.e(k) * this->t2.e(k);
        }

        double dx(double k) {
            return this->t1.e(k) * this->t2.dx(k) + this->t1.dx(k) * this->t2.e(k);
        }
};

template<typename T1, typename T2>
class Divide: public Values<T1, T2> {
    public:
        Divide(T1 t1, T2 t2): Values<T1, T2>(t1, t2) {}

        double e(double k) {
            return this->t1.e(k) / this->t2.e(k);
        }

        double dx(double k) {
            return (this->t1.dx(k) * this->t2.e(k) - this->t1.e(k) * this->t2.dx(k)) /(pow(this->t2.e(k), 2));
        }
};

template<typename T1, typename T2>
class Power: public Values<T1, T2> {
    public:
        Power(T1 t1, T2 t2): Values<T1, T2>(t1, t2) {}

        double e(double k) {
            return pow(this->t1.e(k), this->t2.e(k));
        }

        double dx(double k) {
            return this->t2.e(k) * pow(this->t1.e(k), this->t2.e(k) - 1) * this->t1.dx(k);
        }
};

template<typename T1>
class Sine: public Value<T1> {
    public:
        Sine(T1 t1): Value<T1>(t1) {}

        double e(double v) {
            return sin(this->t1.e(v));
        }

        double dx(double v) {
            return cos(this->t1.e(v)) * this->t1.dx(v);
        }
};

template<typename T1>
class Cosine: public Value<T1> {
    public:
        Cosine(T1 t1): Value<T1>(t1) {}

        double e(double v) {
            return cos(this->t1.e(v));
        }

        double dx(double v) {
            return -sin(this->t1.e(v)) * this->t1.dx(v);
        }
};

template<typename T1>
class Exponential: public Value<T1> {
    public:
        Exponential(T1 t1): Value<T1>(t1) {}

        double e(double v) {
            return exp(this->t1.e(v));
        }

        double dx(double v) {
            return exp(this->t1.e(v)) * this->t1.dx(v);
        }
};

template<typename T1>
class Logarithm: public Value<T1> {
    public:
        Logarithm(T1 t1): Value<T1>(t1) {}

        double e(double v) {
            return log(this->t1.e(v));
        }

        double dx(double v) {
            return 1/(this->t1.e(v)) * this->t1.dx(v);
        }
};

template<typename T1>
Sine<T1> sin(T1 x) {
    return Sine<T1>(x);
}

template<typename T1>
Cosine<T1> cos(T1 x) {
    return Cosine<T1>(x);
}

template<typename T1>
Exponential<T1> exp(T1 x) {
    return Exponential<T1>(x);
}

template<typename T1>
Logarithm<T1> log(T1 x) {
    return Logarithm<T1>(x);
}

template<typename T1, typename T2>
auto operator +(T1 t1, T2 t2) {
    if constexpr(std::is_integral<T1>::value || std::is_floating_point<T1>::value)
        return Sum<Constant, T2>(t1, t2);
    else if constexpr(std::is_integral<T2>::value || std::is_floating_point<T2>::value)
        return Sum<T1, Constant>(t1, t2);
    else
        return Sum<T1, T2>(t1, t2);
}

template<typename T1, typename T2>
auto operator *(T1 t1, T2 t2) {
    if constexpr(std::is_integral<T1>::value || std::is_floating_point<T1>::value)
        return Multiply<Constant, T2>(t1, t2);
    else if constexpr(std::is_integral<T2>::value || std::is_floating_point<T2>::value)
        return Multiply<T1, Constant>(t1, t2);
    else
        return Multiply<T1, T2>(t1, t2);
}

template<typename T1, typename T2>
auto operator -(T1 t1, T2 t2) {
    if constexpr(std::is_integral<T1>::value || std::is_floating_point<T1>::value)
        return Subtract<Constant, T2>(t1, t2);
    else if constexpr(std::is_integral<T2>::value || std::is_floating_point<T2>::value)
        return Subtract<T1, Constant>(t1, t2);
    else
        return Subtract<T1, T2>(t1, t2);
}

template<typename T1, typename T2>
auto operator /(T1 t1, T2 t2) {
    if constexpr(std::is_integral<T1>::value || std::is_floating_point<T1>::value)
        return Divide<Constant, T2>(t1, t2);
    else if constexpr(std::is_integral<T2>::value || std::is_floating_point<T2>::value)
        return Divide<T1, Constant>(t1, t2);
    else
        return Divide<T1, T2>(t1, t2);
}

template<typename T1, typename T2>
Power<T1, Constant> operator ->*(T1 t1, T2 t2) {
    static_assert(!std::is_same_v<double, T2> , "Operador de potenciação definido apenas para inteiros");
    return Power<T1, Constant>(t1, t2);
}

X x;

int main() {
    double v = 0.1;
auto f = 1 / (1 + exp( -2*( x - 1 )->*4 ) );
  
    cout << "f(" << v << ")=" << f.e(v) << ", f'(" << v << ")=" << f.dx(v) << endl;
}