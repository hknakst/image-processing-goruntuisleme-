#pragma once

#include "imge_bmp.h"
#include <atlstr.h>
#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <math.h>
#include "targetver.h"



namespace ödev1zoom {



	
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm için özet
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:

		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Oluþturucu kodunu buraya ekle
			//
		}

	protected:
		/// <summary>
		///Kullanýlan tüm kaynaklarý temizleyin.
		LPCTSTR input;
		BYTE *Buffer, *Raw_Intensity;
		unsigned int Width, Height;
		long Size, new_size;
			unsigned int zoom_height1;
			unsigned int zoom_height2;
			unsigned int zoom_width1;
			unsigned int zoom_width2;


	private: System::Windows::Forms::TextBox^  textBox1;
	protected:
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::ToolStripMenuItem^  uygulaToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  histogramToolStripMenuItem;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;

	private: System::Windows::Forms::ToolStripMenuItem^  histogramEqualizedToolStripMenuItem;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart2;
	private: System::Windows::Forms::ToolStripMenuItem^  kmeansToolStripMenuItem;
	private: System::Windows::Forms::PictureBox^  pictureBox4;
	private: System::Windows::Forms::ToolStripMenuItem^  morfologyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  dilationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  erosionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  erosiondilationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  etiketleToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  nesneyakalaToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  kenarcýkarToolStripMenuItem;




	private: System::Windows::Forms::TextBox^  textBox4;
			 /// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::PictureBox^  pictureBox1;

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  dosyaToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  yükleToolStripMenuItem;




	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::PictureBox^  pictureBox3;

	protected:

	private:
		

		/// <summary>
		///Gerekli tasarýmcý deðiþkeni.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Tasarýmcý desteði için gerekli metot - bu metodun 
		///içeriðini kod düzenleyici ile deðiþtirmeyin.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea2 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->dosyaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->yükleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->uygulaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->histogramToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->histogramEqualizedToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->kmeansToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->morfologyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->dilationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->erosionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->erosiondilationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->etiketleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->nesneyakalaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->chart2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->kenarcýkarToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->BeginInit();
			this->SuspendLayout();
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(28, 38);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(360, 360);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::pictureBox1_MouseDown);
			// 
			// menuStrip1
			// 
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->dosyaToolStripMenuItem,
					this->uygulaToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1698, 28);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// dosyaToolStripMenuItem
			// 
			this->dosyaToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->yükleToolStripMenuItem });
			this->dosyaToolStripMenuItem->Name = L"dosyaToolStripMenuItem";
			this->dosyaToolStripMenuItem->Size = System::Drawing::Size(60, 24);
			this->dosyaToolStripMenuItem->Text = L"dosya";
			// 
			// yükleToolStripMenuItem
			// 
			this->yükleToolStripMenuItem->Name = L"yükleToolStripMenuItem";
			this->yükleToolStripMenuItem->Size = System::Drawing::Size(118, 26);
			this->yükleToolStripMenuItem->Text = L"yükle";
			this->yükleToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::yükleToolStripMenuItem_Click);
			// 
			// uygulaToolStripMenuItem
			// 
			this->uygulaToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {
				this->histogramToolStripMenuItem,
					this->histogramEqualizedToolStripMenuItem, this->kmeansToolStripMenuItem, this->morfologyToolStripMenuItem, this->etiketleToolStripMenuItem,
					this->nesneyakalaToolStripMenuItem, this->kenarcýkarToolStripMenuItem
			});
			this->uygulaToolStripMenuItem->Name = L"uygulaToolStripMenuItem";
			this->uygulaToolStripMenuItem->Size = System::Drawing::Size(65, 24);
			this->uygulaToolStripMenuItem->Text = L"uygula";
			// 
			// histogramToolStripMenuItem
			// 
			this->histogramToolStripMenuItem->Name = L"histogramToolStripMenuItem";
			this->histogramToolStripMenuItem->Size = System::Drawing::Size(220, 26);
			this->histogramToolStripMenuItem->Text = L"histogram";
			this->histogramToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::histogramToolStripMenuItem_Click);
			// 
			// histogramEqualizedToolStripMenuItem
			// 
			this->histogramEqualizedToolStripMenuItem->Name = L"histogramEqualizedToolStripMenuItem";
			this->histogramEqualizedToolStripMenuItem->Size = System::Drawing::Size(220, 26);
			this->histogramEqualizedToolStripMenuItem->Text = L"histogram Equalized";
			this->histogramEqualizedToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::histogramEqualizedToolStripMenuItem_Click);
			// 
			// kmeansToolStripMenuItem
			// 
			this->kmeansToolStripMenuItem->Name = L"kmeansToolStripMenuItem";
			this->kmeansToolStripMenuItem->Size = System::Drawing::Size(220, 26);
			this->kmeansToolStripMenuItem->Text = L"k_means";
			this->kmeansToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::kmeansToolStripMenuItem_Click);
			// 
			// morfologyToolStripMenuItem
			// 
			this->morfologyToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->dilationToolStripMenuItem,
					this->erosionToolStripMenuItem, this->erosiondilationToolStripMenuItem
			});
			this->morfologyToolStripMenuItem->Name = L"morfologyToolStripMenuItem";
			this->morfologyToolStripMenuItem->Size = System::Drawing::Size(220, 26);
			this->morfologyToolStripMenuItem->Text = L"morfology";
			// 
			// dilationToolStripMenuItem
			// 
			this->dilationToolStripMenuItem->Name = L"dilationToolStripMenuItem";
			this->dilationToolStripMenuItem->Size = System::Drawing::Size(194, 26);
			this->dilationToolStripMenuItem->Text = L"dilation";
			this->dilationToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::dilationToolStripMenuItem_Click_1);
			// 
			// erosionToolStripMenuItem
			// 
			this->erosionToolStripMenuItem->Name = L"erosionToolStripMenuItem";
			this->erosionToolStripMenuItem->Size = System::Drawing::Size(194, 26);
			this->erosionToolStripMenuItem->Text = L"erosion";
			this->erosionToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::erosionToolStripMenuItem_Click);
			// 
			// erosiondilationToolStripMenuItem
			// 
			this->erosiondilationToolStripMenuItem->Name = L"erosiondilationToolStripMenuItem";
			this->erosiondilationToolStripMenuItem->Size = System::Drawing::Size(194, 26);
			this->erosiondilationToolStripMenuItem->Text = L"erosion+dilation";
			this->erosiondilationToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::erosiondilationToolStripMenuItem_Click);
			// 
			// etiketleToolStripMenuItem
			// 
			this->etiketleToolStripMenuItem->Name = L"etiketleToolStripMenuItem";
			this->etiketleToolStripMenuItem->Size = System::Drawing::Size(220, 26);
			this->etiketleToolStripMenuItem->Text = L"etiketle";
			this->etiketleToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::etiketleToolStripMenuItem_Click);
			// 
			// nesneyakalaToolStripMenuItem
			// 
			this->nesneyakalaToolStripMenuItem->Name = L"nesneyakalaToolStripMenuItem";
			this->nesneyakalaToolStripMenuItem->Size = System::Drawing::Size(220, 26);
			this->nesneyakalaToolStripMenuItem->Text = L"nesne_yakala";
			this->nesneyakalaToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::nesneyakalaToolStripMenuItem_Click);
			// 
			// pictureBox2
			// 
			this->pictureBox2->Location = System::Drawing::Point(403, 38);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(360, 360);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox2->TabIndex = 7;
			this->pictureBox2->TabStop = false;
			// 
			// pictureBox3
			// 
			this->pictureBox3->Location = System::Drawing::Point(435, 580);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(30, 10);
			this->pictureBox3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox3->TabIndex = 8;
			this->pictureBox3->TabStop = false;
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(105, 438);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 22);
			this->textBox1->TabIndex = 9;
			this->textBox1->Text = L"\r\n";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(250, 438);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(100, 22);
			this->textBox2->TabIndex = 10;
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(105, 484);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(100, 22);
			this->textBox3->TabIndex = 11;
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(250, 484);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(100, 22);
			this->textBox4->TabIndex = 12;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(412, 457);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 13;
			this->button1->Text = L"zoom";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// chart1
			// 
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(403, 580);
			this->chart1->Name = L"chart1";
			series1->ChartArea = L"ChartArea1";
			series1->Legend = L"Legend1";
			series1->Name = L"Histogram";
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(10, 10);
			this->chart1->TabIndex = 14;
			this->chart1->Text = L"chart1";
			// 
			// chart2
			// 
			chartArea2->Name = L"ChartArea1";
			this->chart2->ChartAreas->Add(chartArea2);
			legend2->Name = L"Legend1";
			this->chart2->Legends->Add(legend2);
			this->chart2->Location = System::Drawing::Point(419, 580);
			this->chart2->Name = L"chart2";
			series2->ChartArea = L"ChartArea1";
			series2->Legend = L"Legend1";
			series2->Name = L"Series1";
			this->chart2->Series->Add(series2);
			this->chart2->Size = System::Drawing::Size(10, 10);
			this->chart2->TabIndex = 15;
			this->chart2->Text = L"chart2";
			// 
			// pictureBox4
			// 
			this->pictureBox4->Location = System::Drawing::Point(471, 580);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(26, 10);
			this->pictureBox4->TabIndex = 16;
			this->pictureBox4->TabStop = false;
			// 
			// kenarcýkarToolStripMenuItem
			// 
			this->kenarcýkarToolStripMenuItem->Name = L"kenarcýkarToolStripMenuItem";
			this->kenarcýkarToolStripMenuItem->Size = System::Drawing::Size(220, 26);
			this->kenarcýkarToolStripMenuItem->Text = L"kenar_cýkar";
			this->kenarcýkarToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::kenarcýkarToolStripMenuItem_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1698, 591);
			this->Controls->Add(this->pictureBox4);
			this->Controls->Add(this->chart2);
			this->Controls->Add(this->chart1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->textBox4);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->pictureBox3);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void yükleToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		
		CString str;
		

		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			str = openFileDialog1->FileName;
			input = (LPCTSTR)str;


			Buffer = LoadBMP(Width, Height, Size, input);		//foto'nun RGB bilgileriyle beraber tutulduðu dizi
			Raw_Intensity = ConvertBMPToIntensity(Buffer, Width, Height);	// fotoðrafla ayný boyutda ama ilk indisde fotoðragýn sol alt piksel bilgisi var
			
			pictureBox1->Width = Width;
			pictureBox1->Height = Height;
			pictureBox2->Width = Width;
			pictureBox2->Height = Height;

			this->pictureBox2->Location = System::Drawing::Point(pictureBox1->Left + pictureBox1->Width + 10, 34); // yeni görüntüyünün konumu ayarladýk

			unsigned int row, column;
			Bitmap^ surface = gcnew Bitmap(pictureBox2->Width, pictureBox2->Height); 
			pictureBox2->Image = surface;
			Color c;

			for(row=0; row<Height; row++)
				for (column = 0; column < Width; column++)
				{
					c = Color::FromArgb(Raw_Intensity[row*Width + column], Raw_Intensity[row*Width + column], Raw_Intensity[row*Width + column]);
					surface->SetPixel(column, row, c); // x ve y korinatlarý 3. parametre ise rengi
				}
		}

		this->pictureBox1->ImageLocation = openFileDialog1->FileName;

		this->textBox1->Location = System::Drawing::Point(50, pictureBox1->Height + 40);
		this->textBox2->Location = System::Drawing::Point(150, pictureBox1->Height + 40);
		this->textBox3->Location = System::Drawing::Point(50, pictureBox1->Height + 80);
		this->textBox4->Location = System::Drawing::Point(150, pictureBox1->Height + 80);
		this->button1->Location = System::Drawing::Point(250, pictureBox1->Height + 60);

	}






