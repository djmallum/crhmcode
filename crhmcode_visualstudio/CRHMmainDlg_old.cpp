// CRHMmainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CRHM_GUI.h"
#include "CRHMmainDlg.h"
#include "afxdialogex.h"
//#include "CRHMmain.h"

#include "CConstruct.h"
#include "CRHMAboutBox.h"
#include "StandardConverterUtility.h"
#include "Common.h"
#include "MacroEntryDlg.h"
#include "FlowDiagramDlg.h"

// CRHMmainDlg dialog

IMPLEMENT_DYNAMIC(CRHMmainDlg, CDialogEx)

#define MAX_CFileDialog_FILE_COUNT 99
#define FILE_LIST_BUFFER_SIZE ((MAX_CFileDialog_FILE_COUNT * (MAX_PATH + 1)) + 1)

CRHMmainDlg::CRHMmainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CRHMmainDialog, pParent)
{
	CRHMmain *test = CRHMmain::getInstance();
	test->FormCreate();
	FormActivate();

}

CRHMmainDlg::~CRHMmainDlg()
{
}

void CRHMmainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST5, listbox_varaiables_all);
	DDX_Control(pDX, IDC_LIST3, list_box_variables_selected);
	DDX_Control(pDX, IDC_LIST2, listbox_observations);
	DDX_Control(pDX, IDC_TCHART1, tchart);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, DateTimePicker1);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, DateTimePicker2);
	DDX_Control(pDX, IDC_LIST4, listbox_observations_selected);
}


BEGIN_MESSAGE_MAP(CRHMmainDlg, CDialogEx)
	ON_COMMAND(ID_FILE_OPEN, &CRHMmainDlg::OnFileOpen)
	//ON_COMMAND(ID_RUN32811, &CRHMmainDlg::OnRun32811)
	ON_BN_CLICKED(IDC_BUTTON5, &CRHMmainDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CRHMmainDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CRHMmainDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CRHMmainDlg::OnBnClickedButton8)
	ON_LBN_SELCHANGE(IDC_LIST5, &CRHMmainDlg::OnLbnSelchangeList5)
	ON_LBN_SELCHANGE(IDC_LIST2, &CRHMmainDlg::OnLbnSelchangeList2)
	ON_COMMAND(ID_BUILD_CONSTRUCT, &CRHMmainDlg::OnBuildConstruct)
	ON_COMMAND(ID_RUN_RUNMODEL, &CRHMmainDlg::OnRunRunmodel)
	ON_COMMAND(ID_FILE_SAVE_AS, &CRHMmainDlg::OnFileSaveAs)
	ON_BN_CLICKED(IDC_BUTTON9, &CRHMmainDlg::OnBnClickedButton9)
	ON_COMMAND(ID_FILE_SAVE, &CRHMmainDlg::OnFileSave)
	ON_COMMAND(ID_FILE_CLOSE, &CRHMmainDlg::OnFileClose)
	ON_LBN_DBLCLK(IDC_LIST3, &CRHMmainDlg::OnLbnDblclkList3)
	ON_LBN_DBLCLK(IDC_LIST4, &CRHMmainDlg::OnLbnDblclkList4)
	ON_BN_CLICKED(IDC_BUTTON3, &CRHMmainDlg::OnBnClickedButton3)
	ON_COMMAND(ID_EDIT_UNDO, &CRHMmainDlg::OnOpenObservation)
	ON_COMMAND(ID_EDIT_CUT, &CRHMmainDlg::OnCloseAllObservations)
	ON_COMMAND(ID_BUILD_MACRO, &CRHMmainDlg::OnBuildMacro)
	ON_COMMAND(ID_HELP_ABOUT, &CRHMmainDlg::OnHelpAbout)
	ON_COMMAND(ID_FLOWDIAGRAMS_SHOWDIAGRAM, &CRHMmainDlg::OnFlowdiagramsShowdiagram)
END_MESSAGE_MAP()


// CRHMmainDlg message handlers


