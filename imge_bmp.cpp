#include "stdafx.h"
#include <windows.h>
#include "MyForm4.h"
#include "humoments.h"
#include <iostream>
#define PI 3.14159265


using namespace std;

BYTE* LoadBMP(unsigned int% width, unsigned  int% height, long% size, LPCTSTR bmpfile)
{
	// declare bitmap structures
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	// value to be used in ReadFile funcs
	DWORD bytesread;
	// open file to read from
	HANDLE file = CreateFile(bmpfile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (NULL == file)
		return NULL; // coudn't open file

					 // read file header
	if (ReadFile(file, &bmpheader, sizeof(BITMAPFILEHEADER), &bytesread, NULL) == false) {
		CloseHandle(file);
		return NULL;
	}
	//read bitmap info
	if (ReadFile(file, &bmpinfo, sizeof(BITMAPINFOHEADER), &bytesread, NULL) == false) {
		CloseHandle(file);
		return NULL;
	}
	// check if file is actually a bmp
	if (bmpheader.bfType != 'MB') {
		CloseHandle(file);
		return NULL;
	}
	// get image measurements
	width = bmpinfo.biWidth;
	height = abs(bmpinfo.biHeight);

	// check if bmp is uncompressed
	if (bmpinfo.biCompression != BI_RGB) {
		CloseHandle(file);
		return NULL;
	}
	// check if we have 24 bit bmp
	if (bmpinfo.biBitCount != 24) {
		CloseHandle(file);
		return NULL;
	}

	// create buffer to hold the data
	size = bmpheader.bfSize - bmpheader.bfOffBits;
	BYTE* Buffer = new BYTE[size];
	// move file pointer to start of bitmap data
	SetFilePointer(file, bmpheader.bfOffBits, NULL, FILE_BEGIN);
	// read bmp data
	if (ReadFile(file, Buffer, size, &bytesread, NULL) == false) {
		delete[] Buffer;
		CloseHandle(file);
		return NULL;
	}
	// everything successful here: close file and return buffer
	CloseHandle(file);

	return Buffer;
}//LOADPMB	 // görüntüde herhangi bir sýkýntý yoksa dinamik bir bellek döndürüyor

bool SaveBMP(BYTE* Buffer, int width, int height, long paddedsize, LPCTSTR bmpfile)
{
	// declare bmp structures 
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER info;

	// andinitialize them to zero
	memset(&bmfh, 0, sizeof(BITMAPFILEHEADER));
	memset(&info, 0, sizeof(BITMAPINFOHEADER));

	// fill the fileheader with data
	bmfh.bfType = 0x4d42;       // 0x4d42 = 'BM'
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + paddedsize;
	bmfh.bfOffBits = 0x36;		// number of bytes to start of bitmap bits

								// fill the infoheader

	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = width;
	info.biHeight = height;
	info.biPlanes = 1;			// we only have one bitplane
	info.biBitCount = 24;		// RGB mode is 24 bits
	info.biCompression = BI_RGB;
	info.biSizeImage = 0;		// can be 0 for 24 bit images
	info.biXPelsPerMeter = 0x0ec4;     // paint and PSP use this values
	info.biYPelsPerMeter = 0x0ec4;
	info.biClrUsed = 0;			// we are in RGB mode and have no palette
	info.biClrImportant = 0;    // all colors are important

								// now we open the file to write to
	HANDLE file = CreateFile(bmpfile, GENERIC_WRITE, FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == NULL) {
		CloseHandle(file);
		return false;
	}
	// write file header
	unsigned long bwritten;
	if (WriteFile(file, &bmfh, sizeof(BITMAPFILEHEADER), &bwritten, NULL) == false) {
		CloseHandle(file);
		return false;
	}
	// write infoheader
	if (WriteFile(file, &info, sizeof(BITMAPINFOHEADER), &bwritten, NULL) == false) {
		CloseHandle(file);
		return false;
	}
	// write image data
	if (WriteFile(file, Buffer, paddedsize, &bwritten, NULL) == false) {
		CloseHandle(file);
		return false;
	}

	// and close file
	CloseHandle(file);

	return true;
} // SaveBMP

BYTE* ConvertBMPToIntensity(BYTE* Buffer, int unsigned width, int unsigned height)
{
	// first make sure the parameters are valid
	if ((NULL == Buffer) || (width == 0) || (height == 0))
		return NULL;

	// find the number of padding bytes

	int padding = 0;
	int scanlinebytes = width * 3;
	while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;

	// create new buffer
	BYTE* newbuf = new BYTE[width*height];

	// now we loop trough all bytes of the original buffer, 
	// swap the R and B bytes and the scanlines
	long bufpos = 0;
	long newpos = 0;
	for (int row = 0; row < height; row++)
		for (int column = 0; column < width; column++) {
			newpos = row * width + column;
			bufpos = (height - row - 1) * psw + column * 3;
			newbuf[newpos] = BYTE(0.11*Buffer[bufpos + 2] + 0.59*Buffer[bufpos + 1] + 0.3*Buffer[bufpos]);
		}

	return newbuf;
}//ConvetBMPToIntensity

BYTE* ConvertIntensityToBMP(BYTE* Buffer, int width, int height, long* newsize)
{
	// first make sure the parameters are valid
	if ((NULL == Buffer) || (width == 0) || (height == 0))
		return NULL;

	// now we have to find with how many bytes
	// we have to pad for the next DWORD boundary	

	int padding = 0;
	int scanlinebytes = width * 3;
	while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;
	// we can already store the size of the new padded buffer
	*newsize = height * psw;

	// and create new buffer
	BYTE* newbuf = new BYTE[*newsize];

	// fill the buffer with zero bytes then we dont have to add
	// extra padding zero bytes later on
	memset(newbuf, 0, *newsize);

	// now we loop trough all bytes of the original buffer, 
	// swap the R and B bytes and the scanlines
	long bufpos = 0;
	long newpos = 0;
	for (int row = 0; row < height; row++)
		for (int column = 0; column < width; column++) {
			bufpos = row * width + column;     // position in original buffer
			newpos = (height - row - 1) * psw + column * 3;           // position in padded buffer
			//newpos = (row)* psw + column * 3;
			newbuf[newpos] = Buffer[bufpos];       //  blue
			newbuf[newpos + 1] = Buffer[bufpos];   //  green
			newbuf[newpos + 2] = Buffer[bufpos];   //  red
		}

	return newbuf;
} //ConvertIntensityToBMP

BYTE *zoom(BYTE * Buffer, unsigned int zoom_width1, unsigned int zoom_width2, unsigned int zoom_height1, unsigned int zoom_height2,unsigned int Width)
{
	int first_index = 0;
	unsigned int k=0;
	unsigned int zoom_width = ((zoom_width2 - zoom_width1) + 1);
	unsigned int zoom_height = ((zoom_height2 - zoom_height1) + 1);
	BYTE *Zoom_buffer = new BYTE[(2 * zoom_height - 1)*(2 * zoom_width - 1)];


	for (unsigned int i = zoom_height1; i < zoom_height2; i++)
	{
		for (unsigned int j = zoom_width1; j < zoom_width2 + 1; j++)
		{

			Zoom_buffer[k] = Buffer[(i*Width) + j];
			k++;
			if (j == zoom_width2)
				break;

			double gecici_bellek = ((Buffer[(i*Width) + j]) + (Buffer[(i*Width) + (j + 1)])) / 2;
			Zoom_buffer[k] = gecici_bellek;
			k++;

		}
		k = k + ((2 * zoom_width) - 1);
	}
	k = ((2 * zoom_width) - 1);

	for (unsigned int i = zoom_height1; i < zoom_height2; i++)
	{
		for (int m = 0; m < 2 * zoom_width - 1; m++)
		{
			double gecici_bellek = (Zoom_buffer[first_index + m] + Zoom_buffer[first_index + 2 * (2 * zoom_width - 1) + m]) / 2;
			Zoom_buffer[k] = gecici_bellek;
			k++;
		}
		first_index = first_index + 2 * (2 * zoom_width - 1);
		k = k + ((2 * zoom_width) - 1);
	}
	return Zoom_buffer;



	//****************ARAYA SIFIR KOYARAK ZOOM YAPMA*****************************

	/*unsigned int zoom_width= ((zoom_width2 - zoom_width1) * 2) + 1;
	unsigned int zoom_height= ((zoom_height2 - zoom_height1) * 2) + 1;
	BYTE *Zoom_buffer = new BYTE[zoom_height*zoom_width];

	pictureBox3->Width = zoom_width;
	pictureBox3->Height = zoom_height;
	for (unsigned int i = zoom_height1; i < zoom_height2; i++)
	{
	for (int m = 0;m < (zoom_width);m++)
	{
	Zoom_buffer[k] = 0;
	k++;
	}
	Zoom_buffer[k] = 0;
	k++;
	for (unsigned int j = zoom_width1; j < zoom_width2; j++)
	{
	Zoom_buffer[k] = Raw_Intensity[(i*Width) + j];
	k++;
	Zoom_buffer[k] = 0;
	k++;
	}
	}

	for (int m = 0;m < (zoom_width);m++)
	{
	Zoom_buffer[k] = 0;
	k++;
	}
	this->pictureBox3->Location = System::Drawing::Point(pictureBox2->Left + pictureBox2->Width + 10, 34);

	unsigned int row1, column1;
	Bitmap^ surface1 = gcnew Bitmap(pictureBox3->Width, pictureBox3->Height);
	pictureBox3->Image = surface1;
	Color c1;

	for (row1 = 0; row1<zoom_height; row1++)
	for (column1 = 0; column1 < zoom_width; column1++)
	{
	c1 = Color::FromArgb(Zoom_buffer[(row1*zoom_width)+column1], Zoom_buffer[(row1*zoom_width)+column1], Zoom_buffer[(row1*zoom_width)+column1]);
	surface1->SetPixel(column1, row1, c1);
	}*/




	//**********Araya sýfýr koyarak zoom, daha optimize kodu****************

	/*for (int i = 0; i <zoom_height;i++)
	{
	for (int j = 0; j <zoom_width + 1;j++)
	{
	if ((i % 2 == 0) || (j % 2 == 0))
	Zoom_buffer[i*zoom_width + j] = 0;

	else
	Zoom_buffer[i*zoom_width + j] = Raw_Intensity[zoom_height1 + (i / 2 - 1)*Width + (zoom_width1 + (j / 2 - 1))];
	}
	}*/


	/*delete[] Buffer;
	delete[] Raw_Intensity;
	delete[] Zoom_buffer;*/
}

int *histogram(BYTE *Buffer, unsigned int Width, unsigned int Height)
{
	int *histogram_array = new int[256];
	int numberofpixel = 0;
	for (int i = 0; i<256; i++)
	{
		for (int j = 0; j <= Width * Height; j++)
		{
			if (i == Buffer[j])
			{
				numberofpixel++;
			}
		}
		histogram_array[i] = numberofpixel;
		numberofpixel = 0;
	}

	return histogram_array;
}

BYTE *histogram_equalized(BYTE *Buffer, unsigned int Width, unsigned int Height)
{
	double sum = 0;
	double sum2 = 0;
	int *histogram_array = new int[256];
	histogram_array = histogram(Buffer, Width, Height);

	for (int i = 0; i < 256; i++)
	{
		sum += histogram_array[i];
	}

	int *histogram_equalized_values = new int[256];
	for (int i = 0; i < 256; i++)
	{
		sum2 += histogram_array[i];
		histogram_equalized_values[i] = round((sum2 / sum) * 255.0);	//HÝSTOGRAM BÝLGÝSÝNE BAKARAK EQUALÝZED DEÐERLERÝNÝ DÝZÝYE ATTIK
	}

	BYTE *histogram_equalized = new BYTE[Width*Height];

	histogram_equalized = Buffer;	//FONKSYONU UYGULAMAK ÝSTEDÝÐÝMÝZ GÖRÜNTÜYÜ KOPYALADIK
	
	int gray_value[256];
	for (int i = 0;i < 256;i++)
		gray_value[i] = i;

	for (int i = 0; i < Width*Height; i++)
	{
		if (histogram_equalized_values[histogram_equalized[i]] != gray_value[histogram_equalized[i]])
			histogram_equalized[i] = histogram_equalized_values[histogram_equalized[i]];

	}

	return histogram_equalized;

}

BYTE *k_means(BYTE *Buffer, unsigned int Width, unsigned int Height)
{
	float t1=0, t2=255, t11=-1,t22=-1;
	double sum1 = 0, sum2 = 0;
	double division1 = 0, division2 = 0;

	int *histogram_array = new int[256];
	histogram_array = histogram(Buffer, Width, Height);

	while (true)
	{
		for (int i = 0; i < 256; i++)
		{
			if (fabs(i - t1) < fabs(i - t2))
			{
				sum1 += (i*histogram_array[i]);
				division1 += histogram_array[i];
			}
			else
			{
				sum2 += (i*histogram_array[i]);
				division2 += histogram_array[i];
			}
		}

		t11 = sum1 / division1;
		t22 = sum2 / division2;

		if (t1 == t11 && t2 == t22)
			break;

		t1 = t11;
		t2 = t22;
	}

	BYTE *k_means_buffer = new BYTE[Width*Height];
	k_means_buffer = Buffer;
	for (int i = 0; i < Width * Height; i++)
	{
		if (fabs(k_means_buffer[i] - t11) < fabs(k_means_buffer[i] - t22))
			k_means_buffer[i] = 0;
		else
			k_means_buffer[i] = 255;
	}

	return k_means_buffer;
}

BYTE *dilation(BYTE *Buffer, unsigned int Width, unsigned int Height)
{
	BYTE *dilation = new BYTE[Width*Height];
	int C;
	bool result=0;
	
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			C = (i*Width + j);
			//result=(Buffer[(C - Width -1)] || Buffer[(C - Width)] || Buffer[(C - Width +1)] || Buffer[(C - 1)] || Buffer[C] || Buffer[(C + 1)] || Buffer[(C + Width - 1)] || Buffer[(C + Width)]|| Buffer[(C + Width + 1)]);
			result = ( Buffer[(C - Width)] || Buffer[(C - 1)] || Buffer[C] || Buffer[(C + 1)]  || Buffer[(C + Width)] );
		

			if (result== true)
				dilation[C] = 255;
 			else
				dilation[C] = 0;
			
		}
		
	}

	return dilation;
}