private: System::Void pictureBox1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) //e pointer'ý bana týklanan yerin kordinatýný verir
{
	
}



private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

	 zoom_height1 = Convert::ToInt32(textBox1->Text);
	 zoom_height2 = Convert::ToInt32(textBox2->Text);
	 zoom_width1 = Convert::ToInt32(textBox3->Text);
	 zoom_width2 = Convert::ToInt32(textBox4->Text);

	 

	int ilk_indeks = 0;
	unsigned int k=0;
	unsigned int zoom_width = ((zoom_width2 - zoom_width1) + 1);
	unsigned int zoom_height = ((zoom_height2 - zoom_height1) + 1);
	BYTE *Zoom_buffer = new BYTE[(2*zoom_height-1)*(2*zoom_width-1)];

	pictureBox3->Width =2*zoom_width-1;
	pictureBox3->Height = 2*zoom_height-1;
	
	Zoom_buffer = zoom(Raw_Intensity, zoom_width1, zoom_width2, zoom_height1, zoom_height2, Width);
	this->pictureBox3->Location = System::Drawing::Point(pictureBox2->Left + pictureBox2->Width + 10, 34);

	unsigned int row1, column1;
	Bitmap^ surface1 = gcnew Bitmap(pictureBox3->Width, pictureBox3->Height);
	pictureBox3->Image = surface1;
	Color c1;

	for (row1 = 0; row1<(2*zoom_height-1); row1++)
		for (column1 = 0; column1 <(2 * zoom_width - 1); column1++)
		{
			c1 = Color::FromArgb(Zoom_buffer[(row1*(2 * zoom_width - 1))+column1], Zoom_buffer[(row1*(2 * zoom_width - 1))+column1], Zoom_buffer[(row1*(2 * zoom_width - 1))+column1]);
			surface1->SetPixel(column1, row1, c1);
		}


}





