#ifndef TRAINS_FRESNEL_H
#define TRAINS_FRESNEL_H

#include <maths/Vector.h>

namespace Fresnel {
    template <typename T>
    constexpr T Factorial(T x)
    {
        return x <= 1 ? 1 : x * Factorial(x - 1);
    }

    template <unsigned int N>
    struct Power {
        template <typename T>
        inline static T calc(T x) {
            if (N & 1)
                return x * Power<N - 1>::template calc<T>(x);
            T halfPower = Power<N / 2>::template calc<T>(x);
            return halfPower*halfPower;
        }
    };

    template <>
    struct Power<0> {
        template <typename T>
        inline static T calc(T) {
            return (T)1;
        }
    };

    template <unsigned int N>
    struct FresnelPowerExpansion;

    template <unsigned int N>
    struct FresnelMeta
    { enum {
            accurateS = FresnelPowerExpansion<N>::S::divisor
                            > FresnelPowerExpansion<N - 1>::S::divisor,
            accurateC = FresnelPowerExpansion<N>::C::divisor
                            > FresnelPowerExpansion<N - 1>::C::divisor,
    }; };

    template <>
    struct FresnelMeta<0>
    { enum {
            accurateS = 1,
            accurateC = 1
    }; };

    template <unsigned int N>
    struct FresnelPowerExpansion
    {
        struct S {
            enum {
                powerMult = 4,
                powerAdd = 3,
                power = powerMult * N + powerAdd,
                coefficient = (N & 1) ? -1 : 1,
            };
            constexpr static double divisor = Factorial<double>(2 * N + 1) * power;
        };
        struct C {
            enum {
                powerMult = 4,
                powerAdd = 1,
                power = powerMult * N + powerAdd,
                coefficient = (N & 1) ? -1 : 1,
            };
            constexpr static double divisor = Factorial<double>(2 * N) * power;
        };

        template <typename T>
        inline static maths::Vector<2, T> calc(T x)
        {
            maths::Vector<2, T> ret;
            ret[0] = Power<C::power>::calc(x) * C::coefficient / C::divisor;
            ret[1] = Power<S::power>::calc(x) * S::coefficient / S::divisor;
            return ret;
        }
    };

    template <unsigned int N>
    struct FresnelPowerSeries
    {
        template <typename T>
        inline static maths::Vector<2, T> calc(T x)
        {
            return FresnelPowerExpansion<N>::calc(x) +
                   FresnelPowerSeries<N - 1>::calc(x);
        }
    };

    template <>
    struct FresnelPowerSeries<0>
    {
        template <typename T>
        inline static maths::Vector<2, T> calc(T x)
        {
            return FresnelPowerExpansion<0>::calc(x);
        }
    };
}

template <typename T>
maths::Vector<2, T> fresnel(T x)
{
    // TODO Perform some measurement to see if this is a decent implementation
    // and check whether the compiler generates something sensible
    // TODO extern template this into a C file for float and double
    T xMag = x < 0 ? -x : x;
    maths::Vector<2, T> ret = Fresnel::FresnelPowerExpansion<0>::calc(x);
    switch ((unsigned int)(xMag * 5)) {
    default:ret += Fresnel::FresnelPowerExpansion<9>::calc(x);
    case 8: ret += Fresnel::FresnelPowerExpansion<8>::calc(x);
    case 7: ret += Fresnel::FresnelPowerExpansion<7>::calc(x);
    case 6: ret += Fresnel::FresnelPowerExpansion<6>::calc(x);
    case 5: ret += Fresnel::FresnelPowerExpansion<5>::calc(x);
    case 4: ret += Fresnel::FresnelPowerExpansion<4>::calc(x);
    case 3: ret += Fresnel::FresnelPowerExpansion<3>::calc(x);
    case 2: ret += Fresnel::FresnelPowerExpansion<2>::calc(x);
    case 1: ret += Fresnel::FresnelPowerExpansion<1>::calc(x);
    case 0: ;
    }
    return ret;
}

template <typename T>
T fresnelDirection(T x)
{
    return x * x;
}

#endif // TRAINS_FRESNEL_H
