/**
 * appserver_llist.c
 *
 * NOTICE OF LICENSE
 *
 * This source file is subject to version 3.01 of the PHP license,
 * that is bundled with this package in the file LICENSE, and is
 * available through the world-wide-web at the following url:
 * http://www.php.net/license/3_01.txt
 * If you did not receive a copy of the PHP license and are unable to
 * obtain it through the world-wide-web, please send a note to
 * license@php.net so we can mail you a copy immediately.
 */

/**
 * A doubled linked list implementation
 *
 * @copyright  	Copyright (c) 2013 <info@techdivision.com> - TechDivision GmbH
 * @license    	http://www.php.net/license/3_01.txt
 *              PHP license (PHP 3.01)
 * @author      Johann Zelger <jz@techdivision.com>
 */

#include <stdlib.h>
#include <string.h>

#include "appserver_llist.h"

void appserver_llist_initialise(appserver_llist *list, appserver_llist_destructor destor)
{
	// init properties
	list->head = NULL;
	list->foot = NULL;
	list->destor = destor;
	list->size = 0;
}

appserver_llist *appserver_llist_allocate(appserver_llist_destructor destor)
{
	appserver_llist *list;
	// allocate memory
	list = malloc(sizeof(appserver_llist));
	// initialise list
	appserver_llist_initialise(list, destor);
	// return the list
	return list;
}

appserver_llist_item *appserver_llist_item_allocate(void *data)
{
	appserver_llist_item *item;
	// allocate memory
	item = (appserver_llist_item *) malloc(sizeof(appserver_llist_item));
	// set data
	item->ptr = (void *)data;
	return item;
}


int appserver_llist_add(appserver_llist *list, appserver_llist_item *item, const void *ptr)
{
	appserver_llist_item  *newitem;

	// if no item given
	if (!item) {
		// set foot item
		item = list->foot;
	}

	// init and alloc new item
	newitem = appserver_llist_item_allocate((void *) ptr);

	// check if list is empty
	if (list->size == 0) {
		// set new head item
		list->head = newitem;
		list->head->prev = NULL;
		list->head->next = NULL;
		// add new item as well to the end
		list->foot = newitem;
	// if list is not empty
	} else {
		// set given item to new to new item
		newitem->next = item->next;
		// set given item as prev to new
		newitem->prev = item;
		// check if given item has no next item
		if (item->next == NULL) {
			// set new item to the end
			list->foot = newitem;
		} else {
			// set new item to prev of next of given item
			item->next->prev = newitem;
		}
		// finally set the new item as next for given item
		item->next = newitem;
	}
	// increase list size
	++list->size;
	return 1;
}

int appserver_llist_del(appserver_llist *list, appserver_llist_item *item)
{
	// stop if item or list is not set
    if (item == NULL || list->size == 0) {
        return 0;
    }

    // check if item is head of list
    if (item == list->head) {
    	// set next item to the head of list
        list->head = item->next;
        // if there is no next item
        if (list->head == NULL)
        	// clear foot too
            list->foot = NULL;
        else
        	// clear the previous item
            item->next->prev = NULL;
    } else {
    	// take the next item and set it as next of the previous item
        item->prev->next = item->next;
        // check if next is not set
        if (!item->next)
        	// set the previous to the end
            list->foot = item->prev;
        else
        	// take the prev item and set it as prev of the next item
            item->next->prev = item->prev;
    }

    // if the list was initiated with an destructor
    if (list->destor) {
    	// call it
        list->destor(item->ptr);
    }

    // free the items memory space
    free(item);
    // decrease the size of the list

    --list->size;

    return 0;
}

void appserver_llist_clear(appserver_llist *list)
{
	// if list has still items on it
	while (list->size > 0) {
		// delete foot item
		appserver_llist_del(list, list->foot);
	}
}


/*
 * Local Variables:
 * c-basic-offset: 4
 * tab-width: 4
 * End:
 * vim600: fdm=marker
 * vim: noet sw=4 ts=4
 */
