// EntryFormDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CRHM_GUI.h"
#include "EntryFormDlg.h"
#include "afxdialogex.h"


// EntryFormDlg dialog

IMPLEMENT_DYNAMIC(EntryFormDlg, CDialogEx)

EntryFormDlg::EntryFormDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(EntryForm_DLG, pParent)
{

}

EntryFormDlg::~EntryFormDlg()
{
}

void EntryFormDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(EntryFormDlg, CDialogEx)
END_MESSAGE_MAP()


// EntryFormDlg message handlers

void EntryFormDlg::FormActivate()
{
	//Memo->Lines->Clear();

	//for (int ii = 0; ii < Global::MacroModulesList->Count; ++ii)
	//	Memo->Lines->Add(Global::MacroModulesList->Strings[ii]);

	//Memo->SelStart = 0;
	//Memo->SetFocus();

	//Module::GrpCnt = 0;
	//Module::StructCnt = 0;

	//ParaList = NULL;
	//AKAList = NULL;

	//ModalResult = mrCancel;
}
