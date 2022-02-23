// CRHMAboutBox.cpp : implementation file
//

#include "../stdafx.h"
#include "CRHM_GUI.h"
#include "CRHMAboutBox.h"
#include "afxdialogex.h"


// CRHMAboutBox dialog

IMPLEMENT_DYNAMIC(CRHMAboutBox, CDialogEx)

CRHMAboutBox::CRHMAboutBox(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABOUTBOX, pParent)
{

}

CRHMAboutBox::~CRHMAboutBox()
{
}

void CRHMAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, crhm_version, crhm_version_string);
}


BEGIN_MESSAGE_MAP(CRHMAboutBox, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRHMAboutBox::OnBnClickedOk)
END_MESSAGE_MAP()


// CRHMAboutBox message handlers


void CRHMAboutBox::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


BOOL CRHMAboutBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	CStatic * crhmversion = (CStatic*)GetDlgItem(crhm_version);

	CString cstr(versionstring.c_str());

	crhmversion->SetWindowTextW(cstr);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