BYTE *erosion(BYTE *Buffer, unsigned int Width, unsigned int Height)
{
	BYTE *erosion = new BYTE[Width*Height];
	int C;
	bool result = 0;

	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			C = (i*Width + j);
			result = (Buffer[(C - Width - 1)] && Buffer[(C - Width)] && Buffer[(C - Width + 1)] && Buffer[(C - 1)] && Buffer[C] && Buffer[(C + 1)] && Buffer[(C + Width - 1)] && Buffer[(C + Width)] && Buffer[(C + Width + 1)]);
			//result = (Buffer[(C - Width)] && Buffer[(C - 1)] && Buffer[C] && Buffer[(C + 1)] && Buffer[(C + Width)]);

			if (result == true)
				erosion[C] = 255;
			else
				erosion[C] = 0;

		}

	}

	return erosion;
}

BYTE *label(BYTE *Buffer, unsigned int Width, unsigned int Height)
{
	BYTE *label = new BYTE[Width*Height];
	label = Buffer;

	int C;
	int X = 0;


	for (int i = 1; i < Height - 1; i++)
	{
		for (int j = 1; j < Width - 1; j++)
		{
			C = (i*Width + j);
			int temp = 255;

			if (label[C] == 255)
			{
				int array[9] = { label[(C - Width - 1)] ,label[(C - Width)] , label[(C - Width + 1)] , label[(C - 1)] , label[C] , label[(C + 1)] , label[(C + Width - 1)] , label[(C + Width)] ,label[(C + Width + 1)] };

				for (int i = 0; i < 9; i++)
				{

					if (array[i] != 0 && array[i] != 255)
					{
						if (array[i] < temp)
							temp = array[i];

					}
				}

				if (temp != 255)
					label[C] = temp;

				else
				{
					label[C] = 250 - X;
					X += 2;
				}
			}
		}
	}

	for (int i = Height - 1; i > 1; i--)
	{
		for (int j = Width - 1; j > 1; j--)
		{
			C = (i*Width + j);
			int temp = 255;

			if (label[C] > 0)
			{
				int b[9] = { label[(C - Width - 1)] ,label[(C - Width)] , label[(C - Width + 1)] , label[(C - 1)] , label[C] , label[(C + 1)] , label[(C + Width - 1)] , label[(C + Width)] ,label[(C + Width + 1)] };

				for (int i = 0; i < 9; i++)
				{

					if (b[i] != 0 && b[i] != 255)
					{
						if (b[i] < temp)
							temp = b[i];

					}
				}

				if (temp != 255)
					label[C] = temp;

			}
		}
	}
	return label;

}

