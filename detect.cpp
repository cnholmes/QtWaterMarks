
#include "Radon.h"
#include "watermark.h"
#ifndef _OpenCV_lib
#define _OpenCV_lib
#include "cv.h"
#include "highgui.h"
#include<math.h>
#endif

int detect(IplImage* img,char *res){
	char *s=NULL;
/*	CFileDialog mFileDlg(1,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT, "All Files (*.*)|*.*||",AfxGetMainWnd()); 　
	char filename[256];
	if(mFileDlg.DoModal()==ID_OK)
	{
		filename=mFileDlg.GetPathName;
	}*/
//		IplImage* img=cvLoadImage("watermarked.bmp",1);
	
	if(!img)
	{
		printf("The image can not be loaded!");
		return 0;
	}
	
	CvMat * result=cvCreateMat(img->width,img->height,CV_32FC1);// 用于保存DCT变换后的矩阵
//	CvMat * result=cvCreateMat(img->width,img->height,1);	

	IplImage *corrected=0;
	corrected=RotationCorrection(img);
	result=Block_Dct(corrected,8,8);
	
	s=Check(result,8);
	printf("the detected result is s=%s",s);
    strcpy(res,s);
//	cvNamedWindow("Corrected",1);
//	cvShowImage("Corrected",corrected);
	
//	cvWaitKey(0);
//1	cvReleaseImage(&img);
//1	cvReleaseMat(&result );
//	cvDestroyWindow("Corrected");
	
//1	cvReleaseImage( &corrected );
    return 1;
}
