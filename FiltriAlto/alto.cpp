/*
 * Filtri Passa-Alto
*/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*
 * Filtro Roberts
 * Utilizzando le maschere
*/
Mat roberts(Mat src)
{
	Mat output = src.clone();
	int x,y;
	int x1[9],y1[9];

	int robert_x[9] = {0,0,0,
			    	0,-1,0,
			    	0,0,1};

	int robert_y[9] = {0,0,0,
			    	0,0,-1,
			    	0,1,0};

	for(x=1;x<src.rows-1;x++)
	{
		for(y=1;y<src.cols-1;y++)
		{
		
		x1[0] = src.at<uchar>(x-1,y-1) * robert_x[0];
		x1[1] = src.at<uchar>(x-1,y) * robert_x[1];
		x1[2] = src.at<uchar>(x-1,y+1) * robert_x[2];
		x1[3] = src.at<uchar>(x,y-1) * robert_x[3];
		x1[4] = src.at<uchar>(x,y) * robert_x[4];
		x1[5] = src.at<uchar>(x,y+1) * robert_x[5];
		x1[6] = src.at<uchar>(x+1,y-1) * robert_x[6];
		x1[7] = src.at<uchar>(x+1,y) * robert_x[7];
		x1[8] = src.at<uchar>(x+1,y+1) * robert_x[8];

		int sum1=0;
		for(int i=0; i<9;i++) 
			sum1+=x1[i];
		
		y1[0] = src.at<uchar>(x-1,y-1) * robert_y[0];
		y1[1] = src.at<uchar>(x-1,y) * robert_y[1];
		y1[2] = src.at<uchar>(x-1,y+1) * robert_y[2];
		y1[3] = src.at<uchar>(x,y-1) * robert_y[3];
		y1[4] = src.at<uchar>(x,y) * robert_y[4];
		y1[5] = src.at<uchar>(x,y+1) * robert_y[5];
		y1[6] = src.at<uchar>(x+1,y-1) * robert_y[6];
		y1[7] = src.at<uchar>(x+1,y) * robert_y[7];
		y1[8] = src.at<uchar>(x+1,y+1) * robert_y[8];

		int sum2=0;
		for(int i=0; i<9;i++) 
			sum2+=y1[i];
		
		output.at<uchar>(x,y) = saturate_cast<uchar>(abs(sum1) + abs(sum2));
		}
	}
	
return output;
}

/*
 * Filtro Sobel
 * Utilizzando le maschere
*/
Mat sobel(Mat src)
{
	Mat output = src.clone();
	int x,y;
	int x1[9],y1[9];

	int sobel_x[9] = {-1,-2,-1,
			    	0,0,0,
			    	1,2,1};

	int sobel_y[9] = {-1,0,1,
			    	-2,0,2,
			    	-1,0,1};

	for(x=1;x<src.rows-1;x++)
	{
		for(y=1;y<src.cols-1;y++)
		{
		
		x1[0] = src.at<uchar>(x-1,y-1) * sobel_x[0];
		x1[1] = src.at<uchar>(x-1,y) * sobel_x[1];
		x1[2] = src.at<uchar>(x-1,y+1) * sobel_x[2];
		x1[3] = src.at<uchar>(x,y-1) * sobel_x[3];
		x1[4] = src.at<uchar>(x,y) * sobel_x[4];
		x1[5] = src.at<uchar>(x,y+1) * sobel_x[5];
		x1[6] = src.at<uchar>(x+1,y-1) * sobel_x[6];
		x1[7] = src.at<uchar>(x+1,y) * sobel_x[7];
		x1[8] = src.at<uchar>(x+1,y+1) * sobel_x[8];

		int sum1=0;
		for(int i=0; i<9;i++) 
			sum1+=x1[i];
		
		y1[0] = src.at<uchar>(x-1,y-1) * sobel_y[0];
		y1[1] = src.at<uchar>(x-1,y) * sobel_y[1];
		y1[2] = src.at<uchar>(x-1,y+1) * sobel_y[2];
		y1[3] = src.at<uchar>(x,y-1) * sobel_y[3];
		y1[4] = src.at<uchar>(x,y) * sobel_y[4];
		y1[5] = src.at<uchar>(x,y+1) * sobel_y[5];
		y1[6] = src.at<uchar>(x+1,y-1) * sobel_y[6];
		y1[7] = src.at<uchar>(x+1,y) * sobel_y[7];
		y1[8] = src.at<uchar>(x+1,y+1) * sobel_y[8];

		int sum2=0;
		for(int i=0; i<9;i++) 
			sum2+=y1[i];
		
		output.at<uchar>(x,y) = saturate_cast<uchar>(abs(sum1) + abs(sum2));
		}
	}
	
return output;
}

