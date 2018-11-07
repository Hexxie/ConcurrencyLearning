#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdlib.h>

#include "msgerrno.h"

struct {
    void* message;
    size_t size;
} typedef message_t;

int message_create(message_t *msg);
message_t getMessage();
int message_delete();
const char* message_error_name(int error_code);

#endif
