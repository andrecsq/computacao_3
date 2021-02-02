#include <type_traits>
#include <functional>
#include <iostream>

using std::is_invocable_v, std::invoke, std::is_class_v, std::cout;

template<typename F, typename FB, typename ...Params>
class Bind {
    public:
        Bind( F f, FB fb , Params ...params ) : f(f) { }

        template<typename ...Args>
        auto operator() (Args ...args) {
            if constexpr(is_invocable_v<FB, Params..., Args...>)
                return invoke(f, args...);
            else
                return [this, args...](auto ...outros){ return invoke(f, args..., outros...); };
        }

        F getF() const { return f; }

    private:
        F f;
};

template<typename F, typename FB>
auto bind(Bind<F, FB> f, auto ...args) {
    auto rec = f.getF();
    auto func = [rec, args...](auto... params){ return invoke(rec, args..., params...); };
    Bind b{func, f, args...};
    return b;
}

template<typename F>
auto bind(F f, auto ...args) {
    if constexpr (is_class_v<F>) {
        auto func = [&f, args...](auto... params){ return invoke(f, args..., params...); };
        Bind b{func, f, args...};
        return b;
    } else {
        auto func = [f, args...](auto... params){ return invoke(f, args..., params...); };
        Bind b{func, f, args...};
        return b;
    }
}
    
int mdc( int a, int b ) { return b == 0 ? a : mdc( b, a%b ); }

int main() {
    auto f3 = bind( []( int x, int y, int z ){ cout << x*z << (char) y << " " ; } );
    auto f1 = f3( 5, ';' );
    for( int i = 0; i < 5; i++ )
        f1( i );

}