/*
 * Filtro Prewitt
 * Utilizzando le maschere
*/
Mat prewitt(Mat src)
{
	Mat output = src.clone();
	int x,y;
	int x1[9],y1[9];

	int sobel_x[9] = {-1,-1,-1,
			    	0,0,0,
			    	1,1,1};

	int sobel_y[9] = {-1,0,1,
			    	-1,0,1,
			    	-1,0,1};

	for(x=1;x<src.rows-1;x++)
	{
		for(y=1;y<src.cols-1;y++)
		{
		
		x1[0] = src.at<uchar>(x-1,y-1) * sobel_x[0];
		x1[1] = src.at<uchar>(x-1,y) * sobel_x[1];
		x1[2] = src.at<uchar>(x-1,y+1) * sobel_x[2];
		x1[3] = src.at<uchar>(x,y-1) * sobel_x[3];
		x1[4] = src.at<uchar>(x,y) * sobel_x[4];
		x1[5] = src.at<uchar>(x,y+1) * sobel_x[5];
		x1[6] = src.at<uchar>(x+1,y-1) * sobel_x[6];
		x1[7] = src.at<uchar>(x+1,y) * sobel_x[7];
		x1[8] = src.at<uchar>(x+1,y+1) * sobel_x[8];

		int sum1=0;
		for(int i=0; i<9;i++) 
			sum1+=x1[i];
		
		y1[0] = src.at<uchar>(x-1,y-1)*sobel_y[0];
		y1[1] = src.at<uchar>(x-1,y)*sobel_y[1];
		y1[2] = src.at<uchar>(x-1,y+1)*sobel_y[2];
		y1[3] = src.at<uchar>(x,y-1)*sobel_y[3];
		y1[4] = src.at<uchar>(x,y)*sobel_y[4];
		y1[5] = src.at<uchar>(x,y+1)*sobel_y[5];
		y1[6] = src.at<uchar>(x+1,y-1)*sobel_y[6];
		y1[7] = src.at<uchar>(x+1,y)*sobel_y[7];
		y1[8] = src.at<uchar>(x+1,y+1)*sobel_y[8];

		int sum2=0;
		for(int i=0; i<9;i++) 
			sum2+=y1[i];
		
		output.at<uchar>(x,y) = saturate_cast<uchar>(abs(sum1) + abs(sum2));
		}
	}
	
return output;
}
/*
 * Filtro Roberts
 * Utilizzando la formula
*/
Mat roberts_mio(Mat src)
{
	Mat output = src.clone();
	int x,y;
	
	for(x=1;x<src.rows-1;x++)
	{
		for(y=1;y<src.cols-1;y++)
		{
		int grad_x = src.at<uchar>(x,y) -src.at<uchar>(x+1,y+1);
		int grad_y = src.at<uchar>(x+1,y) - src.at<uchar>(x,y+1);
		output.at<uchar>(x,y) = abs(grad_x) + abs(grad_y);
		}
	}
	
return output;
}


int main(int argc,char**argv)
{
	if(argc != 2)
	{
		printf("Sintassi non corretta! ./alto [imgPath]");
		return -1;
	}
	
	Mat src = imread(argv[1],0);
	namedWindow("Source",WINDOW_AUTOSIZE);
	imshow("Source",src);
	
	Mat rob = roberts(src);
	namedWindow("Roberts_mask",WINDOW_AUTOSIZE);
	imshow("Roberts_mask",rob);
	waitKey(0);
	
	Mat rob2 = roberts_mio(src);
	namedWindow("Roberts_mio",WINDOW_AUTOSIZE);
	imshow("Roberts_mio",rob2);
	waitKey(0);
	
	Mat sob = sobel(src);
	namedWindow("Sobel",WINDOW_AUTOSIZE);
	imshow("Sobel",sob);
	waitKey(0);
	Mat pre = prewitt(src);
	namedWindow("Prewitt",WINDOW_AUTOSIZE);
	imshow("Prewitt",pre);
	waitKey(0);
return 0;
}
