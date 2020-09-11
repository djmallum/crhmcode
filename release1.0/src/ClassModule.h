#pragma once

// 02/20/18
//---------------------------------------------------------------------------

#ifndef ClassModuleH
#define ClassModuleH
//---------------------------------------------------------------------------
#include "TStringList.h"
#include "GlobalCommon.h"
#include "GlobalDll.h"
#include "ClassCRHM.h"
#include <cmath>
#include <string>
#include <time.h>


using namespace std;

class Administer;

class   ClassModule {
public:

	string Name; // Module or Macro name
	string NameRoot; // When Macro gives Root
	string DLLName; // CRHM, Macro or DLL name
	string ID; // Class name. If macro is the macro name
	Administer* OurAdmin {NULL};
	string Version;
	long PeerRank;
	string PeerVar;
	CRHM::LMODULE Lvl;  // module level - BASIC, MACRO, ADVANCE, SUPPORT, CUSTOM, PROTO, OBSOL
	string Description;

	long nhru; // # of HRU's. Can be different from Global::nhru for groups, structurs and comprising modules
	long nobs;
	long nlay;
	long hh;
	long nn;
	unsigned short variation;     // module operating variation
	unsigned short variation_set; // current variation call variation level
	unsigned short variation_max; // maximum - OR of all variation_set

	bool isGroup; // in declgroup macros
	bool isStruct; // in declstruct macros
	long GroupCnt; // in declgroup macros and comprising modules
	long StructCnt; // in declstruct macros and comprising modules
	long ModuleIndx; // in declstruct - module index starting at 1 to nhru

	const long *HRU_struct; // only defined for structure Macros and modules

	long Dummy;

	double** Var_loop_lay_value {NULL};
	double** Par_loop_lay_value {NULL};

	double*** Var_loop_lay_table {NULL};
	double*** Par_loop_lay_table {NULL};

	long Var_NDEFN_cnt = 0;
	long Par_NDEFN_cnt = 0;

	//added by Manishankar to solve the address issue.
	double** t_layvalues {NULL};
	double** rh_layvalues {NULL};


	// long Using_RH_EA_obs;

	ClassModule(string Name, string Version, CRHM::LMODULE Lvl = CRHM::PROTO, long PeerRank = 1000, string PeerVar = "")
		: Name(Name), NameRoot(Name), Version(Version), Lvl(Lvl), PeerRank(PeerRank), PeerVar(PeerVar), HRU_struct(NULL), isGroup(false), isStruct(false),
		nhru(0), nobs(0), nlay(0), hh(0), nn(0), variation(0), variation_set(0), variation_max(0),
		DLLName(""), GroupCnt(0), StructCnt(0), ModuleIndx(0),
		ID(typeid(*this).name()) {
		Dummy = -9999;
	};

	virtual ~ClassModule() {};

	virtual void decl(void) {};
	void initbase(void);
	virtual void init(void) {};
	virtual void run(void) {};
	virtual void run2(double **) {}; //added by Manishankar to solve the address issue.
	virtual void pre_run(void) {};
	virtual void finish(bool good) {};
	ClassModule* link(string Module);
	double Now(void);
	virtual double Function1(long *I, long hh) { return 0.0; };
	virtual double Function2(double *X, long hh) { return 0.0; };
	virtual void reset(void) {

		nhru = 0;
		nobs = 0;
		nlay = 0;

		variation = 0;
		variation_set = 0;
		variation_max = 0;

		isGroup = false;
		isStruct = false;
		GroupCnt = 0;
		StructCnt = 0;
		HRU_struct = NULL;
	};

	bool chkStruct(void) {
		if (HRU_struct != NULL)
			while (hh < nhru && ModuleIndx != HRU_struct[hh])
			{
				++hh;
			}
		return hh < nhru;
	};

	bool chkStruct(long &hh) {
		if (HRU_struct != NULL)
			while (hh < nhru && ModuleIndx != HRU_struct[hh])
			{
				++hh;
			}
		return hh < nhru;
	};

