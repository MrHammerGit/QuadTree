#ifndef _BMP_H
#define _BMP_H

typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef long LONG;

struct BITMAPFILEHEADER
{
    WORD  bfType;	//��ΪBM
    DWORD bfSize;	//ͼ���ܴ�С
    WORD  bfReserved1;
    WORD  bfReserved2;
    DWORD bfOffBits;	//ƫ���������ļ�ͷ����Ϣͷ����ɫ����ֽ���֮��
};

struct BITMAPINFOHEADER
{
    DWORD biSize;		//��Ϣͷ��С
    LONG  biWidth;		//��ȴ�С�����أ�
    LONG  biHeight;		//�߶ȴ�С�����أ�
    WORD  biPlanes;
    WORD  biBitCount;	//ͼ��λ��
    DWORD biCompression;
    DWORD biSizeImage;	//DIB��������С
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
};

struct RGBQUAD		//��ɫ��
{
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
};

#endif
