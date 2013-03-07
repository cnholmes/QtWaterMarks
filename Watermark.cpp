#include "watermark.h"

CvMat *Block_Dct(IplImage *img,int row, int col)
{
	IplImage* imggray=cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);	
	if(img->nChannels>1)
		cvCvtColor(img,imggray,CV_RGB2GRAY);
	else
		cvCopy(img,imggray);
	CvMat *img_mat= cvCreateMat(imggray->height, imggray->width, CV_32FC1);//������ʽ��ͼ��洢
	cvConvert(imggray,img_mat);	
	CvMat *BlockDct=cvCreateMat(imggray->height, imggray->width, CV_32FC1);//���ؽ���������
	
	//��ȡͼ����Ϣ
	int height,width;
	height=imggray->height;
	width=imggray->width;	
	
	
	//����ֿ� ������һ��row*col�Ӿ��������ȡԭ�������ݣ�����DCT�任����ֵ����һ����
	
	for (int ii=0;ii<(int)(height/row);ii++)
	{
		for(int jj=0;jj<(int)(width/col);jj++)
		{
			CvMat *imgmatsub=cvCreateMat(row,col,CV_32FC1);
			CvRect Rect=cvRect(col*jj,row*ii,col,row);
			cvGetSubArr(img_mat,imgmatsub,Rect);//get submat
/*			for(int k=0;k<8;k++)
				for(int l=0;l<8;l++)
				{
					cvmSet(imgmatsub,k,l,cvmGet(img_mat,ii*8+k,jj*8+l));
				}*/
			CvMat* imgdstsub=cvCreateMat(row,col,CV_32FC1);
			cvDCT(imgmatsub,imgdstsub,CV_DXT_FORWARD);//DCT for every submat			
			
			//combine all matsub to  BlockDct
			
			for (int i=0;i<row;i++)
			{
				for(int j=0;j<col;j++)
				{	
					cvmSet(BlockDct,row*ii+i,col*jj+j,cvmGet(imgdstsub,j,i));
				}                                                                                                                                                                                                                                                                                                
			}
			cvReleaseMat(&imgmatsub);
			cvReleaseMat(&imgdstsub);
		}
	}
	cvReleaseImage(&imggray);
	
	cvReleaseMat(&img_mat);
	return BlockDct;
}


CvMat *InverseBlockDct(CvMat *img_mat,int row, int col)
{
	CvMat *Img_Resconstruct_Dct= cvCreateMat(img_mat->height,img_mat->width, CV_32FC1);//ͼ����ʽ��ͼ��洢
	//��ȡͼ����Ϣ
	int height,width;
	height=img_mat->rows;
	width=img_mat->cols;	
	
	//����ֿ� ������һ��row*col�Ӿ��������ȡԭ�������ݣ�����DCT�任����ֵ����һ����
	
	for (int ii=0;ii<(int)(height/row);ii++)
	{
		for(int jj=0;jj<(int)(width/col);jj++)
		{
			CvMat *imgmatsub=cvCreateMat(row,col,CV_32FC1);
			CvRect Rect=cvRect(col*jj,row*ii,col,row);
			cvGetSubArr(img_mat,imgmatsub,Rect);//get submat
			CvMat* imgdstsub=cvCreateMat(row,col,CV_32FC1);
			cvDCT(imgmatsub,imgdstsub,CV_DXT_INVERSE);//IDCT for every submat			
			
			//combine all matsub to  BlockDct
			
			for (int i=0;i<row;i++)
			{
				for(int j=0;j<col;j++)
				{	
					cvmSet(Img_Resconstruct_Dct,row*ii+i,col*jj+j,cvmGet(imgdstsub,j,i));
				}                                                                                                                                                                                                                                                                                                
			}
			cvReleaseMat(&imgmatsub);
			cvReleaseMat(&imgdstsub);
		}
	}
	return Img_Resconstruct_Dct;
}


