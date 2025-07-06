/****************************************************************************
 * @copyright Copyright(C) 2020-2022 Ismartware Limited. All rights reserved.
 * @file list_head.h
 * @brief a simple and generic doubly linked list header
 * @author SD1 & SW team
 ****************************************************************************/
#ifndef LIST_HEAD_H
#define LIST_HEAD_H
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct list_head list_head_t;
struct list_head
{
    list_head_t* prev;
    list_head_t* next;
};

#ifdef __cplusplus
}
#endif

#endif        // end of LIST_HEAD_H
