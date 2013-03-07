#include "Radon.h"
#define DEBUG 0
// 输出整型数据，用于调试用途
void out(int* temp,int n)
{
	//	printf("the matrix is");
	for(int i=0;i<n;i++)
		printf("%d  ",temp[i]);
	printf("\n");
}
//**********************************************************
//对给定整型数据，统计出现频率最高的数据值，并返回这个频率值
//该函数暂时还没有用到
//**********************************************************
int MaxFreq(int *data,int n)
{
	int i,j;
	int *freq,*num;
	
	int count=1;
	int result=0,index=0;
	freq=(int *)malloc(n*sizeof(int));
	num=(int *)malloc(n*sizeof(int));
	num[0]=data[0];
	freq[0]=1;
	for(i=1;i<n;i++)
	{
		for(j=0;j<count;j++)
		{
			if(data[i]==num[j])
				break;
		}
		if(j==count)
		{
			count++;
			num[j]=data[i];
			freq[j]=1;
		}
		else
		{
			freq[j]++;
		}
	}
	if(DEBUG)
	{
		printf("the data:\n");
		out(num,count);
		printf("the frequency:\n");
		out(freq,count);
	}
	
	result=freq[0];
	for(i=0;i<count;i++)
		if(freq[i]>result)
		{
			index=i;
			result=freq[i];
		}
		result=num[index];
		free(freq);
		free(num);
		return result;
}
//*********************************************
//取整函数，采用四舍五入的方式
//*********************************************
int ROUND(double x)
{
	int ret ;
	if(x>2.00e+08)
		x=2.00e+08 ;
	if(x<-2.00e+08)
		x=-2.00e+08 ;
	if(x>0) 
	{
		ret=((x-((int)x))>=0.5)?((int)x+1):((int)x) ;
	}else
	{
		ret=((x-(int)x)<=-0.5)?((int)x-1):((int)x) ;
	}
	return ret ;
};

//******************************************************
//对给定整型数组，计算最大值，并返回这个值
//******************************************************
int Max(int *data,int n)
{
	int i;
	int result=0;
	result=data[0];
	for(i=1;i<n;i++)
	{
		if(data[i]>result) result=data[i];
	}
	return result;
}
//******************************************************
//对给定整型数组，计算最小值，并返回这个值
//******************************************************
int Min(int *data,int n)
{
	int i;
	int result=0;
	result=data[0];
	for(i=1;i<n;i++)
	{
		if(data[i]<result) result=data[i];
	}
	return result;
}