	bool chkStruct(long &hh, long Nhru) {
		if (HRU_struct != NULL)
			while (hh < Nhru && ModuleIndx != HRU_struct[hh])
			{
				++hh;
			}
		return hh < Nhru;
	};

	bool chkStructOff(long &hh, long Nhru) { // for macros
		if (HRU_struct != NULL)
			while (hh < Nhru && ModuleIndx != HRU_struct[hh - 1]) // note offset range
			{
				++hh;
			}
		return hh < Nhru;
	};

	virtual ClassModule* klone(string name) const {
		return NULL;
	};

	string Var_name(ClassModule* thisModule, string S);

	long getdim(CRHM::TDim dimen);

	void AKAhook(TAKA type, string module, string OrgName, string & NewName);

	void AKAhook(TAKA type, string module, string OrgName, string & NewName, string & source, string base);

	int declgrpvar(string variable, string queryvar,
		string help, string units, double **value, double ***layvalue, bool PointPlot = false);

	void declvar(string variable, CRHM::TDim dimen,
		string help, string units, double **value, double ***layvalue = NULL, const int dim = 1,
		bool PointPlot = false, bool StatVar = false, CRHM::TVISIBLE Local = CRHM::USUAL);

	void declvar(string variable, CRHM::TDim dimen,
		string help, string units, long **value, long ***ilayvalue = NULL, const int dim = 1,
		bool PointPlot = false, bool StatVar = false, CRHM::TVISIBLE Local = CRHM::USUAL);

	void decldiag(string variable, CRHM::TDim dimen,
		string help, string units, double **value, double ***layvalue = NULL, const int dim = 1,
		bool PointPlot = false, CRHM::TVISIBLE Local = CRHM::DIAGNOSTIC);

	void declstatdiag(string variable, CRHM::TDim dimen,
		string help, string units, double **value, double ***layvalue = NULL, const int dim = 1,
		bool PointPlot = false, CRHM::TVISIBLE Local = CRHM::DIAGNOSTIC);

	void decldiag(string variable, CRHM::TDim dimen,
		string help, string units, long **value, long ***ilayvalue = NULL, const int dim = 1,
		bool PointPlot = false, CRHM::TVISIBLE Local = CRHM::DIAGNOSTIC);

	void declstatdiag(string variable, CRHM::TDim dimen,
		string help, string units, long **value, long ***ilayvalue = NULL, const int dim = 1,
		bool PointPlot = false, CRHM::TVISIBLE Local = CRHM::DIAGNOSTIC);

	void declstatvar(string variable, CRHM::TDim dimen,
		string help, string units, double **value, double ***layvalue = NULL, const int dim = 1,
		bool PointPlot = false, CRHM::TVISIBLE Local = CRHM::USUAL);

	void declstatvar(string variable, CRHM::TDim dimen,
		string help, string units, long **value, long ***ilayvalue = NULL, const int dim = 1,
		bool PointPlot = false, CRHM::TVISIBLE Local = CRHM::USUAL);

	void decllocal(string variable, CRHM::TDim dimen,
		string help, string units, double **value, double ***layvalue = NULL, const int dim = 1);

	void decllocal(string variable, CRHM::TDim dimen,
		string help, string units, long **value, long ***ilayvalue = NULL, const int dim = 1);

	void declparam(string param, CRHM::TDim dimen,
		string valstr, string minstr, string maxstr,
		string help, string units, const double **value,
		const double ***layvalue = NULL, const int dim = 1, CRHM::TVISIBLE Local = CRHM::USUAL);

	void declparam(string param, CRHM::TDim dimen,
		string valstr, string minstr, string maxstr,
		string help, string units, const long **value,
		const long ***ilayvalue = NULL, const int dim = 1, CRHM::TVISIBLE Local = CRHM::USUAL);

