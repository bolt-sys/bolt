#include "Mathi.h"

UINT64
SYSAPI
Log2I (
    IN UINT64 Value
    )
{
    UINT64 Result = 0;

    while (Value >>= 1) {
        ++Result;
    }

    return Result;
}

UINT64
SYSAPI
Pow2I (
    IN UINT64 Value
    )
{
    UINT64 Result = 1;

    while (Value--) {
        Result <<= 1;
    }

    return Result;
}

UINT64
SYSAPI
DivideRoundUpI (
    IN UINT64 Dividend,
    IN UINT64 Divisor
    )
{
    return (Dividend + Divisor - 1) / Divisor;
}

UINT64
SYSAPI
MaxI (
    IN UINT64 A,
    IN UINT64 B
    )
{
    return A > B ? A : B;
}