#ifndef __KERNEL_H__
#define __KERNEL_H__

#pragma once

#include <RuntimeLib.h>

#include "Kernel/X64/GDT.h"
#include "Kernel/X64/IDT.h"

NORETURN
SYSAPI
VOID
KernelMain (
    VOID
    );

#endif // __KERNEL_H__