float oklid_Distance(float *arr1, float *arr2, int size)
{
	float temp = 0;
	for (int i = 0;i<size;i++)
	{
		temp += pow((arr1[i] - arr2[i]), 2);
	}
	return sqrt(temp);
}

void draw(BYTE *Buffer, unsigned int Width, unsigned int Height,int padding , int x1, int x2, int y1, int y2, int object)
{
	
	
	int r, g, b;
	y1 = Height - y1-1;
	y2 = Height - y2-1;
	x1 = 3 * x1;
	x2 = 3 * x2;
	y1 = 3 * y1;
	y2 = 3 * y2;
	if (object == 0) {
		r = 0;g = 0;b = 255;
	}
	else if (object == 1)
	{
		r = 255;g = 150;b = 255;
	}
	else if(object==2)
	{
		r = 0;g = 255;b = 0;
	}
	else if (object == 3)
	{
		r = 255;g = 0;b =0;
	}
	else if (object == 4)
	{
		r =255;g = 150;b = 0;
	}
	else if (object == 5)
	{
		r = 0;g = 150;b =255;
	}
	else if (object == 6)
	{
		r =0;g = 255;b =100;
	}
	
	
	
	for (int i = x1; i <= x2; i+=3)
	{
		Buffer[y1*Width+ (y1 / 3 * padding) + i] = b;	//BMP 'den gelen paddingleri goz ardý edemeyiz.
		Buffer[y1*Width+ (y1 / 3 * padding) + i+1] =g;
		Buffer[y1*Width+ (y1 / 3 * padding) + i+2] = r;
		Buffer[y2*Width+ (y2 / 3 * padding) + i] = b;
		Buffer[y2*Width+ (y2 / 3 * padding) + i + 1] = g;
		Buffer[y2*Width+ (y2 / 3 * padding) + i + 2] = r;
	}

	for (int i = y2; i <= y1; i+=3)
	{
		Buffer[i*Width+ i / 3 * padding +x1] = b;
		Buffer[i*Width+ i / 3 * padding + x1+1] =g;
		Buffer[i*Width+ i / 3 * padding + x1+2] = r;
		Buffer[i*Width+ i / 3 * padding + x2] = b;
		Buffer[i*Width+ i / 3 * padding + x2+1] = g;
		Buffer[i*Width+ i / 3 * padding + x2+2] =r;
	}

	
}

