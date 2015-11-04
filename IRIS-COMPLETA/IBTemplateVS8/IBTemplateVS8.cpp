/**
* @Author: Tomas Gonzalez - TomasGiS
* @Mail: tomas.gonzalez@urv.cat
* @Description: IB_template
* @Date: May2014
*/

#include "stdafx.h"
#include <iostream>
#include "common.h" 

using namespace cv;
using namespace std;


int main(int argc, _TCHAR* argv[])
{
	cv::Mat src, src_gray, angular, normal;
	cv::Mat src2, src_gray2, angular2, normal2;
	
	double rp = 0.0;
	double rs = 0.0; //Radios
	double xc = 0.0;
	double yc = 0.0;
	int M1, N1, M2, N2;

	//cv::Mat img = cv::imread("c:\\IB\\data\\iris\\img.jpg");

	//namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.

  /// Read the image
  src = cv::imread("c:\\IB\\data\\iris\\Iris7n.jpg");
  src2 = cv::imread("c:\\IB\\data\\iris\\Iris7o.jpg");
  
  if( !src.data )
    { return -1; }

  src_gray = src.clone();
  src_gray2 = src2.clone();
  
  cvtColor( src, src_gray, CV_BGR2GRAY );
  cvtColor( src2, src_gray2, CV_BGR2GRAY );
  
  //cv::threshold(src, src_gray, 127, 255, cv::THRESH_BINARY); 
	
  src_gray.convertTo(src_gray, CV_32F);
  src.convertTo(src, CV_32F);
  src_gray2.convertTo(src_gray2, CV_32F);
  src2.convertTo(src2, CV_32F);

  double * arrays = computeRadios(src_gray);
  rp = arrays[0];
  rs = arrays[1];
  xc = arrays[2];
  yc = arrays[3];
  M1 = cvRound(M_PI*(rs+rp));
  N1 = cvRound(rs);
  M2 = cvRound(M_PI*(rs+rp));
  N2 = cvRound(rs);
  //rows = N, cols = M
  angular2 = computeAngular(src_gray2, rp, rs, xc, yc, M2, N2);
  normal2 = computeNorm(src_gray2, angular2, rp, rs, xc, yc, M2, N2);
  angular = computeAngular(src_gray, rp, rs, xc, yc, M1, N1);
  normal = computeNorm(src_gray, angular, rp, rs, xc, yc, M1, N1);
  
  //arrays = computeRadios(src_gray2);
  //rp = arrays[0];
  //rs = arrays[1];
  //xc = arrays[2];
  //yc = arrays[3];
  
  //rows = N, cols = M
  
  //Matching
  int M = min(M1, M2);
  int N = min(N1, N2);
  double DT = 10000; //
  int CC = 0;
  for(int c=0; c<M-1; c++){
	  double sumai = 0.0;
	  double sumad = 0.0;
	  double D = 0.0;
	  for(int x = c; x<M-1; x++){
		  for(int y = 0; y<N-1; y++){
		  	sumai+=pow((normal.at<float>(y, x)-normal2.at<float>(y, x-c)),2);
		  }
	  }
	  
	  for(int x = 0; x<c-1; x++){
		  for(int y = 0; y<N-1; y++){
		  	sumad+=pow((normal.at<float>(y, x)-normal2.at<float>(y, M-c+x)),2);
		  }
	  }

	  D = sqrt(sumai+sumad);

	  if(D<DT){
		DT=D;
		CC=c;
		//cout<<" c:"<<CC<<" ";
	  }
  }
  int CC1 = CC;
  //for(int c=-1; c<-(M-1); c++){
	 // double sumai = 0.0;
	 // double sumad = 0.0;
	 // double D = 0.0;
	 // for(int x = c; x<M-1; x++){
		//  for(int y = 0; y<N-1; y++){
		//  	sumai+=pow((normal.at<float>(y, x)-normal2.at<float>(y,x+c)),2);
		//  }
	 // }
	 // 
	 // for(int x = 0; x<-(c-1); x++){
		//  for(int y = 0; y<N-1; y++){
		//  	sumad+=pow((normal.at<float>(y, x)-normal2.at<float>(y, M+c+x)),2);
		//  }
	 // }

	 // D = sqrt(sumai+sumad);
	 // DT=(D<DT)?D:DT;
	 // CC=(D<DT)?c:CC;
	 // cout<<" c:"<<c<<" ";
  //}

  cout<<"D: "<<DT<<" C:"<<CC<<endl;

  /// Show your results
  convertScaleAbs(src, src);
  convertScaleAbs(normal, normal);
  convertScaleAbs(normal2, normal2);
  
  namedWindow("Original", CV_WINDOW_AUTOSIZE);
  imshow( "Original", src);
  imshow( "Imagen A", normal);
  imshow( "Imagen I", normal2);
  imwrite( "c:\\IB\\data\\iris\\Iris7A.jpg", normal);
  imwrite( "c:\\IB\\data\\iris\\Iris7I.jpg", normal2);
  waitKey(0);
  return 0;

}

