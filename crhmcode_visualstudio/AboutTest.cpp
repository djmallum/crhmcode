// AboutTest.cpp : implementation file
//

#include "stdafx.h"
#include "CRHM_GUI.h"
#include "AboutTest.h"
#include "afxdialogex.h"


// AboutTest dialog

IMPLEMENT_DYNAMIC(AboutTest, CDialogEx)

AboutTest::AboutTest(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CHRMAboutBox, pParent)
{

}

AboutTest::~AboutTest()
{
}

void AboutTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AboutTest, CDialogEx)
END_MESSAGE_MAP()


// AboutTest message handlers
