#include "ComFun.h"

int Compress(int num, char *datname, char *colorname, char *bmpname)	//���ݵ�ѹ������
{
    int n = log2(num);
    int StackCount(0);

    FILE *inptr = fopen(bmpname, "r");
    if (inptr == NULL)
    {
        printf("Open Error��");
        return 0;
    }

    /************************************************************************/
    /*�ļ�ͷ��ȡ                                                            */
    /************************************************************************/
    BITMAPFILEHEADER bf;
    fread(&bf.bfType, sizeof(bf.bfType), 1, inptr);
    printf("\nͼ�����ϸ��Ϣ���£�\nbf.bfType:%c%c\n", bf.bfType % 256, bf.bfType / 256);
    fread(&bf.bfSize, sizeof(bf.bfSize), 1, inptr);
    printf("bf.bfSize:%d\n", bf.bfSize);
    fread(&bf.bfReserved1, sizeof(bf.bfReserved1), 1, inptr);
    printf("bf.bfReserved1:%d\n", bf.bfReserved1);
    fread(&bf.bfReserved2, sizeof(bf.bfReserved2), 1, inptr);
    printf("bf.bfReserved2:%d\n", bf.bfReserved2);
    fread(&bf.bfOffBits, sizeof(bf.bfOffBits), 1, inptr);
    printf("bf.bfOffBits:%d\n", bf.bfOffBits);

    /************************************************************************/
    /*��Ϣͷ��ȡ                                                            */
    /************************************************************************/
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    printf("bi.biSize:%d\n", bi.biSize);
    printf("bi.biWidth:%d\n", bi.biWidth);
    printf("bi.biHeight:%d\n", bi.biHeight);
    printf("bi.biSizeImage:%d\n", bi.biSizeImage);
    printf("bi.biBitCount:%d\n", bi.biBitCount);

    /************************************************************************/
    /* 1024B��ɫ�����ݵĴ���                                                */
    /************************************************************************/
    FILE *colorptr = fopen(colorname, "w");

    RGBQUAD quad[256];
    fread(quad, sizeof(RGBQUAD), 256, inptr);

    for (int i = 0; i < 256; i++)
    {
        fprintf(colorptr, "%c%c%c%c", quad[i].rgbBlue, quad[i].rgbGreen, quad[i].rgbRed, quad[i].rgbReserved);
    }

    /************************************************************************/
    /*��DIBդ���������ݽ��д���                                             */
    /************************************************************************/
    Node *header = new Node;	//����ջ�ṹ���Դ洢Morton����&��������
    header->pred = NULL;
    header->succ = NULL;

    InfoStruct str;
    int M(0), layer(0);		//Morton�뼰���ݲ�γ�ʼ��
    int count(0);			//����ȡ�˶��ٴ�2*2դ��

    //�ֱ�ʹ��nλ��2*nλ����������ʾ���кź�Morton��
    int *binaryR = new int[n];		//�洢Row�Ķ�����ֵ
    int *binaryC = new int[n];		//�洢Col�Ķ�����ֵ
    int *binaryM = new int[2 * n];	//�洢Morton�Ķ�����ֵ
    int dRow, dCol;			//�кź��кŵ�ʮ����ֵ
    int rest, Mcopy;

    while (M < num * num)	//Morton��Ľ�������������n*n��
    {
        //ͨ��Morton���ֵ������кŵ�ֵ
        Mcopy = M;
        for (int k = 2 * n - 1; k >= 0; k--)	//��Morton��ת��Ϊ������
        {
            rest = Mcopy % 2;
            Mcopy = Mcopy / 2;
            binaryM[k] = rest;
        }
        for (int k = 0; k < 2 * n; k++)			//ͨ��Morton��������к�
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

        //��ȡ2*2��С�����DIBդ����������(ʹ��fseek��ͨ�����к�Ѱ������)
        BYTE *DIBData = new BYTE[4];
        long offset1 = bf.bfOffBits + dRow * num + dCol;
        fseek(inptr, offset1, SEEK_SET);
        fread(&DIBData[0], sizeof(BYTE), 1, inptr);
        long offset2 = bf.bfOffBits + dRow * num + dCol + 1;
        fseek(inptr, offset2, SEEK_SET);
        fread(&DIBData[1], sizeof(BYTE), 1, inptr);
        long offset3 = bf.bfOffBits + (dRow + 1) * num + dCol;
        fseek(inptr, offset3, SEEK_SET);
        fread(&DIBData[2], sizeof(BYTE), 1, inptr);
        long offset4 = bf.bfOffBits + (dRow + 1) * num + dCol + 1;
        fseek(inptr, offset4, SEEK_SET);
        fread(&DIBData[3], sizeof(BYTE), 1, inptr);

        //�Ƚ�����ֵ��ȷ�������ջ
        if (DIBData[0] == DIBData[1] && DIBData[1] == DIBData[2] && DIBData[2] == DIBData[3])	//���е�ֵ��ȣ���ֻȡ���Ͻ�Morton&DIB��ջ
        {
            str.Mvalue = M;
            str.DIB = DIBData[0];
            stackPush(header, str);
            M += 4;
        }
        else
        {
            for (int i = 0; i < 4; i ++)		//���ĸ�����ֵֵ����ջ
            {
                str.Mvalue = M++;
                str.DIB = DIBData[i];
                stackPush(header, str);
            }
        }
        count++;

        //�ж�Mֵ�Ƿ�����Ҫѹ����������ÿ����ȡ��4��2*2���񣬱����ж�ѹ�����ص㣡
        if (count % 4 == 0)
        {
            InfoStruct *store =  new InfoStruct[4];

            for (int i = 0; i <= layer; i++)				//����ĳһ��ε����ݺ���Ҫ����֮ǰ���в��Ҫ����м��ѹ��
            {
                if ((M % (int)pow((double)4, i + 2)) == 0)	//ȷ��������ѹ�����ؼ�������
                {
                    Node *record = header->succ;
                    if (record != NULL) store[0] = record->info;
                    else break;
                    if (record->succ != NULL) store[1] = record->succ->info;
                    else break;
                    if (record->succ->succ != NULL) store[2] = record->succ->succ->info;
                    else break;
                    if (record->succ->succ->succ != NULL) store[3] = record->succ->succ->succ->info;
                    else break;

                    if (store[0].Mvalue - store[1].Mvalue == pow((double)4, i + 1)			//�ж��������ĸ��ڵ�Mֵ�Ƿ����һ��ֵ���ж����˳��ҪŪ��
                            && store[1].Mvalue - store[2].Mvalue == pow((double)4, i + 1)
                            && store[2].Mvalue - store[3].Mvalue == pow((double)4, i + 1)
                            && store[3].DIB == store[2].DIB
                            && store[2].DIB == store[1].DIB
                            && store[1].DIB == store[0].DIB)								//����Ҫ��Mortonֵ���Ϊ4^(layer+1)�����Ҫ������ֵȫ���
                        stackPop(header, 3);
                    else
                        break;
                    record = NULL;
                }
            }
            delete store;
            store = NULL;
        }
        if (count != 1 && (count & count - 1) == 0 && log2(count) % 2 == 0)		//countΪ4�Ĵ���ʱ��������ӡ�ע��:count==1ʱҲ����4^0Ҫ�󣬹�ȥ��
            layer++;
    }
    //���ˣ����е�դ�������Ѷ�ȡ��ϣ��Ĳ����Ѵ洢��ջ��

    /************************************************************************/
    /* ��������Ҫ�����г̱����ѹ����ʽ���Ĳ�����������һ��ѹ������дΪ�ļ�
    /************************************************************************/
    FILE *outptr = fopen(datname, "wb");   //����ʹ�á�wb����ʽ���ļ���������Ȼ0x0A�ᱻдΪ0x0D0A������
    if (outptr == NULL)
    {
        printf("Open Error.");
    }

    //�ڶ���ѹ�������г̱���ѹ������������������õ��Ǽ�ʱ���������������ڵ��޳�ջ�ķ�ʽ
    Node *recorder = header->succ;
    int DIBsame(0);
    while (recorder->succ != NULL)
    {
        BYTE DIBval = recorder->info.DIB;
        while (recorder->succ != NULL && recorder->succ->info.DIB == DIBval)
        {
            DIBsame++;
            recorder = recorder->succ;
        }
        if (DIBsame != 0) stackPop(header, DIBsame);
        if (DIBsame == 0)
        {
            //fprintf(outptr, "%x %x\n", recorder->info.Mvalue, recorder->info.DIB);
            //fprintf(outptr, "%c%c%c", recorder->info.Mvalue % 256, recorder->info.Mvalue / 256, recorder->info.DIB);
            fwrite(&recorder->info.Mvalue, sizeof(recorder->info.Mvalue), 1, outptr);
            fwrite(&recorder->info.DIB, sizeof(recorder->info.DIB), 1, outptr);
            StackCount++;
            recorder = recorder->succ;
            stackPop(header, 1);
            continue;
        }
        DIBsame = 0;

        if (recorder->succ == NULL) break;

        //fprintf(outptr, "%x %x\n", recorder->info.Mvalue, recorder->info.DIB);
        //fprintf(outptr, "%c%c%c", recorder->info.Mvalue % 256, recorder->info.Mvalue / 256, recorder->info.DIB);
        fwrite(&recorder->info.Mvalue, sizeof(recorder->info.Mvalue), 1, outptr);
        fwrite(&recorder->info.DIB, sizeof(recorder->info.DIB), 1, outptr);
        StackCount++;
        recorder = recorder->succ;
        stackPop(header, 1);
    }
    //fprintf(outptr, "%x %x\n", recorder->info.Mvalue, recorder->info.DIB);
    //fprintf(outptr, "%c%c%c", recorder->info.Mvalue % 256, recorder->info.Mvalue / 256, recorder->info.DIB);
    fwrite(&recorder->info.Mvalue, sizeof(recorder->info.Mvalue), 1, outptr);
    fwrite(&recorder->info.DIB, sizeof(recorder->info.DIB), 1, outptr);
    StackCount++;
    stackPop(header, 1);

    delete header;
    header = NULL;

    fclose(colorptr);
    fclose(outptr);
    fclose(inptr);

    return StackCount;
}