//
// Created by Emil Glenn Karpawitz on 8/26/23.
//

#ifndef BOLT_KLIBC_INCLUDE_UNISTD_H_
#define BOLT_KLIBC_INCLUDE_UNISTD_H_

#include "stddef.h"

void write(int fd, const char *buf, size_t count);

#endif //BOLT_KLIBC_INCLUDE_UNISTD_H_