void CRHMmainDlg::OnFileOpen()
{
	 //TODO: Add your command handler code here

	CFile theFile;
	TCHAR szFilters[] = _T("MyType Files (*.prj)|*.prj|All Files (*.*)|*.*||");

//	CFileDialog fileDlg = CFileDialog(TRUE, _T(".txt"), NULL, 0, strFilter);


	CString fileName;
	wchar_t* p = fileName.GetBuffer(FILE_LIST_BUFFER_SIZE);
//	CFileDialog fileDlg(TRUE);
//	OPENFILENAME& ofn = fileDlg.GetOFN();


	CFileDialog fileDlg(TRUE, _T("prj"), _T("*.prj"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

//	ofn.Flags |= OFN_ALLOWMULTISELECT;
//	ofn.lpstrFile = p;
//	ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;

	//CFileDialog filedlg = CFileDialog(true;

	CRHMmain *test = CRHMmain::getInstance();

	// Display the file dialog. When user clicks OK, fileDlg.DoModal()
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
	{
		CString filepath = fileDlg.GetPathName();

		CString filename = fileDlg.GetFileName();
		//m_left.SetWindowText(m_strPathname);

		

		
		 string file_p = CT2A(filepath.GetString());
		 string file_n = CT2A(filename.GetString());

		 OpenProject(file_p, file_n);

		 //test->DoPrjOpen(file_p,file_n);
		 //loadGuiComponents();


		 //adding the observations on opening file.
		 AddObservationsOnOpenProject();

	}


}

void CRHMmainDlg::OpenProject(string filepath, string filename)
{
	CRHMmain *test = CRHMmain::getInstance();
	test->DoPrjOpen(filepath, filename);
	loadGuiComponents();
}

void CRHMmainDlg::loadGuiComponents()
{

	listbox_varaiables_all.ResetContent();
	list_box_variables_selected.ResetContent();
	listbox_observations.ResetContent();
	listbox_observations_selected.ResetContent();


	std::string s = "Hello";

	// get temporary LPSTR (not really safe)
	LPSTR pst = &s[0];
	// get temporary LPCSTR (pretty safe)
	LPCSTR pcstr = s.c_str();


	CRHMmain* crhm_core = CRHMmain::getInstance();
	
	TStringList* variables = CRHMmain::getInstance()->getVariables();

	for (int ii = 0; ii < variables->Count; ii++) {

		//string s = variables->Strings[ii];


		std::string s = variables->Strings[ii];

		CString cvariables(s.c_str());

		if (s != "t")
		{
			listbox_varaiables_all.AddString(cvariables);
		}
	}


	TStringList* observations = CRHMmain::getInstance()->getObservations();

	for (int ii = 0; ii <observations->Count; ii++) {
		//string s = variables->Strings[ii];
		std::string s = observations->Strings[ii];
		CString observations(s.c_str());
		listbox_observations.AddString(observations);
	}

	TStringList* sel_variables = CRHMmain::getInstance()->getSelectedVariables();

	for (int ii = 0; ii <sel_variables->Count; ii++) {
		//string s = variables->Strings[ii];
		std::string s = sel_variables->Strings[ii];
		CString observations(s.c_str());
		//listbox_variables_selected.AddString(observations);
		list_box_variables_selected.AddString(observations);
	}

	TStringList* sel_observations = CRHMmain::getInstance()->getSelectedObservations();

	for (int ii = 0; ii <sel_observations->Count; ii++) {
		//string s = variables->Strings[ii];
		std::string s = sel_observations->Strings[ii];
		CString observations(s.c_str());
		//listbox_o.AddString(observations);
		listbox_observations_selected.AddString(observations);
	}









	CRHMmain *test = CRHMmain::getInstance();
	SetTime(test->GetStartDate(), test->GetEndDate());


	////conversion mechanism
	//COleDateTime time = COleDateTime(1660,5,10,0,0,0);
	//double t = time; //encoding.
	//COleDateTime time2 = COleDateTime(t); //decoding.	

}


void CRHMmainDlg::SetTime(double startDate, double endDate)
{
	COleDateTime oletimeTime(startDate);
	VERIFY(DateTimePicker1.SetTime(oletimeTime));


	COleDateTime oletimeTime2(endDate);
	VERIFY(DateTimePicker2.SetTime(oletimeTime2));
}





void CRHMmainDlg::FormActivate()
{
	//This is for the about box.


	//#ifdef CRHM_DLL
	//#ifdef NO_MODULES
	//	AboutBox->Label2->Caption = String("Version: CRHM_DLL_NONE ") + CurrentVersion;
	//#else CRHM_2
	//	AboutBox->Label2->Caption = String("Version: CRHM_DLL ") + CurrentVersion;
	//#endif
	//#elif defined CRHM_2
	//	AboutBox->Label2->Caption = String("Version: CRHM ") + CurrentVersion;
	//#elif defined QUINTON
	//	AboutBox->Label2->Caption = String("Version: CRHM_QUINTON ") + CurrentVersion;
	//#else
	//	AboutBox->Label2->Caption = String("Version: CRHM ") + Global::CurrentVersion;
	//#endif

	//Global::crhmLog = LogForm->Handle;

	//for (int pp = 0; pp < ParamCount(); ++pp) {
	//	String FileS = ParamStr(pp + 1);
	//	if (FileExistsSp(FileS)) {
	//		if (LowerCase(ExtractFileExt(FileS)) == ".prj") {

	//			CommandLine = true;  // handle autorun differently

	//			OpenDialogPrj->FileName = FileS;
	//			OpenDialogPrj->InitialDir = ExtractFilePath(AnsiReplaceStr(OpenDialogPrj->FileName, "/", "\\")); // Fix for "C:/Program files/CRHM/Examples
	//			if (OpenDialogPrj->InitialDir.IsEmpty())
	//				OpenDialogPrj->InitialDir = GetCurrentDir();

	//			SetCurrentDir(OpenDialogPrj->InitialDir);
	//			OpenDialogObs->InitialDir = OpenDialogPrj->InitialDir;

	//			FileDefaults(Sender);
	//			DoPrjOpen(Sender);
	//		}
	//		else if (LowerCase(ExtractFileExt(FileS)) == ".obs") {
	//			TDateTime Dt = TDateTime::CurrentDateTime();
	//			Chart->Title->Text->Clear();
	//			Chart->Title->Text->Add("Current Date: " + Dt.DateString() + FormatDateTime(" hh:mm", Dt));

	//			OpenDialogObs->FileName = FileS;
	//			String path = ExtractFilePath(AnsiReplaceStr(OpenDialogObs->FileName, "/", "\\")); // Fix for "C:/Program files/CRHM/Examples
	//			if (path.IsEmpty()) {
	//				path = GetCurrentDir();
	//				FileS = path + "\\" + FileS;
	//			}
	//			OpenDialogObs->InitialDir = path;
	//			OpenObsFile(Sender, FileS);

	//			int iFileHandle = FileOpen(FileS, fmOpenRead);
	//			int T = FileGetDate(iFileHandle);
	//			ProjectFileDate = FileDateToDateTime(T);
	//			char Buf[121];
	//			FileRead(iFileHandle, Buf, 120);
	//			String C(Buf);
	//			int n = C.Pos("\r");
	//			C.SetLength(n - 1);
	//			FileClose(iFileHandle);
	//			String SS = ProjectFileDate.DateString() + FormatDateTime(" hh:mm", ProjectFileDate);

	//			Chart->SubTitle->Text->Clear();
	//			Chart->SubTitle->Text->Add(FileS + " Last modified: " + SS);
	//			Chart->SubTitle->Text->Add("Description: " + C);
	//		}
	//		else if (LowerCase(ExtractFileExt(FileS)) == ".par") {
	//			if (!CommandLine) // cannot change parameters unless project loaded
	//				continue;

	//			ifstream DataFile;
	//			int c, Cols;
	//			string ss;

	//			bool TooFewHRUs = false;
	//			string module, param, Descrip, Line, S, s;
	//			ClassPar *thisPar;

	//			OpenDialogPar->FileName = FileS;
	//			String path = ExtractFilePath(AnsiReplaceStr(OpenDialogPar->FileName, "/", "\\")); // Fix for "C:/Program files/CRHM/Examples
	//			if (path.IsEmpty()) {
	//				path = GetCurrentDir();
	//				FileS = path + "\\" + FileS;
	//			}
	//			OpenDialogPar->InitialDir = path;

	//			DataFile.open(OpenDialogPar->FileName.c_str());
	//			if (!DataFile) {
	//				Application->MessageBox("cannot open file", "File Error", MB_OK);
	//				return;
	//			}

	//			getline(DataFile, Descrip);
	//			DataFile.ignore(80, '#');
	//			getline(DataFile, Line);

	//			RemoveSharedParams(); // change shared parameters to module parameters

	//			while (!DataFile.eof()) {
	//				DataFile.clear();

	//				DataFile >> module;
	//				if (module[1] == '#') break;
	//				DataFile >> param;
	//				if (DataFile.eof()) return; // final exit

	//				DataFile.ignore(80, '\n'); // need for character input, why?
	//				S = "";

	//				do {
	//					getline(DataFile, ss);
	//					S += ss + ' ';
	//					while (c = DataFile.peek(), iscntrl(c) || isspace(c))
	//						DataFile.get();

	//				} while (c = DataFile.peek(), !isalpha(c) && c != '#' && !DataFile.eof());

	//				string Sc = S; // use copy
	//				string::iterator pos; // first handle single quotes
	//				bool inField = false;

	//				for (pos = Sc.begin(); pos != Sc.end(); ++pos) {
	//					if (*pos == '\'')
	//						inField = !inField;
	//					if (inField && *pos == ' ')
	//						*pos = '_';
	//				}

	//				istringstream instr; // count columns
	//				instr.clear();
	//				instr.str(Sc);
	//				Cols = 0;
	//				for (;;) {
	//					instr >> s;
	//					if (instr.fail()) break;
	//					++Cols; // # of HRU's in Basin
	//				}

	//				if (module == "Shared") {
	//					// write shared parameters to module parameters
	//					// all parameter values are set to shared values.  If re-defined in a module will be changed.
	//					MapPar::iterator itPar;
	//					for (itPar = Global::MapPars.begin(); itPar != Global::MapPars.end(); ++itPar) {
	//						thisPar = (*itPar).second;
	//						if (thisPar->param == string(param)) {
	//							if (thisPar->dim == Cols / thisPar->lay) // find module parameter for template thisPar->varType == CRHM::Int || thisPar->varType == CRHM::Float ||
	//								break;
	//							else if (thisPar->varType == CRHM::Txt && thisPar->dimen < CRHM::NHRU) // text can have variable length
	//								break;
	//							else { // Added to handle 2D parameters
	//								if (thisPar->param == string(param) && (thisPar->dim == Cols / thisPar->dim))
	//									break;
	//								else
	//									thisPar = NULL;
	//							}
	//						}
	//					}
	//				}
	//				else
	//					thisPar = ClassParFind(module, param); // find module parameter for template

	//				if (thisPar) {
	//					ClassPar *newPar = new ClassPar(*thisPar);
	//					newPar->module = module; // set module name
	//					if (thisPar->varType == CRHM::Txt) {
	//						newPar->Strings->DelimitedText = S.c_str();
	//					}
	//					else {
	//						istringstream instr; // count columns
	//						instr.clear();
	//						instr.str(S);
	//						for (int jj = 0; jj<thisPar->lay; jj++) {
	//							for (int ii = 0; ii < newPar->dim; ii++) {

	//								if (newPar->varType == CRHM::Float)
	//									instr >> newPar->layvalues[jj][ii];
	//								else if (newPar->varType == CRHM::Int) {
	//									float x;
	//									instr >> x;
	//									newPar->ilayvalues[jj][ii] = x;
	//								}
	//							}

	//							if (newPar->dim > Cols) {
	//								if (!TooFewHRUs) {
	//									Application->MessageBox("Too few HRUs defined in the Project file",
	//										"parameters not complete", MB_OK);
	//									TooFewHRUs = true;
	//								}

	//								for (int ii = Cols; ii < newPar->dim; ii++)
	//									if (newPar->varType == CRHM::Float)
	//										newPar->layvalues[jj][ii] = newPar->layvalues[jj][ii - 1];
	//									else
	//										newPar->ilayvalues[jj][ii] = newPar->ilayvalues[jj][ii - 1];
	//							}
	//						}
	//					}

	//					if (module == "Shared") {
	//						SetSharedParams(newPar); // copy template parameter to basin module
	//						delete newPar;
	//					}
	//					else
	//						ClassParSet(newPar);  // can delete newPar!
	//				}
	//				else {
	//					CRHMException Except("Unknown Module Parameter: " + string(module) + " -> " + string(param) +
	//						" in " + string(OpenDialogPrj->FileName.c_str()), ERR);
	//					Application->MessageBox(Except.Message.c_str(), "Unknown Parameter in project file", MB_OK);
	//					LogError(Except);

	//					while (c = DataFile.peek(), !isalpha(c) && c != '#')
	//						DataFile.ignore(256, '\n');
	//				}
	//			} // while

	//			DataFile.close();

	//			SqueezeParams(Sender);  // converts identical parameters to shared parameters
	//		}
	//		else
	//			Application->MessageBox(String(FileS + " - file extension not 'prj', 'obs' or 'par'").c_str(), "Problem with File extension", MB_OK);
	//	}
	//	else
	//		Application->MessageBox(String(FileS + " - file does not exist").c_str(), "Problem with File", MB_OK);
	//}  // for param

	//if (PrjAutoRun->Checked) {
	//	if (!Global::OurModulesList->Count && ListBox4->Items->Count) {
	//		if (Last1->Checked) LastRprtObs(Sender);
	//		if (All1->Checked) AllRprtObs(Sender);
	//		if (DebugScreen1->Checked) DebugScreen1Rprt(Sender);
	//		if (SummaryScreen1->Checked) SummaryScreen1Rprt(Sender);
	//	}
	//	else
	//		RunClick(Sender);

	//	Main->Refresh();
	//}

	//CommandLine = false;

	//if (PrjAutoExit->Checked && !PlotControl->IntervalControl && (All1->Checked || Last1->Checked || DebugScreen1->Checked || SummaryScreen1->Checked)) {
	//	Close();
	//}

	//biweekly1->Checked = true; // keep in step with UpdateForm
} // end of FormActivate



//void CRHMmainDlg::OnRun32811()
//{
//	// TODO: Add your command handler code here
//	CRHMmain *test = CRHMmain::getInstance();
//	test->RunClick();
//	test->cdSeries[0];
//
//
//	int seriesIndex = 0;
//	CSeries series1 = tchart.Series(seriesIndex);
//
//	string values = "";
//
//
//	for (int i = 0; i < test->cdSeries[seriesIndex]->MaxCnt; i++)
//	{
//		//series1.AddXY(test->cdSeries[seriesIndex]->XValues[i], test->cdSeries[seriesIndex]->YValues[i], _T(""), 0);
//		series1.AddXY(test->cdSeries[seriesIndex]->XValues[i], test->cdSeries[seriesIndex]->YValues[i], _T(""), 0);
//		//values += " " + (float)(test->cdSeries[seriesIndex]->YValues[i]);
//	}
//	
//	seriesIndex = 1;
//	CSeries series2 = tchart.Series(seriesIndex);
//	for (int i = 0; i < test->cdSeries[seriesIndex]->MaxCnt; i++)
//	{
//		//series2.AddXY(test->cdSeries[seriesIndex]->XValues[i], test->cdSeries[seriesIndex]->YValues[i], _T(""), 0);
//		series2.AddXY(test->cdSeries[seriesIndex]->XValues[i], test->cdSeries[seriesIndex]->YValues[i], _T(""), 0);
//	}
//
//}


void CRHMmainDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here

	CString str, str2;
	int hru_value = 0;

	GetDlgItemText(IDC_EDIT1, str);
	hru_value = _ttoi(str);
	if (str.Trim().GetLength() > 0)
	{
		if (hru_value > 0)
		{
			hru_value = _ttoi(str) - 1;
			str2.Format(_T("%d"), hru_value);
			SetDlgItemText(IDC_EDIT1, str2);
		}
	}
}


void CRHMmainDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here

	CString str, str2;
	int hru_value = 1;

	GetDlgItemText(IDC_EDIT1, str);
	if (str.Trim().GetLength() > 0) { hru_value = _ttoi(str) + 1; }
	str2.Format(_T("%d"), hru_value);
	SetDlgItemText(IDC_EDIT1, str2);
}


void CRHMmainDlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here


	CString str, str2;
	int hru_value = 0;

	GetDlgItemText(IDC_EDIT2, str);
	hru_value = _ttoi(str);
	if (str.Trim().GetLength() > 0)
	{
		if (hru_value > 0)
		{
			hru_value = _ttoi(str) - 1;
			str2.Format(_T("%d"), hru_value);
			SetDlgItemText(IDC_EDIT2, str2);
		}
	}
}



void CRHMmainDlg::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here

	CString str, str2;
	int hru_value = 1;

	GetDlgItemText(IDC_EDIT2, str);
	if (str.Trim().GetLength() > 0) { hru_value = _ttoi(str) + 1; }
	str2.Format(_T("%d"), hru_value);
	SetDlgItemText(IDC_EDIT2, str2);
}


BOOL CRHMmainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here


	SetDlgItemText(IDC_EDIT1, _T("1"));
	SetDlgItemText(IDC_EDIT2, _T("3"));



	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CRHMmainDlg::OnLbnSelchangeList5()
{
	// TODO: Add your control notification handler code here

	CString str;
	CString cstr;

	int index = listbox_varaiables_all.GetCurSel();

	listbox_varaiables_all.GetText(index, str);

	//CString s = listbox_varaiables_all.GetItemData(index);
	CT2A vstr4(str);
	string vstr = vstr4;

	int count = 0;
	for (int i = 0; i < list_box_variables_selected.GetCount(); i++)
	{
		list_box_variables_selected.GetText(i, cstr);
		CT2A s(cstr);
		string str1 = s;
		int ind = str1.find(vstr+"(");

		if (ind > -1) { count++; }
	}
	count++;
	string vstr2 (vstr);
	string vstr1 = vstr2 + "("+to_string(count)+")";

	CString cstr4(vstr1.c_str());


	list_box_variables_selected.AddString(cstr4);
}



void CRHMmainDlg::AddObservationsOnOpenProject()
{
	CString cstr;
	int c = listbox_observations_selected.GetCount();
	for (int i = 0; i < c; i++)
	{
		listbox_observations_selected.GetText(i, cstr);
		CT2A str(cstr);
		string str2 = str;
		AddObservation(str2);
	}
}


void CRHMmainDlg::AddObservation(string obsstr)
{
	CString str(obsstr.c_str());
	int got = 0;

	for (int i = 0; i < observationsFilesCount; i++)
	{
		for (int j = 0; j < observationsFiles[i]->Count; j++)
		{
			CString str1(observationsFiles[i]->Strings[j].c_str());
			if (str1 == str)
			{
				//CString str2(observationsFiles[i][1]);
				CRHMmain * test = CRHMmain::getInstance();


				//char filepath[128];
				//strcpy(filepath, observationsFiles[i][1]);
				char* filepath = const_cast<char*>(observationsFiles[i]->FilePath.c_str());
				char* obsname = const_cast<char*>(observationsFiles[i]->Strings[j].c_str());

				//test->GetObservationData(filepath, obsname);
				//AddSeriesToTChart();

				AddListBox2Click();

				got = 1;
				break;
			}
		}
		if (got == 1) { break; }



		//CString str1(observationsFiles[i][0]);
		//if (str1 == str)
		//{
		//	//CString str2(observationsFiles[i][1]);
		//	CRHMmain * test = CRHMmain::getInstance();


		//	char filepath[128];
		//	strcpy(filepath, observationsFiles[i][1]);

		//	test->GetObservationData(filepath, observationsFiles[i][0]);
		//	AddSeriesToTChart();
		//	break;
		//}
	}

	if (got == 0)
	{
		CRHMmain * test = CRHMmain::getInstance();
		CT2A ss(str);
		string ss2 = ss;
		char* t = const_cast<char*>(ss2.c_str());
		char* t2 = const_cast<char*>(test->OpenNameObs.c_str());
		//test->GetObservationData(t2, t);
		//AddSeriesToTChart();

		CString cstr;
		int count = listbox_observations.GetCount();
		int selindex = 0;
		for (int i = 0; i < count; i++)
		{
			listbox_observations.GetText(i, cstr);
			CT2A obsname(cstr);
			string obsname2 = obsname;
			obsname2 = obsname2 + "(1)";
			if (obsname2 == obsstr)
			{
				listbox_observations.SetCurSel(i);
				selindex = i;
				break;
			}
		}
		AddListBox2Click();
		listbox_observations.SetSel(selindex, false);

	}

}




void CRHMmainDlg::OnLbnSelchangeList2()
{
	// TODO: Add your control notification handler code here

	CString str;
	CString cstr;
	CString extension("(1)");

	int index = listbox_observations.GetCurSel();
	listbox_observations.GetText(index, str);
	str.Append (extension);


	//CString s = listbox_varaiables_all.GetItemData(index);
	CT2A vstr4(str);
	string vstr = vstr4;

	int count = 0;
	for (int i = 0; i < listbox_observations_selected.GetCount(); i++)
	{
		listbox_observations_selected.GetText(i, cstr);
		CT2A s(cstr);
		string str1 = s;
		//int ind = str1.find(vstr);

		if (str1 == vstr) { count = 1; break; }

		//if (ind > -1) { count = 1; break; }
	} 

	if (count == 0)
	{
		listbox_observations_selected.AddString(str);

		int got = 0;
		for (int i = 0; i < observationsFilesCount; i++)
		{
			for (int j = 0; j < observationsFiles[i]->Count; j++)
			{
				CString str1(observationsFiles[i]->Strings[j].c_str());
				if (str1 == str)
				{
					//CString str2(observationsFiles[i][1]);
					CRHMmain * test = CRHMmain::getInstance();


					//char filepath[128];
					//strcpy(filepath, observationsFiles[i][1]);
					char* filepath = const_cast<char*>(observationsFiles[i]->FilePath.c_str());
					char* obsname = const_cast<char*>(observationsFiles[i]->Strings[j].c_str());

					//test->GetObservationData(filepath, obsname);
					//AddSeriesToTChart();

					AddListBox2Click();

					got = 1;
					break;
				}				
			}
			if (got == 1) { break; }



			//CString str1(observationsFiles[i][0]);
			//if (str1 == str)
			//{
			//	//CString str2(observationsFiles[i][1]);
			//	CRHMmain * test = CRHMmain::getInstance();


			//	char filepath[128];
			//	strcpy(filepath, observationsFiles[i][1]);

			//	test->GetObservationData(filepath, observationsFiles[i][0]);
			//	AddSeriesToTChart();
			//	break;
			//}
		}

		if (got == 0)
		{
			CRHMmain * test = CRHMmain::getInstance();
			CT2A ss(str);
			string ss2 = ss;
			char* t = const_cast<char*>(ss2.c_str());
			char* t2 = const_cast<char*>(test->OpenNameObs.c_str());
			//test->GetObservationData(t2, t);
			//AddSeriesToTChart();
			AddListBox2Click();
		}


	}

	//count++;
	//string vstr2(vstr);
	//string vstr1 = vstr2 + "(" + to_string(count) + ")";

	//CString cstr4(vstr1.c_str());

	//if (count == 1)
	//{
	//	listbox_observations_selected.AddString(cstr4);
	//}

}





void CRHMmainDlg::OnBuildConstruct()
{
	// TODO: Add your command handler code here


	CConstruct *build_form;
	TStringList *all_module_list = CRHMmain::getInstance()->getAllmodules();

	build_form = new CConstruct();
	//build_form->UpdateData();

	//	build_form.listbox_all_modules
	//	build_form->listbox_all_modules = new CListBox();

	//build_form->listbox_all_modules.AddString(_T("test"));


	//CListBox *test = &build_form->listbox_all_modules;
	//AddStringListToListBox(test, all_module_list);

	INT_PTR nRet = build_form->DoModal();

	if (build_form->Execute) {
		InitModules();
		build_form->ReadBuildParameters(); // restore original parameters

		dirty = true;
		loadGuiComponents();

		//ReopenProject();
		//ReopenProject();
		OpenObservation(defaultobservationpath);
	}
	
	//CRHMmain *test = CRHMmain::getInstance();
	//int c = test->ListBox1->Count;

}


void CRHMmainDlg::InitModules() {

	ClassVar *thisVar;

	Global::BuildFlag = CRHM::DECL;

	Box1Disply = CRHM::OUTPUT; // Synchronise ListBox1 disply

							   // executes the DECL portion of the declvar/declparam etc. routines
	for (Global::CurrentModuleRun = 0; Global::CurrentModuleRun < Global::OurModulesList->Count; Global::CurrentModuleRun++) {
		((ClassModule*)Global::OurModulesList->Objects[Global::CurrentModuleRun])->nhru = Global::nhru;
		((ClassModule*)Global::OurModulesList->Objects[Global::CurrentModuleRun])->decl();
	}

	//if (LogForm->DeclErrors > 0)
		//Application->MessageBox("Module errors", "Model Declaration Error", MB_OK);
	Label4Click();
}