	TStringList* declparam(string param, CRHM::TDim dimen, string Texts, string help, TStringList *stringsList, CRHM::TVISIBLE Local = CRHM::USUAL);

	void decldiagparam(string param, CRHM::TDim dimen,
		string valstr, string minstr, string maxstr,
		string help, string units, const double **value,
		const double ***layvalue = NULL, const int dim = 1, CRHM::TVISIBLE Local = CRHM::DIAGNOSTIC);

	void decldiagparam(string param, CRHM::TDim dimen,
		string valstr, string minstr, string maxstr,
		string help, string units, const long **value,
		const long ***ilayvalue = NULL, const int dim = 1, CRHM::TVISIBLE Local = CRHM::DIAGNOSTIC);

	TStringList* decldiagparam(string param, CRHM::TDim dimen, string Texts, string help, TStringList *stringsList, CRHM::TVISIBLE Local = CRHM::DIAGNOSTIC);

	void decllocalparam(string param, CRHM::TDim dimen,
		string valstr, string minstr, string maxstr,
		string help, string units, const double **value,
		const double ***layvalue = NULL, const int dim = 1, CRHM::TVISIBLE Local = CRHM::PRIVATE);

	void decllocalparam(string param, CRHM::TDim dimen,
		string valstr, string minstr, string maxstr,
		string help, string units, const long **value,
		const long ***ilayvalue = NULL, const int dim = 1, CRHM::TVISIBLE Local = CRHM::PRIVATE);

	TStringList* decllocalparam(string param, CRHM::TDim dimen, string Texts, string help, TStringList *stringsList, CRHM::TVISIBLE Local = CRHM::PRIVATE);

	long declgetvar(string source, string name, string units, const double **value, const double ***layvalue = NULL);

	long declgetvar(string source, string name, string units, const long **value, const long ***layvalue = NULL);

	long declreadobs(string variable, CRHM::TDim dimen, string help, string units,
		const double **value, long HRU_index = CRHM::HRU_OBS_t_rh_ea, bool optional = false, const double ***layvalue = NULL);

	long declreadobs(string variable, CRHM::TDim dimen, string help, string units,
		const long **value, long HRU_index = 0, bool optional = false, const long ***layvalue = NULL);

	long declobs(string name, CRHM::TDim dimen, string help, string units, double **value);

	long declobsfunc(string obs, string variable, double **value, CRHM::TFun typeFun, double ***layvalue = NULL, bool optional = false);

	long declobsfunc(string obs, string variable, long **value, CRHM::TFun typeFun, bool optional = false);

	long declputvar(string source, string name, string units, double **value, double ***layvalue = NULL);

	long declputvar(string source, string name, string units, long **value, long ***layvalue = NULL);

	long declputparam(string source, string name, string units, double **value, double ***layvalue = NULL);

	long declgetparam(string source, string name, string units, const double **value, const double ***layvalue = NULL);

	long declputparam(string source, string name, string units, long **value, long ***layvalue = NULL);

	void declgetparam(string source, string name, string units, const long **value, const long ***layvalue = NULL);

	void InitReadObs(void);  // used by RunClick to assign storage and reset Index

	void ReadObs(bool NotStart);  // reads current interval observations

	bool ReadAheadObs(long inc = 0); // reads inc interval observations ahead

	bool ReadAheadObsMacro(long inc = 0); // reads inc interval observations ahead

	bool WriteAheadObsMacro(long inc = 0); // writes interval observations ahead previously set by ReadAheadObs

	void DelReadObs(void);  // used by RunClick to delete storage

	void addtoreadlist(ClassVar *newVar);

	void addtofunctlist(ClassVar *newVar);

	long getdimObs(string variable);

	long FindWildVarFloat(string name, ClassVar* &newVar, bool OnceFlag = false);

	long FindWildParFloat(string name, ClassPar* &newPar, bool Org, bool Root);

	bool Variation_Skip(void);