CvMat* Insert(CvMat *img_mat,char c[8])     //Ƕ��ˮӡ�㷨������ͳ�������������Ĺ�ϵ
{
//	printf("c=%s",c);
	int i,j,k,m,n,d;
	double p;			//d��ʾǶ��ǿ��
	d=2000;
	int Total_Width=(int)(img_mat->cols/8.0);
	int Total_Height=(int)(img_mat->rows/8.0);
	int Total=Total_Width*Total_Height;
	double temp=0;
	int s_ii,s_jj;
	int ii,jj;
	CvPoint pos[8]={{0,2},{1,1},{2,0},{0,3},{1,2},{3,0},{1,0},{0,1}};
	
	for(k=0;k<8;k++)
	{
		
		i=pos[k].x;
		j=pos[k].y;
		m=n=0;
		for(ii=0;ii<Total_Height;ii++)
		{
			for(jj=0;jj<Total_Width;jj++)
			{
				temp=cvmGet(img_mat,ii*8+i,jj*8+j);
				if(temp>0)
					m++;
				if(temp<0)
					n++;
			}
		}
		printf("m=%d n=%d",m,n);
		if(c[k]=='0')     //Ƕ��0ʱ
		{
			
			while(m-n<d)
			{						    
				p=cvmGet(img_mat,i,j);
				for(ii=0;ii<Total_Height;ii++)
				{
					for(jj=0;jj<Total_Width;jj++)
					{					
						temp=cvmGet(img_mat,ii*8+i,jj*8+j);
						if(temp<0)
						{
							if(p>0)
							{
								p=temp;	
								s_ii=ii;s_jj=jj;
							}
							else
							{
								if(p<temp)
								{
									p=temp;s_ii=ii;s_jj=jj;
								}
							}
						}
					}
				}
				cvmSet(img_mat,8*s_ii+i,8*s_jj+j,-1*p);
				printf("position(%d,%d)=%f\n",8*s_ii+i,8*s_jj+j,p);
				m++;n--;
			}					
		}
		if(c[k]=='1')   //Ƕ��1ʱ
		{
			while(n-m<d)
			{
				
				p=cvmGet(img_mat,i,j);
				for(ii=0;ii<Total_Height;ii++)
				{
					for(jj=0;jj<Total_Width;jj++)
					{	
						//					printf("ii=%d,jj=%d\n",ii,jj);
						temp=cvmGet(img_mat,ii*8+i,jj*8+j);
						if(temp>0)
						{
							if(p<0)
							{
								p=temp;s_ii=ii;s_jj=jj;
							}
							else
							{
								if(p>temp)
								{
									p=temp;s_ii=ii;s_jj=jj;
								}
							}
						}				
					}
				}
//				printf("s_ii=%d,s_jj=%d j=%d\n",s_ii,s_jj,j);
				cvmSet(img_mat,8*s_ii+i,8*s_jj+j,-1*p);	
				printf("position(%d,%d)=%f\n",8*s_ii+i,8*s_jj+j,p);
				m--;n++;
			}
		}
	}
	return img_mat;
}

char * Check(CvMat *img_mat,int num)          //���ˮӡ�����ݣ�����ͳ�������������Ĺ�ϵ
{
	int ii,jj;
	double temp;
	char *s=(char *)malloc(num*sizeof(char));
	
	int i,j,k,m,n;
	int Total_Width=(int)(img_mat->cols/8.0);
	int Total_Height=(int)(img_mat->rows/8.0);
	int Total=Total_Width*Total_Height;
	CvPoint pos[8]={{0,2},{1,1},{2,0},{0,3},{1,2},{3,0},{1,0},{0,1}};
	for(k=0;k<num;k++)
	{
		i=pos[k].x;
		j=pos[k].y;
		m=n=0;
		for(ii=0;ii<Total_Height;ii++)
		{
			for(jj=0;jj<Total_Width;jj++)
			{
				temp=cvmGet(img_mat,ii*8+i,jj*8+j);
				if(temp>0)
					m++;
				if(temp<0)
					n++;
			}
		}
		if(m>n)
			s[k]='0';
		else
			s[k]='1';
	}
	s[k]='\0';
	return s;
}

