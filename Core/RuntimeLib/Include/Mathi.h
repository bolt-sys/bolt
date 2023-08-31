#pragma once
#include "Base.h"

UINT64
SYSAPI
Log2I (
    IN UINT64 Value
    );

UINT64
SYSAPI
Pow2I (
    IN UINT64 Value
    );

UINT64
SYSAPI
DivideRoundUpI (
    IN UINT64 Dividend,
    IN UINT64 Divisor
    );

UINT64
SYSAPI
MaxI (
    IN UINT64 A,
    IN UINT64 B
    );
