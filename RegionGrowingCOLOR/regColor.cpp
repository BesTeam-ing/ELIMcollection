/*
 * Region Growing - Colori
*/
#include <iostream>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;

Vec3b seed;
Vec3b colore[3] = {{255,0,0},{255,0,255},{255,255,0}};
int coord_x[3] = {12,22,91}; //Le coordinate da cui leggere il colore
int coord_y[3] = {74,11,12};
Vec3b black = {0,0,0};

bool **visited;
int treshold = 50;
Mat src, regions;
int cord_x,cord_y;

/*
 * Calcolo della distanza RGB
*/
int distanza(Vec3b x, Vec3b y)
{
	int dist;
	
	double distR = x.val[0] - y.val[0];
	double distG = x.val[1] - y.val[1];
	double distB = x.val[2] - y.val[2];
	
	dist = sqrt(pow(distR,2)+pow(distB,2) + pow(distG,2));

return dist;
}

void RegionGrowing(int row, int col,int c)
{
	
	if (row > 0)
	{
		if (!visited[row-1][col])
		{ //controllo pixel != 0
			if (distanza(src.at<Vec3b>(row-1,col),seed) < treshold)
				regions.at<Vec3b>(row-1,col) = colore[c];
			//else regions.at<Vec3b>(row-1,col) = black;
			
			visited[row-1][col] = true;
			RegionGrowing(row-1,col,c);
		}
	}

	if (col > 0)
	{
		if (!visited[row][col-1])
		{ //controllo pixel != 0
			if (distanza(src.at<Vec3b>(row,col-1),seed) < treshold)
				regions.at<Vec3b>(row,col-1) = colore[c];
			//else regions.at<Vec3b>(row,col-1) = black;
			
			visited[row][col-1] = true;
			RegionGrowing(row,col-1,c);
		}
	}

	if (row < src.rows-1)
	{
		if (!visited[row+1][col])
		{ //controllo pixel != 0
			if (distanza(src.at<Vec3b>(row+1,col),seed) < treshold)
				regions.at<Vec3b>(row+1,col) = colore[c];
			//else regions.at<Vec3b>(row+1,col)= black;
			
			visited[row+1][col] = true;
			RegionGrowing(row+1,col,c);
		}
	}

	if (col < src.cols-1)
	{
		if (!visited[row][col+1])
		{ //controllo pixel != 0
			if (distanza(src.at<Vec3b>(row,col+1),seed) < treshold)
				regions.at<Vec3b>(row,col+1) = colore[c];
			//else regions.at<Vec3b>(row,col+1) = black;
			
			visited[row][col+1] = true;
			RegionGrowing(row,col+1,c);
		}
	}
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Sintassi non corretta! ./regColor [imgPath]");
		return -1;
	}
	
	src = imread(argv[1]);
	//resize(src,src,Size(),0.5,0.5);
	namedWindow("Original", WINDOW_AUTOSIZE);
	srand((unsigned)time(NULL));
	imshow("Original",src);
	waitKey(0);

	regions = src.clone();
	visited = new bool*[src.rows];
	for (int i = 0; i < src.rows; i++)
		visited[i] = new bool[src.cols];

	for (int x = 0; x < src.rows; x++)
		for(int y = 0; y < src.cols; y++)
		{
			regions.at<Vec3b>(x,y) = black; //Coloro tutto di nero
			visited[x][y] = false;
			}
			
	//cord_x = rand()%src.rows+1;
	//cord_y = rand()%src.cols+1;

	for (int i = 0; i < 3; i++) //Scorro i 3 punti che avevo scelto
	{
		//Pulisco la matrice dei visitati
		for (int x = 0; x < src.rows; x++)
			for (int y = 0; y < src.cols; y++)
				visited[x][y] = false;	

		seed = src.at<Vec3b>(coord_x[i],coord_y[i]);
		regions.at<Vec3b>(coord_x[i], coord_y[i]) = seed;
		RegionGrowing(coord_x[i], coord_y[i],i);
		circle(regions,Point(coord_y[i],coord_x[i]),3,Scalar(0,0,255),2,8);
	}
	
	namedWindow("Region Growing", WINDOW_AUTOSIZE);
	imshow("Region Growing",regions);
	waitKey(0);

return 0;
}
