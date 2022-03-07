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
        strncpy(sp, target->value, bufsize - 1);
        // sp[bufsize]="\0";
    }
    return target;
}

/* Attempt to remove element from tail of queue.
 * Other attribute is as same as q_remove_head.
 */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    //判斷queue是否有元素
    if (!head || list_empty(head))
        return NULL;

    // target指向最後一個點（head->prev）
    element_t *target = list_entry(head->prev, element_t, list);

    //移除taget
    list_del_init(head->prev);

    // target的value 非空且被移除（初始化）就將value的資料給sp
    if (sp != NULL) {
        strncpy(sp, target->value, bufsize - 1);
    }
    return target;
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
    // https:leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;
    //宣告快慢指標
    struct list_head *fast, *slow;
    for (fast = slow = head->next; fast->next != head && fast != head;
         slow = slow->next, fast = fast->next->next) {
    }

    //找到中間點（慢指標），移除後釋放
    element_t *mid = list_entry(slow, element_t, list);
    list_del_init(slow);
    q_release_element(mid);
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
    if (!head || list_empty(head)) {
        return false;
    }
    //計算這個node殺了幾次
    int count = 0;
    // 宣告兩個指向現在跟下一個list
    struct list_head *first, *second;
    //探訪所有node
    list_for_each_safe (first, second, head) {
        // list轉換成struct
        element_t *entry = list_entry(first, element_t, list);
        element_t *safe = list_entry(second, element_t, list);
        //如果now跟next的字串內容相同，殺了first，並釋放，注意seocnd不能等於head沒value會錯誤
        if (second != head && 0 == strcmp(entry->value, safe->value)) {
            list_del(first);
            q_release_element(entry);
            //砍了一次
            count++;
            //字串跟下一個點不相同，判斷之前有沒有砍過
        } else {
            if (count > 0) {
                list_del(first);
                q_release_element(entry);
                count = 0;
            }
        }
    }
    return true;
}

/* Attempt to swap every two adjacent nodes. */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head))
        return;
    struct list_head *now, *next1;
    // for兩個滿足一個是&&
    for (now = head->next, next1 = now->next; now->next != head && now != head;
         now = now->next, next1 = now->next) {
        //刪除目前，讓n1變到前面，在將now插入n1後
        list_del_init(now);
        list_add(now, next1);
    }
}

/* Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(struct list_head *head)
{
    struct list_head *node, *first, *second;
    // n紀錄總共多少點
    int n = 0;
    int j;
    list_for_each (node, head)
        n++;
    //第一層for迴圈代表已經放了幾個點到最後面執行n-1次即可
    for (int i = 1; i < n; i++) {
        //第二層執行左右交換，並且每次可以根據i來減少交換次數
        for (first = head->next, second = first->next, j = n; i < j;
             j--, second = first->next) {
            //刪除first加入其後面
            list_del_init(first);
            list_add(first, second);
        }
    }
}

/* Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

void q_sort(struct list_head *head)
{
    struct list_head *node, *first, *second;
    // n紀錄總共多少點
    int n = 0;
    int j;
    list_for_each (node, head)
        n++;
    //第一層for迴圈代表已經放了幾個點到最後面執行n-1次即可
    for (int i = 1; i < n; i++) {
        //第二層執行左右交換，並且每次可以根據i來減少交換次數
        for (first = head->next, second = first->next, j = n; i < j;
             j--, second = first->next) {
            //利用element取value
            element_t *current = list_entry(first, element_t, list);
            element_t *current_next = list_entry(second, element_t, list);
            // value大的放後面
            if (strlen(current->value) > strlen(current_next->value)) {
                list_del_init(first);
                list_add(first, second);
            }
        }
    }
}
