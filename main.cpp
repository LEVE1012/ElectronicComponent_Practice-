#include "Main.h"

Main::Main() : wxFrame(nullptr, wxID_ANY, "Demo", wxPoint(30, 30), wxSize(500, 400))
{

    CreateMenu(); // �����˵�
    CreateLayout(); // ��������
}

Main::~Main()
{

}

void Main::CreateMenu() {
    // �����˵���
    wxMenuBar* menuBar = new wxMenuBar();

    // �����ļ��˵�
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(wxID_NEW, "�½�\tCtrl-N", "�������ļ�");
    fileMenu->Append(wxID_OPEN, "��\tCtrl-O", "���ļ�");
    fileMenu->Append(wxID_SAVE, "����\tCtrl-S", "�����ļ�");
    fileMenu->Append(wxID_SAVEAS, "����Ϊ\tCtrl-Shift-S", "���Ϊ�ļ�");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "�˳�\tCtrl-Q", "�˳�Ӧ��");
    menuBar->Append(fileMenu, "�ļ�");

    // �����༭�˵�
    wxMenu* editMenu = new wxMenu();
    editMenu->Append(wxID_ANY, "����\tCtrl-C", "����ѡ������");
    editMenu->Append(wxID_ANY, "����\tCtrl-X", "����ѡ������");
    editMenu->Append(wxID_ANY, "ճ��\tCtrl-V", "�Ӽ�����ճ��");
    menuBar->Append(editMenu, "&�༭");

    // �������߲˵�
    wxMenu* toolMenu = new wxMenu();
    toolMenu->Append(wxID_ANY, "ԭ��ͼ�༭��", "���й��� 1");
    toolMenu->Append(wxID_ANY, "���ű༭��", "���й��� 2");
    toolMenu->Append(wxID_ANY, "PCB�༭��", "���й��� 2");
    menuBar->Append(toolMenu, "&����");

    // �������ò˵�
    wxMenu* settingsMenu = new wxMenu();
    settingsMenu->Append(wxID_ANY, "����", "�����öԻ���");
    menuBar->Append(settingsMenu, "&����");

    // ���������˵�
    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxID_ANY, "����\tF1", "��ʾ���ڶԻ���");
    menuBar->Append(helpMenu, "&����");

    // ���˵������õ����
    SetMenuBar(menuBar);

    // �󶨲˵��¼�
    Bind(wxEVT_MENU, &Main::OnExit, this);
    Bind(wxEVT_MENU, &Main::OnAbout, this);
    Bind(wxEVT_MENU, &Main::OnCopy, this);
    Bind(wxEVT_MENU, &Main::OnCut, this);
    Bind(wxEVT_MENU, &Main::OnPaste, this);
    Bind(wxEVT_MENU, &Main::OnSettings, this);
    Bind(wxEVT_MENU, &Main::OnTool1, this);
    Bind(wxEVT_MENU, &Main::OnTool2, this);
}
void Main::CreateLayout()
{
    wxPanel* leftPanel = new wxPanel(this, wxID_ANY);
    wxPanel* centerPanel = new wxPanel(this, wxID_ANY);
    wxPanel* rightPanel = new wxPanel(this, wxID_ANY);
    wxPanel* toolBarPanel = new wxPanel(this, wxID_ANY);

    wxBoxSizer* hboxMain = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* centerSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* toolBarSizer = new wxBoxSizer(wxVERTICAL);


    wxInitAllImageHandlers();

    // ������ť�ĺ���
    auto createBitmapButton = [this, toolBarPanel, toolBarSizer](const wxString& label, const wxString& imagePath, int buttonId, void (Main::* handler)(wxCommandEvent&)) {
        wxBitmap bitmap;
        if (bitmap.LoadFile(imagePath, wxBITMAP_TYPE_PNG)) {
            wxBitmapButton* button = new wxBitmapButton(toolBarPanel, buttonId, bitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
            button->SetToolTip(label); // ���ù�����ʾ
            toolBarSizer->Add(button, 0, wxEXPAND | wxALL, 2);

            // ���¼�������򵽰�ť
            button->Bind(wxEVT_BUTTON, [this, handler](wxCommandEvent& event) {
                (this->*handler)(event); // ���ó�Ա����
                });
        }
        };

    // ��Ӱ�ť
    createBitmapButton(wxT("ԭ��ͼ�༭��"), "resource/image/schematic.png", wxID_ANY, &Main::OnyltButtonClick);
    createBitmapButton(wxT("���ű༭��"), "resource/image/symbol.png", wxID_ANY + 1, &Main::OnButtonClick);
    createBitmapButton(wxT("PCB�༭��"), "resource/image/PCB.png", wxID_ANY + 2, &Main::OnButtonClick);

    // ���ù��������Ĳ��ֹ�����
    toolBarPanel->SetSizer(toolBarSizer);

    // ����λͼ��ť����ӵ� leftSizer
    wxBitmap createFileBitmap;
    wxBitmap openFileBitmap;

    if (createFileBitmap.LoadFile("resource/image/create_pro.png", wxBITMAP_TYPE_PNG) &&
        openFileBitmap.LoadFile("resource/image/open_pro.png", wxBITMAP_TYPE_PNG)) {

        wxBitmapButton* button_createFile = new wxBitmapButton(leftPanel, wxID_ANY, createFileBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
        button_createFile->SetToolTip("�½�����");
        leftSizer->Add(button_createFile, 0, wxALL, 2);
        button_createFile->Bind(wxEVT_BUTTON, &Main::OnCreateFileButtonClick, this); // ���¼�

        wxBitmapButton* button_openFile = new wxBitmapButton(leftPanel, wxID_ANY, openFileBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
        button_openFile->SetToolTip("�򿪹���");
        leftSizer->Add(button_openFile, 0, wxALL, 2);
        button_openFile->Bind(wxEVT_BUTTON, &Main::OnOpenFileButtonClick, this); // ���¼�
    }


    // ���� leftPanel �Ĳ��ֹ�����
    leftPanel->SetSizer(leftSizer);

    // ����һ���µ�����԰�����̬�ı�
    wxPanel* labelPanel = new wxPanel(centerPanel, wxID_ANY);
    labelPanel->SetBackgroundColour(wxColour(140, 140, 140)); // ���ñ�����ɫΪ���ɫ

    // ������̬�ı�����ӵ� labelPanel
    wxStaticText* label = new wxStaticText(labelPanel, wxID_ANY, "�����ļ�", wxDefaultPosition, wxDefaultSize, 0);
    label->SetForegroundColour(*wxWHITE); // ����ǰ��ɫΪ��ɫ

    // ����һ��ˮƽ���ֹ������Ծ�����ʾ�ı�
    wxBoxSizer* labelSizer = new wxBoxSizer(wxHORIZONTAL);
    labelSizer->Add(label, 1, wxALIGN_CENTER | wxALL, 3); // ���ı���ӵ������У�������ʾ

    // ���� labelPanel �Ĳ��ֹ�����
    labelPanel->SetSizer(labelSizer);

    // �� labelPanel ��ӵ� centerSizer
    centerSizer->Add(labelPanel, 0, wxEXPAND);

    // ���� centerPanel �Ĳ��ֹ�����
    centerPanel->SetSizer(centerSizer);

    // ��������
    wxFont titleFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD); // ��������
    wxFont descriptionFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL); // ��������

    // �����Ҳ���������
    wxStaticText* buttonDescription1 = new wxStaticText(rightPanel, wxID_ANY, "ԭ��ͼ�༭��", wxDefaultPosition, wxDefaultSize, 0);
    buttonDescription1->SetFont(titleFont); // ���ñ�������

    wxStaticText* buttonDescription2 = new wxStaticText(rightPanel, wxID_ANY, "�༭��·ԭ��ͼ", wxDefaultPosition, wxDefaultSize, 0);
    buttonDescription2->SetFont(descriptionFont); // ������������

    wxStaticText* buttonDescription3 = new wxStaticText(rightPanel, wxID_ANY, "���ű༭��", wxDefaultPosition, wxDefaultSize, 0);
    buttonDescription3->SetFont(titleFont); // ���ñ�������

    wxStaticText* buttonDescription4 = new wxStaticText(rightPanel, wxID_ANY, "�༭��·����", wxDefaultPosition, wxDefaultSize, 0);
    buttonDescription4->SetFont(descriptionFont); // ������������

    wxStaticText* buttonDescription5 = new wxStaticText(rightPanel, wxID_ANY, "PCB�༭��", wxDefaultPosition, wxDefaultSize, 0);
    buttonDescription5->SetFont(titleFont); // ���ñ�������

    wxStaticText* buttonDescription6 = new wxStaticText(rightPanel, wxID_ANY, "�༭PCB���", wxDefaultPosition, wxDefaultSize, 0);
    buttonDescription6->SetFont(descriptionFont); // ������������

    rightSizer->Add(buttonDescription1, 0, wxALL, 8);
    rightSizer->Add(buttonDescription2, 0, wxALL, 1);
    rightSizer->Add(buttonDescription3, 0, wxALL, 8);
    rightSizer->Add(buttonDescription4, 0, wxALL, 5);
    rightSizer->Add(buttonDescription5, 0, wxALL, 10);
    rightSizer->Add(buttonDescription6, 0, wxALL, 3);

    // ���� rightPanel �Ĳ��ֹ�����
    rightPanel->SetSizer(rightSizer);

    // �����嵽������
    hboxMain->Add(leftPanel, 2, wxEXPAND | wxALL, 1);
    hboxMain->Add(centerPanel, 19, wxEXPAND | wxALL, 3);
    hboxMain->Add(toolBarPanel, 0, wxEXPAND); // ��ӹ��������
    hboxMain->Add(rightPanel, 19, wxEXPAND);

    // ���������ڵĲ��ֹ�����
    SetSizer(hboxMain);
    Layout(); // ���²���
}


void Main::OnButtonClick(wxCommandEvent& event) {

}



void Main::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void Main::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("����һ���򵥵Ĳ˵�ʾ����", "����", wxOK | wxICON_INFORMATION);
}

