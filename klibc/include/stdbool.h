//
// Created by Robin Alexander Plate on 8/25/23.
//

#ifndef BOLT_KLIBC_STDBOOL_H_
#define BOLT_KLIBC_STDBOOL_H_

#ifndef __cplusplus
typedef _Bool bool;
#define true  ((bool)1==1)
#define false ((bool)0==1)
#endif

#endif // BOLT_KLIBC_STDBOOL_H_
