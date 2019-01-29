// Image_Process1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <math.h>

//User defined includes
#include "imge_bmp.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	LPCTSTR input, output;
	unsigned int Width, Height;
	long Size, new_size;

	cout << "Haydi Bismillah" << endl;
	input = L"C://Users//Hakan58//Desktop//Adsýz.bmp";
	
	BYTE* buffer = LoadBMP(Width, Height, Size, input);		// bufferda goruntunun adresi ve boyutlari tutuluyor
	BYTE* raw_intensity = ConvertBMPToIntensity(buffer, Width, Height); // ham yogunluga cevirme

	char ch;
	cout << "Sonucu diske kaydetsin mi? E/H:"; cin >> ch;
	if ((ch == 'E') || (ch == 'e')) {

		BYTE* display_imge = ConvertIntensityToBMP(raw_intensity, Width, Height, &new_size); // yeni fotografý bmp'ye cevir ve goster 
		output = L"C://Users//Hakan58//Desktop//copy6.bmp";
		if (SaveBMP(display_imge, Width, Height, new_size, output))		//kaydet
			cout << " Output Image was successfully saved" << endl;
		else cout << "Error on saving image" << endl;
		delete[] display_imge;						//display_imge'deki yeni fotografý sil
	}//


	delete[] buffer;
	delete[] raw_intensity;

	return 0;
}