private: System::Void histogramToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

		int *histogram_dizi = new int[256];
		histogram_dizi = histogram(Raw_Intensity, Width, Height);		// verilen goruntunun histogramýný cýkarýyoruz.
		
		for (int i = 0; i<256; i++)
		{
			this->chart1->Series["Histogram"]->Points->AddXY(i, histogram_dizi[i]);		// ayný zamanda grafik olusturmak istersek kullanýlabilir.	
		}
		chart1->Width = 400;
		chart1->Height = 200;
		this->chart1->Location = System::Drawing::Point(pictureBox2->Left + pictureBox2->Width + 20, 40);
		

		
}

private: System::Void histogramEqualizedToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

	BYTE *histogram_equalized_buffer= new BYTE[Width*Height];
	histogram_equalized_buffer = histogram_equalized(Raw_Intensity, Width, Height);


	pictureBox3->Width = Width;
	pictureBox3->Height = Height;
	this->pictureBox3->Location = System::Drawing::Point(pictureBox2->Left + pictureBox2->Width + 10, 34);

	unsigned int row, column;
	Bitmap^ surface = gcnew Bitmap(pictureBox3->Width, pictureBox3->Height);
	pictureBox3->Image = surface;
	Color c;

	for (row = 0; row<Height; row++)
		for (column = 0; column <Width; column++)
		{
			c = Color::FromArgb(histogram_equalized_buffer[(row*Width) + column], histogram_equalized_buffer[(row*Width) + column], histogram_equalized_buffer[(row*Width) + column]);
			surface->SetPixel(column, row, c);
		}

}



