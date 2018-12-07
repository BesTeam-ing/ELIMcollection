/*
 * Region Growing - Bianco/Nero
*/
#include <iostream>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;

int seed[2];
bool **visited;
int treshold = 100;
Mat src, regions;
int cord_x,cord_y;

void mouse_call(int event, int x,int y,int flag,void *param)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		cout<<"X= "<<x<<endl<<"Y= "<<y<<endl<<endl;
		cord_x = x;
		cord_y = y;
		
	}
}

void RegionGrowing(int row, int col)
{
	if(row > 0)
	{
		if (!visited[row-1][col])
		{ //controllo pixel != 0
			if (abs(src.at<uchar>(row-1,col) - seed[0]) < treshold)
				regions.at<uchar>(row-1,col) = seed[0];
			else 
				regions.at<uchar>(row-1,col) = seed[1];
			
			visited[row-1][col] = true;
			RegionGrowing(row-1,col);
		}
	}

	if (col > 0)
	{
		if (!visited[row][col-1]
		{ //controllo pixel != 0
			if (abs(src.at<uchar>(row,col-1) - seed[0]) < treshold)
				regions.at<uchar>(row,col-1) = seed[0];
			else 
				regions.at<uchar>(row,col-1) = seed[1];

			visited[row][col-1] = true;
			RegionGrowing(row,col-1);
		}
	}

	if(row<src.rows-1)
	{
		if (!visited[row+1][col])
		{ //controllo pixel != 0
			if(abs(src.at<uchar>(row+1,col) - seed[0]) < treshold)
				regions.at<uchar>(row+1,col) = seed[0];
			else 
				regions.at<uchar>(row+1,col) = seed[1];
			
			visited[row+1][col] = true;
			RegionGrowing(row+1,col);
		}
	}

	if(col<src.cols-1)
	{
		if (!visited[row][col+1])
		{ //controllo pixel != 0
			if(abs(src.at<uchar>(row,col+1) - seed[0]) < treshold)
				regions.at<uchar>(row,col+1) = seed[0];			
			else 
				regions.at<uchar>(row,col+1) = seed[1];
			
			visited[row][col+1] = true;
			RegionGrowing(row,col+1);
		}
	}
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Sintassi non corretta! ./region [imgPath]");
		return -1;
	}
	
	src = imread(argv[1],0);
	resize(src,src,Size(),0.5,0.5);
	namedWindow("Original", WINDOW_AUTOSIZE);
	//setMouseCallback("Original",mouse_call); //Cliccare su un punto dell'immagine e premere invio
	imshow("Original",src);
	waitKey(0);

	regions = src.clone();
	visited = new bool*[src.rows];
	for (int i = 0; i < src.rows; i++)
		visited[i] = new bool[src.cols];
//Inizializzo la matrice visitati
	for(int x=0; x<src.rows; x++)
		for(int y=0; y<src.cols; y++)
			visited[x][y] = false;
			
	seed[0] = 40; //Da commentare se abilitato setMouseCallback
	cout<<seed[0]<<endl;
	seed[1] = 0; //Da commentare se abilitato setMouseCallback
	
	if (seed[0] <= treshold) {
		seed[1] = seed[0];
		seed[0] = 0;
 	}

	RegionGrowing(cord_x, cord_y);
	namedWindow("Region Growing", WINDOW_AUTOSIZE);
	imshow("Region Growing",regions);
	waitKey(0);

return 0;
}
