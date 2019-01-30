
#pragma once
#include <windows.h>
#include "MyForm4.h"

BYTE* LoadBMP(unsigned int% width, unsigned int% height, long% size, LPCTSTR bmpfile);
bool  SaveBMP(BYTE* Buffer, int width, int height, long paddedsize, LPCTSTR bmpfile);
BYTE* ConvertBMPToIntensity(BYTE* Buffer, unsigned int width, unsigned int height);
BYTE* ConvertIntensityToBMP(BYTE* Buffer, int width, int height, long* newsize);
BYTE *zoom(BYTE * Buffer, unsigned int zoom_width1, unsigned int zoom_width2, unsigned int zoom_height1, unsigned int zoom_height2,unsigned int Width);
int *histogram(BYTE *Buffer, unsigned int width, unsigned int height);
BYTE *histogram_equalized(BYTE *Buffer, unsigned int width, unsigned int height);
BYTE *k_means(BYTE *Buffer, unsigned int Width, unsigned int Height);
BYTE *dilation(BYTE *Buffer, unsigned int Width, unsigned int Height);
BYTE *erosion(BYTE *Buffer, unsigned int Width, unsigned int Height);
BYTE *label(BYTE *Buffer, unsigned int Width, unsigned int Height);
float oklid_Distance(float *arr1, float *arr2, int size);
BYTE *object_recognition(BYTE *Buffer, unsigned int Width, unsigned int Height);
void draw(BYTE *Buffer, unsigned int Width, unsigned int Height,int padding, int x1, int x2, int y1, int y2, int renk);
BYTE *canny_Andhough(BYTE *Buffer, unsigned int Width, unsigned int Height);