private: System::Void kmeansToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	
	BYTE *k_means_buffer = new BYTE[Width*Height];
	k_means_buffer = k_means(Raw_Intensity, Width, Height);

	pictureBox4->Width = Width;
	pictureBox4->Height = Height;
	this->pictureBox4->Location = System::Drawing::Point(pictureBox2->Left + pictureBox2->Width + 10, 34);

	unsigned int row, column;
	Bitmap^ surface = gcnew Bitmap(pictureBox4->Width, pictureBox4->Height);
	pictureBox4->Image = surface;
	Color c;

	for (row = 0; row<Height; row++)
		for (column = 0; column <Width; column++)
		{
			c = Color::FromArgb(k_means_buffer[(row*Width) + column], k_means_buffer[(row*Width) + column], k_means_buffer[(row*Width) + column]);
			surface->SetPixel(column, row, c);
		}
}


private: System::Void dilationToolStripMenuItem_Click_1(System::Object^  sender, System::EventArgs^  e) {

	BYTE *k_means_buffer = new BYTE[Width*Height];
	k_means_buffer = k_means(Raw_Intensity, Width, Height);

	BYTE *dilation_buffer = new BYTE[Width*Height];
	dilation_buffer = dilation(k_means_buffer, Width, Height);

	pictureBox4->Width = Width;
	pictureBox4->Height =Height;
	this->pictureBox4->Location = System::Drawing::Point(pictureBox2->Left + pictureBox2->Width + 10, 34);

	unsigned int row, column;
	Bitmap^ surface = gcnew Bitmap(pictureBox4->Width, pictureBox4->Height);
	pictureBox4->Image = surface;
	Color c;

	for (row = 0; row<Height; row++)
		for (column = 0; column <Width; column++)
		{
			c = Color::FromArgb(dilation_buffer[(row*Width) + column], dilation_buffer[(row*Width) + column], dilation_buffer[(row*Width) + column]);
			surface->SetPixel(column, row, c);
		}
}



