/*
 * Split & Merge
*/
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int treshold;

struct region
{
	vector<region> childs;
	Rect roi;
	Mat m;
	bool isAttivo;
	
	Scalar colore;
};

region split(Mat src, Rect roi,bool(*verifica)(Mat))
{
	region R;
	
	R.roi = roi;
	R.m = src;
	R.isAttivo = true;
	
	bool ver = verifica(src);
	
	if (ver)
	{
		Scalar mean,s;
		meanStdDev(src,mean,s);
		R.colore = mean;
	}
	
	else 
	{
		int w = src.cols/2;
		int h = src.rows/2;
		
		region R1 = split(src(Rect(0,0,w,h)),Rect(roi.x,roi.y,w,h),verifica);
		region R2 = split(src(Rect(w,0,w,h)),Rect(roi.x+w,roi.y,w,h),verifica);
		region R3 = split(src(Rect(0,h,w,h)),Rect(roi.x,roi.y+h,w,h),verifica);
		region R4 = split(src(Rect(w,h,w,h)),Rect(roi.x+w,roi.y+h,w,h),verifica);

		//Aggiungo ad R
		R.childs.push_back(R1);
		R.childs.push_back(R2);
		R.childs.push_back(R3);
		R.childs.push_back(R4);
	}
return R;
}

bool unisciRegioni(region parent,Mat src,region r1,region r2,bool (*verifica)(Mat))
{
	if(r1.childs.size() == 0 && r2.childs.size() == 0)
	{
		Rect R1 = r1.roi;
		Rect R2 = r2.roi;
		Rect R12 = R1|R2;
		
		if (verifica(src(R12)) )
		{
			r1.roi = R12;
			r2.isAttivo = false;
			return true;
		}
	}
	return false;
}

void merge(Mat src,region &R,bool (*verifica)(Mat))
{
	bool row1,row2,col1,col2;
	row1=row2=col1=col2 = false;
	
	if (R.childs.size() < 1)
		return;
	
	row1 = unisciRegioni(R,src,R.childs[0],R.childs[1],verifica);
	row2 = unisciRegioni(R,src,R.childs[2],R.childs[3],verifica);
	
	if (!row1 && !row2)
	{
		col1 = unisciRegioni(R,src,R.childs[0],R.childs[2],verifica);
		col2 = unisciRegioni(R,src,R.childs[1],R.childs[3],verifica);
	}
	
	for (int i = 0; i < R.childs.size(); i++)
	{
		if (R.childs[i].childs.size()>0)
			merge(src,R.childs[i],verifica);
	}
}

bool verifica(Mat src)
{
	Scalar media,s;
	meanStdDev(src,media,s);
	
	if (s[0] <= 5.8 || src.cols * src.rows <= treshold)
		return true;
	
	return false;
}

void colora(Mat src, region R)
{
	if (R.childs.size()==0 && R.isAttivo == true)
		rectangle(src,R.roi,R.colore,FILLED);
	
	for (int i = 0; i < R.childs.size(); i++)
		colora(src,R.childs[i]);
	
}
int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("Sintassi non corretta! ./regColor [imgPath] [Treshold]");
		return -1;
	}

	Mat src = imread(argv[1]);
	treshold = atoi(argv[2]);
	region R;
	
	namedWindow("Src",WINDOW_AUTOSIZE);
	imshow("Src",src);
	waitKey(0);
	
	R = split(src,Rect(0,0,src.cols,src.rows),verifica);
	merge(src,R,&verifica);
	
	Mat output = src.clone();
	colora(output,R);
	namedWindow("Split",WINDOW_AUTOSIZE);
	imshow("Split",output);
	waitKey(0);

return 0;
}
