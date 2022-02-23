#include <string>

#pragma once


// CRHMAboutBox dialog

class CRHMAboutBox : public CDialogEx
{
	DECLARE_DYNAMIC(CRHMAboutBox)

public:
	CRHMAboutBox(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CRHMAboutBox();

	CStatic crhm_version_string;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	std::string versionstring = "Version NON DLL 4.02";
};
