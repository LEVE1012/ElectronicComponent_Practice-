#pragma once
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/listbox.h>
#include <wx/notebook.h>
#include <wx/display.h>
#include "cMain.h"

/*
主界面板块
*/

class Main : public wxFrame
{
public:
    Main();
    ~Main();

public:
    // 菜单条的功能函数
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnCut(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);
    void OnTool1(wxCommandEvent& event);
    void OnTool2(wxCommandEvent& event);


    // 其他私有方法
    void CreateMenu();                 // 创建菜单
    void CreateLayout();               // 创建布局

    void OnButtonClick(wxCommandEvent& event);
    void OnyltButtonClick(wxCommandEvent& event);
    void OnOpenFileButtonClick(wxCommandEvent& event);
    void OnCreateFileButtonClick(wxCommandEvent& event);
    // 其他可能的成员变量
    wxListBox* recentOperations;       // 最近常用的操作列表
    wxListBox* projectFiles;           // 项目文件列表

};
