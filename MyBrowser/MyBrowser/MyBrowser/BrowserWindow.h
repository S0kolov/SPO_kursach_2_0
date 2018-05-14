#pragma once
#include <string>
#include <vector>


namespace MyBrowser {

	using namespace std;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
	public ref class BrowserWindow : public System::Windows::Forms::Form 
	{
	public:
		BrowserWindow()
		{
			InitializeComponent();
		}
	protected:
		~BrowserWindow()
		{
			
		}
	private: System::Windows::Forms::Button^  Eject;
	private: System::Windows::Forms::Button^  information;
	private: System::Windows::Forms::Button^  autorun;

	void MarshalString(String ^s, string& os);
	System::ComponentModel::Container ^components;
		
#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			System::Windows::Forms::ListBox^  usbList;
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(BrowserWindow::typeid));
			this->Eject = (gcnew System::Windows::Forms::Button());
			this->information = (gcnew System::Windows::Forms::Button());
			this->autorun = (gcnew System::Windows::Forms::Button());
			usbList = (gcnew System::Windows::Forms::ListBox());
			this->SuspendLayout();
			// 
			// usbList
			// 
			usbList->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			usbList->Cursor = System::Windows::Forms::Cursors::Hand;
			usbList->Font = (gcnew System::Drawing::Font(L"Times New Roman", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			usbList->FormattingEnabled = true;
			usbList->ItemHeight = 33;
			usbList->Location = System::Drawing::Point(5, 77);
			usbList->Name = L"usbList";
			usbList->ScrollAlwaysVisible = true;
			usbList->SelectionMode = System::Windows::Forms::SelectionMode::MultiSimple;
			usbList->Size = System::Drawing::Size(494, 233);
			usbList->TabIndex = 13;
			// 
			// Eject
			// 
			this->Eject->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->Eject->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->Eject->FlatAppearance->BorderColor = System::Drawing::SystemColors::ButtonShadow;
			this->Eject->FlatAppearance->BorderSize = 3;
			this->Eject->FlatAppearance->MouseDownBackColor = System::Drawing::Color::LightCyan;
			this->Eject->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Azure;
			this->Eject->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Eject->Font = (gcnew System::Drawing::Font(L"Times New Roman", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->Eject->ForeColor = System::Drawing::Color::Black;
			this->Eject->Location = System::Drawing::Point(5, 12);
			this->Eject->Name = L"Eject";
			this->Eject->Size = System::Drawing::Size(179, 42);
			this->Eject->TabIndex = 14;
			this->Eject->Text = L"Eject";
			this->Eject->UseVisualStyleBackColor = false;
			this->Eject->Click += gcnew System::EventHandler(this, &BrowserWindow::Eject_Click);
			// 
			// information
			// 
			this->information->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->information->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->information->FlatAppearance->BorderColor = System::Drawing::SystemColors::ButtonShadow;
			this->information->FlatAppearance->BorderSize = 3;
			this->information->FlatAppearance->MouseDownBackColor = System::Drawing::Color::LightCyan;
			this->information->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Azure;
			this->information->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->information->Font = (gcnew System::Drawing::Font(L"Times New Roman", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->information->ForeColor = System::Drawing::Color::Black;
			this->information->Location = System::Drawing::Point(165, 12);
			this->information->Name = L"information";
			this->information->Size = System::Drawing::Size(179, 42);
			this->information->TabIndex = 15;
			this->information->Text = L"Information";
			this->information->UseVisualStyleBackColor = false;
			this->information->Click += gcnew System::EventHandler(this, &BrowserWindow::information_Click);
			// 
			// autorun
			// 
			this->autorun->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->autorun->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->autorun->FlatAppearance->BorderColor = System::Drawing::SystemColors::ButtonShadow;
			this->autorun->FlatAppearance->BorderSize = 3;
			this->autorun->FlatAppearance->MouseDownBackColor = System::Drawing::Color::LightCyan;
			this->autorun->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Azure;
			this->autorun->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->autorun->Font = (gcnew System::Drawing::Font(L"Times New Roman", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->autorun->ForeColor = System::Drawing::Color::Black;
			this->autorun->Location = System::Drawing::Point(350, 12);
			this->autorun->Name = L"autorun";
			this->autorun->Size = System::Drawing::Size(138, 42);
			this->autorun->TabIndex = 16;
			this->autorun->Text = L"Autorun";
			this->autorun->UseVisualStyleBackColor = false;
			this->autorun->Click += gcnew System::EventHandler(this, &BrowserWindow::autorun_Click);
			// 
			// BrowserWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(14, 29);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->BackColor = System::Drawing::SystemColors::Window;
			this->ClientSize = System::Drawing::Size(512, 344);
			this->Controls->Add(this->autorun);
			this->Controls->Add(this->information);
			this->Controls->Add(this->Eject);
			this->Controls->Add(usbList);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->ForeColor = System::Drawing::SystemColors::InactiveCaption;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->MaximizeBox = false;
			this->Name = L"BrowserWindow";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Usb-help";
			this->Load += gcnew System::EventHandler(this, &BrowserWindow::BrowserWindow_Load);
			this->ResumeLayout(false);

		}
	#pragma endregion
		
		private: System::Void BrowserWindow_Load(System::Object^  sender, System::EventArgs^  e)
		{
			/*
			то, что нужно сделать при загрузке
			*/

		}
	
private: System::Void Eject_Click(System::Object^  sender, System::EventArgs^  e) {
	/*
		действие при двойном клике на извлечение, пример :
		!!! _usbList- контейнер девайстов
		!!! usbList - сам лист в форме
		if (this->usbList->SelectedIndex == -1) 
		MessageBox::Show("To delete select something", "Help for user", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
		while (this->usbList->SelectedIndex > -1)
		{
		string temp = "temp";
		this->MarshalStringFavorite(this->usbList->SelectedItem->ToString(), temp);
		this->_usbList->deleteUnit(temp); 
		this->usbList->Items->RemoveAt(this->usbList->SelectedIndex);
		}
		}
	*/
}
private: System::Void information_Click(System::Object^  sender, System::EventArgs^  e)
{
	/*
	действие при двойном клике и выборе информации, пример :
		!!! _usbList- контейнер девайстов
		!!! usbList - сам лист в форме
		if (this->usbList->SelectedIndex == -1) 
		MessageBox::Show("To delete select something", "Help for user", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
		while (this->usbList->SelectedIndex > -1)
		{
		this->_usbList[this->usbList->SelectedIndex]; - нужный элемент
		}
		}
	*/
}
private: System::Void autorun_Click(System::Object^  sender, System::EventArgs^  e) 
{
	/*
	аналогично выше
	*/
}
};
}