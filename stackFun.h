#ifndef STACKFUN_H_
#define STACKFUN_H_

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "bmp.h"

struct InfoStruct
{
    WORD Mvalue;		//Mortonֵ
    BYTE DIB;			//դ������ֵ
};

struct Node				//ջ�ڵ�
{
    InfoStruct info;
    struct Node *pred;
    struct Node *succ;
};

void stackPush(Node *header, InfoStruct str)	//��ջ��ѹ��һ���ڵ�
{
    Node *newNode = new Node;
    newNode->info = str;
    newNode->pred = header;
    newNode->succ = header->succ;

    header->succ = newNode;		//��ϵҪ������ȫ
    header->succ->pred = newNode;

    newNode = NULL;
    return;
}
void stackPop(Node *header, int p)				//��ջ��ɾ��p���ڵ�
{
    while(p > 0)
    {
        Node *record = header->succ;
        header->succ = record->succ;
        if (record->succ != NULL) record->succ->pred = header;
        delete record;
        record = NULL;

        p--;
    }

    return;
}

// Node* stackPop(Node *header, Node *record, int p)	//ɾ��recordǰ���p���ڵ�
// {
//     while(p > 0)
//     {
//         Node *del = record->pred;
//
//         //record->pred = del->pred;
//         //del->pred->succ = record;
//         del->pred->succ = del->succ;
//         del->succ->pred = del->pred;
//
//         delete del;
//         del = NULL;
//
//         p--;
//     }
//
//     return record;
// }

#endif