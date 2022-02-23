#include "CRHM_GUI.h"
#include "afxdialogex.h"
#include "../../src/core/CRHMmain.h"
#include "../../src/core/ClassModule.h"
#include "../../src/core/MacroUnit.h"

#pragma once


// CReport dialog

class CReport : public CDialog
{
	DECLARE_DYNAMIC(CReport)

public:
	CReport(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CReport();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = REPORT_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:

	CListBox groupsListBox;
	CEdit extractedGroupText;
	CStatic staticText;

	virtual BOOL OnInitDialog();

	afx_msg void OnSave();
	afx_msg void OnExit();
	afx_msg void SelectGroup();



	//CStatic reportlabel;
	void ExtractGroupFile(ClassModule *thisModule);
	long Add_NHRU(string S, std::vector<std::string>* MacroModulesList);
	void AddMacroCode(string S, std::vector<std::string>* MacroModulesList, std::vector<std::string> * Memo1, long& MacroPos);
	

	string defaultprojectpath;
};