	bool AnyOne(double *Data, int Cnt, double Val);

	bool UsingObservations(void);

	long FindModule_from_parameter(string source, string param);
};

class   Administer {

public:
	TStringList * DLLModuleList;       // modules defined in DLL
	TStringList *DLLModelList;        // models defined in DLL
	TStringList *DLLModelModuleList;  // modules used in DLL models in CommaText
	string Version;
	string DLLName;
	string HelpFile;

	Administer(string Version, string _HelpFile);
	~Administer();

	void   MacroClear();
	void   MacroUpdate();
	void   AddModule(ClassModule *Module);
	void   AddModel(string ModelName, string ModelModules);
	void   LoadCRHM(string DllName);



	//void   Accept(int Result = mbYes);
	void   Accept(int Result = 0);



};

class Myparser {
	const char *exp_ptr;  // points to the expression
	char token[80]; // holds current token
	char tok_type {'\0'};  // holds token's type

	int row {0};
	int col {0};
	int repeat {0};
	int rowrepeat {0};
	int Bang {0};
	bool repeatset {false};
	ClassPar* LocalPar {NULL};

	void eval_exp2(double &result);
	void eval_exp3(double &result);
	void eval_exp4(double &result);
	void eval_exp5(double &result);
	void eval_exp6(double &result);
	void atom(double &result);
	void get_token();
	void serror(int error);
	int isdelim(char c);
public:
	Myparser();
	void eval_exp(ClassPar *Par);
};

class   ClassClark {

public:
	ClassClark(const double* inVar, double* outVar, const double* kstorage, const double* lag, const long nhru, const double setlag = -1);
	~ClassClark();
	void DoClark();
	void DoClark(const long hh);
	double ChangeStorage(const double* kstorage, const long hh);
	double ChangeLag(const double *newlag, const long hh);
	double Left(int hh);

private:
	const double* kstorage;
	const  double* inVar;
	double* outVar;

	double** LagArray;

	double* LastIn; //
	double* LastOut; //

	double* c01; // storage constant from K
	double* c2;  // storage constant from K
	double* NO_lag_release;  // released from storage when lag set to zero

	long nhru;
	long* maxlag; // maximum lag - i.e. storage

	long* ilag; // lag interval (hours)
	long* ulag; // lag interval (#intervals)

};

class   ClassMuskingum {

public:
	ClassMuskingum(const double* inVar, double* outVar, const double* kstorage, const double* route_X_M, const double* lag, const long nhru, const double setlag = -1);
	~ClassMuskingum();
	void DoMuskingum();
	void DoMuskingum(const long hh);
	void ChangeLag(const double *newlag, const long hh);
	double Left(int hh);


	double* c0; // storage constant from K
	double* c1; // storage constant from K
	double* c2; // storage constant from K
	double prevdate {0.0};

private:
	const double* kstorage {NULL};
	const  double* inVar;
	double* outVar;

	double** LagArray;

	double* LastIn; //
	double* LastOut; //

	long nhru;
	long* maxlag; // maximum lag - i.e. storage

	long* ilag; // lag interval (hours)
	long* ulag; // lag interval (#intervals)
};


static string getTimeGG() {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%A, %B %d, %Y", timeinfo);
	string strr = buffer;
	/*
	time_t t1 = GetCurrentTime();
	CTime t2 = t1 + CTimeSpan(0, 1, 0, 0);
	CString cs = t1.Format(_T("%A, %B %d, %Y"));

	CT2A ct(cs);
	string strr(ct);
	*/
	return (strr);
};

/*static string getTimeGG() {
CTime t1 = CTime::GetCurrentTime();
CTime t2 = t1 + CTimeSpan(0, 1, 0, 0);
CString cs = t1.Format(_T("%A, %B %d, %Y"));

CT2A ct(cs);
string strr(ct);
return (strr);
};*/


TAKA AKAtype(std::string type);

#endif
