//////////////////////////////////////////////////////////////////////////
//
// pgAdmin III - PostgreSQL Tools
// Copyright (C) 2002, The pgAdmin Development Team
// This software is released under the pgAdmin Public Licence
//
// frmMain.cpp - The main form
//
//////////////////////////////////////////////////////////////////////////

// wxWindows headers
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/splitter.h>
#include <wx/toolbar.h>
#include <wx/tbarsmpl.h>
#include <wx/imaglist.h>
#include <wx/tipdlg.h>
#include <wx/stc/stc.h>

// App headers
#include "../../pgAdmin3.h"
#include "frmMain.h"
#include "frmAbout.h"
#include "../controls/ctlSQLBox.h"

// Icons
#include "../../images/aggregate.xpm"
#include "../../images/arguments.xpm"
#include "../../images/baddatabase.xpm"
#include "../../images/check.xpm"
#include "../../images/closeddatabase.xpm"
#include "../../images/column.xpm"
#include "../../images/connect.xpm"
#include "../../images/create.xpm"
#include "../../images/database.xpm"
#include "../../images/domain.xpm"
#include "../../images/drop.xpm"
#include "../../images/function.xpm"
#include "../../images/group.xpm"
#include "../../images/hiproperty.xpm"
#include "../../images/index.xpm"
#include "../../images/indexcolumn.xpm"
#include "../../images/language.xpm"
#include "../../images/namespace.xpm"
#include "../../images/operator.xpm"
#include "../../images/pgAdmin3.xpm"
#include "../../images/properties.xpm"
#include "../../images/property.xpm"
#include "../../images/public.xpm"
#include "../../images/record.xpm"
#include "../../images/refresh.xpm"
#include "../../images/relationship.xpm"
#include "../../images/rule.xpm"
#include "../../images/sequence.xpm"
#include "../../images/server.xpm"
#include "../../images/sql.xpm"
#include "../../images/statistics.xpm"
#include "../../images/stop.xpm"
#include "../../images/table.xpm"
#include "../../images/trigger.xpm"
#include "../../images/type.xpm"
#include "../../images/user.xpm"
#include "../../images/vacuum.xpm"
#include "../../images/view.xpm"
#include "../../images/viewdata.xpm"

// Event table
BEGIN_EVENT_TABLE(frmMain, wxFrame)
    EVT_MENU(MNU_EXIT, frmMain::OnExit)
    EVT_MENU(MNU_ABOUT, frmMain::OnAbout)
    EVT_MENU(MNU_TIPOFTHEDAY, frmMain::TipOfTheDay)
END_EVENT_TABLE()

