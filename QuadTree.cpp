/****************************************************************************************
 * QuadTree
 * Function:Compression and uncompression of 8-bit bmp image file using quadtree coding method
 * by: Xu Zhiyuan, xzy_1108@163.com
 * 2015/05/06
******************************************************************************************/

#include "Compress.h"
#include "unCompress.h"

int main()
{
    int num, n;
    char bmpname[50], datname[50], colorname[50];

    printf("#ע���˳����������Ϊ8bit(256ɫλ)bmpͼ������Ĳ�������ѹ��\n#���е����ݾ���.\\QuadTree\\QuadTreeĿ¼�²���\n");
    printf("\n������ 1-ѹ��BMP ���� 2-��ѹ��BMP  ");
    int x;
    scanf("%d", &x);
    if (x == 1)
    {
        printf("\n������ͼ������Ƽ���׺����");
        scanf("%s", bmpname);
        printf("\n������������ͼ��ı߳�(ȷ��ͼ��߳���СΪ2�Ĵ���)��");
        scanf("%d", &num);
        if (num & (num - 1))	//num��Ϊ2���ݴη�
        {
            printf("\n���ɽ����Ĳ������룬��ȷ������ͼ��߳���СΪ2���ݴη���\n");
            return 0;
        }
        else
            n = log2(num);

        printf("\n������ͼ��ѹ����ѹ���ļ������Ƽ���׺����");		//��ν��·���������
        scanf("%s", datname);
        printf("\n������ͼ��ѹ����������ɫ���ļ������Ƽ���׺����");		//��ν��·���������
        scanf("%s", colorname);

        printf("\n��ѹ��Ϊ %d ������\n", Compress(num, datname, colorname, bmpname));
    }
    else if (x == 2)
    {
        printf("\n������ѹ���ļ������Ƽ���׺����");		//��ν��·���������
        scanf("%s", datname);
        printf("\n��������ɫ���ļ������Ƽ���׺����");
        scanf("%s", colorname);
        printf("\n������������ͼ��ı߳�(ȷ��ͼ��߳���СΪ2�Ĵ���)��");
        scanf("%d", &num);
        if (num & (num - 1))	//num��Ϊ2���ݴη�
        {
            printf("\n���ɽ����Ĳ������룬��ȷ������ͼ��߳���СΪ2���ݴη���\n");
            return 0;
        }
        else
            n = log2(num);
        printf("\n�������ѹ��ͼ������Ƽ���׺����");
        scanf("%s", bmpname);

        unCompress(num, datname, colorname, bmpname);
        printf("\n��ѹ����ϣ�\n");
    }
    else
        printf("\n��������ȷ�ı�ţ�\n");

    system("pause");
    return 0;
}