#pragma once

#include "wx/wx.h"

class MainFrame : public wxFrame
{
public:
    MainFrame();
    ~MainFrame();

public:
    wxButton* m_btn1 = nullptr;
    wxTextCtrl* m_txt1 = nullptr;
    wxListBox* m_list1 = nullptr;
};
