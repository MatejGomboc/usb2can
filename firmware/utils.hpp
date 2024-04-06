#pragma once

#include <cstdint>

namespace Utils {

//! divides two integers and rounds the result to the nearest multiple of divisor
template<typename Tin, typename Tout>
static inline Tout intCeilDiv(Tin dividend, Tin divisor)
{
    return (dividend + divisor - 1) / divisor;
}

//! check if the n-th bit is set in value
template<typename T>
static inline bool isBitSet(T value, uint8_t n)
{
    return (value >> n) & 1;
}

//! sets the n-th bit in value
template<typename T>
static inline void setBit(T& value, uint8_t n)
{
    value |= 1 << n;
}

//! clears the n-th bit in val
template<typename T>
static inline void clearBit(T& value, uint8_t n)
{
    value &= ~(1 << n);
}

}