BYTE *object_recognition(BYTE *Buffer, unsigned int Width, unsigned int Height)
{
	int label_array[50];		//nesnelerin etiketlerinin tutulacagý dizi max 50 nesne icin tanýmlý
	int C;		//center
	int value = 0;
	int index = 0;

	for (int i = 1; i < Height - 1; i++)
	{
		for (int j = 1; j < Width - 1; j++)
		{
			C = (i*Width + j);

			if (Buffer[C] > 0)					//goruntude geziyoruz ve 0'dan buyuk pixel'e rastlarsak yani bu piksel bir nesnenin parcasý ise
			{
				for (int k = 0; k < 50; k++)
				{
					if (Buffer[C] == label_array[k])		// eger etiketlenmis nesne zaten dizide varsa deger 1'e setlenir
						value = 1;
				}
				if (value != 1)								//deger 1'den farklýysa yani bu etikete sahip nesne bu dizide yoksa diziye eklenir 
				{
					label_array[index] = Buffer[C];
					index++;
					label_array[index] = -1;				//nesnenin sonunu belirlemek için en son etiket degerinden sonra -1 atanýyor
				}
				value = 0;									//degeri tekrar 0'a setliyerek donguye devam ederiz
			}

		}
	}									//BU DONGULER SAYESÝNDE ETIKET DEGERLERÝNÝ BÝR DÝZÝDE TUTARIZ VE KAC NESNE ETIKETLENMIS BILEBILIRIZ

	

	//DAHA ONCEDEN HESAPLANMIS NESNELERIN MOMENTLERINI TANIMLADIK
	float momentvalue[7];
	float mercimek[7] = { 0.165087,0.00170739,0.00708642,0.000142945,2.59475e-07,-0.00802825,1.58015e-07 };
	float cekirdek[7] = { 0.234149,0.0277601,1.0764,0.237477,0.105781,-0.196802,-0.0101849 };
	float eriste[7] = { 1.00171,0.997429,22.9338,19.5061,761.183,19.5376,-11.6029 };
	float fasulye[7] = { 0.190439,0.0107762,0.0305301,0.00227828,3.78958e-05,0.00298993,2.54525e-05 };
	float mýsýr[7] = { 0.164747,0.000834593,0.0931484,0.00139435,2.86284e-06,-0.0279052,5.64801e-06 };
	float nohut[7] = { 0.166182,0.00138244,0.156274,0.0031489,0.000338772,-0.0143759,-0.000127367 };
	float para[7] = { 0.16244,0.00104876,0.0017776,1.98324e-05,-3.45616e-09,-0.00703107,9.6867e-10 };
	
	//NESNELERI RENKLERLE IFADE ETMEK ICIN GORUNTUMUZUN BMP MODUNA CEVRÝLMÝS HALÝNÝ TEMP_BUFFER'A YUKLEDK
	long new_size;
	BYTE *temp_buffer= ConvertIntensityToBMP(Buffer, Width, Height,&new_size);
	int padding = new_size / Height - 3 * Width;

	int x1, x2, y1, y2;
	int a = 0, b = Width, c = 0, d = Height;
	for (int k = 0; label_array[k] != -1; k++)	//etiketlerin tutuldugu dizinin sonuna gelene kadar dongude kaliriz
	{
		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				C = (i*Width + j);

				if (Buffer[C] == label_array[k])		//goruntumuzde gezeriz ve etiketlenmis pixelleri buluruz
				{
					if (j > a)							//nesnelerin uc sýnýrlarýný belirledik
						a = j;
					if (j < b)
						b = j;

					if (i > c)
						c = i;
					if (i < d)
						d = i;

					x1 = b;								//belirledigimiz sýnýrladý x1,x2,y1,y2 degiskenlerine atadik
					x2 = a;

					y1 = d;
					y2 = c;


				}

			}
		}
	
		//her sinirlari belirlenen nesne icin dinamik bir dizi olusturduk
		int *arrayOf_object = new int[(x2 - x1 + 1)*(y2 - y1 + 1)];
		int h = 0;
		
		//moment hesaplamak için nesneleri olusturdugumuz dizi içine atttýk
		for (int i = y1; i <= y2; i++)
			for (int  j = x1; j <= x2; j++)
			{
				arrayOf_object[h] = Buffer[i*Width+j];
				h++;
				
			}
		
		//GORUNTU ICINDEN ETIKETLENMIS NESNEYI BULDUK VE SINIRLARINI BELIRLEDIKDEN SONRA BIR DIZIYE ATTIK
		//BU DIZIYI NESNENIN MOMENTINI HESAPLAMAK ICIN OBJE'YE VERDÝK
		HuMoments obje(arrayOf_object, (x2 - x1 + 1), (y2 - y1 + 1));
		obje.calcOrgins();
		obje.calcInvariantMoments();
		float *moments = obje.getInvariantMoments();	//nesnenin hesaplanmýs 7 momentini bir diziye attýk
		
		//nesnenin moment deðeri ile diðer momenti belli olan nesnelerin momentleri oklid'e göre tek tek kýyaslanýr bir diziye atýlýr 
		momentvalue[0] = oklid_Distance(moments, mercimek, 7);
		momentvalue[1] = oklid_Distance(moments, eriste, 7);
		momentvalue[2] = oklid_Distance(moments, nohut, 7);
		momentvalue[3] = oklid_Distance(moments, fasulye, 7);
		momentvalue[4] = oklid_Distance(moments, cekirdek, 7);
		momentvalue[5] = oklid_Distance(moments, mýsýr, 7);
		momentvalue[6] = oklid_Distance(moments, para, 7);
		
		//daha önceden bilinen hangi nesneye daha yakýnsa o nesneyi yakýn olan olarak tanimlariz
		float byComparison=10000000;
		int object_identification = 0;
		for (int i = 0; i < 7; i++)
		{
			if (momentvalue[i] < byComparison)
			{
				byComparison = momentvalue[i];
				object_identification = i;		//7 nesneden hangisine daha yakinsa ona setlenir
			}
		}

		delete[] arrayOf_object;		//dinamik dizimiz yer kaplamamsi icin her nesne hesabi sonrasinda silinir.
		//sýnýrlarý bulunan nesneleri kare içine alma(renksiz intensity goruntude cizme
		for (int i = x1; i <= x2; i++)
		{
			Buffer[y1*Width + i] = 255;
			Buffer[y2*Width + i] = 255;
		}

		for (int i = y1; i <= y2; i++)
		{
			Buffer[i*Width + x1] = 255;
			Buffer[i*Width + x2] = 255;
		}
		a = 0, b = Width, c = 0, d = Height;		//her nesne icin hesap yapabilmek icin sinirlari eski haline setleriz

		draw(temp_buffer, Width, Height,padding, x1, x2, y1, y2, object_identification);		//cizme fonksyonumuza sinirlari ve nesneyi vererek ona ozel renkle cevreleriz
		
	}
	
	LPCTSTR output;
	output = L"C://Users//Hakan58//Desktop//copy6.bmp";				//BMP goruntumuzu kaydederiz
	SaveBMP(temp_buffer, Width, Height, new_size, output);

	return temp_buffer;


}

