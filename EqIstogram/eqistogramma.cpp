/*
 * Equalizzazione Istogramma
*/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int occorrenze[256];

/*
 * Funzione per disegnare l'istogramma di "src"
*/
Mat istogramma(Mat src)
{
	Mat output(512,256,DataType<uchar>::type);
	int x,y;
	
	for (int i=0; i<256; i++) 
		occorrenze[i] = 0;
	
	for(x=0; x<src.rows; x++)
	{
		for(y=0; y<src.cols; y++)
		{
			occorrenze[src.at<uchar>(x,y)]++;
		}
	}
	
	for (x=0; x<output.rows; x++)
	{
		for(y=0; y<output.cols; y++)
		{
			output.at<uchar>(x,y) = 255; // Coloro di bianco
			}
		}

	int nOccorrenze;

	for (int i = 0; i<256; i++)
	{
		nOccorrenze = occorrenze[i]/50;
		for(int j=0; j<nOccorrenze; j++)
			output.at<uchar>((output.rows-1)-j,i) = 0;
		
	}
	return output;
	
}
/*
 * Funzione per equalizzare l'istogramma "ist"
*/
Mat equalizza(Mat ist,Mat src)
{
	Mat output = src.clone();
	int LUT[256][2];
	int x,y;
	int count;
	
	for (int i=0; i<ist.cols; i++)
	{
		count=0;
		for(int j=0; j<=i; j++) 
			count += occorrenze[j];
		LUT[i][0] = count;
	}
	
	for (int i=0; i<ist.cols; i++)
	{
		float ni = LUT[i][0];
		float T = (src.cols * src.rows);
		float L = ist.cols - 1;
		
		float eq = (ni/T)*L;
		
		if (eq>255) 
			LUT[i][1] = 255;
		else 
			LUT[i][1] = eq;
	}
	
	for(x=0; x<output.rows;x++)
	{
		for(y=0; y<output.cols; y++)
		{
			output.at<uchar>(x,y) = LUT[src.at<uchar>(x,y)][1];
		}
	}
	
return output;
}

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("Sintassi non corretta! ./eqistogramma [imgPath]");
		return -1;
	}

	Mat src = imread(argv[1],0);
	namedWindow("Source",WINDOW_AUTOSIZE);
	imshow("Source",src);
	waitKey(0);
	
	Mat istogram = istogramma(src);
	namedWindow("Istogramma",WINDOW_AUTOSIZE);
	imshow("Istogramma",istogram);
	waitKey(0);
	
	Mat equalizzato = equalizza(istogram,src);
	namedWindow("Istogramma Equalizzato",WINDOW_AUTOSIZE);
	imshow("Istogramma Equalizzato",equalizzato);
	waitKey(0);
	
	Mat istogram2 = istogramma(equalizzato);
	namedWindow("Istogramma2",WINDOW_AUTOSIZE);
	imshow("Istogramma2",istogram2);
	waitKey(0);
	
return 0;
}
