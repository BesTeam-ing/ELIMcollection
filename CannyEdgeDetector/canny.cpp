/*
 * Filtri Passa-Alto
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

float **dirGrad;

/*
 * Filtro Gaussiano
*/
Mat gauss(Mat src)
{
	Mat output = src.clone();
	int x,y;
	int mask[9] = {1,2,1,
				2,8,2,
				1,2,1};
	int arr[9];
	int denominatore =0;

	for (int i=0; i<9; i++)
		denominatore += mask[i];

	for(x=1; x<src.rows-1; x++)
	{
		for(y=1; y<src.cols-1; y++)
		{
		arr[0] = src.at<uchar>(x-1,y-1) * mask[0];
		arr[1] = src.at<uchar>(x-1,y) * mask[1];
		arr[2] = src.at<uchar>(x-1,y+1) * mask[2];
		arr[3] = src.at<uchar>(x,y-1) * mask[3];
		arr[4] = src.at<uchar>(x,y) * mask[4];
		arr[5] = src.at<uchar>(x,y+1) * mask[5];
		arr[6] = src.at<uchar>(x+1,y-1) * mask[6];
		arr[7] = src.at<uchar>(x+1,y) * mask[7];
		arr[8] = src.at<uchar>(x+1,y+1) * mask[8];
		
		int avg =0;
		for (int i=0; i<9; i++) 
			avg += arr[i];
		avg /= denominatore;

		output.at<uchar>(x,y) = avg;
		}
	}

	
return output;
}

/*
 * Filtro Roberts
*/
Mat robert(Mat src)
{
	Mat output = src.clone();
	int g_x[] = {0,0,0,
				0,-1,0,
				0,0,1};

	int g_y[] = {0,0,0,
				0,0,-1,
				0,1,0};
	int x,y;
	for(x=1; x<src.rows-1; x++)
	{
		for(y=1; y<src.cols-1; y++)
		{
		int grad_x = 0;
		int grad_y=0;
		int cont = 0;

		for(int i=-1; i<=1; i++)
		{
			for(int j=-1; j<=1; j++)
			{

			grad_x += src.at<uchar>(x+i,y+j) * g_x[cont];
			grad_y += src.at<uchar>(x+i,y+j) * g_y[cont];
			cont++;

			}
		}
		dirGrad[x][y] = atan2(grad_y,grad_x);
		output.at<uchar>(x,y) = abs(grad_x) + abs(grad_y);
			}
	}
return output;
}

/*
 * Soppressione dei non massimi
*/
Mat soppressione(Mat src)
{
	Mat out = src.clone();
	int x,y;
	for(x=1; x<src.rows-1; x++)
	{
		for(y=1; y<src.cols-1; y++)
		{
		if (dirGrad[x][y]>=0 && dirGrad[x][y]<=(3/4)*M_PI)
		{
			if (src.at<uchar>(x,y) < src.at<uchar>(x,y-1) ||
			 src.at<uchar>(x,y) < src.at<uchar>(x,y+1) ||
			 src.at<uchar>(x,y) < src.at<uchar>(x+1,y) || 
			 src.at<uchar>(x,y) < src.at<uchar>(x-1,y) ) 
				out.at<uchar>(x,y) = 0;
			}
		}
	}
	
return out;
}

/*
 * Sogliatura per isteresi
*/
Mat sogliatura(Mat src,int min, int max)
{
	Mat out = src.clone();
	int x,y;
	for (x=0; x<src.rows; x++)
	{
		for (y=0; y<src.cols; y++)
		{
		if (src.at<uchar>(x,y)<min) 
			out.at<uchar>(x,y) = 0;

		else if (src.at<uchar>(x,y)>max) 
			out.at<uchar>(x,y) = 255;

		else {
			if (src.at<uchar>(x+1,y)>max ||
			 src.at<uchar>(x-1,y) > max ||
			 src.at<uchar>(x,y+1) > max ||
			 src.at<uchar>(x,y-1) > max)

				out.at<uchar>(x,y) = 255;

			else out.at<uchar>(x,y) = 0;
				}
		}
	}
return out;
}

int main(int argc, char** argv)
{
	if(argc != 4)
	{
		printf("Sintassi non corretta! ./canny [imgPath] [T1] [T2]");
		return -1;
	}

	Mat src = imread(argv[1],0);
	int T1 = atoi(argv[2]); //25
	int T2 = atoi(argv[3]);	//50
	imshow("Source",src);
	waitKey(0);

	Mat gau = gauss(src);
	dirGrad = new float * [src.rows];
	for (int i=0; i<src.rows; i++)
		dirGrad[i] = new float[src.cols];

	Mat rob = robert(gau);
	Mat mass = soppressione(rob);
	Mat canny = sogliatura(mass,T1,T2);
	imshow("Canny",canny);
	waitKey(0);

return 0;
}
