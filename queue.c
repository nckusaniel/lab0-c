#include "queue.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "harness.h"

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
    //檢查head
    if (head == NULL)
        return false;
    //宣告element來存入s字串
    element_t *n = malloc(sizeof(element_t));
    //判斷是否存在
    if (!n) {
        return false;
    }
    //給新節點n的value配置空間
    //這樣錯n->value =malloc(sizeof(strlen(s)+1));
    n->value = malloc(strlen(s) + 1);
    //要判斷n=>value是否存在不然下面可能異味
    if (!n->value) {
        free(n);  //不佳這個會出現Segmentation fault occurred.  You dereferenced
                  // a NULL or invalid pointer
        return false;
    }
    //複製資料
    //因為strcpy可能造成益位因此使用strncpy，且
    // strncpy不會自動在目的陣列加上空字元 但s_ele已加入，所以不用管
    strncpy(n->value, s, strlen(s) + 1);
    //將n加入頭
    list_add(&n->list, head);
    return true;
}

/* Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (head == NULL)
        return false;
    element_t *n = malloc(sizeof(element_t));
    if (!n) {
        return false;
    }
    n->value = malloc(strlen(s) + 1);
    if (!n->value) {
        free(n);
        return false;
    }
    strncpy(n->value, s, strlen(s) + 1);
    list_add_tail(&n->list, head);
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
        sp[bufsize - 1] = '\0';
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
        sp[bufsize - 1] = '\0';
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

/* Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
//從頭開始刪除元素，插入頭
void q_reverse(struct list_head *head)
{
    //要考慮head為null的情況
    if (!head || list_empty(head)) {
        return;
    }
    struct list_head *node, *safe;
    list_for_each_safe (node, safe, head) {
        list_move(node, head);
    }
}

/* Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

//將資料合併
struct list_head *merge_twolist(struct list_head *list1,
                                struct list_head *list2)
{
    struct list_head *head = NULL;
    struct list_head **ptr = &head;
    while (list1 && list2) {
        //比大小<0代表l1小
        if (strcmp(list_entry(list1, element_t, list)->value,
                   list_entry(list2, element_t, list)->value) < 0) {
            *ptr = list1;
            list1 = list1->next;
        } else {
            *ptr = list2;
            list2 = list2->next;
        }
        ptr = &(*ptr)->next;
    }
    // l1和l2作or運算並型態轉換存回ptr
    *ptr = (struct list_head *) ((uintptr_t) list1 | (uintptr_t) list2);
    return head;
}
//將資料切割（divide
struct list_head *merge_cut(struct list_head *node1)
{
    //邊界條件
    if (!node1 || !node1->next) {
        return node1;
    }
    struct list_head *slow = node1, *fast = slow->next;
    for (; fast && fast->next; fast = fast->next->next)
        slow = slow->next;
    // mid是新串列開頭
    struct list_head *mid = slow->next;
    slow->next = NULL;
    struct list_head *left = merge_cut(node1);
    struct list_head *right = merge_cut(mid);
    return merge_twolist(left, right);
}


void q_sort(struct list_head *head)
{
    //邊界條件
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    //首先要將傳進來的head改變成單向的linklist
    head->prev->next = NULL;
    head->next = merge_cut(head->next);
    struct list_head *now = head, *next = head->next;
    while (next) {
        next->prev = now;
        now = next;
        next = next->next;
    }
    now->next = head;
    head->prev = now;
}


// //先配置一個節點暫存一下
// struct list_head *tmp_head = malloc(sizeof(struct list_head));
// struct list_head *ptr = tmp_head, *l1_head, *l2_head;
// l1_head = list1;
// l2_head = list2;
// // l1_head存頭list1的頭，因為尾巴不是null而是接回來
// //用do-List是因為頭尾相接，傳統的while(l1&&l2)在只有一個元素時會成立，但是我需要串街比較
// do {
//     //宣告來取用東西
//     element_t *list1_ele = list_entry(list1, element_t, list);
//     element_t *list2_ele = list_entry(list1, element_t, list);
//     //計算大小作串接，1的元素大於二就
//     if (strcmp(list1_ele->value, list2_ele->value) > 0) {
//         ptr->next = list1;
//         list1->prev = ptr;
//         list1 = list1->next;
//     } else  //代表l2比l1大ptr指過去，l2後移
//     {
//         //更改兩個指標來搞
//         ptr->next = list2;
//         list2->prev = ptr;
//         list2 = list2->next;
//     }
//     ptr = ptr->next;
// } while (list1 != l1_head || list2 != l2_head);
// //執行到這邊代表有人到結尾了，令一個串過來
// ptr->next = l1_head ? list1 : list2;
// return tmp_head->next;