BYTE *canny_Andhough(BYTE *Buffer, unsigned int Width, unsigned int Height)
{
	BYTE *Raw_Intensity;
	Raw_Intensity = ConvertBMPToIntensity(Buffer, Width, Height);

	int C,index=0;	//Center
	int result;
	int *vertical_derivative = new int[(Width - 2)*(Height - 2)];
	for (int i = 1; i < Height-1; i++)
	{
		for (int j = 1; j < Width-1; j++)
		{
			C = (i*Width + j);
			// 1 2 1 - 0 0 0 - -1 -2 -1  maskesini gezidiriyoruz	//kenar yönü yatayda
			result = (1* Raw_Intensity[(C - Width - 1)] + 2* Raw_Intensity[(C - Width)] + 1* Raw_Intensity[(C - Width + 1)] + 0* Raw_Intensity[(C - 1)] + 0* Raw_Intensity[C] + 0* Raw_Intensity[(C + 1)] + (-1)*Raw_Intensity[(C + Width - 1)] + (-2)*Raw_Intensity[(C + Width)] + (-1)*Raw_Intensity[(C + Width + 1)]);
			vertical_derivative[index] = result;
			index++;
		}
	}
	
	index = 0;
	int *horizontal_derivative = new int[(Width - 2)*(Height - 2)];
	for (int i = 1; i < Height - 1; i++)
	{
		for (int j = 1; j < Width - 1; j++)
		{
			C = (i*Width + j);
			// 1 0 -1 -  2 0 -2 -  1 0 -1  maskesini gezidiriyoruz		//kenar yönü dikeyde
			result = (1 * Raw_Intensity[(C - Width - 1)] + 0 * Raw_Intensity[(C - Width)] + (-1) *Raw_Intensity[(C - Width + 1)] + 2* Raw_Intensity[(C - 1)] + 0 * Raw_Intensity[C] + (-2) * Raw_Intensity[(C + 1)] + 1* Raw_Intensity[(C + Width - 1)] + 0* Raw_Intensity[(C + Width)] + (-1)*Raw_Intensity[(C + Width + 1)]);
			horizontal_derivative[index] = result;
			index++;
		}
	}

	BYTE *deneme6 = new BYTE[(Width - 2) * (Height - 2)];
	for (int i = 0; i < Height - 2; i++)
	{
		for (int j = 0; j < Width - 2; j++)
		{
			C = i * (Width - 2) + j;
			deneme6[C] = round(horizontal_derivative[C] / 4);
		}
	}
	long new_size6;
	BYTE *temp_buffer6 = ConvertIntensityToBMP(deneme6, Width - 2, Height - 2, &new_size6);
	LPCTSTR output6;
	output6 = L"C://Users//Hakan58//Desktop//kenar_fotolar//ytürev.bmp";				//BMP goruntumuzu kaydederiz
	SaveBMP(temp_buffer6, Width - 2, Height - 2, new_size6, output6);


	BYTE *deneme5 = new BYTE[(Width - 2) * (Height - 2)];
	for (int i = 0; i < Height - 2; i++)
	{
		for (int j = 0; j < Width - 2; j++)
		{
			C = i * (Width - 2) + j;
			deneme5[C] = round(vertical_derivative[C] / 4);
		}
	}
	long new_size5;
	BYTE *temp_buffer5 = ConvertIntensityToBMP(deneme5, Width - 2, Height - 2, &new_size5);
	LPCTSTR output5;
	output5 = L"C://Users//Hakan58//Desktop//kenar_fotolar//dtürev.bmp";				//BMP goruntumuzu kaydederiz
	SaveBMP(temp_buffer5, Width - 2, Height - 2, new_size5, output5);

	int *edge_image = new int[(Width - 2)*(Height * 2)];
	int *edge_image1 = new int[(Width - 2)*(Height * 2)];
	int sonuc2;
	for (int i = 0; i < Height - 2; i++)
	{
		for (int j = 0; j < Width -2; j++)
		{
			C = (i*(Width-2) + j);
			// 2 maske sonucunu topluyoruz  not: 255den daha buyuk degeler olabýlýr max 1020 olabýlýr
			result = abs(vertical_derivative[C]) + abs(horizontal_derivative[C]);
			sonuc2 = vertical_derivative[C] + horizontal_derivative[C];
			edge_image[C] = result;
			edge_image1[C] = sonuc2;
		}
	}
	
	BYTE *deneme4 = new BYTE[(Width - 2) * (Height - 2)];
	for (int i = 0; i < Height - 2; i++)
	{
		for (int j = 0; j < Width - 2; j++)
		{
			C = i * (Width - 2) + j;
			deneme4[C] = round(edge_image1[C] / 4);
		}
	}
	long new_size4;
	BYTE *temp_buffer4 = ConvertIntensityToBMP(deneme4, Width - 2, Height - 2, &new_size4);
	LPCTSTR output4;
	output4 = L"C://Users//Hakan58//Desktop//kenar_fotolar//edge.bmp";				//BMP goruntumuzu kaydederiz
	SaveBMP(temp_buffer4, Width - 2, Height - 2, new_size4, output4);
	
	
	float Q; //Angle
	
	for (int i = 0; i < Height - 2; i++)
	{
		for (int j = 0; j < Width - 2; j++)
		{
			C = (i*(Width-2) + j);
			
			Q = atan2(vertical_derivative[C] , horizontal_derivative[C])*180/PI;		//q=tan^-1((dI/dy)/(dI/dx))		gradient drection
	
			// buldugumuz aciya gore buyukluk kýyaslamasý yapacagýz ve eger buyukse aynen kalacak degilse 0 atanacak boylece non-maximum suppresion matrisimizi elde edecegiz.
			if ((0 <= Q && Q < 22.5) || (337.5 <= Q && Q<= 360) || (157.5 <= Q && Q < 202.5) || (0 > Q && Q> -22.5) || (-157.5 >= Q && Q > -202.5) || (-337.5 >= Q && Q >= -360))	// acý bu araliktaysa yatayda
			{
				if (j == 0)
				{
					if (edge_image[C] > edge_image[C + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else if (j == (Width - 2)-1)
				{
					if (edge_image[C] > edge_image[C - 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else
				{
					if (edge_image[C] > edge_image[C - 1] && edge_image[C] > edge_image[C + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				
			}

			else if ((22.5 <= Q && Q < 67.5) || (202.5 <= Q && Q < 247.5) || (-112.5 >= Q && Q > -157.5) || (-292.5 >= Q && Q > -337.5))		// acý bu araliktaysa caprazda(45 derece)
			{

				if ((j == 0 && i == 0) || (i == (Height - 2-1) && j == (Width - 2)-1))
				{
					edge_image[C] = edge_image[C];
				}
					
				else if ((i == 0) || (j == (Width - 2)-1))
				{
					if (edge_image[C] > edge_image[C + (Width - 2) - 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

				else if ((j == 0) || (i == (Height - 2)-1))
				{
					if (edge_image[C] > edge_image[(C - (Width - 2) + 1)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else
				{
					if (edge_image[C] > edge_image[(C - (Width - 2) + 1)] && edge_image[C] > edge_image[C + (Width - 2) - 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				
			}

			else if ((67.5 <= Q && Q < 112.5) || (247.5 <= Q && Q < 292.5) || (-67.5 >= Q && Q> -112.5) || (-247.5 >= Q && Q> -292.5))	// acý bu aralikta ise dikeyde (90 derece)
			{
				if (i == 0)
				{
					if (edge_image[C] > edge_image[C + (Width - 2)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else if (i == (Height - 2-1))
				{
					if (edge_image[C] > edge_image[(C - (Width - 2))])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else
				{
					if (edge_image[C] > edge_image[C - (Width - 2)] && edge_image[C] > edge_image[C + (Width - 2)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
			}

			else if ((112.5 <=Q && Q < 157.5) || (292.5 <= Q && Q < 337.5) || (-22.5 >= Q && Q > -67.5) || (-202.5 >= Q && Q > -247.5))	// acý bu aralikta ise caprazda (135 derece)
			{
				if ((j == 0 && i == (Height - 2)-1) || (i == 0 && j == (Width - 2)-1))
				{
					edge_image[C] = edge_image[C];
				}

				else if (j == 0 || i == 0)
				{
					if (edge_image[C] > edge_image[C + (Width - 2) + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

				else if (i == (Height - 2-1) || j == (Width - 2-1))
				{
					if (edge_image[C] > edge_image[(C - (Width - 2) - 1)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

				else
				{
					if (edge_image[C] > edge_image[(C - (Width - 2) - 1)] && edge_image[C] > edge_image[C + (Width - 2) + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				
			}
			
		}
	}

	BYTE *deneme3 = new BYTE[(Width - 2) * (Height - 2)];
	for (int i = 0; i < Height - 2; i++)
	{
		for (int j = 0; j < Width - 2; j++)
		{
			C = i * (Width - 2) + j;
			deneme3[C] = round(edge_image[C]/4);

		}
	}
	long new_size3;
	BYTE *temp_buffer3 = ConvertIntensityToBMP(deneme3, Width - 2, Height - 2, &new_size3);
	LPCTSTR output3;
	output3 = L"C://Users//Hakan58//Desktop//kenar_fotolar//non-maximum-suppresion.bmp";				//BMP goruntumuzu kaydederiz
	SaveBMP(temp_buffer3, Width - 2, Height - 2, new_size3, output3);

	
	// NONMAXÝMUM SUPPRESÝON MATRÝSÝMÝZÝDE BULDUKDAON SONRA HERHANGÝ BÝR TLOW VE THÝGH DEGERÝ SECÝP BINARY GORUNTU ELDE EDICEZ
	// BAZI SAYILAR BINARY OLMAZSA BU SEFER KENAR YONUNDE KOMSULUKLARA BAKICAZ

	//Tlow degerini 20 Thigh degerini 2000 olarak belirledik ve bu sinira gore binary goruntumuzu olusturduk
	int *nonmaximum_suppression = new int[(Width - 2)*(Height - 2)];
	nonmaximum_suppression = edge_image;

	//goruntudeki en buyuk degeri bulduk
	int max_pixel = 0;
	for (int i = 0; i < Height - 2; i++)
		for (int j = 0; j < Width - 2; j++)
		{
			C = i * (Width - 2) + j;
			if (nonmaximum_suppression[C] > max_pixel)
				max_pixel = nonmaximum_suppression[C];
			
		}
	
	//Tlow ve Thýgh degerlerýmý belirledik
	for (int i = 0; i < Height - 2; i++)
	{
		for (int j = 0; j < Width - 2; j++)
		{
			C = i * (Width - 2) + j;
			if (nonmaximum_suppression[C] <= 40)
			{
				nonmaximum_suppression[C] = 0;
			}
			else if (nonmaximum_suppression[C] >=max_pixel-50)
			{
				nonmaximum_suppression[C] = 1;
			}
			
		}
	}


	BYTE *deneme1 = new BYTE[(Width - 2) * (Height - 2)];
	for (int i = 0; i < Height - 2; i++)
	{
		for (int j = 0; j < Width - 2; j++)
		{
			C = i * (Width - 2) + j;
			deneme1[C] = round(nonmaximum_suppression[C] / 4);

		}
	}
	long new_size1;
	BYTE *temp_buffer1 = ConvertIntensityToBMP(deneme1, Width - 2, Height - 2, &new_size1);
	LPCTSTR output1;
	output1 = L"C://Users//Hakan58//Desktop//kenar_fotolar//hysteric_threshold.bmp";				//BMP goruntumuzu kaydederiz
	SaveBMP(temp_buffer1, Width - 2, Height - 2, new_size1, output1);

	//Tlow ve Thigh arasýndaki degeler icin kenar dogrultusu yonunde komsuluklara bakarak asýl binary goruntumuzu elde edicez


	for (int i = 0; i < Height - 2; i++)
	{
		for (int j = 0; j < Width - 2; j++)
		{
			C = i * (Width - 2) + j;
			if (nonmaximum_suppression[C] != 0 && nonmaximum_suppression[C] != 1)
			{
				
				 Q = atan2(vertical_derivative[C], horizontal_derivative[C]) * 180 / PI;		//q=tan^-1((dI/dy)/(dI/dx))		gradient drection
				

				//buldugumuz acýnýn bu sefer tersine gore yani kenar dogrultusu boyunca komsularýna bakicaz eger 1 ise 1, 0 ise 0 vericez ikisi varsa 0'ý tercih edecegiz.
				if ((0 <= Q && Q < 22.5) || (337.5 <= Q && Q <= 360) || (157.5 <= Q && Q < 202.5) || (0 > Q && Q > -22.5) || (-157.5 >= Q && Q > -202.5) || (-337.5 >= Q && Q >= -360))	// acý bu araliktaysa dikeyde kenar dogrultusunda komsuluklarýna bakýlýr
				{
					if (i == 0)
					{
						if (nonmaximum_suppression[C + (Width - 2)] == 1)
							nonmaximum_suppression[C] =1;
						else if(nonmaximum_suppression[C + (Width - 2)]==0)
							nonmaximum_suppression[C] = 0;


					}
					else if (i == (Height - 2) - 1)
					{
						if (nonmaximum_suppression[(C - (Width - 2))] == 1)
							nonmaximum_suppression[C] =1;
						else if(nonmaximum_suppression[(C - (Width - 2))] == 0)
							nonmaximum_suppression[C] = 0;
					}
					else
					{
						if ((nonmaximum_suppression[(C - (Width - 2))] == 1 && nonmaximum_suppression[C + (Width - 2)] == 1))
							nonmaximum_suppression[C] = 1;

						else if((nonmaximum_suppression[(C - (Width - 2))] == 0 && nonmaximum_suppression[C + (Width - 2)] == 0) || (nonmaximum_suppression[(C - (Width - 2))] == 0 && nonmaximum_suppression[C + (Width - 2)] == 1) || (nonmaximum_suppression[(C - (Width - 2))] == 1 && nonmaximum_suppression[C + (Width - 2)] == 0))
							nonmaximum_suppression[C] = 0;
					}

				}

				else if ((22.5 <= Q && Q < 67.5) || (202.5 <= Q && Q < 247.5) || (-112.5 >= Q && Q > -157.5) || (-292.5 >= Q && Q > -337.5))		// acý bu araliktaysa caprazda(135 derece) kenar dogrultusunda komsuluklara bakýlýr
				{

					if ((j == 0 && i == (Height - 2) - 1) || (i == 0 && j == (Width - 2) - 1))
					{
						nonmaximum_suppression[C] = 0;
					}

					else if (j == 0 || i == 0)
					{
						if (nonmaximum_suppression[C + (Width - 2) + 1] == 1)
							nonmaximum_suppression[C] = 1;
						else if(nonmaximum_suppression[C + (Width - 2) + 1] == 0)
							nonmaximum_suppression[C] = 0;
					}

					else if (i == (Height - 2 - 1) || j == (Width - 2 - 1))
					{
						if (nonmaximum_suppression[C - (Width - 2) - 1] == 1)
							nonmaximum_suppression[C] = 1;
						else if(nonmaximum_suppression[C - (Width - 2) - 1] == 0)
							nonmaximum_suppression[C] = 0;
					}

					else
					{
						if ((nonmaximum_suppression[(C - (Width - 2) - 1)] == 1 && nonmaximum_suppression[C + (Width - 2) + 1] == 1))
							nonmaximum_suppression[C] = 1;

						else if((nonmaximum_suppression[(C - (Width - 2) - 1)] == 0 && nonmaximum_suppression[C + (Width - 2) + 1] == 0) || (nonmaximum_suppression[(C - (Width - 2) - 1)] == 0 && nonmaximum_suppression[C + (Width - 2) + 1] == 1) || (nonmaximum_suppression[(C - (Width - 2) - 1)] == 1 && nonmaximum_suppression[C + (Width - 2) + 1] == 0))
							nonmaximum_suppression[C] = 0;
					}

				}

				else if ((67.5 <= Q && Q < 112.5) || (247.5 <= Q && Q < 292.5) || (-67.5 >= Q && Q > -112.5) || (-247.5 >= Q && Q > -292.5))	// acý bu aralikta ise yatayda kenar dogrultusunda komsulklara bakýlýr (0 derece)
				{

					if (j == 0)
					{
						if (nonmaximum_suppression[C + 1] == 1)
							nonmaximum_suppression[C] =1;
						else if(nonmaximum_suppression[C + 1] == 0)
							nonmaximum_suppression[C] = 0;
					}
					else if (j == (Width - 2) - 1)
					{
						if (nonmaximum_suppression[C - 1] == 1)
							nonmaximum_suppression[C] = nonmaximum_suppression[C - 1];
						else if (nonmaximum_suppression[C - 1] == 0)
							nonmaximum_suppression[C] = 0;
					}
					else
					{
						if ((nonmaximum_suppression[C - 1] == 1 && nonmaximum_suppression[C + 1] == 1))
							nonmaximum_suppression[C] = 1;

						else if((nonmaximum_suppression[C - 1] == 0 && nonmaximum_suppression[C + 1] == 0) || (nonmaximum_suppression[C - 1] == 0 && nonmaximum_suppression[C + 1] == 1) || (nonmaximum_suppression[C - 1] == 1 && nonmaximum_suppression[C + 1] == 0))
							nonmaximum_suppression[C] = 0;
					}

				}

				else if ((112.5 <= Q && Q < 157.5) || (292.5 <= Q && Q < 337.5) || (-22.5 >= Q && Q > -67.5) || (-202.5 >= Q && Q > -247.5))	// acý bu aralikta ise caprazda (45 derece) kenar dogrultusunda komsuluklara bakýlýr
				{

					if ((j == 0 && i == 0) || (i == (Height - 2 - 1) && j == (Width - 2 - 1)))
					{
						nonmaximum_suppression[C] = 0;
					}

					else if ((i == 0) || (j == (Width - 2) - 1))
					{
						if (nonmaximum_suppression[C + (Width - 2) - 1] == 1)
							nonmaximum_suppression[C] = 1;
						else if (nonmaximum_suppression[C + (Width - 2) - 1] == 0)
							nonmaximum_suppression[C] == 0;

					}

					else if ((j == 0) || (i == (Height - 2) - 1))
					{
						if (nonmaximum_suppression[C + (Width - 2) + 1] == 1)
							nonmaximum_suppression[C] =1;
						else if(nonmaximum_suppression[C + (Width - 2) + 1] == 0)
							nonmaximum_suppression[C] == 0;
					}
					else
					{
						if ((nonmaximum_suppression[(C - (Width - 2) + 1)] == 1 && nonmaximum_suppression[C + (Width - 2) - 1] == 1))
							nonmaximum_suppression[C] = 1;

						else if((nonmaximum_suppression[(C - (Width - 2) + 1)] == 0 && nonmaximum_suppression[C + (Width - 2) - 1] == 0) || (nonmaximum_suppression[(C - (Width - 2) + 1)] == 0 && nonmaximum_suppression[C + (Width - 2) - 1] == 1) || (nonmaximum_suppression[(C - (Width - 2) + 1)] == 1 && nonmaximum_suppression[C + (Width - 2) - 1] == 0))
							nonmaximum_suppression[C] = 0;
					}
				}
			}

			/*else
				binary_image[M] = nonmaximum_suppression[M];*/
		}
	}
	

	int *binary_edge_image = new int[(Width - 2)*(Height - 2)];
	for (int i = 0; i < Height - 2; i++)
	{
		for (int j = 0; j < Width - 2; j++)
		{
			C = i * (Width - 2) + j;
			if (nonmaximum_suppression[C] == 0)
			{
				binary_edge_image[C] = 0;
			}
			/*else if (nonmaximum_suppression[M] == 1)
			{
				binary_edge_image[M] = 1;
			}*/
			else
			{
				binary_edge_image[C] = 1;
			}
		}
	}

	BYTE *deneme = new BYTE[(Width - 2) * (Height - 2)];
	for (int i = 0; i < Height - 2; i++)
	{
		for (int j = 0; j < Width - 2; j++)
		{
			C = i * (Width - 2) + j;
			if (binary_edge_image[C] == 0)
			{
				deneme[C] = 0;
			}
			else
			{
				deneme[C] = 255;
			}
			
		}
	}
	long new_size;
	BYTE *temp_buffer = ConvertIntensityToBMP(deneme, Width-2, Height-2, &new_size);
	LPCTSTR output;
	output = L"C://Users//Hakan58//Desktop//kenar_fotolar//binary.bmp";				//BMP goruntumuzu kaydederiz
	SaveBMP(temp_buffer, Width-2, Height-2, new_size, output);

	int d_limit = (Height*4);
	int *hough_transform = new int[d_limit* 360];
	
	for(int i=0; i<d_limit; i++)
		for (int j = 0; j < 360; j++)
		{
			C = i *360 + j;
			hough_transform[C] = 0;
		}

		int d;
		int M2;
		int r,c;

		for (int i = 0; i < Height - 2; i++)
		{
			for (int j = 0; j < Width - 2; j++)
			{
				C = i * (Width - 2) + j;
				if (binary_edge_image[C] == 1)
				{					
					Q = round(atan2 (vertical_derivative[C],horizontal_derivative[C]) * 180 / PI);	//kenar yonunu bulduk

					//negatif yonle gelen acýlarda aslýnda ayni yonu verdigi icin pozitife tamamladik
					if (Q < 0)
						Q = 360 + Q;

					if (Q == 270)
						Q = 90;
					if (Q == 180|| Q==360)
						Q = 0;

					r = i + 1;
					c = j + 1;
					d = abs(r * round(sin(Q)) + c * round(cos(Q)));
					
					M2 = d * 360 + Q;
					hough_transform[M2] =hough_transform[M2]+1;
				}
				
					
			}
		}

		BYTE *deneme7 = new BYTE[4*Height * 360];
		for (int i = 0; i < Height*4; i++)
		{
			for (int j = 0; j < 360; j++)
			{
				C = i * 360 + j;
				
					if(hough_transform[C]>10)
						deneme7[C] =255;
					else
					{
						deneme7[C] = 0;
					}
				
			}
		}
		long new_size7;
		BYTE *temp_buffer7 = ConvertIntensityToBMP(deneme7,360, Height*4, &new_size7);
		LPCTSTR output7;
		output7 = L"C://Users//Hakan58//Desktop//kenar_fotolar//hough_transform.bmp";				//BMP goruntumuzu kaydederiz
		SaveBMP(temp_buffer7,360, Height*4, new_size7, output7);


		//kenar ve yonlerýnýn tutuldugu diziler
		int kenar_indisleri[500];
		int acý_indisleri[500];
		int k = 0;
		for (int r = 0; r < d_limit; r++)
		{
			for (int c = 0; c < 360; c++)
			{
				C = r * 360 + c;
				if(hough_transform[C] >20)
				{
					kenar_indisleri[k] = r;
					acý_indisleri[k] = c;
					k++;
				}
			}
		}
		kenar_indisleri[k] = -1;

		/*for (int i = 0; kenar_indisleri[i] != -1; i++)
		for (int j = 0; kenar_indisleri[j] != -1;j++)
		{
			if (kenar_indisleri[j] < kenar_indisleri[j + 1])
			{
				int gecici = kenar_indisleri[j + 1];
				kenar_indisleri[j + 1] = kenar_indisleri[j];
				kenar_indisleri[j] =gecici;
				int gecici2 = acý_indisleri[j + 1];
				acý_indisleri[j + 1] = acý_indisleri[j];
				acý_indisleri[j] = gecici2;
			}

		}*/



		int padding = 0;
		int scanlinebytes = Width * 3;
		while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
			padding++;
		// get the padded scanline width
		int psw = scanlinebytes + padding;
		int new_height;
		long newpos;
		int red=0, green=0, blue=255;
		int red1 = 255, green1 = 0, blue1 = 0;
		
		//dongunun sinirlarý max kenar sayýsýný asmamalý
		for (int i =0; kenar_indisleri[i]!=-1; i++)
		{
			if (acý_indisleri[i] == 0)
			{
				for (int j = 0; j < Height; j++)
				{
					newpos = j * psw + kenar_indisleri[i]*3;
					Buffer[newpos] = red;
					Buffer[newpos + 1] = green;
					Buffer[newpos + 2] = blue;
				}
				
			}
			else if (acý_indisleri[i] == 90)
			{
				 new_height = (Height - kenar_indisleri[i] - 1);
					
				for (int column = 0; column < Width; column++)
					{
					newpos = new_height * psw + column*3;
						Buffer[newpos] = red1;
						Buffer[newpos+1] = green1;
						Buffer[newpos+2] = blue1;
					}
			}
		}
			
		
		

	/*delete[] yatayda_turev;
	delete[] dikeyde_turev;*/

	return Buffer;
}