//************************************************************
//计算数据的Radon变换，类似于matlab 中的Radon变换函数的实现。
//************************************************************
void radonc(uchar *input,double* output,int mWidth,int mHeight,int numAngels,int rSize,int step) 
{
	int m,n,t,r ,rho;
	int rhomax;//max length of row  ;
	int rc;//the center of row ;
	double x,y;
	
	int mt=numAngels ;
	double deg2rad = 3.14159265358979 / 180.0;
	double costheta,sintheta ;
	double a,b;//y=ax+b ;
	int ymax,ymin,xmax,xmin,xtemp,ytemp,rhooffset ;
	double up,low ;
	int floor1=0 ;
	int mStep=step ;
	m=ROUND(mWidth/2) ;
	n=ROUND(mHeight/2) ;
	
	rhomax=rSize ;
	rc=ROUND(rhomax/2) ;
	for(t=1;t<46;t++)
	{
		costheta=cos(t*deg2rad) ;
		sintheta=sin(t*deg2rad) ;
		a=-costheta/sintheta ;//y=ax+b ;//note that here can rhoptimized ;
		for(r=0;r<rhomax;r++)
		{
			rho=r-rc +1;
			b=rho/sintheta ;
			ymax=MINX((int)(ROUND(-a*m+b)),(n-1)) ;
			ymin=MAXX((int)(ROUND(a*m+b)),(-n)) ;
			for(ytemp=ymin;ytemp<ymax+1;ytemp++)
			{
				x=(ytemp-b)/a ;
				xtemp=(int)floor(x) ;
				floor1=xtemp ;
				up=x-floor1 ;
				low=1-up ;
				
				xtemp=MAXX(floor1,-m) ;
				xtemp=MINX(xtemp,(m-2)) ;
				output[(rhomax-r)*mt+mt-t]+=low*(double)(uchar)input[(ytemp+n)*mStep+xtemp+m]; ;
				output[(rhomax-r)*mt+mt-t]+=up*(double)(uchar)input[(ytemp+n)*mStep+xtemp+m+1] ;
			}
		}
	}
	for(t=46;t<91;t++)
	{
		costheta=cos(t*deg2rad) ;
		sintheta=sin(t*deg2rad) ;
		a=-costheta/sintheta ;//y=ax+b ;//note that here can rhoptimized ;
		for(r=0;r<rhomax;r++)
		{
			rho=r-rc+1 ;
			b=rho/sintheta ;
			xmax=MINX((int)(ROUND((-n-b)/a)),(m-1)) ;
			xmin=MAXX((int)(ROUND((n-b)/a)),(-m)) ;
			
			for(xtemp=xmin;xtemp<xmax+1;xtemp++)
			{
				y=(a*xtemp+b) ;
				ytemp=(int)floor(y) ;
				floor1=ytemp ;
				up=y-floor1;
				
				low=1-up ;
				ytemp=MAXX(floor1,(-n)) ;
				ytemp=MINX(ytemp,(n-2)) ;
				output[(rhomax-r)*mt+mt-t]+=low*(double)(uchar)input[(ytemp+n)*mStep+xtemp+m];
				output[(rhomax-r)*mt+mt-t]+=up*(double)(uchar)input[(ytemp+n+1)*mStep+xtemp+m];
			}
		}
	}
	for( t=91;t<136;t++)
	{
		costheta=cos(t*deg2rad) ;
		sintheta=sin(t*deg2rad) ;
		a=-costheta/sintheta ;//y=ax+b ;//note that here can rhoptimized ;
		for(r=0;r<rhomax;r++)
		{
			rho=r-rc+1 ;
			b=rho/sintheta ;
			xmax=MINX((int)(ROUND((n-b)/a)),m-1) ;
			xmin=MAXX((int)(ROUND((-n-b)/a)),(-m)) ;
			for(xtemp=xmin;xtemp<xmax+1;xtemp++)
			{
				y=(a*xtemp+b) ;
				ytemp=(int)floor(y) ;
				floor1=ytemp ;
				up=y-floor1 ;
				low=1-up ;
				
				ytemp=MAXX(floor1,(-n)) ;
				ytemp=MINX(ytemp,(n-2)) ;
				output[(rhomax-r)*mt+mt-t]+=low*(double)(uchar)input[(ytemp+n)*mStep+xtemp+m]; 
				output[(rhomax-r)*mt+mt-t]+=up*(double)(uchar)input[(ytemp+n+1)*mStep+xtemp+m]; 
			}
		}
		
	}
	for(t=136;t<180;t++)
	{
		costheta=cos(t*deg2rad) ;
		sintheta=sin(t*deg2rad) ;
		a=-costheta/sintheta ;//y=ax+b ;//note that here can rhoptimized ;
		for(r=0;r<rhomax;r++)
		{
			rho=r-rc+1 ;
			b=rho/sintheta ;
			ymax=MINX((ROUND(a*m+b)),(n-1)) ;
			ymin=MAXX((ROUND(-a*m+b)),(-n)) ;
			for(ytemp=ymin;ytemp<ymax+1;ytemp++)
			{
				x=(ytemp-b)/a ;
				xtemp=(int)floor(x) ;
				
				floor1=xtemp ;
				up=x-floor1 ;
				low=1-up ;
				
				xtemp=MAXX(floor1,(-m)) ;
				xtemp=MINX(xtemp,(m-2)) ;
				output[(rhomax-r)*mt+mt-t]+=low*(double)(uchar)input[(ytemp+n)*mStep+xtemp+m];
				output[(rhomax-r)*mt+mt-t]+=up*(double)(uchar)input[(ytemp+n)*mStep+xtemp+m+1];
			}
		}
	}
	
	t=0 ;
	rhooffset=ROUND(((rhomax-mWidth)/2)) ;
	for(xtemp=0;xtemp<mWidth;xtemp++)
	{
		r=xtemp+rhooffset+1 ;
		r=rhomax-r+1 ;
		for(ytemp=0;ytemp<mHeight;ytemp++)
		{
			output[r*mt+t]+=(double)(uchar)input[ytemp*mStep+xtemp] ;
		}
	}
}
//****************************************************
//对图像进行Radon变换，返回变换后的数据
//****************************************************
IplImage *Radon(IplImage *pSrc)
{
	IplImage *pRadon=NULL ;
	int mWidth ,mHeight,mStep  ;
	int len,numAngels ;
	double *pRadonData ;
	if(pSrc==NULL)
		return pRadon;
	if(pSrc->nChannels!=1) 
		return pRadon;
	mWidth=pSrc->width ;
	mHeight=pSrc->height ;
	mStep=pSrc->widthStep ;
	
	len=(int)ceil(sqrt((double)(mWidth*mWidth+mHeight*mHeight)))+1 ;
	numAngels=180 ;
	pRadon=cvCreateImage(cvSize(numAngels,len),IPL_DEPTH_64F,1) ;
	pRadonData=(double*)(pRadon->imageData) ;
	radonc((uchar*)(pSrc->imageData),pRadonData,mWidth,mHeight,180,len-1,mStep) ;
	return pRadon ;
}
//*****************************************************************
//统计Radon变换后矩阵中数据的最大值，返回最大和次大值对应的theta值
//以及对应的最大值、次大值出现的位置
//*****************************************************************
void GetMax(IplImage *pRadon,double **info)
{
	double theta1,theta2,len1,len2 ;
	int mWidth,mHeight,mStep ;
	int i,j ;
	double temp1,temp,temp2;
	double *pData ;
	
	mWidth=pRadon->width ;
	mHeight=pRadon->height ;
	mStep=pRadon->widthStep ;
	pData=(double*)(pRadon->imageData) ;
	
	temp1=0.0 ;
	theta1=len1=0.0 ;
	
	for(i=0;i<mHeight;i++)
	{
		for(j=0;j<mWidth;j++)
		{
			temp=fabs(pData[i*mWidth+j]);
			if(temp>temp1)
			{
				temp1=temp ;
				theta1=j ;
				len1=i ;
			}
		}
	}
	pData[(int)(len1*mWidth+theta1)]=0;
	for(i=0;i<mHeight;i++)
	{
		for(j=0;j<mWidth;j++)
		{
			temp=fabs(pData[i*mWidth+j]);
			if(temp>temp2)
			{
				temp2=temp ;
				theta2=j ;
				len2=i ;
			}
		}
	}
	(*info)[0]=theta1 ;
	(*info)[1]=len1 ;
	(*info)[2]=temp1 ;
	(*info)[3]=theta2;
	(*info)[4]=len2;
	(*info)[5]=temp2;	
	return;
}