void CRHMmainDlg::Label4Click() {
	CRHMmain *test = CRHMmain::getInstance();
	CConstruct *buildform = new CConstruct();


	ClassVar *thisVar;
	MapVar::iterator itVar;

	string Labels[] = { "Variables", "Variables by Module", "Diagnostic Variables", "Private Variables" };
	string Newname;

	++Box1Disply;
	if (Box1Disply > CRHM::PRIVATE)
		Box1Disply = CRHM::OUTPUT;

	//Label4->Caption = Labels[Box1Disply];
	test->ListBox1->Clear();

	//Max_Name_Width1 = 0;
	//Max_Name_Width2 = 0;

	//SendMessage(ListBox1->Handle, LB_SETHORIZONTALEXTENT, 150, 0);

	if (Box1Disply == CRHM::OUTPUT) {
		test->ListBox1->Sorted = true;

		for (itVar = Global::MapVars.begin(); itVar != Global::MapVars.end(); itVar++) {
			thisVar = (*itVar).second;
			if (thisVar->varType < CRHM::Read && thisVar->visibility == CRHM::USUAL &&
				thisVar->dimen != CRHM::NREB &&
				(thisVar->values || thisVar->ivalues) && !thisVar->FileData) {
				Newname = DeclObsName(thisVar);
				if (buildform->IndexOf(test->ListBox1, Newname) == -1) {
					test->ListBox1->AddObject(Newname, (TObject*)thisVar);
					//if (Newname.Length() > Max_Name_Width2)
						//Max_Name_Width2 = Newname.Length();
				}
			}
		}
	}
	else {
		test->ListBox1->Sorted = false;
		string S0;

		for (int ii = 0; ii < Global::OurModulesList->Count; ii++) {
			string S = Global::OurModulesList->Strings[ii];
			ClassModule* thisModule = (ClassModule*)Global::OurModulesList->Objects[ii];
			if (S0 != S) {
				if (!(S0.length() == 0))
					test->ListBox1->Add(" ");
				S0 = S;
				if (thisModule->variation != 0) {
					string AA("#0");
					AA[1] += log(thisModule->variation) / log(2) + 1;
					S0 += AA;
				}

				S0 = "   --- " + S0;
				test->ListBox1->AddObject(S0 + " ---", Global::OurModulesList->Objects[ii]);
				//if (S0.Length() > Max_Name_Width1)
					//Max_Name_Width1 = S0.Length();
			}

			switch (Box1Disply) { // display in module/group
			case CRHM::USUAL:
				for (itVar = Global::MapVars.begin(); itVar != Global::MapVars.end(); itVar++) {
					thisVar = (*itVar).second;
					if (S == thisVar->module.c_str() && thisVar->visibility == CRHM::USUAL &&
						Variation_Decide(thisVar->variation_set, thisModule->variation) &&
						thisVar->dimen != CRHM::NREB &&
						(thisVar->values || thisVar->ivalues) && !thisVar->FileData) {

						//if (UpDownOBSIndx->Max < thisVar->lay && thisVar->dimen == CRHM::NDEF)
							//UpDownOBSIndx->Max = thisVar->lay;
						Newname = DeclObsName(thisVar);
						if (buildform->IndexOf(test->ListBox1, Newname) == -1) {
							test->ListBox1->AddObject(Newname, (TObject*)thisVar);
							//if (Newname.length() > Max_Name_Width2)
								//Max_Name_Width2 = Newname.Length();
						}
					}
				}
				break;

			case CRHM::DIAGNOSTIC: // display diagnostic variables
				for (itVar = Global::MapVars.begin(); itVar != Global::MapVars.end(); itVar++) {
					thisVar = (*itVar).second;
					if (S == thisVar->module.c_str() && thisVar->visibility == CRHM::DIAGNOSTIC &&
						thisVar->dimen != CRHM::NREB &&
						(thisVar->values || thisVar->ivalues) && !thisVar->FileData) {
						Newname = DeclObsName(thisVar);
						if (buildform->IndexOf(test->ListBox1, Newname) == -1) {
							test->ListBox1->AddObject(Newname, (TObject*)thisVar);
							//if (Newname.Length() > Max_Name_Width2)
								//Max_Name_Width2 = Newname.Length();
						}
					}
				}
				break;

			case CRHM::PRIVATE: // display Local variables
				for (itVar = Global::MapVars.begin(); itVar != Global::MapVars.end(); itVar++) {
					thisVar = (*itVar).second;
					if (S == thisVar->module.c_str() && thisVar->visibility == CRHM::PRIVATE &&
						thisVar->dimen != CRHM::NREB &&
						(thisVar->values || thisVar->ivalues) && !thisVar->FileData) {
						Newname = DeclObsName(thisVar);
						if (buildform->IndexOf(test->ListBox1, Newname) == -1) {
							test->ListBox1->AddObject(Newname, (TObject*)thisVar);
							//if (Newname.Length() > Max_Name_Width2)
								//Max_Name_Width2 = Newname.Length();
						}
					}
				}
				break;
			}
		} // for
	} // else

	//if (Max_Name_Width2 > Max_Name_Width1) // Max_Name_Width1 only title, Max_Name_Width2 handles other names
		//Max_Name_Width1 = Max_Name_Width2;

	//if (Max_Name_Width1 * 6 > ListBox1->Width)
		//ListBox1->ScrollWidth = Max_Name_Width1 * 6;

	//Max_Name_Width1 += 5; // handles array element

	//if (Max_Name_Width2 * 6 > ListBox3->Width)
		//ListBox3->ScrollWidth = Max_Name_Width1 * 6;

	//if (Max_Name_Width2 * 6 > ListBox2->Width)
		//ListBox2->ScrollWidth = Max_Name_Width2 * 6;

	//Max_Name_Width2 += 5; // handles array element

	//if (Max_Name_Width2 * 6 > ListBox4->Width)
		//ListBox4->ScrollWidth = Max_Name_Width2 * 6;

}


string CRHMmainDlg::DeclObsName(ClassVar *thisVar) {

	string Newname = thisVar->name.c_str();
	int jj = Global::OurModulesList->IndexOf(thisVar->module.c_str()); // find module
	int found = Newname.find("#");
	if (jj > -1 && found > 0) { // -1 for "obs" and "#" for declared "obs"
		ClassModule* thisModule = (ClassModule*)Global::OurModulesList->Objects[jj];
		if (thisModule->isGroup) { // if group add suffix
			string AA;
			Common::GroupEnding(AA, thisModule->GroupCnt);
			Newname += AA;
		}
	}
	return Newname;
}

bool CRHMmainDlg::Variation_Decide(int Variation_set, long Variation) {

	VandP V; V.Set(Variation);

	long variations = V.GetV(); // & 2047;

	if ((Variation_set & 2048) != 0 && variations == 0 || // handles VARIATION_0
		(Variation_set & 4096) != 0 ||                        // handles VARIATION_0
		(Variation_set == 0) ||                               // handles VARIATION_ORG
		(variations & Variation_set) != 0)                 // handles #1, #2, #4 etc.
		return true;
	else
		return false;
}


void CRHMmainDlg::OnRunRunmodel()
{
	CRHMmain * crhmmain = CRHMmain::getInstance();
	TStringList *tslist = crhmmain->getSelectedObservations();

	//tchart.RemoveAllSeries();

	//CRHMmain * crhmmain = CRHMmain::getInstance();
	SaveProject();
	RunClickFunctionOld();

}


void CRHMmainDlg::RunClickFunctionOld()
{
	CRHMmain *test = CRHMmain::getInstance();

	if (test->ObsFilesList->Count == 0)
	{
		MessageBox(_T("Please open an observation file."));
		return;
	}

	test->RunClick();
	int seriesCount = test->SeriesCnt;

	if (seriesCount == 0) { return; }

	CSeries series[100];
	for (int i = 0; i < seriesCount; i++)
	{
		string s = test->getSelectedVariables()->Strings[i];
		CString cs(s.c_str());

		int removed = 0;

		//if a variable is preexisting, we need to delete that variable.
		int cnt = tchart.get_SeriesCount();
		for (int j = 0; j < cnt; j++)
		{
			if (tchart.SeriesTitleLegend(j) == cs)
			{
				tchart.RemoveSeries(j);
				removed = 1;
				break;
			}
		}

		tchart.AddSeries(0);
		if (removed == 1) { series[i] = tchart.Series(cnt-1); }
		else { series[i] = tchart.Series(cnt); }
		
		series[i].put_LegendTitle(cs);
	}

	string values = "";
	int seriesIndex = 0;
	int TotalCount = test->cdSeries[0]->MaxCnt;

	for (int i = 0; i < TotalCount; i++)
	{
		for (int j = 0; j < seriesCount; j++)
		{
			int y, m, d, h, mi;
			StandardConverterUtility::GetDateTimeElements(test->cdSeries[j]->XValues[i], &y, &m, &d, &h, &mi);
			string dt = to_string(m) + "/" + to_string(d) + "/" + to_string(y);
			CString str(dt.c_str());
			LPCTSTR dtstr = (LPCTSTR)str;
			series[j].AddXY(test->cdSeries[j]->XValues[i], test->cdSeries[j]->YValues[i], dtstr, series[j].get_Color());
		}
		if (i % 500 == 0) { tchart.Repaint(); }
	}
	tchart.Repaint();



	//int j = 0;
	//for (int i = 1; i <= Iterations; i++)
	//{
	//	if (j >= TotalCount) { break; }
	//	for (int k = 0; k < PerIteration; k++)
	//	{
	//		if (j >= TotalCount) { break; }
	//		for (int n = 0; n < seriesCount; n++)
	//		{
	//			int y, m, d, h, mi;
	//			StandardConverterUtility::GetDateTimeElements(test->cdSeries[n]->XValues[j], &y, &m, &d, &h, &mi);
	//			string dt = to_string(m) + "/" + to_string(d) + "/" + to_string(y);
	//			CString str(dt.c_str());
	//			LPCTSTR dtstr = (LPCTSTR)str;

	//			series[n].AddXY(test->cdSeries[n]->XValues[j], test->cdSeries[n]->YValues[j], dtstr, series[n].get_Color());
	//		}
	//		j++;
	//	}
	//	tchart.Repaint();
	//}
}


