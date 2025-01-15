#include "Main.h"

Main::Main() : wxFrame(nullptr, wxID_ANY, "Demo", wxPoint(30, 30), wxSize(500, 400))
{

    CreateMenu(); // 创建菜单
    CreateLayout(); // 创建布局
}

Main::~Main()
{

}

void Main::CreateMenu() {
    // 创建菜单栏
    wxMenuBar* menuBar = new wxMenuBar();

    // 创建文件菜单
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(wxID_NEW, "新建\tCtrl-N", "创建新文件");
    fileMenu->Append(wxID_OPEN, "打开\tCtrl-O", "打开文件");
    fileMenu->Append(wxID_SAVE, "保存\tCtrl-S", "保存文件");
    fileMenu->Append(wxID_SAVEAS, "保存为\tCtrl-Shift-S", "另存为文件");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "退出\tCtrl-Q", "退出应用");
    menuBar->Append(fileMenu, "文件");

    // 创建编辑菜单
    wxMenu* editMenu = new wxMenu();
    editMenu->Append(wxID_ANY, "复制\tCtrl-C", "复制选定内容");
    editMenu->Append(wxID_ANY, "剪切\tCtrl-X", "剪切选定内容");
    editMenu->Append(wxID_ANY, "粘贴\tCtrl-V", "从剪贴板粘贴");
    menuBar->Append(editMenu, "&编辑");

    // 创建工具菜单
    wxMenu* toolMenu = new wxMenu();
    toolMenu->Append(wxID_ANY, "原理图编辑器", "运行工具 1");
    toolMenu->Append(wxID_ANY, "符号编辑器", "运行工具 2");
    toolMenu->Append(wxID_ANY, "PCB编辑器", "运行工具 2");
    menuBar->Append(toolMenu, "&工具");

    // 创建设置菜单
    wxMenu* settingsMenu = new wxMenu();
    settingsMenu->Append(wxID_ANY, "设置", "打开设置对话框");
    menuBar->Append(settingsMenu, "&设置");

    // 创建帮助菜单
    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxID_ANY, "关于\tF1", "显示关于对话框");
    menuBar->Append(helpMenu, "&帮助");

    // 将菜单栏设置到框架
    SetMenuBar(menuBar);

    // 绑定菜单事件
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

    // 创建按钮的函数
    auto createBitmapButton = [this, toolBarPanel, toolBarSizer](const wxString& label, const wxString& imagePath, int buttonId, void (Main::* handler)(wxCommandEvent&)) {
        wxBitmap bitmap;
        if (bitmap.LoadFile(imagePath, wxBITMAP_TYPE_PNG)) {
            wxBitmapButton* button = new wxBitmapButton(toolBarPanel, buttonId, bitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
            button->SetToolTip(label); // 设置工具提示
            toolBarSizer->Add(button, 0, wxEXPAND | wxALL, 2);

            // 绑定事件处理程序到按钮
            button->Bind(wxEVT_BUTTON, [this, handler](wxCommandEvent& event) {
                (this->*handler)(event); // 调用成员函数
                });
        }
        };

    // 添加按钮
    createBitmapButton(wxT("原理图编辑器"), "resource/image/schematic.png", wxID_ANY, &Main::OnyltButtonClick);
    createBitmapButton(wxT("符号编辑器"), "resource/image/symbol.png", wxID_ANY + 1, &Main::OnButtonClick);
    createBitmapButton(wxT("PCB编辑器"), "resource/image/PCB.png", wxID_ANY + 2, &Main::OnButtonClick);

    // 设置工具栏面板的布局管理器
    toolBarPanel->SetSizer(toolBarSizer);

    // 创建位图按钮并添加到 leftSizer
    wxBitmap createFileBitmap;
    wxBitmap openFileBitmap;

    if (createFileBitmap.LoadFile("resource/image/create_pro.png", wxBITMAP_TYPE_PNG) &&
        openFileBitmap.LoadFile("resource/image/open_pro.png", wxBITMAP_TYPE_PNG)) {

        wxBitmapButton* button_createFile = new wxBitmapButton(leftPanel, wxID_ANY, createFileBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
        button_createFile->SetToolTip("新建工程");
        leftSizer->Add(button_createFile, 0, wxALL, 2);
        button_createFile->Bind(wxEVT_BUTTON, &Main::OnCreateFileButtonClick, this); // 绑定事件

        wxBitmapButton* button_openFile = new wxBitmapButton(leftPanel, wxID_ANY, openFileBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
        button_openFile->SetToolTip("打开工程");
        leftSizer->Add(button_openFile, 0, wxALL, 2);
        button_openFile->Bind(wxEVT_BUTTON, &Main::OnOpenFileButtonClick, this); // 绑定事件
    }


    // 设置 leftPanel 的布局管理器
    leftPanel->SetSizer(leftSizer);

    // 创建一个新的面板以包裹静态文本
    wxPanel* labelPanel = new wxPanel(centerPanel, wxID_ANY);
    labelPanel->SetBackgroundColour(wxColour(140, 140, 140)); // 设置背景颜色为深灰色

    // 创建静态文本并添加到 labelPanel
    wxStaticText* label = new wxStaticText(labelPanel, wxID_ANY, "工程文件", wxDefaultPosition, wxDefaultSize, 0);
    label->SetForegroundColour(*wxWHITE); // 设置前景色为白色

    // 创建一个水平布局管理器以居中显示文本
    wxBoxSizer* labelSizer = new wxBoxSizer(wxHORIZONTAL);
    labelSizer->Add(label, 1, wxALIGN_CENTER | wxALL, 3); // 将文本添加到布局中，居中显示

    // 设置 labelPanel 的布局管理器
    labelPanel->SetSizer(labelSizer);

    // 将 labelPanel 添加到 centerSizer
    centerSizer->Add(labelPanel, 0, wxEXPAND);

    // 设置 centerPanel 的布局管理器
    centerPanel->SetSizer(centerSizer);

    // 设置字体
    wxFont titleFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD); // 标题字体
    wxFont descriptionFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL); // 描述字体

    // 创建右侧面板的内容
    wxStaticText* buttonDescription1 = new wxStaticText(rightPanel, wxID_ANY, "原理图编辑器", wxDefaultPosition, wxDefaultSize, 0);
    buttonDescription1->SetFont(titleFont); // 设置标题字体

    wxStaticText* buttonDescription2 = new wxStaticText(rightPanel, wxID_ANY, "编辑电路原理图", wxDefaultPosition, wxDefaultSize, 0);
    buttonDescription2->SetFont(descriptionFont); // 设置描述字体

    wxStaticText* buttonDescription3 = new wxStaticText(rightPanel, wxID_ANY, "符号编辑器", wxDefaultPosition, wxDefaultSize, 0);
    buttonDescription3->SetFont(titleFont); // 设置标题字体

    wxStaticText* buttonDescription4 = new wxStaticText(rightPanel, wxID_ANY, "编辑电路符号", wxDefaultPosition, wxDefaultSize, 0);
    buttonDescription4->SetFont(descriptionFont); // 设置描述字体

    wxStaticText* buttonDescription5 = new wxStaticText(rightPanel, wxID_ANY, "PCB编辑器", wxDefaultPosition, wxDefaultSize, 0);
    buttonDescription5->SetFont(titleFont); // 设置标题字体

    wxStaticText* buttonDescription6 = new wxStaticText(rightPanel, wxID_ANY, "编辑PCB设计", wxDefaultPosition, wxDefaultSize, 0);
    buttonDescription6->SetFont(descriptionFont); // 设置描述字体

    rightSizer->Add(buttonDescription1, 0, wxALL, 8);
    rightSizer->Add(buttonDescription2, 0, wxALL, 1);
    rightSizer->Add(buttonDescription3, 0, wxALL, 8);
    rightSizer->Add(buttonDescription4, 0, wxALL, 5);
    rightSizer->Add(buttonDescription5, 0, wxALL, 10);
    rightSizer->Add(buttonDescription6, 0, wxALL, 3);

    // 设置 rightPanel 的布局管理器
    rightPanel->SetSizer(rightSizer);

    // 添加面板到主布局
    hboxMain->Add(leftPanel, 2, wxEXPAND | wxALL, 1);
    hboxMain->Add(centerPanel, 19, wxEXPAND | wxALL, 3);
    hboxMain->Add(toolBarPanel, 0, wxEXPAND); // 添加工具栏面板
    hboxMain->Add(rightPanel, 19, wxEXPAND);

    // 设置主窗口的布局管理器
    SetSizer(hboxMain);
    Layout(); // 更新布局
}