/**************************************************************************/
//利用投影的方式去掉图像边缘的白边
//*************************************************************************/
IplImage* CropWhiteBorder(IplImage *imgDst)
{
/******************************************************************
Input: The image have been rotation corrected or with white border
output: The image after removing the white border is saved in the file:
cropped.bmp
	*********************************************************************/
	IplImage* pCannyImg=0;
	IplImage *pImage=cvCreateImage(cvGetSize(imgDst),IPL_DEPTH_8U,1);
	//边缘查找和裁剪
	pCannyImg=cvCreateImage(cvGetSize(imgDst),IPL_DEPTH_8U,1);
	IplImage* smooth=cvCreateImage(cvGetSize(imgDst),IPL_DEPTH_8U,1);
	if(imgDst->nChannels>1)
	{
		cvCvtColor(imgDst, pImage, CV_BGR2GRAY);
	}
	else
	{
		cvCopy(imgDst,pImage);
	}

	cvSmooth(pImage,smooth,CV_GAUSSIAN);
	cvCanny(smooth, pCannyImg, 50, 150, 3);
#if DEBUG
	cvNamedWindow("Crop_Canny",1);
	cvShowImage("Crop_Canny",pCannyImg);
	cvWaitKey(0);
	cvDestroyWindow("Crop_Canny");
#endif
	CvScalar s;
	int row,col,row1,col1;//variable to save top-left and right-bottom position
//	cvSaveImage("pCannyImg1.bmp",pCannyImg);
	int sum=0,*Hist_x,*Hist_y;
	int i,j,begin,end;
	char found=false;
	//To Find the Top-left position	
	Hist_x=(int *)malloc(pCannyImg->height*sizeof(int));
	Hist_y=(int *)malloc(pCannyImg->width*sizeof(int));
	for(i=0;i<pCannyImg->height;i++)//计算水平投影
	{
		sum=0;
		for(j=0;j<pCannyImg->width;j++)
		{
			s=cvGet2D(pCannyImg,i,j);
			if(s.val[0]>1)
				sum+=1;
		}
		Hist_x[i]=sum;
	}
	for(j=0;j<pCannyImg->width;j++)//计算垂直投影
	{
		sum=0;
		for(i=0;i<pCannyImg->height;i++)
		{
			s=cvGet2D(pCannyImg,i,j);
			if(s.val[0]>1)
				sum+=1;
		}
		Hist_y[j]=sum;
	}
	for(i=0;i<pCannyImg->height;i++)
	{
		if(Hist_x[i]>0)
		{
			begin=i;
			while(Hist_x[i]>0&&i<pCannyImg->height)
			{
				i++;
			}
			end=i;
			if(end-begin>50){found=true; break;}//长度超过50像素的块才需要保留
		}
	}
	if(found)
	{
		row=begin;
		row1=end;
	}
	found=false;
	for(i=0;i<pCannyImg->width;i++)
	{
		if(Hist_y[i]>0)
		{
			begin=i;
			while(Hist_y[i]>0&&i<pCannyImg->width)i++;
			end=i;
			if(end-begin>50){found=true; break;} //长度超过50像素的块才需要保留
		}
	}
	if(found)
	{
		col=begin;
		col1=end;
	}
	free(Hist_x);
	free(Hist_y);
#if DEBUG
	printf("Found boundary:row=%d,col=%d,row1=%d,col1=%d",row,col,row1,col1);
#endif
	if(col1<=col||row1<=row)//如果没有检测到物体则返回空
	{
		cvReleaseImage(&pCannyImg);
		cvReleaseImage(&pImage);	
		cvReleaseImage(&smooth);
		IplImage *temp=0;
		return temp;
	}
	cvSetImageROI(imgDst,cvRect(col,row,col1-col,row1-row));  
	IplImage *dst = cvCreateImage(cvSize(col1-col,row1-row),IPL_DEPTH_8U,imgDst->nChannels); 
	cvCopy(imgDst,dst);  
	cvResetImageROI(imgDst);  
//	cvSaveImage("cropped.bmp",dst);	

	cvReleaseImage(&pCannyImg);
	cvReleaseImage(&pImage);	
	cvReleaseImage(&smooth);
	return dst;	
}

