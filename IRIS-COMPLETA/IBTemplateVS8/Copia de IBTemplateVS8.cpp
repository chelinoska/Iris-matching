/**
* @Author: Tomas Gonzalez - TomasGiS
* @Mail: tomas.gonzalez@urv.cat
* @Description: IB_template
* @Date: May2014
*/

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "common.h" 

using namespace cv;
using namespace std;

cv::Mat src, src_gray, result;
double rp = 0.0;
double rs = 0.0; //Radios
double xc = 0.0;
double yc = 0.0;
int b = 0;
int M, N;
int coord[2][3] = {{0, 0, 0},{0, 0, 0}};

int main(int argc, _TCHAR* argv[])
{
	double alpha = 0.0;
	double r = 0.0;
	int xp = 0;
	int yp = 0;

	//cv::Mat img = cv::imread("c:\\IB\\data\\iris\\img.jpg");

	//namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.

  /// Read the image
  src = cv::imread("c:\\IB\\data\\iris\\Iris3.jpg");
  
  if( !src.data )
    { return -1; }

  src_gray = src.clone();
  
  cvtColor( src, src_gray, CV_BGR2GRAY );
  //cv::threshold(src, src_gray, 127, 255, cv::THRESH_BINARY); 
	
  src_gray.convertTo(src_gray, CV_32F);
  src.convertTo(src, CV_32F);

  computeR();
  M = cvRound(M_PI*(rs+rp));
  N = cvRound(rs);
  //rows = N, cols = M
  result = Mat::zeros(N, M, CV_32F);
  
  //Angular Conversion
  for(int x=0; x<src.rows; x++){
	  for(int y=0; y<src.cols; y++){
		  alpha = atan2(x-yc, y-xc);
		  r = sqrt(pow(x-xc, 2)+pow(y-yc, 2));
		  if(r>rp && r<rs){
			  xp = cvRound((M-1)*(alpha+M_PI)/(2*M_PI));
			  yp = cvRound((N-1)*(r-rp)/(rs-rp));
			  if(xp>0 && yp>0 && xp<M-1 && yp<N-1){
				  result.at<float>(yp, xp) = src_gray.at<float>(x, y);
			  }
		  }
	  }
  }

  //Normalization
	cv::Mat resultpar = result.clone();
	for(xp=0; xp<M; xp++){
	  for(yp=0; yp<N; yp++){
		alpha = (2*M_PI*xp)/(M-1)-M_PI;
		r = rp + ((rs-rp)*yp)/(N-1);
		int x = cvRound(r*sinf(alpha)+xc);
		int y = cvRound(r*cosf(alpha)+yc);
		 if(x>0 && y>0 && x<src_gray.cols && y<src_gray.rows){
			result.at<float>(yp, xp) = src_gray.at<float>(y, x);
		 }
	  }
  }

  /// Show your results
  convertScaleAbs(src, src);
  convertScaleAbs(result, result);
  convertScaleAbs(resultpar, resultpar);
  
  namedWindow("Original", CV_WINDOW_AUTOSIZE);
  imshow( "Original", src);
  imshow( "Sin Normalizar", resultpar);
  imshow( "Normalizada", result);

  //Matching
  for(int c=0; c<M-1; c++){

	  double sumai = 0.0;
	  double sumad = 0.0;
	  double D = 0.0;
	  for(int x = c; x<M-1; x++){
		  for(int y = 0; y<N-1; y++){
		  
		  }
	  }
  
  }

  waitKey(0);
  return 0;

}

int * computeR(){

	int val[4] =  {0, 0, 0, 0};
	int posx = cvRound(src.rows/2);
	int posy = cvRound(src.cols/2);

	for(int i = 0; i<src_gray.cols; i++){
	  if(src_gray.at<float>(posx, i)>0 && b == 0){
		cout << "Coord: "<<posx<<","<<i<<endl;
		coord[0][0] = i;
		i=i+10;
		b = 1;
	  }
	  if(src_gray.at<float>(posx, i)==0 && b == 1){
		cout << "Coord: "<<posx<<","<<i<<endl;
		coord[0][1] = i;
		i=i+10;
		b = 2;
	  }
	  if(src_gray.at<float>(posx, i)>0 && b == 2){
		cout << "Coord: "<<posx<<","<<i<<endl;
		coord[0][2] = i;
		b = 3;
		break;
	  }
  }

  b=0;
  for(int i = 0; i<src_gray.rows; i++){
	  if(src_gray.at<float>(i, posy)>0 && b == 0){
		cout << "Coord: "<<i<<","<<posy<<endl;
		coord[1][0] = i;
		i=i+10;
		b = 1;
	  }
	  if(src_gray.at<float>(i, posy)==0 && b == 1){
		cout << "Coord: "<<i<<","<<posy<<endl;
		coord[1][1] = i;
		i=i+10;
		b = 2;
	  }
	  if(src_gray.at<float>(i, posy)>0 && b == 2){
		cout << "Coord: "<<i<<","<<posy<<endl;
		coord[1][2] = i;
		b = 3;
		break;
	  }
  }

  rp = min(abs((double)(coord[0][2]-coord[0][1]))/2, abs((double)(coord[1][2]-coord[1][1])/2));
  rs = min(abs((double)(coord[0][0]-coord[0][2]))-rp, abs((double)(coord[1][0]-coord[1][2]))-rp);
  xc = coord[0][1]+rp;
  yc = coord[1][1]+rp;

  cout<<"rp: "<<rp<<"  "<<"rs: "<<rs<<endl;
  cout<<"centro("<<xc<<","<<yc<<")"<<endl;
  circle(src, Point(cvRound(xc), cvRound(yc)), 4, Scalar(0,0,255), -1, 8);

  return val;
}