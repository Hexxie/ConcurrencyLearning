#include "message.h"

int message_create(message_t *msg) {

    msg = (message_t*)malloc(sizeof(message_t));

    if(msg) {
        return SUCCESS;
    }

    return NO_MEM;
}

int message_delete(message_t *msg) {
    free(msg);
}


const char* message_error_name(int error_code){
    switch (error_code) {
        case SUCCESS:
            return "Success";
        case NO_MEM:
            return "Not enough memory";
    }
}