void Main::OnButtonClick(wxCommandEvent& event) {

}



void Main::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void Main::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("这是一个简单的菜单示例。", "关于", wxOK | wxICON_INFORMATION);
}

void Main::OnCopy(wxCommandEvent& event)
{
    // 实现复制功能
    wxMessageBox("触发了复制操作。", "复制", wxOK | wxICON_INFORMATION);
}

void Main::OnCut(wxCommandEvent& event)
{
    // 实现剪切功能
    wxMessageBox("触发了剪切操作。", "剪切", wxOK | wxICON_INFORMATION);
}

void Main::OnPaste(wxCommandEvent& event)
{
    // 实现粘贴功能
    wxMessageBox("触发了粘贴操作。", "粘贴", wxOK | wxICON_INFORMATION);
}

void Main::OnSettings(wxCommandEvent& event)
{
    // 实现设置对话框
    wxMessageBox("打开了设置对话框。", "设置", wxOK | wxICON_INFORMATION);
}

void Main::OnTool1(wxCommandEvent& event)
{
    
    
}

void Main::OnTool2(wxCommandEvent& event)
{
    // 实现工具 2 功能
    wxMessageBox("工具 2 被执行。", "工具 2", wxOK | wxICON_INFORMATION);
}
void Main::OnyltButtonClick(wxCommandEvent& event) {
    // 创建一个新的 Frame 窗口
    cMain* cmain = new cMain(this);

    // 显示新窗口
    cmain->Show(true);
}

void Main::OnOpenFileButtonClick(wxCommandEvent& event)
{
    // 处理打开工程的逻辑
    wxFileDialog openFileDialog(this, "打开工程文件", "", "",
        "工程文件 (*.json)|*.json|所有文件 (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK)
    {
        wxString path = openFileDialog.GetPath();
        // 在这里添加打开文件的逻辑
        wxLogMessage("打开文件: %s", path);
    }
}

void Main::OnCreateFileButtonClick(wxCommandEvent& event)
{
    // 处理新建工程的逻辑
    wxTextEntryDialog dialog(this, "请输入新工程的名称:", "新建工程", "");

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString projectName = dialog.GetValue();
        // 在这里添加新建工程的逻辑
        wxLogMessage("新建工程: %s", projectName);
    }
}
