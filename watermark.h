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
CvMat* Insert(CvMat *img_mat,char c[8]);       //嵌入水印算法
char * Check(CvMat *img_mat,int num);          //检测水印的数据
double Psnr(IplImage *image1,IplImage *image2);//峰值信噪比
CvMat *ZigZag_Matrix();                        //ZigZag Scan Matrix
double * ZigZag(CvMat *data);                  //ZigZag扫描
CvMat* inv_ZigZag(double line[]);           //逆ZigZag扫描

CvMat *TileImage(IplImage *image, int m,int n);//重叠的方式延展图像
double *Rand(int m,int key);
IplImage * embedding(IplImage *img, IplImage *logo,double strength);
CvMat *extract(IplImage *img,int key);
double correlation(CvMat *dst, CvMat *org);