void Main::OnCopy(wxCommandEvent& event)
{
    // ʵ�ָ��ƹ���
    wxMessageBox("�����˸��Ʋ�����", "����", wxOK | wxICON_INFORMATION);
}

void Main::OnCut(wxCommandEvent& event)
{
    // ʵ�ּ��й���
    wxMessageBox("�����˼��в�����", "����", wxOK | wxICON_INFORMATION);
}

void Main::OnPaste(wxCommandEvent& event)
{
    // ʵ��ճ������
    wxMessageBox("������ճ��������", "ճ��", wxOK | wxICON_INFORMATION);
}

void Main::OnSettings(wxCommandEvent& event)
{
    // ʵ�����öԻ���
    wxMessageBox("�������öԻ���", "����", wxOK | wxICON_INFORMATION);
}

void Main::OnTool1(wxCommandEvent& event)
{
    
    
}

void Main::OnTool2(wxCommandEvent& event)
{
    // ʵ�ֹ��� 2 ����
    wxMessageBox("���� 2 ��ִ�С�", "���� 2", wxOK | wxICON_INFORMATION);
}
void Main::OnyltButtonClick(wxCommandEvent& event) {
    // ����һ���µ� Frame ����
    cMain* cmain = new cMain(this);

    // ��ʾ�´���
    cmain->Show(true);
}

void Main::OnOpenFileButtonClick(wxCommandEvent& event)
{
    // ����򿪹��̵��߼�
    wxFileDialog openFileDialog(this, "�򿪹����ļ�", "", "",
        "�����ļ� (*.json)|*.json|�����ļ� (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK)
    {
        wxString path = openFileDialog.GetPath();
        // ��������Ӵ��ļ����߼�
        wxLogMessage("���ļ�: %s", path);
    }
}

void Main::OnCreateFileButtonClick(wxCommandEvent& event)
{
    // �����½����̵��߼�
    wxTextEntryDialog dialog(this, "�������¹��̵�����:", "�½�����", "");

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString projectName = dialog.GetValue();
        // ����������½����̵��߼�
        wxLogMessage("�½�����: %s", projectName);
    }
}
