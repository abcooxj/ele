#ifndef __KEY_H_
#define __KEY_H_

#include "main.h"
#include "stdbool.h"

struct keys
{
    uint8_t judge_sta;
    bool key_sta;
    bool short_flag;
};

extern struct keys key[];
void key_serv(void);

#endif