//************************************************
//对给定输入图像，检测倾斜角度，并进行逆旋转
//************************************************
IplImage *RotationCorrection(IplImage *img)
{
	if(!img)
	{
		printf("The image can not be loaded!");
		return 0;
	}
	IplImage* imggray=cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);	
	if(img->nChannels==3)
	{
		cvCvtColor(img,imggray,CV_RGB2GRAY);//转换为灰度图
	}
	else
		imggray=cvCloneImage(img);
	
	IplImage *pCannyImg=cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);//保存边缘图像
	IplImage* smooth=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	cvSmooth(imggray,smooth,CV_GAUSSIAN);
	cvCanny(smooth, pCannyImg, 50, 150, 3);
	
	//Find the background color, for the rotation correction color fill
	CvScalar backcolor=cvScalar(255,255,255),s,t,sum_t=cvScalar(0);
	int count=0;
	for(int i=1;i<10;i++)
		for(int j=0;j<img->width;j++)
		{
			s=cvGet2D(pCannyImg,i,j);
			if(s.val[0]>1) break;
			t=cvGet2D(img,i,j);
			sum_t.val[0]+=t.val[0];
			sum_t.val[1]+=t.val[1];
			sum_t.val[2]+=t.val[2];
			count++;
		}
		if(count>0)
		{		backcolor.val[0]=(int)(sum_t.val[0]/count);
		backcolor.val[1]=(int)(sum_t.val[1]/count);
		backcolor.val[2]=(int)(sum_t.val[2]/count);
		}
		
#if DEBUG
		printf("the background color:%f, %f %f\n",backcolor.val[0],backcolor.val[1],backcolor.val[2]);
#endif		
		//************************************************
		
		IplImage *Result=NULL;
		Result=Radon(pCannyImg); //进行Radon变换
		double *info=(double *)malloc(6*sizeof(double));
		GetMax(Result,&info);
		double theta1=info[0];
		double theta2=info[3];
		double theta;
		if(theta2>theta1)
		{
			theta1=theta2;
			theta2=info[0];
		}
		theta=theta2;
		if(theta>90)
		{
			if(theta<135)
				theta-=90;
			else
				theta=theta-180;
		}
		if(theta>45)
		{
			theta-=90;
		}
		
#if DEBUG
		printf("the result: theta=%f, len1=%f, temp=%f theta2=%f len2=%f, temp2=%f\n",theta,info[1],info[2],info[3],info[4],info[5]);
#endif
		if(fabs(info[2])<42000)
		{
			theta=0;
		}
		delete []info;
