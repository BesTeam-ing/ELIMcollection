/*
 * K-means
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <cmath>

using namespace std;
using namespace cv;

const int kGroup = 5;
const int conta = 100;
int* centersX;
int* centersY;

/*
 * Funzione kmeans
*/
bool kmeans(Mat src, Mat values, Mat output)
{
	int x,y;
	int idCentroide;
	Vec3b val;
	
	Mat medieInit(1, kGroup, CV_8UC3);
	
	float dist,min;
	
	for(int i = 0; i < kGroup; i++)
	{
		medieInit.at<Vec3b>(0,i) = values.at<Vec3b>(0,i);
	}
	
	for(x = 0; x < src.rows; x++)
	{
		for(y = 0; y < src.cols; y++)
		{
			min = sqrt(pow(output.at<Vec3b>(x,y).val[0] - values.at<Vec3b>(0,0).val[0],2) +
			 		pow(output.at<Vec3b>(x,y).val[1] - values.at<Vec3b>(0,0).val[1],2)+
			 		pow(output.at<Vec3b>(x,y).val[2] - values.at<Vec3b>(0,0).val[2],2)
			 		);

		for(int i = 1; i < kGroup; i++)
		{
			dist = sqrt(pow(output.at<Vec3b>(x,y).val[0] - values.at<Vec3b>(0,i).val[0],2) +
			 		pow(output.at<Vec3b>(x,y).val[1] - values.at<Vec3b>(0,i).val[1],2) +
			 		pow(output.at<Vec3b>(x,y).val[2] - values.at<Vec3b>(0,i).val[2],2)
			 		);
		
			if (dist < min) 
			{
				min = dist;
				idCentroide = i;
				val = values.at<Vec3b>(0,i);
			}
		}

		output.at<Vec3b>(x,y) = val;

		values.at<Vec3b>(0,idCentroide).val[0] = (int) ((values.at<Vec3b>(0,idCentroide).val[0] + val.val[0])/2.0);
		values.at<Vec3b>(0,idCentroide).val[1] = (int) ((values.at<Vec3b>(0,idCentroide).val[1] + val.val[1])/2.0); 
		values.at<Vec3b>(0,idCentroide).val[2] = (int) ((values.at<Vec3b>(0,idCentroide).val[2] + val.val[2])/2.0); 
		
		}
	}

	bool changed = false;
	
	for (int i=0; i<kGroup; i++)
	{
		if (values.at<Vec3b>(0,i) != medieInit.at<Vec3b>(0,i))
		{
			changed = true;
			medieInit.at<Vec3b>(0,i) = values.at<Vec3b>(0,i);
		}
	}

	if (!changed)
		return true;
	else 
		return false;
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Sintassi non corretta! ./kmeans [imgPath]");
		return -1;
	}

	Mat src = imread(argv[1],1);
	namedWindow("Source",WINDOW_AUTOSIZE);
	imshow("Source",src);
	waitKey(0);
	
	centersX = new int[kGroup];
	centersY = new int[kGroup];
	
	srand((unsigned)time(NULL));
	
	Mat values(1,kGroup,CV_8UC3);
	Mat output = src.clone();
	
	for(int i =0; i<kGroup; i++)
	{
		centersX[i] = rand()%src.rows;
		centersY[i] = rand()%src.cols;
		values.at<Vec3b>(0,i) = src.at<Vec3b>(centersX[i],centersY[i]);
	}
	int i = 1;
	bool end = false;
	
	while (!end && i < 100)
	{
		end = kmeans(src,values,output);
		i++;
	}
	
	namedWindow("K-Means",WINDOW_AUTOSIZE);
	imshow("K-Means",output);
	waitKey(0);

//Mostro i colori che sono stati scelti casualmente //Non necessario
	Mat showPixel(conta*kGroup,conta,CV_8UC3);
	for(int x = 0; x < conta; x++)
	{
		for(int y=0; y<showPixel.cols;y++)
		{
			showPixel.at<Vec3b>(x+(conta*1),y) = values.at<Vec3b>(0,0);
			showPixel.at<Vec3b>(x+(conta*2),y) = values.at<Vec3b>(0,1);
			showPixel.at<Vec3b>(x+(conta*3),y) = values.at<Vec3b>(0,2);
			showPixel.at<Vec3b>(x+(conta*4),y) = values.at<Vec3b>(0,3);
		}
	}
	namedWindow("Clusters",WINDOW_AUTOSIZE);
	imshow("Clusters",showPixel);
	waitKey(0);
	
return 0;
}