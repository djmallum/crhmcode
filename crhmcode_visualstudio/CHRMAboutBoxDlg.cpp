// CHRMAboutBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CRHM_GUI.h"
#include "CHRMAboutBoxDlg.h"
#include "afxdialogex.h"


// CHRMAboutBoxDlg dialog

IMPLEMENT_DYNAMIC(CHRMAboutBoxDlg, CDialogEx)

CHRMAboutBoxDlg::CHRMAboutBoxDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CHRMAboutBox, pParent)
{

}

CHRMAboutBoxDlg::~CHRMAboutBoxDlg()
{
}

void CHRMAboutBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHRMAboutBoxDlg, CDialogEx)
END_MESSAGE_MAP()


// CHRMAboutBoxDlg message handlers