double Psnr(IplImage *image1,IplImage *image2)//��ֵ�����
{
	double mse,psnr,temp;
	mse=psnr=0;	
	
	if(image1->width!=image2->width||image1->height!=image2->height||image1->nChannels!=image2->nChannels)
	{
		printf("The two image size not match!\n");
		return 0;
	}
	for(int i=0;i<image1->height;i++)
	{
		for(int j=0;j<image1->width;j++)
		{
			temp=((double)(cvGet2D(image1,i,j).val[0])-(double)(cvGet2D(image2,i,j).val[0]));
			mse+=temp*temp;
		}
	}
	mse=mse/(image1->width*image1->height);
	printf("the MSE=%f\n",mse);
	psnr=10*log10(255*255/mse);
	return psnr;
}

//////////////////////////////////////////
// ZigZag Scan Matrix
//////////////////////////////////////////
CvMat *ZigZag_Matrix()
{
	CvMat *zz=cvCreateMat(N*N,2,CV_32FC1);
	int u=-1,v=1,inc=1;
	for(int i=0;i<N*N;i++)
	{
		v-=inc;u+=inc;
		if(u>N-1)
		{
			v=v+2;u=N-1;inc=-1;
		}
		else if(v==-1)
		{
			v=0;inc=-1;
		}
		else if(v>N-1)
		{
			u+=2;v=N-1;inc=1;
		}
		else if(u==-1)
		{
			u=0;inc=1;
		}
		cvmSet(zz,i,0,u);
		cvmSet(zz,i,1,v);
	}
	return zz;
}
//////////////////////////////////////////
// ZigZag Scan a Matrix into a Line order
// input: matrix
// Ouptput: line order array
//////////////////////////////////////////
double * ZigZag(CvMat *data)
{
	double *result=(double *)malloc(data->width*data->height*sizeof(double));
	CvMat *ZZ=cvCreateMat(N*N,2,CV_32FC1);
	int i=0;
	ZZ=ZigZag_Matrix();
	for(i=0;i<N*N;i++)
	{
		int k=(int)cvmGet(ZZ,i,0);
		int m=(int)cvmGet(ZZ,i,1);
		result[i]=cvmGet(data,k,m);
	}
	cvReleaseMat(&ZZ);
	return result;
}