private: System::Void erosionToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

	BYTE *k_means_buffer = new BYTE[Width*Height];
	k_means_buffer = k_means(Raw_Intensity, Width, Height);

	
	BYTE *erosion_buffer = new BYTE[Width*Height];
	erosion_buffer = erosion(k_means_buffer, Width, Height);


	pictureBox4->Width = Width;
	pictureBox4->Height = Height;
	this->pictureBox4->Location = System::Drawing::Point(pictureBox2->Left + pictureBox2->Width + 10, 34);

	unsigned int row, column;
	Bitmap^ surface = gcnew Bitmap(pictureBox4->Width, pictureBox4->Height);
	pictureBox4->Image = surface;
	Color c;

	for (row = 0; row<Height; row++)
		for (column = 0; column <Width; column++)
		{
			c = Color::FromArgb(erosion_buffer[(row*Width) + column], erosion_buffer[(row*Width) + column], erosion_buffer[(row*Width) + column]);
			surface->SetPixel(column, row, c);
		}
}




private: System::Void erosiondilationToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

	BYTE *k_means_buffer = new BYTE[Width*Height];
	k_means_buffer = k_means(Raw_Intensity, Width, Height);

	BYTE *erosion_buffer = new BYTE[Width*Height];
	erosion_buffer = erosion(k_means_buffer, Width, Height);


	BYTE *dilation_buffer = new BYTE[Width*Height];
	dilation_buffer = dilation(erosion_buffer, Width, Height);

	pictureBox4->Width = Width;
	pictureBox4->Height = Height;
	this->pictureBox4->Location = System::Drawing::Point(pictureBox2->Left + pictureBox2->Width + 10, 34);

	unsigned int row, column;
	Bitmap^ surface = gcnew Bitmap(pictureBox4->Width, pictureBox4->Height);
	pictureBox4->Image = surface;
	Color c;

	for (row = 0; row<Height; row++)
		for (column = 0; column <Width; column++)
		{
			c = Color::FromArgb(dilation_buffer[(row*Width) + column], dilation_buffer[(row*Width) + column], dilation_buffer[(row*Width) + column]);
			surface->SetPixel(column, row, c);
		}
}






