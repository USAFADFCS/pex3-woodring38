/** pagequeue.c
 * ===========================================================
 * Name: Tanner Woodring, __ ___ 2026
 * Section: CS483 / M3
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer

    PageQueue* pq = (PageQueue*) malloc(maxSize * sizeof(PqNode*));
    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;
    pq->maxSize = maxSize;

    return pq;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    long position = -1;
    PqNode* currentNode = pq->tail;

    for(int i = 0; i < pq->size; i++){
        
        if(currentNode->pageNum == pageNum){
            position = i;
            if(currentNode == pq->head && currentNode == pq->tail){
                //do nothing
            }
            else if(currentNode == pq->tail){
                //do nothing
            }
            else if(currentNode == pq->head){
                pq->head = currentNode->next;
                pq->head->prev = NULL;
                currentNode->next = NULL;
                pq->tail->next = currentNode;
                currentNode->prev = pq->tail;
                pq->tail = currentNode;
            }
            else {
                currentNode->prev->next = currentNode->next;
                currentNode->next->prev = currentNode->prev;
                currentNode->next = NULL;
                pq->tail->next = currentNode;
                currentNode->prev = pq->tail;
                pq->tail = currentNode;
            }
            
            break;
        }
        if(currentNode->prev != NULL){
            currentNode = currentNode->prev;
        }
        
    }
    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    if(position == -1){
        
        PqNode* newNode = (PqNode*) malloc(sizeof(PqNode));
        newNode->pageNum = pageNum;

        if(pq->size == 0){
            pq->head = newNode;
            pq->tail = newNode;
            newNode->prev = NULL;
            newNode->next = NULL;
            pq->size++;
        }
        else{
            newNode->prev = pq->tail;
            newNode->next = NULL;
            pq->tail->next = newNode;
            pq->tail = newNode;
            pq->size++;
        }
        
        if(pq->size > pq->maxSize){
            PqNode* remove = pq->head;
            pq->head = remove->next;
            pq->head->prev = NULL;
            free(remove);
            pq->size--;
        }
    }
    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.

    return position;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.
    PqNode* currentNode = pq->head;
    for(int i = 0; i < pq->size - 1; i++){
        pq->head = currentNode->next;
        pq->head->prev = NULL;
        free(currentNode);
        currentNode = pq->head;
    }

    free(pq->head);

    free(pq);
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
    int j = pq->size;
    PqNode* currentNode = pq->head;
    printf("%ld ", currentNode->pageNum);

    for(int i = 1; i < j; i++){
        currentNode = currentNode->next;
        printf("%ld ", currentNode->pageNum);
    }
}