void CRHMmainDlg::AddSeriesToTChart(TSeries * tseries)
{
	//CRHMmain * test = CRHMmain::getInstance();

	CSeries series;
	tchart.AddSeries(0);
	int count = tchart.get_SeriesCount();
	series = tchart.Series(count - 1);

	CString cstr(tseries->Title.c_str());
	//CString extension("(1)");
	//cstr.Append(extension);

	series.put_LegendTitle(cstr);

	for (int i = 0; i < tseries->Used; i++)
	{
		int y, m, d, h, mi;
		StandardConverterUtility::GetDateTimeElements(tseries->XValues[i], &y, &m, &d, &h, &mi);
		string dt = to_string(m) + "/" + to_string(d) + "/" + to_string(y);
		CString str(dt.c_str());
		LPCTSTR dtstr = (LPCTSTR)str;
		series.AddXY(tseries->XValues[i], tseries->YValues[i], dtstr, series.get_Color());

		//if (i % 500 == 0) { tchart.Repaint(); }		
	}
	tchart.Repaint();
}


void CRHMmainDlg::AddSeriesToTChart()
{
	CRHMmain * test = CRHMmain::getInstance();

	CSeries series;
	tchart.AddSeries(0);
	int count = tchart.get_SeriesCount();
	series = tchart.Series(count-1);

	CString cstr(test->observationseries->Title.c_str());

	series.put_LegendTitle(cstr);

	for (int i = 0; i < test->observationseries->Count(); i++)
	{
		int y, m, d, h, mi;
		StandardConverterUtility::GetDateTimeElements(test->observationseries->XValues[i], &y, &m, &d, &h, &mi);
		string dt = to_string(m) + "/" + to_string(d) + "/" + to_string(y);
		CString str(dt.c_str());
		LPCTSTR dtstr = (LPCTSTR)str;

		series.AddXY(test->observationseries->XValues[i], test->observationseries->YValues[i], dtstr, series.get_Color());
	}
	tchart.Repaint();

}


//running the console crhm version for getting chart data and drawing the chart.
void CRHMmainDlg::RunClickFunction()
{
	CRHMmain *test = CRHMmain::getInstance();
	string commandstring = "Console_CRHM_Visual_Studio.exe 2 Console_CRHM_Visual_Studio.exe " + test->OpenProjectPath;
	system(commandstring.c_str());


	float data[10000][10];
	int count = 0;


	FILE * file = fopen("CRHM_output_1.obs", "r");
	if (file != NULL)
	{
		int got = 0;
		char line[128]; /* or other suitable maximum line size */
		while (fgets(line, sizeof line, file) != NULL) /* read a line */
		{
			if (line[0] == '#')
			{
				got = 1; break;
			}
		}

		float date, v;

		int seriesCount = test->ListBox3->Count;


		CSeries series[100];
		for (int i = 0; i < seriesCount; i++)
		{
			tchart.AddSeries(0);
			series[i] = tchart.Series(i);
			string s = test->ListBox3->Strings[i];
			CString cs(s.c_str());
			series[i].put_LegendTitle(cs);
		}


		string values = "";
		int seriesIndex = 0;
		int brk = 0;

		while (1)
		{
			for (int j = 1; j <= 50; j++)
			{
				if (fscanf(file, "%f", &date) != EOF)
				{
					for (int n = 0; n < seriesCount; n++)
					{
						fscanf(file, "%f", &v);
						int y, m, d, h, mi;
						StandardConverterUtility::GetDateTimeElements(date, &y, &m, &d, &h, &mi);
						string dt = to_string(m) + "/" + to_string(d) + "/" + to_string(y);
						CString str(dt.c_str());
						LPCTSTR dtstr = (LPCTSTR)str;

						series[n].AddXY(date, v, dtstr, series[n].get_Color());
					}
				}
				else { brk = 1; break; }
			}
			if (brk == 1) { break; }
			tchart.Repaint();
		}
		fclose(file);
	}

}



void CRHMmainDlg::OnFileSaveAs()
{
	// TODO: Add your command handler code here
	TCHAR szFilters[] = _T("MyType Files (*.prj)|*.prj|All Files (*.*)|*.*||");

	//	CFileDialog fileDlg = CFileDialog(TRUE, _T(".txt"), NULL, 0, strFilter);

	CRHMAboutBox *test;
	
	test = new CRHMAboutBox();
	//CWnd *label = test->GetDlgItem(IDC_STATIC);


	CString fileName;
	wchar_t* p = fileName.GetBuffer(FILE_LIST_BUFFER_SIZE);
	//CFileDialog fileDlg(TRUE);
	//OPENFILENAME& ofn = fileDlg.GetOFN();


	CFileDialog fileDlg(FALSE, _T("prj"), _T("*.prj"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);


	bool result = fileDlg.DoModal();

	if (result)
	{
		//	MessageBox(0, dlg->GetPathName(), L"Draw", 0);

		//TStringList *obs = readFromListbox(listbox_observations_selected);
		//CRHMmain::getInstance()->setSelectedObservatoions();


		CRHMmain * crhmmain = CRHMmain::getInstance();

		//setting the dialog contents to CRHMmain.
		COleDateTime time1;
		DateTimePicker1.GetTime(time1);
		crhmmain->setStartDate(time1);

		COleDateTime time2;
		DateTimePicker2.GetTime(time2);
		crhmmain->setEndDate(time2);

		

		TStringList * selected_variables = GetSelectedVariablesFromListBox(IDC_LIST3);
		TStringList * selected_observations = GetSelectedObservationsFromListBox(IDC_LIST4);
		crhmmain->setSelectedVariables(selected_variables);
		crhmmain->setSelectedObservatoions(selected_observations);

		fileName = fileDlg.GetPathName();
		string filepath = CT2A(fileName.GetString());
		crhmmain->SaveProject("Description - to be added", filepath);

		MessageBox(_T("Your project has been saved."));
		OnFileClose();
	}
}



//This function returns all items from a listbox.
TStringList * CRHMmainDlg::GetSelectedVariablesFromListBox(int listboxid)
{
	TStringList * list = new TStringList();	
	CListBox * listbox = (CListBox*)GetDlgItem(listboxid);

	int itemcount = listbox->GetCount();
	for (int i = 0; i < itemcount; i++)
	{
		CString cstr;
		listbox->GetText(i, cstr);
		CT2A str(cstr);
		string s = str;

		//list->Add(s);
		TObject * obj = GetObjectOfVariable(s);
		list->AddObject(s, obj);
	}
	return list;
}


TStringList * CRHMmainDlg::GetSelectedObservationsFromListBox(int listboxid)
{
	TStringList * list = new TStringList();
	CListBox * listbox = (CListBox*)GetDlgItem(listboxid);

	int itemcount = listbox->GetCount();
	for (int i = 0; i < itemcount; i++)
	{
		CString cstr;
		listbox->GetText(i, cstr);
		CT2A str(cstr);
		string s = str;

		//list->Add(s);
		TObject * obj = GetObjectOfObservation(s);
		list->AddObject(s, obj);
	}
	return list;
}




//This function sets the items to a listbox.
void CRHMmainDlg::SetItemsToListBox(int listboxid, TStringList * list)
{
	CListBox * listbox = (CListBox*)GetDlgItem(listboxid);
	int count = list->Count;

	for (int i = 0; i < count; i++)
	{
		string str = list->array[i].get_Name();
		CString cstr(str.c_str());
		listbox->AddString(cstr);
	}
}


void CRHMmainDlg::OnBnClickedButton9()
{
	// TODO: Add your control notification handler code here


	//CRHMAboutBox aboutbox;

	CRHMmain * test = CRHMmain::getInstance();
	test->GetObservationData("Badlake73_76.obs", "rh");
	AddSeriesToTChart();


	//test->GetCaptionFromAboutBox();


	//TStringList * t = GetItemsFromListBox(IDC_LIST3);
	//SetItemsToListBox(IDC_LIST3, t);
	
	//t = StandardConverterUtility::GetItemsFromListBox((CListBox*)GetDlgItem(IDC_LIST3));
}


void CRHMmainDlg::OnFileSave()
{
	// TODO: Add your command handler code here

	SaveProject();
}


void CRHMmainDlg::SaveProject()
{
	CRHMmain * crhmmain = CRHMmain::getInstance();

	//setting the dialog contents to CRHMmain.
	COleDateTime time1;
	DateTimePicker1.GetTime(time1);
	crhmmain->setStartDate(time1);

	COleDateTime time2;
	DateTimePicker2.GetTime(time2);
	crhmmain->setEndDate(time2);

	TStringList * selected_variables = GetSelectedVariablesFromListBox(IDC_LIST3);
	TStringList * selected_observations = GetSelectedObservationsFromListBox(IDC_LIST4);
	crhmmain->setSelectedVariables(selected_variables);
	crhmmain->setSelectedObservatoions(selected_observations);

	if (crhmmain->OpenProjectPath.length() == 0) 
	{ 
		crhmmain->OpenProjectPath = defaultprojectpath; 
		//OnFileSaveAs();
	}

	crhmmain->SaveProject("Description - to be added", crhmmain->OpenProjectPath);
	
}



TObject * CRHMmainDlg::GetObjectOfVariable(string vname)
{
	CRHMmain * t = CRHMmain::getInstance();

	for (int i = 0; i < t->ListBox1->Count; i++)
	{
		string str = t->ListBox1->Strings[i];

		int ind = vname.find(str+"(");

		if (ind > -1)
		//if (t->ListBox1->Strings[i] == vname)
		{
			TObject * obj = t->ListBox1->Objects[i];
			return obj;
		}
	}
}

TObject * CRHMmainDlg::GetObjectOfObservation(string vname)
{
	CRHMmain * t = CRHMmain::getInstance();

	for (int i = 0; i < t->ListBox2->Count; i++)
	{
		string str = t->ListBox2->Strings[i];

		int ind = vname.find(str+"(");

		if (ind > -1)
			//if (t->ListBox1->Strings[i] == vname)
		{
			ClassVar * thisVar;;
			thisVar = (ClassVar *)t->ListBox2->Objects[i];
			TObject * obj = (TObject*)thisVar;
			return obj;
		}
	}
}


void CRHMmainDlg::OnFileClose()
{
	// TODO: Add your command handler code here

	CloseProject();
}

void CRHMmainDlg::CloseProject()
{
	CRHMmain * t = CRHMmain::getInstance();


	listbox_varaiables_all.ResetContent();
	ASSERT(listbox_varaiables_all.GetCount() == 0);
	t->ListBox1->Clear();

	list_box_variables_selected.ResetContent();
	ASSERT(list_box_variables_selected.GetCount() == 0);
	t->ListBox3->Clear();

	listbox_observations.ResetContent();
	ASSERT(listbox_observations.GetCount() == 0);
	t->ListBox2->Clear();

	listbox_observations_selected.ResetContent();
	ASSERT(listbox_observations_selected.GetCount() == 0);
	t->ListBox4->Clear();

	COleDateTime oletimeTime(StandardConverterUtility::GetCurrentDateTime());
	VERIFY(DateTimePicker1.SetTime(oletimeTime));
	VERIFY(DateTimePicker2.SetTime(oletimeTime));

	t->FormDestroy();
	t->FormCreate();

	this->EndDialog(1);

	CRHMmainDlg maindlg = new CRHMmainDlg();
	maindlg.DoModal();
}

void CRHMmainDlg::ReopenProject()
{
	//CloseProject();
	SaveProject();
	OpenProject(defaultprojectpath, defaultprojectname);
	//SaveProject();
}

//double clicking an item in the selected variable list box will delete that item.
void CRHMmainDlg::OnLbnDblclkList3()
{
	// TODO: Add your control notification handler code here


	CString str;
	CString cstr;
	string svariable;

	int index = list_box_variables_selected.GetCurSel();
	list_box_variables_selected.GetText(index, str);

	CT2A vstr4(str);
	string vstr = vstr4;

	//deleting the corresponding series from tchart.

	int scount = tchart.get_SeriesCount();
	for (int i = 0; i < scount; i++)
	{
		CString title = tchart.SeriesTitleLegend(i);
		if (title == str) { tchart.RemoveSeries(i); break; }
	}

	int c = list_box_variables_selected.GetCount();

	for (int i = c - 1; i >= 0; i--)
	{
		list_box_variables_selected.GetText(i, cstr);
		CT2A s(cstr);
		string str1 = s;

		if (str1.c_str() == str)
		{
			list_box_variables_selected.DeleteString(i);
			break;
		}
	}





	/*
	CString str;
	CString cstr;
	string svariable;

	int index = list_box_variables_selected.GetCurSel();
	list_box_variables_selected.GetText(index, str);
	
	CT2A vstr4(str);
	string vstr = vstr4;

	int count = 0;
	for (int i = 0; i < listbox_varaiables_all.GetCount(); i++)
	{
		listbox_varaiables_all.GetText(i, cstr);
		CT2A s(cstr);
		string str1 = s;
		int ind = vstr.find(str1);

		if (ind > -1) { svariable = str1; break; }
	}

	int c = list_box_variables_selected.GetCount();

	for (int i = c - 1; i >= 0; i--)
	{
		list_box_variables_selected.GetText(i, cstr);
		CT2A s(cstr);
		string str1 = s;
		int ind = str1.find(svariable);
		if (ind > -1)
		{
			list_box_variables_selected.DeleteString(i);
			break;
		}
	}
	*/
}

//double clicking an item in the selected observations list box will delete that item.
void CRHMmainDlg::OnLbnDblclkList4()
{
	// TODO: Add your control notification handler code here

	CString str;
	CString cstr;
	string svariable;

	int index = listbox_observations_selected.GetCurSel();
	listbox_observations_selected.GetText(index, str);

	CT2A vstr4(str);
	string vstr = vstr4;

	//deleting the corresponding series from tchart.

	int scount = tchart.get_SeriesCount();
	for (int i = 0; i < scount; i++)
	{
		CString title = tchart.SeriesTitleLegend(i);
		if (title == str) { tchart.RemoveSeries(i); break; }
	}


	/*
	int count = 0;
	for (int i = 0; i < listbox_observations.GetCount(); i++)
	{
		listbox_observations.GetText(i, cstr);
		CT2A s(cstr);
		string str1 = s;
		int ind = vstr.find(str1);

		if (ind > -1) { svariable = str1; break; }
	}

	

	svariable = svariable + "(1)";
	*/

	int c = listbox_observations_selected.GetCount();

	for (int i = c - 1; i >= 0; i--)
	{
		listbox_observations_selected.GetText(i, cstr);
		CT2A s(cstr);
		string str1 = s;

		if (str1.c_str() == str)
		{
			listbox_observations_selected.DeleteString(i);
			break;
		}

		//int ind = str1.find(svariable);
		//if (ind > -1)
		//{
		//}
	}
}

//Flip-Tic functionality is implemented here.
void CRHMmainDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here

	CSeries series;
	bool active;

	int count = tchart.get_SeriesCount();

	for (int i = 0; i < count; i++)
	{
		series = tchart.Series(i);
		active = series.get_Active();
		series.put_Active(!active);
	}
}


