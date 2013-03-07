#ifndef _OpenCV_lib
#define _OpenCV_lib
#include <cxcore.h>
#include <cv.h>
#include <cvaux.h>
//#include "conio.h"
//#include "afx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <highgui.h>

#define pi 3.1415926
#define MAXX(x,y) ((x) > (y) ? (x) : (y))
#define MINX(x,y) ((x) > (y) ? (y):(x))
//#define ROUND(X) (((X-(int)X)>=0.5)?((int)X+1):((int)X))
//#define floor(X) (X>0)?(int(X)):((int)X-1)
#define uchar unsigned char
#endif
void radonc(uchar *input,double* output,int mWidth,int mHeight,int numAngels,int rSize,int step) ;
IplImage *Radon(IplImage *pSrc);
void GetMax(IplImage *pRadon,double **info);
IplImage* CropWhiteBorder(IplImage *imgDst);
IplImage *RotationCorrection(IplImage *img);
int ROUND(double x);
int Max(int *data,int n);
int Min(int *data,int n);
IplImage* RawCropBorder(IplImage *imgDst);
IplImage* RotationCorrection1(IplImage *imgDst);
