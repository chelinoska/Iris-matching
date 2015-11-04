// stdafx.h: archivo de inclusi�n de los archivos de inclusi�n est�ndar del sistema
// o archivos de inclusi�n espec�ficos de un proyecto utilizados frecuentemente,
// pero rara vez modificados
//

#pragma once


#define _USE_MATH_DEFINES
#include <cmath>
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include "opencv.hpp"

using namespace cv;

// TODO: mencionar aqu� los encabezados adicionales que el programa necesita
double * computeRadios(cv::Mat iris);
cv::Mat computeAngular(cv::Mat iris, double rp, double rs, double xc, double yc, int M, int N);
cv::Mat computeNorm(cv::Mat iris, cv::Mat result, double rp, double rs, double xc, double yc, int M, int N);
