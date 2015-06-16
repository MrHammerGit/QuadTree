#include "ComFun.h"

void unCompress(int num, char *datname, char *colorname, char *bmpname)
{
    int n = log2(num);

    FILE *inptr = fopen(datname, "rb");		//����ʹ�á�rb����ʽ���ļ���������Ȼ0x0A�ᱻ�ж�ΪEOF
    if (inptr == NULL)
    {
        printf("Open Error!");
        return;
    }
    FILE *colorptr = fopen(colorname, "r");
    if (colorptr == NULL)
    {
        printf("Open Error!");
        return;
    }
    FILE *imgptr = fopen(bmpname, "w");
    if (imgptr == NULL)
    {
        printf("Open Error!");
        return;
    }

    /************************************************************************/
    /* д���ļ�ͷ��Ϣ                                                       */
    /************************************************************************/
    BITMAPFILEHEADER bf;
    bf.bfType = 19778;
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    bf.bfOffBits = 1078;
    bf.bfSize = num * num + bf.bfOffBits;

    fwrite(&bf.bfType, sizeof(bf.bfType), 1, imgptr);
    fwrite(&bf.bfSize, sizeof(bf.bfSize), 1, imgptr);
    fwrite(&bf.bfReserved1, sizeof(bf.bfReserved1), 1, imgptr);
    fwrite(&bf.bfReserved2, sizeof(bf.bfReserved2), 1, imgptr);
    fwrite(&bf.bfOffBits, sizeof(bf.bfOffBits), 1, imgptr);

    /************************************************************************/
    /* д����Ϣͷ                                                       */
    /************************************************************************/
    BITMAPINFOHEADER bi;
    bi.biSize = 40;
    bi.biWidth = num;
    bi.biHeight = num;
    bi.biPlanes = 1;
    bi.biBitCount = 8;
    bi.biCompression = 0;
    bi.biSizeImage = num * num;
    bi.biXPelsPerMeter = 3780;
    bi.biYPelsPerMeter = 3780;
    bi.biClrImportant = 0;
    bi.biClrUsed = 0;
    fwrite(&bi, sizeof(bi), 1, imgptr);

    /************************************************************************/
    /* ��ȡ��ɫ����Ϣ��д��                                                 */
    /************************************************************************/
    RGBQUAD quad[256];
    fread(quad, sizeof(RGBQUAD), 256, colorptr);
    fwrite(quad, sizeof(RGBQUAD), 256, imgptr);

    /************************************************************************/
    /* �ؼ����ݵĶ�ȡ���߶���д                                             */
    /************************************************************************/
    InfoStruct Info;
    WORD end = num * num;
    int Row, Col;

    while(fread(&Info.Mvalue, sizeof(Info.Mvalue), 1, inptr) == 1)		//��ȡ���ݣ�����ʹ��rb��ʽ
    {
        fread(&Info.DIB, sizeof(Info.DIB), 1, inptr);
        for (WORD i = Info.Mvalue; i < end; i++)
        {
            Row = MortonToRow(n, i);
            Col = MortonToCol(n, i);

            long offset = bf.bfOffBits + Row * num + Col;				//ͨ�����к�Ѱ�ҵ�д�����ݵ�λ��
            fseek(imgptr, offset, SEEK_SET);
            fwrite(&Info.DIB, sizeof(Info.DIB), 1, imgptr);
        }
        end = Info.Mvalue;
    }

    fclose(inptr);
    fclose(colorptr);
    fclose(imgptr);

    return;
}
