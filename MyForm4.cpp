#include "stdafx.h"
#include "MyForm4.h"
#include <iostream>
#include <windows.h>
#include <math.h>

//User defined includes
#include "imge_bmp.h"


using namespace ödev1zoom;
using namespace System;
using namespace System::Windows::Forms;

[STAThread]



void Main(array<String^>^ args) {

	std::cout << "adadsa";
	Application::EnableVisualStyles;
	Application::SetCompatibleTextRenderingDefault(false);
	ödev1zoom::MyForm form;
	Application::Run(%form);
	
	
}