//进行旋转处理，theta是角度，angle是弧度
		double angle=theta*CV_PI/180;
		double a=sin(angle);
		double b=cos(angle);
		int w_src=img->width;
		int h_src=img->height;
		int w_dst=int(h_src*fabs(a)+w_src*fabs(b));
		int h_dst=int(w_src*fabs(a)+h_src*fabs(b));
		double map[6];
		CvMat M=cvMat(2,3,CV_64FC1,map);
		CvPoint2D32f pt=cvPoint2D32f(w_src/2,h_src/2);
		cv2DRotationMatrix(pt,-theta,1.0,&M);
		map[2]+=(w_dst-w_src)/2;
		map[5]+=(h_dst-h_src)/2;
		IplImage *imgDst=cvCreateImage(cvSize(w_dst,h_dst),img->depth,img->nChannels);
		IplImage *corrected=0;
		if(fabs(theta)>1e-3)//有旋转则进行逆旋转，否则保持现状
		{
			cvWarpAffine(img,imgDst,&M,CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS,backcolor);//图像旋转校正
#if DEBUG
				cvNamedWindow("After_Rotation",1);
				cvShowImage("After_Rotation",imgDst);
				cvWaitKey(0);
				cvDestroyWindow("After_Rotation");
#endif
		}
		else
		{
			cvCopy(img,imgDst);;
		}
		if(corrected=CropWhiteBorder(imgDst)) //裁剪掉旋转后的白边
		{
			printf("The white border is deleted!\n");
			//		corrected=cvLoadImage("cropped.bmp",-1);
#if DEBUG
				cvNamedWindow("Noborder",1);
				cvShowImage("Noborder",corrected);
				cvWaitKey(0);
				cvDestroyWindow("Noborder");
#endif
		}
		IplImage *Crp=CropWhiteBorder(corrected);

		cvReleaseImage(&imggray);
		cvReleaseImage(&pCannyImg);
		cvReleaseImage(&smooth);
		cvReleaseImage(&Result);

		cvReleaseImage(&corrected);
		cvReleaseImage(&imgDst);
		return Crp;	
}

