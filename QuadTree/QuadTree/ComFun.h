#ifndef COMFUN_H_
#define COMFUN_H_

#include "stackFun.h"

//���numΪ2^n
int log2(int num)
{
    if (num == 1)
        return 0;
    else
        return 1 + log2(num >> 1);
}

//ͨ��ʮ����Morton������к�
int MortonToRow(int n, WORD M)
{
    //�ֱ�ʹ��nλ��2*nλ����������ʾ���кź�Morton��
    int *binaryR = new int[n];		//�洢Row�Ķ�����ֵ
    int *binaryC = new int[n];		//�洢Col�Ķ�����ֵ
    int *binaryM = new int[2 * n];	//�洢Morton�Ķ�����ֵ
    int dRow, dCol;		//Row��Col��ʮ����ֵ
    int rest, Mcopy;

    //ͨ��Morton���ֵ������кŵ�ֵ
    Mcopy = M;
    for (int k = 2 * n - 1; k >= 0; k--)	//��Morton��ת��Ϊ������
    {
        rest = Mcopy % 2;
        Mcopy = Mcopy / 2;
        binaryM[k] = rest;
    }
    for (int k = 0; k < 2 * n; k++)	//ͨ��Morton��������к�
    {
        if (k % 2 == 0)
            binaryR[k / 2] = binaryM[k];
        else
            binaryC[k / 2] = binaryM[k];
    }

    dRow = 0;
    dCol = 0;	//�ͼ��������Ӧ�÷�������
    for (int k = n - 1, q = 1; k >= 0; k--, q *= 2)		//�����кŵĶ�����ֵ��Ϊʮ����ֵ
    {
        dRow += binaryR[k] * q;
        dCol += binaryC[k] * q;
    }

    return dRow;
}

//ͨ��ʮ����Morton������к�
int MortonToCol(int n, WORD M)
{
    //�ֱ�ʹ��nλ��2*nλ����������ʾ���кź�Morton��
    int *binaryR = new int[n];		//�洢Row�Ķ�����ֵ
    int *binaryC = new int[n];		//�洢Col�Ķ�����ֵ
    int *binaryM = new int[2 * n];	//�洢Morton�Ķ�����ֵ
    int dRow, dCol;		//Row��Col��ʮ����ֵ
    int rest, Mcopy;

    //ͨ��Morton���ֵ������кŵ�ֵ
    Mcopy = M;
    for (int k = 2 * n - 1; k >= 0; k--)	//��Morton��ת��Ϊ������
    {
        rest = Mcopy % 2;
        Mcopy = Mcopy / 2;
        binaryM[k] = rest;
    }
    for (int k = 0; k < 2 * n; k++)	//ͨ��Morton��������к�
    {
        if (k % 2 == 0)
            binaryR[k / 2] = binaryM[k];
        else
            binaryC[k / 2] = binaryM[k];
    }

    dRow = 0;
    dCol = 0;	//�ͼ��������Ӧ�÷�������
    for (int k = n - 1, q = 1; k >= 0; k--, q *= 2)		//�����кŵĶ�����ֵ��Ϊʮ����ֵ
    {
        dRow += binaryR[k] * q;
        dCol += binaryC[k] * q;
    }

    return dCol;
}

#endif