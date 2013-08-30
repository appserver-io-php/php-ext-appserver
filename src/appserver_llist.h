/*
 * appserver_llist.h
 *
 *  Created on: 23.08.2013
 *      Author: zelgerj
 */

#ifndef __APPSERVER_LLIST_H__
#define __APPSERVER_LLIST_H__

#include <stddef.h>

typedef void (*appserver_llist_dtor)(void *, void *);

typedef struct _appserver_llist_element {
    void *ptr;

    struct _appserver_llist_element *prev;
    struct _appserver_llist_element *next;
} appserver_llist_element;

typedef struct _appserver_llist {
    appserver_llist_element *head;
    appserver_llist_element *tail;

    appserver_llist_dtor dtor;

    size_t size;
} appserver_llist;

appserver_llist *appserver_llist_alloc(appserver_llist_dtor dtor);
void appserver_llist_init(appserver_llist *l, appserver_llist_dtor dtor);
int appserver_llist_insert_next(appserver_llist *l, appserver_llist_element *e, const void *p);
int appserver_llist_insert_prev(appserver_llist *l, appserver_llist_element *e, const void *p);
int appserver_llist_remove(appserver_llist *l, appserver_llist_element *e, void *user);
int appserver_llist_remove_next(appserver_llist *l, appserver_llist_element *e, void *user);

appserver_llist_element *appserver_llist_jump(appserver_llist *l, int where, int pos);
size_t appserver_llist_count(appserver_llist *l);
void appserver_llist_empty(appserver_llist *l, void *user);
void appserver_llist_destroy(appserver_llist *l, void *user);

#if !defined(LIST_HEAD)
#define LIST_HEAD 0
#endif

#if !defined(LIST_TAIL)
#define LIST_TAIL 1
#endif

#define APPSERVER_LLIST_HEAD(__l) ((__l)->head)
#define APPSERVER_LLIST_TAIL(__l) ((__l)->tail)
#define APPSERVER_LLIST_NEXT(__e) ((__e)->next)
#define APPSERVER_LLIST_PREV(__e) ((__e)->prev)
#define APPSERVER_LLIST_VALP(__e) ((__e)->ptr)
#define APPSERVER_LLIST_IS_TAIL(__e) ((__e)->next ? 0 : 1)
#define APPSERVER_LLIST_IS_HEAD(__e) ((__e)->prev ? 0 : 1)
#define APPSERVER_LLIST_COUNT(__l) ((__l)->size)

#endif /* __APPSERVER_LLIST_H__ */