/**************************************************************************/
//对微小倾斜角度进行三角形查找校正，做进一步的微小角度矫正
//*************************************************************************/
IplImage* RotationCorrection1(IplImage *img)
{
	IplImage* pCannyImg=0;
	CvScalar s;
	IplImage *pImage=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	//边缘查找和裁剪
	pCannyImg=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	IplImage* smooth=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	if(img->nChannels>1)
	{
		cvCvtColor(img, pImage, CV_BGR2GRAY);
	}
	else
	{
		cvCopy(img,pImage);
	}
	cvSmooth(pImage,smooth,CV_GAUSSIAN);
	cvCanny(smooth, pCannyImg, 50, 150, 3);
	//	cvDilate( pCannyImg, pCannyImg, 0, 1 );
	//	cvErode(pCannyImg, pCannyImg, 0, 1);
	double sum=0;
	int i,j;
	bool found=false;
	//To Find the Top-left position	
	for(i=0;i<pCannyImg->height;i++)
	{
		sum=0;
		for(j=0;j<pCannyImg->width;j++)
		{
			s=cvGet2D(pCannyImg,i,j);
			if(s.val[0]>1)
				sum+=1;
		}
		if(sum>pCannyImg->width*0.9)
		{
			found=true;
			break;
		}
	}
	if(!found)
	{
		for(j=0;j<pCannyImg->width;j++)
	{
		sum=0;
		for(i=0;i<pCannyImg->height;i++)
		{
			s=cvGet2D(pCannyImg,i,j);
			if(s.val[0]>1)
				sum+=1;
		}
		if(sum>pCannyImg->height*0.9)
		{
			found=true;
			break;
		}

	}
	}
	//found=true意味着存在水平或垂直边，不需要校正

	if(found)//纵横边如果水平或垂直则不需要校正
	{
		cvReleaseImage(&pImage);
		cvReleaseImage(&smooth);
		cvReleaseImage(&pCannyImg);
		IplImage* corrected=cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
		cvCopy(img,corrected);
		return corrected;
	}

	//******************************************
	//Find the background color
	CvScalar backcolor=cvScalar(255,255,255);//,t,sum_t=cvScalar(0);
	int count=0,k;
/*	for(i=1;i<8;i++)
		for(j=0;j<img->width;j++)
		{
			s=cvGet2D(pCannyImg,i,j);
			if(s.val[0]>1) break;
			t=cvGet2D(img,i,j);
			sum_t.val[0]+=t.val[0];
			sum_t.val[1]+=t.val[1];
			sum_t.val[2]+=t.val[2];
			count++;
		}
		if(count>0)
		{		
			backcolor.val[0]=(int)(sum_t.val[0]/count);
			backcolor.val[1]=(int)(sum_t.val[1]/count);
			backcolor.val[2]=(int)(sum_t.val[2]/count);
		}*/
#if DEBUG
		cvNamedWindow("SecondCrop_Canny",1);
		cvShowImage("SecondCrop_Canny",pCannyImg);
		cvWaitKey(0);
		cvDestroyWindow("SecondCrop_Canny");
#endif
		int sign=1;
		double theta;
		//To Find the Top-left Triangle edge's length
		//To computer the small angle for adjustment
		double *row,*col;
		row=(double *)malloc(pCannyImg->height*sizeof(double)); //保存直角三角形竖边长度
		col=(double *)malloc(pCannyImg->height*sizeof(double));//保存直角三角形横边长度
		for(i=0;i<pCannyImg->height;i++)
		{
			for(j=i;j<pCannyImg->width;j++) 
			{
				s=cvGet2D(pCannyImg,i,j);
				if(s.val[0]>1)break;
			}
			col[i]=j-i; //横边长度值
			for(k=i;k<pCannyImg->height;k++)
			{
				s=cvGet2D(pCannyImg,k,i);
				if (s.val[0]>1) break;
			}
			row[i]=k-i;  //竖边长度值
			if(i==j||k==i)break; 
		}
		count=i; //统计的所有三角形个数
		sum=0;
		if(count>2) //最后两个最小的三角形计算角度误差较大，因此丢弃。其他三角形计算正切值，然后求平均
		{
			if(col[1]<row[1])
			{
				for(k=1;k<count-1;k++)
					sum+=col[k]/row[k];
			}
			else
			{
				for(k=1;k<count-1;k++)
					sum+=row[k]/col[k];	
				sign=-1;//顺时针旋转校正标记
			}
			sum/=(count-2);//计算出倾斜角度正切的平均值
		}
		else
			sum=0;//计算出倾斜角度正切的平均值
		theta=sign*atan(sum);//弧度为单位的旋转角;
		IplImage *imgDst=0;
		IplImage *corrected=0;
		double angle=theta/CV_PI*180.0; //角度为单位的旋转角
		double a=sin(theta);
		double b=cos(theta);
		int w_src=img->width;
		int h_src=img->height;
		int w_dst=int(h_src*fabs(a)+w_src*fabs(b));
		int h_dst=int(w_src*fabs(a)+h_src*fabs(b));
		double map[6];
		CvMat M=cvMat(2,3,CV_64FC1,map);
		CvPoint2D32f pt=cvPoint2D32f(w_src/2,h_src/2);
		
		if(fabs(theta)>0.0025) //对应倾斜超过2个像素的角度才需要校正
		{		
			cv2DRotationMatrix(pt,angle,1.0,&M);
			map[2]+=(w_dst-w_src)/2;
			map[5]+=(h_dst-h_src)/2;
			imgDst=cvCreateImage(cvSize(w_dst,h_dst),img->depth,img->nChannels);
			cvWarpAffine(img,imgDst,&M,CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS,backcolor);//图像旋转校正
#if DEBUG
			{cvNamedWindow("AfterSecondRotationCorrection",1);
				cvShowImage("AfterSecondRotationCorrection",imgDst);
				cvWaitKey(0);
				cvDestroyWindow("AfterSecondRotationCorrection");}
#endif
		}
		else
		{
			imgDst=cvCreateImage(cvSize(w_src,h_src),img->depth,img->nChannels);
			cvCopy(img,imgDst);
		}
		if(corrected=CropWhiteBorder(imgDst)) //去掉旋转后的白边
		{
			printf("The white border is deleted!\n");
			//		corrected=cvLoadImage("cropped.bmp",-1);
#if DEBUG
			{
				cvNamedWindow("Noborder",1);
				cvShowImage("Noborder",corrected);
				cvWaitKey(0);
				cvDestroyWindow("Noborder");
			}
#endif
		}
		free(row);
		free(col);
		cvReleaseImage(&pImage);
		cvReleaseImage(&smooth);
		cvReleaseImage(&pCannyImg);
		cvReleaseImage(&imgDst);
		return corrected;	
}

