/*
 * Hough per Cerchi
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

vector<Vec3f> houghCerchi(Mat src,int R1,int R2,int treshold)
{
	int raggio = (R2-R1)+1;
	
	int X_c = src.rows;
	int Y_c = src.cols;
	int size[3] = {raggio,X_c,Y_c};
	
	Mat accumulatore = Mat(3,size,CV_64F);
	
	int x,y,z;
	for (x = 0; x < X_c; x++)
	{
		for (y = 0; y < Y_c; y++)
		{
			if (src.at<uchar>(x,y) > 250)
			{
				for(int r = R1; r <= R2; r++)
				{
					for(int t = 0; t < 360; t++)
					{
						int A = abs(x - r*(cos(t*M_PI/180)));
						int B = abs(y - r*(sin(t*M_PI/180)));
					
						accumulatore.at<float>(r-R1,A,B)++;
					}
				}
			}
		}
	}

	vector<Vec3f> cerchi;
		for (x = 0; x < size[0]; x++)
		{
			for (y = 0; y < size[1]; y++)
			{
				for (z = 0; z < size[2]; z++)
				{
					if(accumulatore.at<float>(x,y,z) > treshold)
					{
						Vec3f c(x+R1,y,z);
						cerchi.push_back(c);
					}
				}
			}
		}
return cerchi;
}

int main(int argc,char**argv)
{
	if(argc != 5)
	{
		printf("Sintassi non corretta! ./houghcerchi [imgPath] [RaggioMIN] [RaggioMAX] [Treshold]");
		return -1;
	}
	Mat src = imread(argv[1],0);
	Mat src1 = imread(argv[1],1);

	int R1 = atoi(argv[2]); //25
	int R2 = atoi(argv[3]);	//50
	int tresh = atoi(argv[4]); //150 o 200
	namedWindow("Source",WINDOW_AUTOSIZE);
	imshow("Source",src);
	waitKey(0);

	Mat blr,cny;
	//Applico il filtro di blurring
	blur(src,blr,Size(7,7));
	//Applico il Canny
	Canny(blr,cny,50,150,3);
	
	vector<Vec3f> cerchio = houghCerchi(cny,R1,R2,tresh);

	//Disegno i cerchi
	for (int x = 0; x < cerchio.size(); x++)
	{
		Point center(cvRound(cerchio[x][2]),cvRound(cerchio[x][1]));
		int rad = cvRound(cerchio[x][0]);
		
		circle(src1,center,rad,Scalar(0,255,0),1,8,0);
	}

	namedWindow("HoughCerchi",WINDOW_AUTOSIZE);
	imshow("HoughCerchi",src1);
	waitKey(0);

return 0;
}