frmMain::frmMain(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
    // Icon
    SetIcon(wxIcon(pgAdmin3_xpm));

    // Build menus
    mnuBar = new wxMenuBar();

    // File Menu
    wxMenu *mnuFile = new wxMenu;
    mnuFile->Append(MNU_CONNECT, wxT("&Connect..."), wxT("Connect to a PostgreSQL server"));
    mnuFile->Append(MNU_PASSWORD, wxT("C&hange password..."), wxT("Change your password"));
    mnuFile->AppendSeparator();
    mnuFile->Append(MNU_SAVEDEFINITION, wxT("&Save definition..."), wxT("Save the SQL definition of the selected object"));
    mnuFile->Append(MNU_SAVESCHEMA, wxT("S&ave DB schema..."), wxT("Save the schema of the current database"));
    mnuFile->AppendSeparator();
    mnuFile->Append(MNU_EXIT, wxT("E&xit"), wxT("Quit this program"));
    mnuBar->Append(mnuFile, wxT("&File"));

    // Tools Menu
    wxMenu *mnuTools = new wxMenu;
    mnuTools->Append(MNU_UPGRADEWIZARD, wxT("&Upgrade Wizard..."), wxT("Run the upgrade wizard"));
    mnuTools->AppendSeparator();
    mnuTools->Append(MNU_OPTIONS, wxT("&Options..."), wxT("Show options dialog"));
    mnuBar->Append(mnuTools, wxT("&Tools"));

    // View Menu
    wxMenu *mnuView = new wxMenu;
    mnuView->Append(MNU_SYSTEMOBJECTS, wxT("&System objects"), wxT("Show or hide system objects"));
    mnuBar->Append(mnuView, wxT("&View"));

    // Help Menu
    wxMenu *mnuHelp = new wxMenu;
    mnuHelp->Append(MNU_CONTENTS, wxT("&Help..."), wxT("Open the helpfile"));
    mnuHelp->Append(MNU_TIPOFTHEDAY, wxT("&Tip of the day..."), wxT("Show a tip of the day"));
    mnuHelp->AppendSeparator();
    mnuHelp->Append(MNU_ABOUT, wxT("&About..."), wxT("Show about dialog"));
    mnuBar->Append(mnuHelp, wxT("&Help"));

    // Add the Menubar
    SetMenuBar(mnuBar);

    // Status bar
    CreateStatusBar(6);
    static const int iWidths[6] = {-1, 50, 100, 100, 100, 100};
    SetStatusWidths(6, iWidths);
    SetStatusText(wxT("Ready."), 0);
    SetStatusText(wxT("0 Secs"), 1);
    SetStatusText(wxT("Object: None"), 2);
    SetStatusText(wxT("Schema: None"), 3);
    SetStatusText(wxT("Database: None"), 4);
    SetStatusText(wxT("Server: None"), 5);

    // Toolbar bar

    CreateToolBar();

    // Return objects
    stBar = GetStatusBar();
    tlBar = GetToolBar();

    // Set up toolbar
    wxBitmap tlBarBitmaps[10];
    tlBar->SetToolBitmapSize(wxSize(32, 32));
    tlBarBitmaps[0] = wxBitmap(connect_xpm);
    tlBarBitmaps[1] = wxBitmap(refresh_xpm);
    tlBarBitmaps[2] = wxBitmap(create_xpm);
    tlBarBitmaps[3] = wxBitmap(drop_xpm);
    tlBarBitmaps[4] = wxBitmap(properties_xpm);
    tlBarBitmaps[5] = wxBitmap(sql_xpm);
    tlBarBitmaps[6] = wxBitmap(viewdata_xpm);
    tlBarBitmaps[7] = wxBitmap(vacuum_xpm);
    tlBarBitmaps[8] = wxBitmap(record_xpm);
    tlBarBitmaps[9] = wxBitmap(stop_xpm);

    tlBar->AddTool(BTN_CONNECT, wxT("Connect"), tlBarBitmaps[0], wxT("Connect to a server"), wxITEM_NORMAL);
    tlBar->AddTool(BTN_REFRESH, wxT("Refresh"), tlBarBitmaps[1], wxT("Refrsh the data below the selected object"), wxITEM_NORMAL);
    tlBar->AddSeparator();
    tlBar->AddTool(BTN_CREATE, wxT("Create"), tlBarBitmaps[2], wxT("Create a new object of the same type as the selected object"), wxITEM_NORMAL);
    tlBar->AddTool(BTN_DROP, wxT("Drop"), tlBarBitmaps[3], wxT("Drop the currently selected object"), wxITEM_NORMAL);
    tlBar->AddTool(BTN_PROPERTIES, wxT("Properties"), tlBarBitmaps[4], wxT("Display/edit the properties of the selected object"), wxITEM_NORMAL);
    tlBar->AddSeparator();
    tlBar->AddTool(BTN_SQL, wxT("SQL"), tlBarBitmaps[5], wxT("Execute arbitrary SQL queries"), wxITEM_NORMAL);
    tlBar->AddTool(BTN_VIEWDATA, wxT("View Data"), tlBarBitmaps[6], wxT("View the data in the selected object"), wxITEM_NORMAL);
    tlBar->AddTool(BTN_VACUUM, wxT("Vacuum"), tlBarBitmaps[7], wxT("Vacuum the current database or table"), wxITEM_NORMAL);
    tlBar->AddSeparator();
    tlBar->AddTool(BTN_RECORD, wxT("Record"), tlBarBitmaps[8], wxT("Record a query log"), wxITEM_NORMAL);
    tlBar->AddTool(BTN_STOP, wxT("Stop"), tlBarBitmaps[9], wxT("Stop recording the query log"), wxITEM_NORMAL);

    tlBar->Realize();
    
    // Setup the vertical splitter & treeview
    wxSplitterWindow* splVertical = new wxSplitterWindow(this, -1, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN);
    wxSplitterWindow* splHorizontal = new wxSplitterWindow(splVertical, -1, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN);
    tvBrowser = new wxTreeCtrl(splVertical, -1, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxSIMPLE_BORDER);
    splVertical->SplitVertically(tvBrowser, splHorizontal, 200);
    splVertical->SetMinimumPaneSize(50);

    // Setup the horizontal splitter for the listview & sql pane
    wxNotebook* nbListViews = new wxNotebook(splHorizontal, -1, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM);
    lvProperties = new wxListCtrl(nbListViews, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxSIMPLE_BORDER);
    lvStatistics = new wxListCtrl(nbListViews, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxSIMPLE_BORDER);
    nbListViews->AddPage(lvProperties, wxT("Properties"));
    nbListViews->AddPage(lvStatistics, wxT("Statistics"));
    txtSQLPane = new ctlSQLBox(splHorizontal, -1, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxSIMPLE_BORDER | wxTE_READONLY | wxTE_RICH2);
    txtSQLPane->SetBackgroundColour(*wxLIGHT_GREY);
    splHorizontal->SplitHorizontally(nbListViews, txtSQLPane, 300);
    splHorizontal->SetMinimumPaneSize(50);

    //Setup a Browser imagemap
    wxImageList *ilBrowser = new wxImageList(16, 16);
    //Associate the Browser imagemap to the Browser
    tvBrowser->SetImageList(ilBrowser);
    //Stuff the Image List
    ilBrowser->Add(wxIcon(server_xpm));
    ilBrowser->Add(wxIcon(database_xpm));
    ilBrowser->Add(wxIcon(language_xpm));
    ilBrowser->Add(wxIcon(namespace_xpm));
    ilBrowser->Add(wxIcon(aggregate_xpm));
    ilBrowser->Add(wxIcon(function_xpm));
    ilBrowser->Add(wxIcon(operator_xpm));
    ilBrowser->Add(wxIcon(sequence_xpm));
    ilBrowser->Add(wxIcon(table_xpm));
    ilBrowser->Add(wxIcon(type_xpm));
    ilBrowser->Add(wxIcon(view_xpm));
    ilBrowser->Add(wxIcon(user_xpm));
    ilBrowser->Add(wxIcon(group_xpm));
    ilBrowser->Add(wxIcon(baddatabase_xpm));
    ilBrowser->Add(wxIcon(closeddatabase_xpm));

    // Add some treeview items
    wxTreeItemId itmDummy = tvBrowser->AddRoot(wxT("Root node"),0);
    tvBrowser->AppendItem(itmDummy, wxT("Child Node #1"));
    tvBrowser->AppendItem(itmDummy, wxT("Child Node #2"));
    tvBrowser->Expand(itmDummy);
    itmDummy = tvBrowser->AppendItem(itmDummy, wxT("Child Node #3"));
    tvBrowser->AppendItem(itmDummy, wxT("Child Node #4"));
    tvBrowser->AppendItem(itmDummy, wxT("Child Node #5"));

    //Setup a listview imagemap
    wxImageList *ilProperties = new wxImageList(16, 16);
    //Associate the listview imagemap to the listview
    lvProperties->SetImageList(ilProperties, wxIMAGE_LIST_SMALL);
    //Stuff the BrowserImage Listu:
    ilProperties->Add(wxIcon(property_xpm));

    // Add some listview items
    lvProperties->InsertColumn(0, wxT("Property"), wxLIST_FORMAT_LEFT, 100);
    lvProperties->InsertColumn(1, wxT("Value"), wxLIST_FORMAT_LEFT, 400);

    // This is the bit that puts it all on one line over 2 colums
    lvProperties->InsertItem(0, wxT("Property #1"), 0);
    lvProperties->SetItem(0, 1, wxT("Property #1a"));

    lvProperties->InsertItem(1, wxT("Property #2"), 0);
    lvProperties->SetItem(1, 1, wxT("Property #2a"));

    //Setup a listview imagemap
    wxImageList *ilStatistics = new wxImageList(16, 16);
    //Associate the listview imagemap to the listview
    lvStatistics->SetImageList(ilStatistics, wxIMAGE_LIST_SMALL);
    //Stuff the BrowserImage Listu:
    ilStatistics->Add(wxIcon(statistics_xpm));

    // Add some listview items
    lvStatistics->InsertColumn(0, wxT("Statistic"), wxLIST_FORMAT_LEFT, 100);
    lvStatistics->InsertColumn(1, wxT("Value"), wxLIST_FORMAT_LEFT, 400);

    // This is the bit that puts it all on one line over 2 colums
    lvStatistics->InsertItem(0, wxT("Statistic #1"), 0);
    lvStatistics->SetItem(0, 1, wxT("Statistic #1a"));

    lvStatistics->InsertItem(1, wxT("Statistic #2"), 0);
    lvStatistics->SetItem(1, 1, wxT("Statistic #2a"));


    // Setup the SQL Pane
    txtSQLPane->InsertText(0, wxT("-- Select all records from pg_class\nSELECT\n  *\nFROM\n  pg_class\nWHERE\n relname LIKE 'pg_%'\nORDER BY\n  rename;"));
}

frmMain::~frmMain()
{
    extern sysSettings *objSettings;
    objSettings->SetFrmMainWidth(GetSize().x);
    objSettings->SetFrmMainHeight(GetSize().y);
    objSettings->SetFrmMainLeft(GetPosition().x);
    objSettings->SetFrmMainTop(GetPosition().y);
}

// Event handlers
void frmMain::OnExit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void frmMain::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    frmAbout *winAbout = new frmAbout(this);
    winAbout->Show(TRUE);
}

void frmMain::TipOfTheDay()
{
    extern sysSettings *objSettings;
    wxTipProvider *tipProvider = wxCreateFileTipProvider(wxT("tips.txt"), objSettings->GetNextTipOfTheDay());
    objSettings->SetShowTipOfTheDay(wxShowTip(this, tipProvider));
    objSettings->SetNextTipOfTheDay(tipProvider->GetCurrentTip());
    delete tipProvider;
}