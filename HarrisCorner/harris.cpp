/*
 * Harris Corner Detection
*/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat L_X,L_Y;
Mat src1;
/*
 * Funzione per trasformare una immagine "src" in b/n
*/
Mat blackwhite(Mat src)
{
	Mat output = src.clone();
	int x,y;
	
	for (x=0; x < src.rows; x++)
	{
		for (y=0; y < src.cols; y++)
		{
			if (src.at<uchar>(x,y)<128)
				output.at<uchar>(x,y) = 0;
			else 
				output.at<uchar>(x,y) = 255;
		}
	}

return output;
}

/*
 * Filtro Roberts con formula
*/
void roberts(Mat src)
{
	L_X = Mat(src.rows, src.cols, DataType<float>::type);
	L_Y = L_X.clone();
	int x,y;

	for (x=1; x < src.rows-1; x++)
	{
		for (y=1;y<src.cols-1; y++)
		{
			L_X.at<float>(x,y) = abs(src.at<uchar>(x+1,y+1) - src.at<uchar>(x,y));
			L_Y.at<float>(x,y) = abs(src.at<uchar>(x+1,y) - src.at<uchar>(x,y+1));
		}
	}
}

/*
 * Funzione Harris
*/
void harris(Mat src,int treshold)
{
	float det,trace;
	int x,y;
	float C00,C11,C10;

	for (x = 3; x < src.rows-3; x++)
	{
		for (y = 3; y<src.cols-3; y++)
		{
			C00=C11=C10=0;
		
			for(int i=-3; i<=3; i++)
			{
				for(int j=-3; j<=3; j++)
				{
					C00 += pow(L_X.at<float>(x+i,y+j),2);
					C11 += pow(L_Y.at<float>(x+i,y+j),2);
					C10 += L_X.at<float>(x+i,y+j) * L_Y.at<float>(x+i,y+j);
				}
			}
			det = (C00 * C11) - (C10 * C10);
			trace = C00 + C11;
			double k = 0.04; //Fattore K
			double R = det - k * (pow(trace,2));
			
			if (R > treshold * 1e+11) //Moltiplico per 1e+11 perche' ho bisogno di un treshold molto alto
			{
				circle(src1, Point(y,x), 5, Scalar(255,255,0), 2, 8, 0);
			}
		}
	}
}

int main(int argc, char **argv){
	if(argc != 3)
	{
		printf("Sintassi non corretta! ./harris [imgPath] [Treshold]");
		return -1;
	}

	Mat src = imread(argv[1],0);
	src1 = imread(argv[1],1);
	int tresh = atoi(argv[2]); //5
	
	namedWindow("Source",WINDOW_AUTOSIZE);
	imshow("Source",src);
	waitKey(0);

	Mat bw = blackwhite(src);
	roberts(bw);
	harris(bw,tresh);
	
	namedWindow("Harris",WINDOW_AUTOSIZE);
	imshow("Harris",src1);
	waitKey(0);
	
return 0;
}