void CRHMmainDlg::OnOpenObservation()
{
	// TODO: Add your command handler code here
	CFile theFile;
	TCHAR szFilters[] = _T("MyType Files (*.obs)|*.obs|All Files (*.*)|*.*||");

	CString fileName;
	wchar_t* p = fileName.GetBuffer(FILE_LIST_BUFFER_SIZE);

	CFileDialog fileDlg(TRUE, _T("obs"), _T("*.obs"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (fileDlg.DoModal() == IDOK)
	{
		CString filepath = fileDlg.GetPathName();

		CString filename = fileDlg.GetFileName();


		string file_p = CT2A(filepath.GetString());
		string file_n = CT2A(filename.GetString());

		OpenObservation(file_p);


		//CRHMmain *test = CRHMmain::getInstance();
		//test->OpenObsFile(file_p);

		//TStringList * observations = test->getObservations();
		//for (int i = 0; i < observations->Count; i++)
		//{
		//	CString cstr(observations->Strings[i].c_str());
		//	listbox_observations.AddString(cstr);
		//}

		////CRHMmain *test = CRHMmain::getInstance();
		//SetTime(test->GetStartDate(), test->GetEndDate());
		//SaveProject();

		//char* t = const_cast<char*>(file_p.c_str());
		//test->GetObservationNames(t);
		//char line[128];
		//FILE *fp = fopen("datatransfer.txt", "r");
		//observationsFiles[observationsFilesCount] = new TStringList();
		//while (fgets(line, sizeof line, fp) != NULL)
		//{			
		//	int l = strlen(line);
		//	if (line[l - 1] == '\n') { line[l - 1] = '\0'; }

		//	CString cstr(line);
		//	listbox_observations.AddString(cstr);

		//	string str(line);
		//	observationsFiles[observationsFilesCount]->AddObject(str, NULL);
		//}
		//fclose(fp);
		//observationsFiles[observationsFilesCount]->FilePath = file_p;
		//observationsFilesCount++;
	}
	
}

void CRHMmainDlg::OpenObservation(string obsfilepath)
{
	CRHMmain *test = CRHMmain::getInstance();
	test->OpenObsFile(obsfilepath);

	TStringList * observations = test->getObservations();
	for (int i = 0; i < observations->Count; i++)
	{
		CString cstr(observations->Strings[i].c_str());
		listbox_observations.AddString(cstr);
	}

	//CRHMmain *test = CRHMmain::getInstance();
	SetTime(test->GetStartDate(), test->GetEndDate());
	SaveProject();
	ReopenProject();
}


void CRHMmainDlg::OnCloseAllObservations()
{
	// TODO: Add your command handler code here

	listbox_observations.ResetContent();
	listbox_observations_selected.ResetContent();
	tchart.RemoveAllSeries();	
}


void CRHMmainDlg::AddListBox2Click()
{
	CRHMmain * test = CRHMmain::getInstance();

	//TComponent *pComponent = PopupListBox2->PopupComponent;
	//if (pComponent) {
		//if ((pComponent->ClassNameIs("TListBox") && pComponent->Name == "ListBox2") || (pComponent->ClassNameIs("TLabel") && pComponent->Name == "LabelFunct")) {

			
			//ListBox2 = listbox_observations

			int StartCnt = listbox_observations.GetCount();;
			for (int ii = 0; ii < StartCnt; ++ii) {


				int selection = listbox_observations.GetCurSel();

				//Selected[ii]
				if (selection > -1 && selection == ii) {

					//int Indx = UpDownOBSIndx->Position;
					int Indx = 1;
					int IndxMax = ((ClassVar *)test->ListBox2->Objects[ii])->cnt;
					if (Indx > IndxMax) Indx = IndxMax;

					string S = test->ListBox2->Strings[ii] + "(" + to_string(Indx) + ")" + test->Sstrings[test->Funct];
					string seriesTitle = S; // test->ListBox2->Strings[ii];

					int iii = test->ListBox4->IndexOf(test->ListBox2->Strings[ii]);

					if (iii == -1) {
						TSeries *cdSeries = NULL;

						if (((ClassVar *)test->ListBox2->Objects[ii])->FileData->Times == NULL)
						{
							//cdSeries = new TLineSeries(this);
							cdSeries = new TSeries(50000);
						}
						else
						{
							//cdSeries = (TLineSeries *) new TPointSeries(this);
							cdSeries = new TSeries(50000);
						}

						ClassVar* thisVar = (ClassVar*)test->ListBox2->Objects[ii];
						cdSeries->Tag = (int)thisVar; // ClassVar*
						//cdSeries->VertAxis = (TVertAxis)thisVar->TchrtOpt;

						//cdSeries->XValues->DateTime = TRUE;
						//CurrentColor = colours[ii % ncolours];
						//cdSeries->Color = CurrentColor;
						//cdSeries->ParentChart = Chart;
						//cdSeries->Title = S;
						cdSeries->Title = seriesTitle;
						test->ListBox4->AddObject(S, (TObject *)cdSeries);
						AddObsPlot((ClassVar *)test->ListBox2->Objects[ii], cdSeries, S, test->Funct);

					} // if
				}  // if
			} //for
		//}
	//}
}
//


//-------------added by Manishankar -------------------

void CRHMmainDlg::AddDataToSeries(CSeries series, double xvalue, double yvalue)
{
	int y, m, d, h, mi;
	StandardConverterUtility::GetDateTimeElements(xvalue, &y, &m, &d, &h, &mi);
	string dt = to_string(m) + "/" + to_string(d) + "/" + to_string(y);
	CString str(dt.c_str());
	LPCTSTR dtstr = (LPCTSTR)str;

	series.AddXY(xvalue, yvalue, dtstr, series.get_Color());
}

//-----------------------------------------------------




void CRHMmainDlg::AddObsPlot(ClassVar *thisVar, TSeries *cdSeries, string S, CRHM::TFun Funct) {

	CRHMmain * test = CRHMmain::getInstance();



	//---------------added by Manishankar

	//CSeries series;
	//tchart.AddSeries(0);
	//int count = tchart.get_SeriesCount();
	//series = tchart.Series(count - 1);

	//CString cstr(cdSeries->Title.c_str());

	//series.put_LegendTitle(cstr);


	//int y, m, d, h, mi;
	//StandardConverterUtility::GetDateTimeElements(tseries->XValues[i], &y, &m, &d, &h, &mi);
	//string dt = to_string(m) + "/" + to_string(d) + "/" + to_string(y);
	//CString str(dt.c_str());
	//LPCTSTR dtstr = (LPCTSTR)str;

	//series.AddXY(tseries->XValues[i], tseries->YValues[i], dtstr, series.get_Color());

	//-------------------------------------




	ClassVar *newVar;
	Global::HRU_OBS = Global::HRU_OBS_DIRECT; // always correct?

	//Chart->Axes->Items[2]->Title->Caption = thisVar->units.c_str();

	float **Data = thisVar->FileData->Data;
	double xx;
	double DTstartR = test->GetStartDate();
	double DTendR = test->GetEndDate();

	SetTime(DTstartR, DTendR);

	if (DTstartR >= DTendR) return;

	TDateTime Save_DTnow = Global::DTnow; // Save

	double MyInterval = thisVar->FileData->Interval;
	long DTmin = INT((DTstartR - Global::DTstart)* thisVar->FileData->Freq)*Global::Freq / thisVar->FileData->Freq;
	long DTmax = INT((DTendR - Global::DTstart)* thisVar->FileData->Freq)*Global::Freq / thisVar->FileData->Freq;






	//long jj1 = S.LastDelimiter("(");
	long jj1 = S.rfind("(");
	//long jj2 = S.LastDelimiter(")");
	long jj2 = S.rfind(")");

	long Indx;
	string::size_type pp;
	pp = thisVar->name.rfind('(');
	bool AlreadyIndex = (pp != string::npos); // handles exported variables in Obs files

	if (test->ListHruNames && thisVar->varType < CRHM::Read) // using names
		Indx = test->ListHruNames->IndexOf(S.substr(jj1 + 1, jj2 - jj1 - 1)) - 1;
	else {
		if (thisVar->root != "" || AlreadyIndex)
			Indx = 0; // (non observations always 0
		else
			Indx = stoi(S.substr(jj1 + 1, jj2 - jj1 - 1)) - 1;
	}

	long IndxMin = thisVar->FileData->IndxMin;
	long IndxMax = thisVar->FileData->IndxMax;

	if (thisVar->FileData->Times != NULL) { // display sparse data
		if (Global::Freq == 1)
			--DTendR;

		float Sum = 0.0;

		for (long ii = 0; ii < thisVar->FileData->Lines; ++ii) {
			if (thisVar->FileData->Times[ii] < DTstartR) continue;
			if (thisVar->FileData->Times[ii] > DTendR) continue;

			xx = Data[thisVar->offset + Indx][ii];

			if (Funct == CRHM::TOT) {
				Sum += xx;
				xx = Sum;
			}

			cdSeries->AddXY(thisVar->FileData->Times[ii], xx);
			//AddDataToSeries(series, thisVar->FileData->Times[ii], xx);
		}
	}

	else if (Funct <= CRHM::MJ_W) { // display simple observations

		//if (MyInterval == 1) cdSeries->Stairs = true;

		for (Global::DTindx = DTmin; Global::DTindx < DTmax; Global::DTindx++) {
			Global::DTnow = Global::DTstart + Global::Interval*Global::DTindx + Global::Interval;

			if (MyInterval >= 1) --Global::DTnow;

			if (Global::DTindx*thisVar->FileData->Freq / Global::Freq >= IndxMin
				&& Global::DTindx*thisVar->FileData->Freq / Global::Freq <= IndxMax) {




				xx = Data[thisVar->offset + Indx][(Global::DTindx*thisVar->FileData->Freq / Global::Freq - IndxMin)];
				//xx = Data[Indx][(Global::DTindx*thisVar->FileData->Freq / Global::Freq - IndxMin)];

				if (xx > 0)
				{
					int a = 10;
				}


				if (Funct == CRHM::FOBS)
					; // do nothing
				else if (Funct == CRHM::VP_SAT) {
					if (xx > 0.0)
						xx = 0.611 * exp(17.27*xx / (xx + 237.3));
					else
						xx = 0.611 * exp(21.88*xx / (xx + 265.5));
				}
				else if (Funct == CRHM::W_MJ)
					xx *= thisVar->FileData->Interval * 86400 / 1.0E6;
				else if (Funct == CRHM::MJ_W)
					xx *= 1.0E6 / 86400 / thisVar->FileData->Interval;

				cdSeries->AddXY(Global::DTnow, xx);
				//AddDataToSeries(series, Global::DTnow, xx);
			}
		}
	}
	else { // display observations functions
		//cdSeries->Stairs = true;
		// N.B. object FileData copied. If Obs function - Obs deletes else if VarObsFunct ListBox4 deletes.
		newVar = new ClassVar(*thisVar);

		newVar->name = S.c_str();

		newVar->FileData->DataFileName = "Copy";


		string::size_type pp = thisVar->units.find_last_of(")");

		if (thisVar->FileData->Freq > 1 && (thisVar->units[pp - 1] == 'd'))   //  || TBase == 0
			thisVar->Daily = TRUE;
		else
			thisVar->Daily = FALSE;

		if (newVar->root == "") { // Observation
			if (thisVar->FileData->Freq == 1)
				newVar->LoopFunct = &ClassVar::LoopFirst;
			else if (thisVar->Daily)
				newVar->LoopFunct = &ClassVar::LoopFirst;
			else
				newVar->LoopFunct = &ClassVar::LoopRange;
		}
		else { // Variable
			if (thisVar->Daily)
				newVar->LoopFunct = &ClassVar::LoopLast;
			else
				newVar->LoopFunct = &ClassVar::LoopRange;
		}

		newVar->FunctVar = thisVar;

		switch (Funct) {
		case CRHM::AVG:
			newVar->UserFunct_ = &ClassVar::Tot_;
			newVar->FunKind = CRHM::AVG;
			break;
		case CRHM::MIN:
			newVar->UserFunct_ = &ClassVar::Min_;
			newVar->FunKind = CRHM::MIN;
			break;
		case CRHM::MAX:
			newVar->UserFunct_ = &ClassVar::Max_;
			newVar->FunKind = CRHM::MAX;
			break;
		case CRHM::TOT:
			newVar->UserFunct_ = &ClassVar::Tot_;
			newVar->FunKind = CRHM::TOT;
			break;
		case CRHM::POS:
			newVar->UserFunct_ = &ClassVar::Pos_;
			newVar->FunKind = CRHM::POS;
			break;
		case CRHM::FIRST:
			newVar->UserFunct_ = &ClassVar::First_;
			newVar->FunKind = CRHM::FIRST;
			newVar->LoopFunct = &ClassVar::LoopFirst;
			break;
		case CRHM::LAST:
			newVar->UserFunct_ = &ClassVar::Last_;
			newVar->FunKind = CRHM::LAST;
			newVar->LoopFunct = &ClassVar::LoopLast;
			break;
		case CRHM::CNT:
			newVar->UserFunct_ = &ClassVar::Count_;
			newVar->FunKind = CRHM::CNT;
			break;
		case CRHM::CNT0:
			newVar->UserFunct_ = &ClassVar::Count0_;
			newVar->FunKind = CRHM::CNT0;
			break;
		case CRHM::DLTA:
			newVar->UserFunct_ = &ClassVar::First_;
			newVar->LoopFunct = &ClassVar::LoopFirst;
			newVar->FunKind = CRHM::DLTA;
			break;
		} // switch

		bool First = false;
		long Next = -1;
		long Days = 0;
		long LastDays = 0;
		long Lastkk = 0;
		long CurrentIndx = -1;
		long LastIndex = -1;
		long itime[6];
		long Greatest;
		long DTminX = DTmin;
		if (IndxMin > 0)
			DTminX = IndxMin;
		float Delta0 = 0.0;
		float First0;
		float Temp;
		dattim("now", itime);

		for (Global::DTindx = DTmin; Global::DTindx < DTmax; Global::DTindx += Global::Freq) {
			Global::DTnow = Global::DTstart + Global::Interval*Global::DTindx + Global::Interval;

			if (Global::DTindx*Global::Freq / thisVar->FileData->Freq >= IndxMin)
				if (Global::DTindx*thisVar->FileData->Freq / Global::Freq > IndxMax)
					break;
				else {
					if (Global::Interval >= 1) --Global::DTnow;

					dattim("now", itime);

					switch (TBase) {

					case 0: // daily
						if (Next == -1 || Next != itime[2]) {
							Next = itime[2];
							First = TRUE;
						}
						break;
					case 1: // water annual
						if (Next == -1 || itime[0] == Next && itime[1] == water_year_month) {
							if (Next == -1 && itime[1] < water_year_month)
								Next = itime[0];
							else
								Next = itime[0] + 1;
							First = TRUE;
						}
						break;
					case 2: // annual
						if (Next == -1 || itime[0] == Next && itime[1] == 1) {
							Next = itime[0] + 1;
							First = TRUE;
						}
						break;
					case 3: // monthly
						if (Next == -1 || Next == itime[1]) {
							Next = (itime[1]) % 12 + 1;
							First = TRUE;
						}
						break;
					case 4: // All - do nothing
						if (Next == -1) {
							Next = 0;
							First = TRUE; // do nothing
						}
					} // switch

					CurrentIndx = (Global::DTindx - DTminX) / thisVar->FileData->Freq - 1;

					if (First) {
						if (Global::DTindx > DTmin && Global::DTindx > IndxMin) { // Handle RUN starting after beginning of primary obs file and secondary obs file later
							switch (Funct) {
							case CRHM::DLTA:
								Temp = cdSeries->YValue((Global::DTindx - DTmin) / thisVar->FileData->Freq - 1);
								cdSeries->YValues[CurrentIndx] -= Delta0;
								Delta0 = Temp; // fall through
							case CRHM::AVG:
							case CRHM::MIN: // duplicate last
							case CRHM::MAX: // duplicate last
							case CRHM::TOT: // duplicate last
							case CRHM::POS: // duplicate last
							case CRHM::LAST: // duplicate last
							case CRHM::CNT:  // duplicate last
							case CRHM::CNT0: // duplicate last
								if (ObsFunct_Toggle == 1) // show final value
									for (long jj = LastIndex + 1; jj <= CurrentIndx; ++jj)
										cdSeries->YValues[jj] = cdSeries->YValues[CurrentIndx];
								break;
							case CRHM::FIRST: // duplicate first
								for (long jj = LastIndex + 1; jj <= CurrentIndx; ++jj)
									cdSeries->YValues[jj] = First0;
								break;
							} // switch
						}
						else if (Funct == CRHM::DLTA && TBase) { // only very first time
							(newVar->*(newVar->LoopFunct))(Indx);
							Delta0 = newVar->values[Indx];

							newVar->UserFunct_ = &ClassVar::Last_; // change from First interval to Last interval
							newVar->FunKind = CRHM::LAST;
							newVar->LoopFunct = &ClassVar::LoopLast;
						}

						Lastkk = Global::DTindx;
						if (CurrentIndx > -1) // skip first time
							LastIndex = CurrentIndx;

						switch (Funct) { // beginning of period reset
						case CRHM::MAX:
							newVar->values[Indx] = -1000000.0;
							break;
						case CRHM::MIN:
							newVar->values[Indx] = 1000000.0;
							break;
						case CRHM::AVG:
						case CRHM::TOT:
						case CRHM::CNT:
						case CRHM::CNT0:
						case CRHM::DLTA:
						case CRHM::POS:
							newVar->values[Indx] = 0.0;
						} // switch

						LastDays = Days;
						Days = 0.0;
					} // if First

					(newVar->*(newVar->LoopFunct))(Indx);

					xx = newVar->values[Indx];
					cdSeries->AddXY(Global::DTnow, xx);
					//AddDataToSeries(series, Global::DTnow, xx);

					if (First)
						First0 = xx;

					if (Global::DTindx > DTmin && Global::DTindx > IndxMin) {
						switch (Funct) {
						case CRHM::AVG:
							Greatest = Days;
							if (LastDays > Days)
								Greatest = LastDays;
							cdSeries->YValues[CurrentIndx] /= (Global::Freq*Greatest);
							LastDays = 0;
							break;
						case CRHM::DLTA:
							if (!First)
								cdSeries->YValues[CurrentIndx] -= Delta0;
							break;
						} // switch
					}

					++Days;

					First = FALSE;
				} // if
		} // for

		if (Global::DTindx > DTmin && Global::DTindx > IndxMin) { // Handle RUN starting after beginning of primary obs file and secondary obs file later
			CurrentIndx = (Global::DTindx - DTminX) / thisVar->FileData->Freq - 1;
			switch (Funct) {
			case CRHM::AVG:
				Greatest = Days;
				if (LastDays > Days)
					Greatest = LastDays;
				cdSeries->YValues[CurrentIndx] /= (Global::Freq*Greatest);
				if (ObsFunct_Toggle == 1) // show final value
					for (long jj = LastIndex + 1; jj <= CurrentIndx; ++jj)
						cdSeries->YValues[jj] = cdSeries->YValues[CurrentIndx];
				break;
			case CRHM::DLTA:
				cdSeries->YValues[CurrentIndx] -= Delta0;
			case CRHM::MIN: // duplicate last
			case CRHM::MAX: // duplicate last
			case CRHM::TOT: // duplicate last
			case CRHM::POS: // duplicate last
			case CRHM::LAST: // duplicate last
			case CRHM::CNT:  // duplicate last
			case CRHM::CNT0: // duplicate last
				if (ObsFunct_Toggle == 1) // show final value
					for (long jj = LastIndex + 1; jj <= CurrentIndx; ++jj)
						cdSeries->YValues[jj] = cdSeries->YValues[CurrentIndx];
				break;
			case CRHM::FIRST: // duplicate first
				for (long jj = LastIndex + 1; jj <= CurrentIndx; ++jj)
					cdSeries->YValues[jj] = First0;
				break;
			} // switch
		}

		delete newVar->FileData; // created in this routine
		delete newVar; // created in this routine

		cdSeries->Tag = NULL;
	} // else

	Global::DTnow = Save_DTnow; // restore

	//if (!CommandLine)  // do not repaint
	//tchart.Repaint();

	AddSeriesToTChart(cdSeries);
	


} // end of AddObsPlot


void HruNameClick() {

	CRHMmain * test = CRHMmain::getInstance();

	MapPar::iterator itPar;
	ClassPar *newPar;
	long Hru, Lay;

	test->ListBox3->Sorted = false;

	if (!test->ListHruNames) {

		test->ListHruNames = new TStringList;
		test->MapGrpNames = new MapstrSS;

		if ((itPar = Global::MapPars.find("basin basin_name")) != Global::MapPars.end()) {
			newPar = (*itPar).second;
			test->ListHruNames->Add(newPar->Strings->Strings[0]);
		}
		else
			test->ListHruNames->Add("Basin");

		if ((itPar = Global::MapPars.find("basin hru_names")) != Global::MapPars.end()) {
			newPar = (*itPar).second;

			for (int ii = 0; ii < newPar->Strings->Count; ++ii)
				test->ListHruNames->Add(newPar->Strings->Strings[ii]);
		}
		else {
			for (int ii = 0; ii < Global::nhru; ++ii)
				test->ListHruNames->Add("hru" + to_string(ii));
		}

		for (int ii = 0; ii < Global::OurModulesList->Count; ++ii) {
			ClassModule* Mod = (ClassModule*)Global::OurModulesList->Objects[ii];
			string S = Global::OurModulesList->Strings[ii];

			if (Mod->GroupCnt > 0 || Mod->StructCnt > 0) {

				if (Mod->GroupCnt > 0) {
					string AA;
					Common::GroupEnding(AA, Mod->GroupCnt);
					Pairstr Item = Pairstr(AA.c_str(), ('@' + Mod->Name).c_str());
					test->MapGrpNames->insert(Item);
				}
				else if (Mod->StructCnt > 0) {
					string AA;
					Common::GroupEnding(AA, Mod->GroupCnt);
					Pairstr Item = Pairstr(AA.c_str(), ('@' + Mod->Name).c_str());
					test->MapGrpNames->insert(Item);
				}
			}
		}

		for (int ii = 0; test->ListBox3->Count > ii; ++ii) {
			string S = test->ListBox3->Strings[ii];

			ClassVar *thisVar = (ClassVar *)test->ListBox3->Objects[ii];

			test->HruNames = false;
			S = test->ExtractHruLay(S, Hru, Lay);

			test->HruNames = true;
			S = test->BuildHru(S, Hru, thisVar->dimen);

			if (Lay)
				S = test->BuildLay(S, Lay);

			int Indx = S.find("@"); // Insert Group & Struct names
			if (Indx) {
				MapstrSS::iterator it;
				if ((it = test->MapGrpNames->find(S.substr(Indx, 2))) != test->MapGrpNames->end()) {
					string NewName = (*it).second;
					S.replace(Indx, 2, "");
					S.insert(Indx, NewName);
				}
			}
			test->ListBox3->Strings[ii] = S;
		}
	}
	else { // currently using HRU names
		for (int ii = 0; test->ListBox3->Count > ii; ++ii) {
			string S = test->ListBox3->Strings[ii];

			test->HruNames = true;
			S = test->ExtractHruLay(S, Hru, Lay);

			test->HruNames = false;
			S = test->BuildHru(S, Hru, CRHM::NHRU);  // fill parameter

			if (Lay)
				S = test->BuildLay(S, Lay);

			int Indx = S.find("@");
			if (Indx) { // group or structure name
				MapstrSS::iterator it;
				for (it = test->MapGrpNames->begin(); it != test->MapGrpNames->end(); ++it) {
					int pos = S.find((*it).second);
					if (pos) { // found name
						string NewName = (*it).first;
						string Name = (*it).second;
						int Len = Name.length();
						int End = S.find("(");
						if (End - pos == Len) { // correct length
							S.replace(pos, Len, "");
							S.insert(pos, NewName);
							break;
						}
					}
				}
			}
			test->ListBox3->Strings[ii] = S;
		}

		delete test->ListHruNames;
		test->ListHruNames = NULL;
		delete test->MapGrpNames;
		test->MapGrpNames = NULL;
	}

	test->ListBox3->Sorted = false;
}


void CRHMmainDlg::OnBuildMacro()
{
	// TODO: Add your command handler code here

	MacroEntryDlg * macroentry;
	macroentry = new MacroEntryDlg();
	//macroentry->DoModal();

	if (macroentry->DoModal())
	{
		CRHMmain * t = CRHMmain::getInstance();
		t->MacroClick();
		loadGuiComponents();
		//RunClickFunctionOld();

		ReopenProject();
	}
}


void CRHMmainDlg::OnHelpAbout()
{
	// TODO: Add your command handler code here

	CRHMAboutBox aboutbox;
	aboutbox.DoModal();


}


void CRHMmainDlg::OnFlowdiagramsShowdiagram()
{
	// TODO: Add your command handler code here

	FlowDiagramDlg flowdiagram;
	flowdiagram.DoModal();

}
