#ifndef _OpenCV_lib

#define _OpenCV_lib
#pragma comment (lib,"highgui.lib")
#pragma comment (lib,"cv.lib")
#pragma comment (lib,"cxcore.lib")
#pragma comment (lib,"cvaux.lib")
#include <cxcore.h>
#include <cv.h>
#include <cvaux.h>
/*#include <stdio.h>
#include <stdlib.h>*/
#include <math.h>
#include <highgui.h>
#define mod(a,b) (a-(int)(a/b)*b)
#define pi 3.1415926
#define MAXX(x,y) ((x) > (y) ? (x) : (y))
#define MINX(x,y) ((x) > (y) ? (y):(x))
#define uchar unsigned char
#define N 8
#define DEBUG 0
#endif
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

CvMat *Block_Dct(IplImage *img,int row, int col);

CvMat *InverseBlockDct(CvMat *img_mat,int row, int col);
CvMat* Insert(CvMat *img_mat,char c[8]);       //Ƕ��ˮӡ�㷨
char * Check(CvMat *img_mat,int num);          //���ˮӡ������
double Psnr(IplImage *image1,IplImage *image2);//��ֵ�����
CvMat *ZigZag_Matrix();                        //ZigZag Scan Matrix
double * ZigZag(CvMat *data);                  //ZigZagɨ��
CvMat* inv_ZigZag(double line[]);           //��ZigZagɨ��

CvMat *TileImage(IplImage *image, int m,int n);//�ص��ķ�ʽ��չͼ��
double *Rand(int m,int key);
IplImage * embedding(IplImage *img, IplImage *logo,double strength);
CvMat *extract(IplImage *img,int key);
double correlation(CvMat *dst, CvMat *org);

