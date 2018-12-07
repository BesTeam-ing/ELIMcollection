/*
 * Flusso Ottico per soli 2 immagini
 * ! INCOMPLETO !
*/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat get_fx(Mat img1, Mat img2)
{
	Mat fx;
	
	Mat kernel = Mat::ones(2,2,CV_64F);
	kernel.at<double>(0,0) = -1.0;
	kernel.at<double>(1,0) = -1.0;
	
	Mat out1,out2;
	filter2D(img1,out1,-1,kernel);
	filter2D(img2,out2,-1,kernel);
	
	fx = out1+out2;

return fx;
}

Mat get_fy(Mat img1, Mat img2)
{
	Mat fy;
	
	Mat kernel = Mat::ones(2,2,CV_64F);
	kernel.at<double>(0,0) = -1.0;
	kernel.at<double>(0,1) = -1.0;
	
	Mat out1,out2;
	filter2D(img1,out1,-1,kernel);
	filter2D(img2,out2,-1,kernel);
	
	fy = out1+out2;

return fy;
}

Mat get_ft(Mat img1, Mat img2)
{
	Mat ft;
	
	Mat kernel = Mat::ones(2,2,CV_64F);
	kernel.mul(-1);
	
	Mat out1,out2;
	filter2D(img1,out1,-1,kernel);
	kernel.mul(-1);
	filter2D(img2,out2,-1,kernel);
	
	ft = out1 + out2;

return ft;
}

bool isInside(Mat src,int x,int y)
{
	if(x > 0 && x < src.rows-1 && y > 0 && y < src.cols-1)
		return true;
	return false;
}

double somma_double(Mat src, int x,int y)
{
	if (!isInside(src,x,y))
		return 0;
	
	double val = 0.0;
	int temp = 0;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (isInside(src,i,j))
			{
				temp++;
				val += src.at<double>(x+i,y+j);
			}
		}
	}
	if (temp == 9)
		return val;
	else
		return src.at<double>(x,y) * 9;
}

Mat somma_mat(Mat src)
{
	Mat output = Mat::zeros(src.rows,src.cols,CV_64F);
	int x,y;
	
	for (x = 1; x < src.rows-1; x++)
	{
		for (y = 1; y < src.cols-1; y++)
			output.at<double>(x,y) = somma_double(src,x,y);
		
	}
return output;
}

void Lucas(Mat src1, Mat src2, Mat& u, Mat& v)
{
	Mat fx = get_fx(src1,src2);
	Mat fy = get_fy(src1,src2);
	Mat ft = get_ft(src1,src2);
	
	Mat fx2 = fx.mul(fx);
	Mat fy2 = fy.mul(fy);
	Mat fxfy = fx.mul(fy);
	Mat fxft = fx.mul(ft);
	Mat fyft = fy.mul(ft);
	
	Mat Sfx2 = somma_mat(fx2);
	Mat Sfy2 = somma_mat(fy2);
	Mat Sfxfy = somma_mat(fxfy);
	Mat Sfxft = somma_mat(fxft);
	Mat Sfyft = somma_mat(fyft);
	
	Mat tmp = Sfx2.mul(Sfy2) - Sfxfy.mul(Sfxfy);
	u = Sfxfy.mul(Sfyft) - Sfy2.mul(Sfxft);
	v = Sfxft.mul(Sfxfy) - Sfx2.mul(Sfyft);
	divide(u,tmp,u);
	divide(v,tmp,v);
}

/*
 * TODO: QUESTA FUNZIONE NON è COMPLETA! NON STAMPA QUELLO CHE DOVREBBE!!
 * BISOGNA INSERIRE UNA FUNZIONE PER STAMPARE u,v !!!!
*/
void stampa(Mat src,Mat& output,Mat u, Mat v,int treshold)
{
	int x,y;
	
	for ( x = 0; x < src.rows; x++)
	{
		for ( y = 0; y < src.cols; y++)
		{
		
			double modulo = sqrt(pow(u.at<double>(x,y),2) + pow(v.at<double>(x,y),2));
			int cast = saturate_cast<uchar>(modulo);
			//cout<<cast<<endl;
			if (modulo > 0 && modulo > treshold)
			{
		
			//arrowedLine(output,Point(y,x),Point(y+1,x+1),Scalar(0,0,255),2,1);
			output.at<Vec3b>(x,y) = cast;
			//if(u.at<double>(x,y)!=0 ) output.at<Vec3b>(x,y) = {0,255,255};
			//else if (v.at<double>(x,y)!=0 )output.at<Vec3b>(x,y) = {255,255,0};
			}
			//else output.at<Vec3b>(x,y) = {0,0,0};
		//if(u.at<double>(x,y)!=0 ) output.at<Vec3b>(x,y) = {0,0,255};
		//else if (v.at<double>(x,y)!=0 )output.at<Vec3b>(x,y) = {255,0,0};
		}
	}
}

int main(int argc, char** argv)
{
	if(argc != 4)
	{
		printf("Sintassi non corretta! ./flusso [imgPath1] [imgPath2] [Treshold]");
		return -1;
	}

	Mat src = imread(argv[1],0);
	Mat src2 = imread(argv[2],0);
	
	int treshold = atoi(argv[3]);
	
	src.convertTo(src,CV_64F,1.0/255,0);
	src2.convertTo(src2,CV_64F,1.0/255,0);
	
	Mat u = Mat(src.rows,src.cols,CV_64F);
	Mat v = u.clone();
	
	Lucas(src,src2,u,v);
	Mat output = imread(argv[1],1);
	stampa(src,output,u,v,treshold); //TODO: Migliorare/Correggere la stampa!
	
	imshow("Output",output);
	waitKey(0);
	
return 0;
}