///////////////////////////////////////////////
// Inverse ZigZag Scan of Matrix from an array
//////////////////////////////////////////////
CvMat* inv_ZigZag(double line[N*N])
{
	int i=0;
	CvMat *ZZ=cvCreateMat(N*N,2,CV_32FC1);
	ZZ=ZigZag_Matrix();
	CvMat *data=cvCreateMat(N,N,CV_32FC1);
	for(i=0;i<N*N;i++)
	{
		int k=(int)cvmGet(ZZ,i,0);
		int m=(int)cvmGet(ZZ,i,1);
		cvmSet(data,k,m,line[i]);
	}
	cvReleaseMat(&ZZ);
	return data;
}
//////////////////////////////////////////
// Repetition the image into another size (m,n)
//////////////////////////////////////////
CvMat *TileImage(IplImage *image, int m,int n)
{
	int i=0,j=0,k=0,l=0;
	int height=image->height;
	int width=image->width;
	int width_count=(int)floor((double)n/(double)width);
	int height_count=(int)floor((double)m/(double)height);
	CvMat *img_mat= cvCreateMat(height, width, CV_32FC1);//������ʽ��ͼ��洢
	cvConvert(image,img_mat);
	CvMat *result=cvCreateMat(m, n, CV_32FC1);
	if(height>=m)
	{
		if(width>=n)
		{
			for(i=0;i<m;i++)
				for(j=1;j<n;j++)
					cvmSet(result,i,j,cvmGet(img_mat,i,j));
		}
		else
		{
			for(k=0;k<width_count;k++)
				for(i=0;i<m;i++)
					for(j=0;j<width;j++)
						cvmSet(result,i,k*width+j,cvmGet(img_mat,i,j));
			for(i=0;i<m;i++)
				for(j=0;j<mod(n,width);j++)
					cvmSet(result,i,k*width+j,cvmGet(img_mat,i,j));
		}
	}
	else
	{
		if(width>=n)
		{
			for(k=0;k<height_count;k++)
				for(i=0;i<height;i++)
					for(j=0;j<n;j++)
						cvmSet(result,k*height+i,j,cvmGet(img_mat,i,j));
			for(i=0;i<mod(m,height);i++)
				for(j=0;j<n;j++)
					cvmSet(result,k*height+i,j,cvmGet(img_mat,i,j));
		}
		else
		{
			for(k=0;k<height_count;k++)
				for(l=0;l<width_count;l++)
					for(i=0;i<height;i++)
						for(j=0;j<width;j++)
							cvmSet(result,k*height+i,l*width+j,cvmGet(img_mat,i,j));
			for(k=0;k<height_count;k++)
				for(i=0;i<height;i++)
					for(j=0;j<mod(n,width);j++)
						cvmSet(result,k*height+i,l*width+j,cvmGet(img_mat,i,j));
			for(l=0;l<width_count;l++)
				for(i=0;i<mod(m,height);i++)
					for(j=0;j<width;j++)
						cvmSet(result,k*height+i,l*width+j,cvmGet(img_mat,i,j));
			for(i=0;i<mod(m,height);i++)
				for(j=0;j<mod(n,width);j++)
					cvmSet(result,k*height+i,l*width+j,cvmGet(img_mat,i,j));
		}
	}
	cvReleaseMat(&img_mat);
	return result;
}

double *Rand(int m,int key) //�������ɳ���Ϊm�ģ���ԿΪkey�����������
{
	int i;	
  srand(key);  
  double *result=(double *)malloc(m*sizeof(double));
  for (i=0; i<m; i++) 
  {  
	  result[i]=(double)(rand()%1000)/999.0;//RAND_MAX;
	}  
  return result;
}

//*************************************************************************
// ����ϵ��֮���ϵ���Ƶ�ˮӡǶ��
//*************************************************************************
IplImage * embedding(IplImage *img, IplImage *logo,double strength)//DCT_relationship based method to Embed the watermark
{
	int i,j,k,l;
	int width, height,width_count,height_count;
	width=img->width;
	height=img->height;
	width_count=(int)floor(width/8.0);
	height_count=(int)floor(height/8.0);
	CvMat *watermarked=0;

	CvMat *blockDCT=Block_Dct(img,8,8);
	CvMat *tiledLogo=TileImage(logo,height_count,width_count);
	int bit=0;
	CvMat *temp=cvCreateMat(8,8,CV_32FC1);
	
	for(i=0;i<height_count;i++)
		for(j=0;j<width_count;j++)
		{
			
//			printf("block no.(%d,%d)\n",i,j);
			CvRect Rect=cvRect(8*j,8*i,8,8);
			cvGetSubArr(blockDCT,temp,Rect);//get submat
			bit=(int)cvmGet(tiledLogo,i,j);
#if DEBUG
			printf("bit=%d \n",bit);
#endif

			double center=0,left=0,below=0,t=0;
			for(k=6;k>=0;k--)
			{
				l=7-k;
				center=cvmGet(temp,k,l);
				left=cvmGet(temp,k,l-1);
				below=cvmGet(temp,k+1,l);
				if(bit)
				{
					t=max(left,below);
					if(center<t)
					{
						center=t+strength;
					}
					else
						if(fabs(center-t)<strength)center=t+strength;
				}
				else
				{
					t=min(left,below);
					if(center>t)
						center=t-strength;
					else
						if(fabs(center-t)<strength)
							center=t-strength;
				}
				cvmSet(blockDCT,i*8+k,j*8+l,center);
			}
			for(k=2;k>0;k--)
			{
				l=4-k;
				center=cvmGet(temp,k,l);
				left=cvmGet(temp,k,l-1);
				below=cvmGet(temp,k+1,l);
				if(bit)
				{
					t=max(left,below);
					if(center<t)
					{
						center=t+strength;
					}
					else
						if(fabs(center-t)<strength)center=t+strength;
				}
				else
				{
					t=min(left,below);
					if(center>t)
						center=t-strength;
					else
						if(fabs(center-t)<strength)
							center=t-strength;
				}
				cvmSet(blockDCT,i*8+k,j*8+l,center);
			}

		}

	watermarked=InverseBlockDct(blockDCT,8,8);
	IplImage * watermarkedImg=cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
	cvConvert(watermarked,watermarkedImg);

	cvReleaseMat(&watermarked);
	cvReleaseMat(&blockDCT);
	cvReleaseMat(&tiledLogo);
	cvReleaseMat(&temp);
	return watermarkedImg;
}