cv::Mat computeNorm(cv::Mat iris, cv::Mat result, double rp, double rs, double xc, double yc, int M, int N){
	
	double alpha = 0.0;
	double r = 0.0;
	cv::Mat result2 = result.clone();

	//Normalization
	for(int xp=0; xp<M; xp++){
		for(int yp=0; yp<N; yp++){
			alpha = (2*M_PI*xp)/(M-1)-M_PI;
			r = rp + ((rs-rp)*yp)/(N-1);
			int x = cvRound(r*sinf(alpha)+xc);
			int y = cvRound(r*cosf(alpha)+yc);
			if(x>0 && y>0 && x<iris.cols && y<iris.rows){
				result2.at<float>(yp, xp) = iris.at<float>(y, x);
			}
		}
  	}

	return result2;

}

cv::Mat computeAngular(cv::Mat iris, double rp, double rs, double xc, double yc, int M, int N){

	cv::Mat result;
	result = Mat::zeros(N, M, CV_32F);
	int xp = 0;
	int yp = 0;
	double alpha = 0.0;
	double r = 0.0;


	//Angular Conversion
	for(int x=0; x<iris.rows; x++){
		for(int y=0; y<iris.cols; y++){
			alpha = atan2(x-yc, y-xc);
			r = sqrt(pow(x-xc, 2)+pow(y-yc, 2));
			if(r>rp && r<rs){
				xp = cvRound((M-1)*(alpha+M_PI)/(2*M_PI));
				yp = cvRound((N-1)*(r-rp)/(rs-rp));
				if(xp>0 && yp>0 && xp<M-1 && yp<N-1){
					result.at<float>(yp, xp) = iris.at<float>(x, y);
				}
			}
		}
	  }


	return result;
}


double * computeRadios(cv::Mat iris){

	int coord[2][3] = {{0, 0, 0},{0, 0, 0}};
	double val[4] = {0, 0, 0, 0};
	int posx = cvRound(iris.rows/2);
	int posy = cvRound(iris.cols/2);
	int b = 0;

	for(int i = 0; i<iris.cols; i++){
	  if(iris.at<float>(posx, i)>0 && b == 0){
		cout << "Coord: "<<posx<<","<<i<<endl;
		coord[0][0] = i;
		i=i+10;
		b = 1;
	  }
	  if(iris.at<float>(posx, i)==0 && b == 1){
		cout << "Coord: "<<posx<<","<<i<<endl;
		coord[0][1] = i;
		i=i+10;
		b = 2;
	  }
	  if(iris.at<float>(posx, i)>0 && b == 2){
		cout << "Coord: "<<posx<<","<<i<<endl;
		coord[0][2] = i;
		b = 3;
		break;
	  }
  }

  b=0;
  for(int i = 0; i<iris.rows; i++){
	  if(iris.at<float>(i, posy)>0 && b == 0){
		cout << "Coord: "<<i<<","<<posy<<endl;
		coord[1][0] = i;
		i=i+10;
		b = 1;
	  }
	  if(iris.at<float>(i, posy)==0 && b == 1){
		cout << "Coord: "<<i<<","<<posy<<endl;
		coord[1][1] = i;
		i=i+10;
		b = 2;
	  }
	  if(iris.at<float>(i, posy)>0 && b == 2){
		cout << "Coord: "<<i<<","<<posy<<endl;
		coord[1][2] = i;
		b = 3;
		break;
	  }
  }

  val[0] = min(abs((double)(coord[0][2]-coord[0][1]))/2, abs((double)(coord[1][2]-coord[1][1])/2)); //rp
  val[1] = min(abs((double)(coord[0][0]-coord[0][2]))-val[0], abs((double)(coord[1][0]-coord[1][2]))-val[0]); //rs
  val[2] = coord[0][1]+val[0]; //xc
  val[3] = coord[1][1]+val[0]; //yx

//  cout<<"rp: "<<rp<<"  "<<"rs: "<<rs<<endl;
//  cout<<"centro("<<xc<<","<<yc<<")"<<endl;
//  circle(iris, Point(cvRound(xc), cvRound(yc)), 4, Scalar(0,0,255), -1, 8);

  return val;
}
