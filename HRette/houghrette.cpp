/*
 * Hough per Rette
*/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<Vec2d> houghRette(Mat src,int treshold)
{
	int x,y;
	double R;

	if (src.rows > src.cols) 
		R = src.rows;
	else
		R= src.cols;
	
	double Rho = R*sqrt(2.0);
	double Theta = 180;
	
	Mat accumulatore = Mat::zeros(Rho,Theta,CV_8U);
	
	for (x = 0; x < src.rows; x++)
	{
		for (y = 0; y < src.cols; y++)
		{
			if (src.at<uchar>(x,y) > 250)
			{
				for (int t = 0; t < Theta; t++)
				{
					double R = abs(x*sin(t*(M_PI/180)) + y*cos(t*(M_PI/180)));
					accumulatore.at<uchar>(round(R),t)++;
				}
			}
		}
	}
	
	vector<Vec2d> lines;
	for (x = 0; x < src.rows; x++)
	{
		for (y=0;y<src.cols;y++)
		{
			if (accumulatore.at<uchar>(x,y)>treshold)
			{
				Vec2d l(x,y*(M_PI/180));
				lines.push_back(l);
			}
		}
	}
return lines;
}

void stampa(vector<Vec2d> lines, Mat dst)
{
	int x;
	
		for(x = 0; x < lines.size(); x++)
		{
			double Rho = lines[x][0];
			double Theta = lines[x][1];
			
			double A = cos(Theta);
			double B = sin(Theta);
			
			double x0 = A*Rho;
			double y0 = B*Rho;
			
			Point p1,p2;
			
			p1.x = round(x0 + 1000 * -B);
			p1.y = round(y0 + 1000 * A);
			p2.x = round(x0 - 1000 * -B);
			p2.y = round(y0 - 1000 * A);
			
			line(dst,p1,p2,Scalar(0,255,0),1,8);
		}

}

int main(int argc,char** argv)
{
	if(argc != 3)
	{
		printf("Sintassi non corretta! ./houghrette [imgPath] [Treshold]");
		return -1;
	}
	Mat src = imread(argv[1],1);
	int tresh = atoi(argv[2]); //150

	//Applico un filtro di blurring
	Mat bl;
	blur(src,bl,Size(7,7));

	//Applico Canny
	Mat can;
	Canny(bl,can,50,150,3);
	
	vector <Vec2d> lines = houghRette(can,tresh);
	namedWindow("Source",WINDOW_AUTOSIZE);
	imshow("Source",src);
	
	stampa(lines,src);
	
	namedWindow("Risultato",WINDOW_AUTOSIZE);
	imshow("Risultato",src);
	waitKey(0);

return 0;
}
