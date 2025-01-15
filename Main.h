#pragma once
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/listbox.h>
#include <wx/notebook.h>
#include <wx/display.h>
#include "cMain.h"

/*
��������
*/

class Main : public wxFrame
{
public:
    Main();
    ~Main();

public:
    // �˵����Ĺ��ܺ���
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnCut(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);
    void OnTool1(wxCommandEvent& event);
    void OnTool2(wxCommandEvent& event);


    // ����˽�з���
    void CreateMenu();                 // �����˵�
    void CreateLayout();               // ��������

    void OnButtonClick(wxCommandEvent& event);
    void OnyltButtonClick(wxCommandEvent& event);
    void OnOpenFileButtonClick(wxCommandEvent& event);
    void OnCreateFileButtonClick(wxCommandEvent& event);
    // �������ܵĳ�Ա����
    wxListBox* recentOperations;       // ������õĲ����б�
    wxListBox* projectFiles;           // ��Ŀ�ļ��б�

};
