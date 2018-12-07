/*
 * Filtri Passa-Basso
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <algorithm>

using namespace std;
using namespace cv;

/*
 * Funzione per aggiungere rumore Sale e Pepe
*/
Mat addSalt(Mat src)
{
	Mat output = src.clone();
	int x,y;
	
	for(x = 0; x < (src.rows*src.cols)/100; x++){
		int rand_x = rand()%src.rows;
		int rand_y = rand()%src.cols;
		int rand_k = rand()%100;
		if (rand_k%2 == 0)
			output.at<uchar>(rand_x,rand_y) = 0;
		else 
			output.at<uchar>(rand_x,rand_y) = 255;
	}
return output;
}

/*
 * Filtro Mediana
*/
Mat mediana(Mat src)
{
	Mat output = src.clone();
	int x,y;
	int mask[9] = {0,0,0,0,0,0,0,0,0};
	
	for(x=1;x<src.rows-1;x++){
		for(y=1;y<src.cols-1;y++){
		
		mask[0] = src.at<uchar>(x-1,y-1);
		mask[1] = src.at<uchar>(x-1,y);
		mask[2] = src.at<uchar>(x-1,y+1);
		mask[3] = src.at<uchar>(x,y-1);
		mask[4] = src.at<uchar>(x,y);
		mask[5] = src.at<uchar>(x,y+1);
		mask[6] = src.at<uchar>(x+1,y-1);
		mask[7] = src.at<uchar>(x+1,y);
		mask[8] = src.at<uchar>(x+1,y+1);
	
		sort(begin(mask),end(mask));
		output.at<uchar>(x,y) = mask[4]; // Elemento mediano
		}
	}
return output;	
}

/*
 * Filtro Media
*/
Mat media_calc(Mat src)
{
	Mat output = src.clone();
	int x,y;
	int mask[9] = {0,0,0,0,0,0,0,0,0};

	for(x=1;x<src.rows-1;x++){
		for(y=1;y<src.cols-1;y++){
		
		mask[0] = src.at<uchar>(x-1,y-1);
		mask[1] = src.at<uchar>(x-1,y);
		mask[2] = src.at<uchar>(x-1,y+1);
		mask[3] = src.at<uchar>(x,y-1);
		mask[4] = src.at<uchar>(x,y);
		mask[5] = src.at<uchar>(x,y+1);
		mask[6] = src.at<uchar>(x+1,y-1);
		mask[7] = src.at<uchar>(x+1,y);
		mask[8] = src.at<uchar>(x+1,y+1);
		
		int avg=0;
		for(int i=0; i<9;i++)
			avg+=mask[i];

		output.at<uchar>(x,y) = avg/9;
		}
	}
	
return output;
}

int main(int argc,char**argv)
{
	if(argc != 2)
	{
		printf("Sintassi non corretta! ./basso [imgPath]");
		return -1;
	} 
	
	Mat src = imread(argv[1],0);
	srand(time(NULL));
	namedWindow("Source",WINDOW_AUTOSIZE);
	imshow("Source",src);
	waitKey(0);
	
	Mat sale = addSalt(src);
	namedWindow("SalePepe",WINDOW_AUTOSIZE);
	imshow("SalePepe",sale);
	waitKey(0);
	
	Mat median = mediana(sale);
	namedWindow("Mediana",WINDOW_AUTOSIZE);
	imshow("Mediana",median);
	waitKey(0);
	
	Mat media = media_calc(sale);
	namedWindow("Media",WINDOW_AUTOSIZE);
	imshow("Media",media);
	waitKey(0);
	
return 0;
}
