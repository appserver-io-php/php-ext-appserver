/**
 * appserver_llist.h
 *
 * NOTICE OF LICENSE
 *
 * This source file is subject to the Open Software License (OSL 3.0)
 * that is available through the world-wide-web at this URL:
 * http://opensource.org/licenses/osl-3.0.php
 */

/**
 * A doubled linked list implementation
 *
 * @copyright  	Copyright (c) 2013 <info@techdivision.com> - TechDivision GmbH
 * @license    	http://opensource.org/licenses/osl-3.0.php
 *              Open Software License (OSL 3.0)
 * @author      Johann Zelger <jz@techdivision.com>
 */

#ifndef APPSERVER_LLIST_H
#define APPSERVER_LLIST_H

#include <stddef.h>

typedef struct _appserver_llist_item
{
	void *ptr;
	struct _appserver_llist_item *next;
	struct _appserver_llist_item *prev;

} appserver_llist_item;

typedef void (*appserver_llist_destructor)(void *);

typedef struct _appserver_llist
{
	appserver_llist_item *head;
	appserver_llist_item *foot;
	appserver_llist_destructor destor;
	size_t size;

} appserver_llist;

size_t appserver_llist_count(appserver_llist *list);

appserver_llist *appserver_llist_allocate(appserver_llist_destructor destor);
void appserver_llist_initialise(appserver_llist *list, appserver_llist_destructor destor);
int appserver_llist_add(appserver_llist *list, appserver_llist_item *item, const void *data);
int appserver_llist_del(appserver_llist *list, appserver_llist_item *item);
void appserver_llist_clear(appserver_llist *list);

#endif