//*************************************************************************
// ����ϵ��֮���ϵ���Ƶ�ˮӡ���
//*************************************************************************
CvMat *extract(IplImage *img,int key)////DCT_Cox based method to extract the watermark
{
	int i,j,k;
	int width, height,width_count,height_count;
	width=img->width;
	height=img->height;
	width_count=(int)floor(width/8.0);
	height_count=(int)floor(height/8.0);
	CvMat *blockDCT=Block_Dct(img,8,8);
	CvMat *tiledLogo=cvCreateMat(height_count,width_count,CV_32FC1);
	int bit=0;
	double s=0;
	CvMat *temp=cvCreateMat(8,8,CV_32FC1);

	for(i=0;i<height_count;i++)
		for(j=0;j<width_count;j++)
		{
			
//			printf("block no.(%d,%d)\n",i,j);
			CvRect Rect=cvRect(8*j,8*i,8,8);
			cvGetSubArr(blockDCT,temp,Rect);//get submat

			double center=0,left=0,below=0,t_max=0,t_min=0;
			double result[9];
			int l=0;
			for(k=6;k>=0;k--)
			{
				l=7-k;
				center=cvmGet(temp,k,l);
				left=cvmGet(temp,k,l-1);
				below=cvmGet(temp,k+1,l);
				result[l-1]=0;
				if(center>(left+below)/2.0)
				{
					result[l-1]=1;
				}
//				printf("center=%f,left=%f,below=%f, result[l-1]=%f ",center,left,below,result[l-1]);
			}
			for(k=2;k>0;k--)
			{
				l=4-k;
				center=cvmGet(temp,k,l);
				left=cvmGet(temp,k,l-1);
				below=cvmGet(temp,k+1,l);
				result[7+l-2]=0;
				if(center>(left+below)/2.0)
				{
					result[7+l-2]=1;
				}
//				printf("center=%f,left=%f,below=%f, result[l-1]=%f ",center,left,below,result[l-1]);
			}
			s=0;
			for(k=0;k<11;k++)s+=result[k];
			if(s>5)bit=255;
			else bit=0;
			printf("bit=%d\n",bit);
//			cvWaitKey(0);
			cvmSet(tiledLogo,i,j,bit);
		}
	cvReleaseMat(&blockDCT);
	cvReleaseMat(&temp);
	return tiledLogo;
}
//********************************************************
//������������ȷ��
//********************************************************
double correlation(CvMat *dst, CvMat *org)
{
	int i,j,width,height;
	width=dst->width;
	height=dst->height;
	if(width!=org->width||height!=org->height)
	{
		printf("The correlation can not be computed since the size doesnt match\n");
		return 0;
	}

	double sum=0,temp_dst,temp_org;
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
		{
			temp_dst=cvmGet(dst,i,j);
			temp_org=cvmGet(org,i,j);
			if(fabs(temp_dst-temp_org)<1)
				sum+=1.0;
		}
	return (sum/(double)(width*height));
}