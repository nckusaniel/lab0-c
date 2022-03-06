#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/* Create empty queue.
 * Return NULL if could not allocate space.
 */
struct list_head *q_new()
{
    struct list_head *node = malloc(sizeof(struct list_head));
    if (!node) {
        return NULL;
    }
    INIT_LIST_HEAD(node);
    return node;
    // malloc head call node
    // no space return node
    // node point to itself in both pre and next
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    element_t *entry, *safe;
    if (!l)
        return;
    list_for_each_entry_safe (entry, safe, l, list)
        q_release_element(entry);
    free(l);
    //宣告兩個element_t指標（enrty在前，safe在後）
    //判斷指標l是否為空的
    //利用list_for_each_entry_safe，探索l串列之element_t結構
    //並利用q_release_element 釋放entry
    //釋放l
}

/* Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(struct list_head *head, char *s)
{
    if (head == NULL)
        return false;
    element_t *q = malloc(sizeof(element_t));
    if (!q)
        return false;
    //配置value字串所需的空間
    q->value = malloc(sizeof(strlen(s + 1)));
    //將s複製給value
    strncpy(q->value, s, strlen(s) + 1);
    list_add(&q->list, head);
    return true;
    //宣告q準備插入queue
    //判斷q存不存在
    //利用list_add來插入資料
}

/* Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *q = malloc(sizeof(element_t));
    if (!q)
        return false;
    q->value = malloc(sizeof(strlen(s + 1)));
    strncpy(q->value, s, (strlen(s) + 1));
    list_add_tail(&q->list, head);
    return true;
}

/* Attempt to remove element from head of queue.
 * Return target element.
 * Return NULL if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 *
 * NOTE: "remove" is different from "delete"
 * The space used by the list element and the string should not be freed.
 * The only thing "remove" need to do is unlink it.
 *
 * Reference:
 * https://english.stackexchange.com/questions/52508/difference-between-delete-and-remove
 */

element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    //判斷queue是否有元素
    if (!head || list_empty(head))
        return NULL;

    // target指向第一個點
    element_t *target = list_entry(head->next, element_t, list);

    //移除taget
    list_del_init(head->next);

    // target的value 非空且被移除（初始化）就將value的資料給sp
    if (sp != NULL) {
        sp = malloc(sizeof(strlen(target->value + 1)));
        strncpy(sp, target->value, strlen(target->value) + 1);
    }
    return target;
}

/* Attempt to remove element from tail of queue.
 * Other attribute is as same as q_remove_head.
 */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    return NULL;
}

/* WARN: This is for external usage, don't modify it
 * Attempt to release element.
 */
void q_release_element(element_t *e)
{
    free(e->value);
    free(e);
}

/* Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in list.
 * The middle node of a linked list of size n is the
 * ⌊n / 2⌋th node from the start using 0-based indexing.
 * If there're six element, the third member should be return.
 * Return true if successful.
 * Return false if list is NULL or empty.
 */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string,
 * leaving only distinct strings from the original list.
 * Return true if successful.
 * Return false if list is NULL.
 *
 * Note: this function always be called after sorting, in other words,
 * list is guaranteed to be sorted in ascending order.
 */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Attempt to swap every two adjacent nodes. */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(struct list_head *head) {}

/* Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(struct list_head *head) {}