private: System::Void etiketleToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

	BYTE *k_means_buffer = new BYTE[Width*Height];
	k_means_buffer = k_means(Raw_Intensity, Width, Height);

	BYTE *erosion_buffer = new BYTE[Width*Height];
	erosion_buffer = erosion(k_means_buffer, Width, Height);

	BYTE *dilation_buffer = new BYTE[Width*Height];
	dilation_buffer = dilation(erosion_buffer, Width, Height);

	BYTE *etiket_buffer = new BYTE[Width*Height];
	etiket_buffer = label(dilation_buffer, Width, Height);

	pictureBox4->Width = Width;
	pictureBox4->Height = Height;
	this->pictureBox4->Location = System::Drawing::Point(pictureBox2->Left + pictureBox2->Width + 10, 34);

	unsigned int row, column;
	Bitmap^ surface = gcnew Bitmap(pictureBox4->Width, pictureBox4->Height);
	pictureBox4->Image = surface;
	Color c;

	for (row = 0; row<Height; row++)
		for (column = 0; column <Width; column++)
		{
			c = Color::FromArgb(etiket_buffer[(row*Width) + column], etiket_buffer[(row*Width) + column], etiket_buffer[(row*Width) + column]);
			surface->SetPixel(column, row, c);
		}
}





private: System::Void nesneyakalaToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

	BYTE *k_means_buffer = new BYTE[Width*Height];
	k_means_buffer = k_means(Raw_Intensity, Width, Height);

	BYTE *erosion_buffer = new BYTE[Width*Height];
	erosion_buffer = erosion(k_means_buffer, Width, Height);

	BYTE *dilation_buffer = new BYTE[Width*Height];
	dilation_buffer = dilation(erosion_buffer, Width, Height);

	BYTE *etiket_buffer = new BYTE[Width*Height];
	etiket_buffer = label(dilation_buffer, Width, Height);

	BYTE *nesne_yakala_buffer = new BYTE[Width*Height];
	nesne_yakala_buffer = object_recognition(etiket_buffer, Width, Height);

	pictureBox4->Width = Width;
	pictureBox4->Height = Height;
	this->pictureBox4->Location = System::Drawing::Point(pictureBox2->Left + pictureBox2->Width + 10, 34);

	

	unsigned int row, column;
	Bitmap^ surface = gcnew Bitmap(pictureBox4->Width, pictureBox4->Height);
	pictureBox4->Image = surface;
	Color c;

	int padding = 0;
	int scanlinebytes = Width * 3;
	while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;

	long newpos;
	for (row = 0; row < Height; row++)
	{
		for (column = 0; column < Width; column++)
		{
			newpos = (Height - row - 1) * psw + column * 3;
			c = Color::FromArgb(nesne_yakala_buffer[newpos], nesne_yakala_buffer[newpos + 1], nesne_yakala_buffer[newpos + 2]);
			surface->SetPixel(column, row, c);
		}
	}
}





private: System::Void kenarcýkarToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

	
	BYTE *Buffer_kenar=Buffer;
	BYTE *kenarbul_buffer = new BYTE[(Width*Height)];
	kenarbul_buffer = canny_Andhough(Buffer_kenar, Width, Height);

	pictureBox4->Width = Width;
	pictureBox4->Height = Height;
	this->pictureBox4->Location = System::Drawing::Point(pictureBox2->Left + pictureBox2->Width + 10, 34);

	unsigned int row, column;
	Bitmap^ surface = gcnew Bitmap(pictureBox4->Width, pictureBox4->Height);
	pictureBox4->Image = surface;
	Color c;

	int padding = 0;
	int scanlinebytes = Width * 3;
	while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;

	long newpos;
	for (row = 0; row < Height; row++)
	{
		for (column = 0; column < Width; column++)
		{
			newpos = (Height - row - 1) * psw + column * 3;
			c = Color::FromArgb(kenarbul_buffer[newpos], kenarbul_buffer[newpos + 1], kenarbul_buffer[newpos + 2]);
			surface->SetPixel(column, row, c);
		}
	}
	surface->Save(L"C://Users//Hakan58//Desktop//kenar_fotolar//sonuc.bmp");
	

	


}
};
}


