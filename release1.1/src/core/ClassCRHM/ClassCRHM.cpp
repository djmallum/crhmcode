
// 11/28/17
//---------------------------------------------------------------------------

#if defined(_WIN32)
#include "stdafx.h"
#endif

#include <stdio.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <bitset>
#include <math.h>
#include <cmath>


#include "ClassCRHM.h"
#include "Classfilter.h"
#include "NewModules.h"
#include "stddef.h"
#include "GlobalDll.h"

//---------------------------------------------------------------------------
//#pragma package(smart_init)

//---------------------------------------------------------------------------

//#define M_PI           3.14159265358979323846  /* pi */

using namespace std;

//---------------------------------------------------------------------------
/*
extern double xLimit;
extern long lLimit;
*/
//double xLimit;
//long lLimit;

 // used by functions

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
void setdim(string name, long dim) {
	string s = name;

	if (name == "nhru") {
		Update_Main_DIM(TDim::NHRU, dim);
	}
	else if (name == "nobs") {
		Update_Main_DIM(TDim::NOBS, dim);
	}
	else if (name == "nlay") {
		Update_Main_DIM(TDim::NLAY, dim);
	}
}

//---------------------------------------------------------------------------
//double sqr(double X) { return X * X; }



//---------------------------------------------------------------------------
ClassPar *ClassParFindShared(string name) { // where name is concatenation of MODULE and NAME
	MapPar::iterator itPar;

	if ((itPar = Global::SharedMapPars.find(name)) == Global::SharedMapPars.end()) {
		CRHMException Except("Could not find Parameter: " + name, TExcept::WARNING);
		LogError(Except);
		return NULL;
	}
	return ((*itPar).second);
}

//---------------------------------------------------------------------------
ClassPar *ClassParFind(string name) { // where name is concatenation of MODULE and NAME
	MapPar::iterator itPar;

	if ((itPar = Global::MapPars.find(name)) == Global::MapPars.end()) {
		CRHMException Except("Could not find Parameter: " + name, TExcept::WARNING);
		LogError(Except);
		return NULL;
	}
	return ((*itPar).second);
}

//---------------------------------------------------------------------------
ClassPar *ClassParFindPar(string name) { // where name is parameter
	MapPar::iterator itPar;
	ClassPar *thisPar;

	for (itPar = Global::MapPars.begin(); itPar != Global::MapPars.end(); ++itPar) {
		thisPar = (*itPar).second;
		if (thisPar->param == name)
			return ((*itPar).second);
	}
	return (NULL);
}

//---------------------------------------------------------------------------
long ClassParFindParBld(ClassPar *thisPar) { // where parameter is BasinMapPars
	Mapstr::iterator itPar;                   // creates individual module parameters
	//ClassPar *newPar;
	string sparam, smodule;
	long Count = 0;

	for (itPar = Global::Mapdeclpar.begin(); itPar != Global::Mapdeclpar.end(); ++itPar) {
		sparam = (itPar->second.first).c_str();
		smodule = (itPar->first).c_str();
		if (sparam == thisPar->param) {
			MapPar::iterator itPar0 = Global::MapPars.find(smodule + ' ' + thisPar->param);
			if (itPar0 != Global::MapPars.end()) // already there?
				continue;
			if (Global::OurModulesList->IndexOf(smodule.c_str()) == -1) // in project?
				continue; // NO!
			++Count;
			ClassPar *newPar = new ClassPar(*thisPar);
			newPar->module = smodule;
			PairPar Item = PairPar(newPar->module + ' ' + newPar->param, newPar);
			Global::MapPars.insert(Item);
		}
	}

	return (Count);
}

//---------------------------------------------------------------------------
void ClassParSet(ClassPar *p) {
	MapPar::iterator itPar;
	ClassPar *thisPar;

	// if new parameter is identical to 'Shared' version, delete it!

	if ((itPar = Global::MapPars.find("Shared " + p->param)) != Global::MapPars.end()) {
		thisPar = ((*itPar).second);
		if (thisPar->Same(*p)) {
			delete p;
			return;
		}
	}

	// if parameter already exists - change to new value and delete it!

	if ((itPar = Global::MapPars.find(p->module + ' ' + p->param)) != Global::MapPars.end()) {
		thisPar = ((*itPar).second);
		if (!thisPar->Same(*p) && thisPar->dim == p->dim)
			thisPar->Change(*p);
		delete p;
		return;
	}

	// create new parameter
	else {
		PairPar Item = PairPar(p->module + ' ' + p->param, p);
		Global::MapPars.insert(Item);
	}
}

//---------------------------------------------------------------------------
ClassPar *ClassParFind(string module, string param) { // routine concatenates MODULE and NAME
	MapPar::iterator itPar;

	if ((itPar = Global::MapPars.find(module + ' ' + param)) != Global::MapPars.end()) {
		return ((*itPar).second);
	}

	if ((itPar = Global::MapPars.find("Shared " + param)) != Global::MapPars.end())
		return ((*itPar).second);
	else {
		long jj = -1;
		if (Global::OldModuleName != NULL)
		{
			jj = Global::OldModuleName->IndexOf(module.c_str()); // Check if renamed module. Find old name
		}
		if (jj > -1) {
			string s = Global::NewModuleName->Strings[jj] + ""; // two step to remove \0
			if ((itPar = Global::MapPars.find(s + ' ' + param)) != Global::MapPars.end()) {
				return ((*itPar).second);
			}
		}
		LogError(CRHMException("Parameter defined in module: " + param + ", " + module + " not found", TExcept::WARNING));
		return NULL;
	}
}

//---------------------------------------------------------------------------
ClassPar *ClassParFindRev(string module, string param) {
	MapPar::iterator itPar;

	if ((itPar = Global::MapPars.find("Shared " + param)) != Global::MapPars.end())
		return ((*itPar).second);

	if ((itPar = Global::MapPars.find(module + ' ' + param)) != Global::MapPars.end()) {
		return ((*itPar).second);
	}
	else {
		LogError(CRHMException("Parameter defined in module: " + param + ", " + module + " not found", TExcept::TERMINATE));
		return NULL;
	}
}

//---------------------------------------------------------------------------
ClassPar::~ClassPar() {

	if (varType == TVar::Float) {
		for (int ii = 0; ii < lay; ++ii) delete[] layvalues[ii];

		delete[] layvalues;  //Array [nhru] [lay]
		values = NULL;
	}
	else if (varType == TVar::Int) {
		for (int ii = 0; ii < lay; ++ii) delete[] ilayvalues[ii];

		delete[] ilayvalues; //Array [nhru] [lay]
		ivalues = NULL;
	}
	else if (varType == TVar::Txt) {
		delete Strings;
	}
	varType = TVar::none;
}

//---------------------------------------------------------------------------
ClassVar *ClassVarFind(string name) {
	MapVar::iterator itVar;

	if ((itVar = Global::MapVars.find(name)) == Global::MapVars.end())
		if ((itVar = Global::MapVars.find('#' + name)) == Global::MapVars.end()) { // could be local
			CRHMException Except("Could not find Variable: " + name, TExcept::WARNING);
			LogError(Except);
			return NULL;
		}
	return ((*itVar).second);
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

// not complete

//---------------------------------------------------------------------------

// not complete

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void ClassVar::review_HRU_OBS(void) {

	if (name == "t" || name == "t_max" || name == "t_min" || name == "rh" || name == "ea")
		HRU_OBS_indexed = 0;
	else if (name == "p" || name == "ppt")
		HRU_OBS_indexed = 1;
	else if (name == "u")
		HRU_OBS_indexed = 2;
	else if (name == "Qsi" || name == "SunAct" || name == "Qli")
		HRU_OBS_indexed = 3;
	else
		HRU_OBS_indexed = 4;
}










//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

//Changed by Manishankar 2018/09/10

long julian(string when) {
	int Y, M, D, H, Min;
	double DtBegin;
	//CTime
	// calendar

	if (when == "start") {
		StandardConverterUtility::DecodeDateTime(Global::DTstart, &Y, &M, &D, &H, &Min);
		DtBegin = StandardConverterUtility::EncodeDateTime((Word)Y - 1, (Word)12, (Word)31, 0, 0);
		return  INT(Global::DTstart - DtBegin);  // -0.999/Global::Freq
	}
	else if (when == "end") {
		StandardConverterUtility::DecodeDateTime(Global::DTend, &Y, &M, &D, &H, &Min);
		DtBegin = StandardConverterUtility::EncodeDateTime((Word)Y - 1, (Word)12, (Word)31, 0, 0);
		return  INT(Global::DTend - DtBegin);
	}
	else { // now
		StandardConverterUtility::DecodeDateTime(Global::DTnow, &Y, &M, &D, &H, &Min);
		DtBegin = StandardConverterUtility::EncodeDateTime((Word)Y - 1, (Word)12, (Word)31, 0, 0);
		return  INT(Global::DTnow - DtBegin);
	}
}







//---------------------------------------------------------------------------
//Changed by Manishankar 2018/09/10

long julian(double DT) {
	int Y, M, D, H, Min;
	double DtBegin;

	StandardConverterUtility::DecodeDateTime(DT, &Y, &M, &D, &H, &Min);
	DtBegin = StandardConverterUtility::EncodeDateTime(Y - 1, 12, 31, 0, 0);
	return  INT(DT - DtBegin);
}


//---------------------------------------------------------------------------
//Changed by Manishankar 2018/09/10

long MonthDaysCnt(double DT) {
	int Y, M, D, H, Min;
	double DTNext;

	StandardConverterUtility::DecodeDateTime(DT, &Y, &M, &D, &H, &Min);
	++M;

	if (M > 12) { // next year
		M = 1;
		++Y;
	}

	DTNext = StandardConverterUtility::EncodeDateTime((Word)Y, (Word)M, (Word)D, 0, 0);
	return  INT(DTNext - DT);
}


//---------------------------------------------------------------------------
//Changed by Manishankar 2018/09/10

double Julian(string when) {
	int Y, M, D, H, Min;
	double DtBegin;

	// calendar

	if (when == "start") {
		StandardConverterUtility::DecodeDateTime(Global::DTstart, &Y, &M, &D, &H, &Min);
		DtBegin = StandardConverterUtility::EncodeDateTime((Word)Y - 1, (Word)12, (Word)31, 0, 0);
		return  (double)(Global::DTstart - DtBegin);  // -0.999/Global::Freq
	}
	else if (when == "end") {
		StandardConverterUtility::DecodeDateTime(Global::DTend, &Y, &M, &D, &H, &Min);
		DtBegin = StandardConverterUtility::EncodeDateTime((Word)Y - 1, (Word)12, (Word)31, 0, 0);
		return  (double)(Global::DTend - DtBegin);
	}
	else { // now
		StandardConverterUtility::DecodeDateTime(Global::DTnow, &Y, &M, &D, &H, &Min);
		DtBegin = StandardConverterUtility::EncodeDateTime((Word)Y - 1, (Word)12, (Word)31, 0, 0);
		return  double((double)Global::DTnow - DtBegin);
	}
}


//---------------------------------------------------------------------------
bool last_timestep(void) {
	if (Global::DTindx == Global::DTmax - 1)
		return true;
	else
		return false;
}

//---------------------------------------------------------------------------

//Changed by Manishankar. 2018/09/11

void dattim(string type, long *itime) { // dimension itime[6]
	int Year, Month, Day, H, Min, Sec = 0;
	//int MSec; (unreferenced commented out jhs507)

	if (type == "start") {
		StandardConverterUtility::DecodeDateTime(Global::DTstart, &Year, &Month, &Day, &H, &Min);
		//DecodeTime(Global::DTstart, &H, &Min, &Sec);
	}
	else if (type == "end") {
		StandardConverterUtility::DecodeDateTime(Global::DTend, &Year, &Month, &Day, &H, &Min);
		//DecodeTime(Global::DTend, &Hour, &Min, &Sec);
	}
	else {
		StandardConverterUtility::DecodeDateTime(Global::DTnow, &Year, &Month, &Day, &H, &Min);
		//DecodeTime(Global::DTnow, &Hour, &Min, &Sec);
	}
	itime[0] = Year;
	itime[1] = Month;
	itime[2] = Day;
	itime[3] = H;
	itime[4] = Min;
	itime[5] = Sec;
}



//---------------------------------------------------------------------------
void dattim(double DT, long *itime) { // dimension itime[6]
	int Year, Month, Day, H, Min, Sec = 0;
	//int MSec; (unreferenced commented out jhs507)

	StandardConverterUtility::DecodeDateTime(DT, &Year, &Month, &Day, &H, &Min);
	//DecodeTime(DT, &Hour, &Min, &Sec);

	itime[0] = Year;
	itime[1] = Month;
	itime[2] = Day;
	itime[3] = H;
	itime[4] = Min;
	itime[5] = Sec;
}

//---------------------------------------------------------------------------
void ModVarRemove(MapVar & MapVars) {

	ClassVar *thisVar;
	ClassVar *thisVar2;
	ClassPar *thisPar;

	MapVar::iterator itVar;
	MapVar::iterator itVar2, itVar3;
	MapPar::iterator itPar;

	string S, S2;

	// No observation(ie < Read) - Removes all variable (obs and module).  Done indirectly,
	//   so that variable is deleted only once
	// Observation(ie > Read) - removes only module variables
	for (itVar = Global::MapVars.begin(); itVar != Global::MapVars.end(); itVar++) {
		thisVar = (*itVar).second;
		if (thisVar != NULL) {
			S = (*itVar).first;
			if (thisVar->varType < TVar::Read) {
				for (itVar2 = Global::MapVars.begin(); itVar2 != Global::MapVars.end(); itVar2++) {
					thisVar2 = (*itVar2).second;
					if (thisVar2 != NULL) {
						S2 = (*itVar2).first;
						if (thisVar == thisVar2)
							(*itVar2).second = NULL;
					}
				}
				if (thisVar->module != "")
					delete thisVar; // releases everything
				else
					S = "Fault";
			}
			else {
				// Observation - removes duplicate map entries to ClassVar leaving the 'obs' entries.

				if (S != ("obs " + thisVar->name))
					(*itVar).second = NULL;
			}
		} // if
	}  // for

	   // Removes entries from map.  Do it like this as iteration pointer cannot point to erased item
	   // Removes entries flagged above.

	for (itVar = Global::MapVars.begin(); itVar != Global::MapVars.end();) {
		thisVar = (*itVar).second;
		//    S = (*itVar).first;
		if (thisVar == NULL)
			Global::MapVars.erase(itVar++);
		else
			++itVar;
	}

	// Observation - demotes variables to simple observation type from ReadI and ReadF
	for (itVar = Global::MapVars.begin(); itVar != Global::MapVars.end(); itVar++) {
		thisVar = (*itVar).second;
		switch (thisVar->varType) {
		case TVar::ReadI:
			if (thisVar->lay > 0) { // handles NFREQ
				for (int ii = 0; ii < thisVar->lay; ++ii)
					delete[] thisVar->ilayvalues[ii];

				delete[] thisVar->ilayvalues;  //Array [nhru] [lay]
				thisVar->lay = 0;
				thisVar->ilayvalues = NULL;
			}

			delete[]thisVar->ivalues;
			thisVar->ivalues = NULL;
			thisVar->varType = TVar::Read;
			thisVar->dim = thisVar->cnt;
			break;
		case TVar::ReadF:

			if (thisVar->lay > 0) { // handles NFREQ
				for (int ii = 0; ii < thisVar->lay; ++ii)
					delete[] thisVar->layvalues[ii];

				delete[] thisVar->layvalues;  //Array [nhru] [lay]
				thisVar->lay = 0;
				thisVar->layvalues = NULL;
			}

			delete[]thisVar->values;
			thisVar->values = NULL;
			thisVar->varType = TVar::Read;
			thisVar->dim = thisVar->cnt;
			break;
		case TVar::Int:
		case TVar::Float:
			exit(1); // should never happen
		default:
			break;
		}
	}
	// No modules - No parameters !

	for (itPar = Global::MapPars.begin(); itPar != Global::MapPars.end(); itPar++) {
		thisPar = (*itPar).second;
		delete thisPar;
	}
	Global::MapPars.clear();
	Global::SharedMapPars.clear();
}

//---------------------------------------------------------------------------
ClassVar *declread(string module, string name, long cnt, long offset,
	ClassData * FileData, string Comment) {
	MapVar::iterator itVar;
	ClassVar *thisVar;
	string s1(module);
	string s2(name);
	string units;

	if (Comment.empty())
		Comment = "No information in observation file";
	else {
		units = extract_units(Comment);
	}

	if (cnt > Global::nobs) {
		Global::nobs = cnt; // allow it to display
		Global::maxobs = cnt;
	}

	// executes following code only if observation file replaced
	if ((itVar = Global::MapVars.find(s1 + " " + s2)) != Global::MapVars.end()) {
		thisVar = (*itVar).second;
		if (thisVar->varType >= TVar::Read) {
			LogError(CRHMException("Duplicate Observation variable: " + s1 + " " + s2, TExcept::ERR));
			return NULL;
		}
		//    Convert convert; convert.CheckUnitsObs(units, thisVar->units, name); // check original observation units
		//    if(thisVar->FileData == NULL)
		//      return thisVar;

		thisVar->FileData = FileData;
		thisVar->cnt = cnt;
		thisVar->offset = offset;
		/*
		if(thisVar->cnt && thisVar->dim != thisVar->cnt) {
		thisVar->ReleaseM(); // releases everything

		if(thisVar->lay > 0) {
		thisVar->layvalues = new double *[thisVar->lay];
		for(int ii = 0; ii < thisVar->lay; ii++)
		thisVar->layvalues[ii] = new double[thisVar->dim];
		}
		thisVar->values = new double[thisVar->dim];
		}
		*/
		if (thisVar->varType == TVar::Int)
			thisVar->varType = TVar::ReadI;
		if (thisVar->varType == TVar::Float)
			thisVar->varType = TVar::ReadF;

		return thisVar;
	}

	thisVar = new ClassVar(module, name, cnt, offset, FileData);

	if (thisVar->name == "p" || thisVar->name == "ppt") // must be NHRU for routine "ReadVar" when obs file loaded after modules.
		thisVar->dimen = TDim::NHRU;

	thisVar->varType = TVar::Read;
	thisVar->units = units;
	thisVar->help = Comment;

	PairVar Item = PairVar(s1 + ' ' + s2, thisVar);
	Global::MapVars.insert(Item);

	return thisVar;
}




//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


void Classmacro::addfilter(string Line) {

	Classfilter *NewFilter;

	string S = Line.substr(1, Common::trim(Line).length());

	if (Line.substr(1, 1) == "$") return; // handle comment

	long bb = S.find('(');

	long ee;
	//  for(ee = 1; ee < bb; ++ee)
	ee = S.find_first_of(" \t");

	string ToVar = "";
	if (bb > ee)    // filter returns value
		ToVar = S.substr(0, ee);
	else
		ee = 1;      // only "sim" ForceInterval and TimeShift at present

	S = S.substr(ee);
	S = Common::trim(S);

	ee = S.find('(');
	string Filter = S.substr(0, ee);
	string FilterStuff = S.substr(ee);
	FilterStuff = Common::trim(FilterStuff);

	if (ToVar.empty()) {
		if (Filter == "Sim") NewFilter = (Classfilter*) new ClassSim(File, ToVar, FilterStuff);
		else if (Filter == "ForceInterval") NewFilter = (Classfilter*) new ClassForce(File, ToVar, FilterStuff);
		else if (Filter == "TimeShift") NewFilter = (Classfilter*) new ClassTimeshift(File, ToVar, FilterStuff);
		else {
			string Mess = "Filter: '" + Filter + " has no 'to variable'";
			/*      CRHMException Except(Mess.c_str(), ERR);
			Message(Mess.c_str(), "Error in filter in observation file", mbOK);
			LogError(Except);*/
			return;
		}
	}
	else if (Filter == "missing") NewFilter = (Classfilter*) new ClassMissing(File, ToVar, FilterStuff);
	else if (Filter == "missing0") NewFilter = (Classfilter*) new ClassMissing0(File, ToVar, FilterStuff);
	else if (Filter == "missingC") NewFilter = (Classfilter*) new ClassMissingC(File, ToVar, FilterStuff);
	else if (Filter == "missingFlag") NewFilter = (Classfilter*) new ClassMissingFlag(File, ToVar, FilterStuff);
	else if (Filter == "missingFlagAfter") NewFilter = (Classfilter*) new ClassMissingFlagAfter(File, ToVar, FilterStuff);
	else if (Filter == "missingrepl") NewFilter = (Classfilter*) new ClassMissingrepl(File, ToVar, FilterStuff);
	else if (Filter == "missinginter") {
		NewFilter = (Classfilter*) new ClassMissingInter(File, ToVar, FilterStuff);
		++Interpolation;
	}
	else if (Filter == "ea") NewFilter = (Classfilter*) new Classea(File, ToVar, FilterStuff);
	else if (Filter == "rh") NewFilter = (Classfilter*) new Classrh(File, ToVar, FilterStuff);  //I have added this from the borland version.
	else if (Filter == "RH_WtoI") NewFilter = (Classfilter*) new ClassRH_WtoI(File, ToVar, FilterStuff);
	else if (Filter == "add") NewFilter = (Classfilter*) new Classadd(File, ToVar, FilterStuff);
	else if (Filter == "sub") NewFilter = (Classfilter*) new Classsub(File, ToVar, FilterStuff);
	else if (Filter == "mul") NewFilter = (Classfilter*) new Classmul(File, ToVar, FilterStuff);
	else if (Filter == "div") NewFilter = (Classfilter*) new Classdiv(File, ToVar, FilterStuff);
	else if (Filter == "addV") NewFilter = (Classfilter*) new ClassaddV(File, ToVar, FilterStuff);
	else if (Filter == "subV") NewFilter = (Classfilter*) new ClasssubV(File, ToVar, FilterStuff);
	else if (Filter == "mulV") NewFilter = (Classfilter*) new ClassmulV(File, ToVar, FilterStuff);
	else if (Filter == "divV") NewFilter = (Classfilter*) new ClassdivV(File, ToVar, FilterStuff);
	else if (Filter == "refwind") NewFilter = (Classfilter*) new Classrefwind(File, ToVar, FilterStuff);
	else if (Filter == "const") NewFilter = (Classfilter*) new Classconst(File, ToVar, FilterStuff);
	else if (Filter == "sine") NewFilter = (Classfilter*) new Classsin(File, ToVar, FilterStuff);
	else if (Filter == "sin") NewFilter = (Classfilter*) new Classsin(File, ToVar, FilterStuff); //I have added this from the borland version.
	else if (Filter == "cos") NewFilter = (Classfilter*) new Classcos(File, ToVar, FilterStuff); //I have added this from the borland version.
	else if (Filter == "abs") NewFilter = (Classfilter*) new Classabs(File, ToVar, FilterStuff); //I have added this from the borland version.
	else if (Filter == "square") NewFilter = (Classfilter*) new Classsquare(File, ToVar, FilterStuff);
	else if (Filter == "ramp") NewFilter = (Classfilter*) new Classramp(File, ToVar, FilterStuff);
	else if (Filter == "pulse") NewFilter = (Classfilter*) new Classpulse(File, ToVar, FilterStuff);
	else if (Filter == "exp") NewFilter = (Classfilter*) new Classexp(File, ToVar, FilterStuff);
	else if (Filter == "expV") NewFilter = (Classfilter*) new Classexpv(File, ToVar, FilterStuff);
	else if (Filter == "log") NewFilter = (Classfilter*) new Classlog(File, ToVar, FilterStuff);
	else if (Filter == "logV") NewFilter = (Classfilter*) new Classlogv(File, ToVar, FilterStuff);
	else if (Filter == "pow") NewFilter = (Classfilter*) new Classpow(File, ToVar, FilterStuff);
	else if (Filter == "powV") NewFilter = (Classfilter*) new Classpowv(File, ToVar, FilterStuff);
	else if (Filter == "time") NewFilter = (Classfilter*) new Classtime(File, ToVar, FilterStuff);
	else if (Filter == "julian") NewFilter = (Classfilter*) new Classjulian(File, ToVar, FilterStuff);
	else if (Filter == "random") NewFilter = (Classfilter*) new Classrandom(File, ToVar, FilterStuff);
	else if (Filter == "poly") NewFilter = (Classfilter*) new Classpoly(File, ToVar, FilterStuff);
	else if (Filter == "polyV") NewFilter = (Classfilter*) new Classpolyv(File, ToVar, FilterStuff);
	else if (Filter == "smear") NewFilter = (Classfilter*) new ClassSmear(File, ToVar, FilterStuff);
	else if (Filter == "FtoC") NewFilter = (Classfilter*) new ClassFtoC(File, ToVar, FilterStuff);
	else if (Filter == "KtoC") NewFilter = (Classfilter*) new ClassKtoC(File, ToVar, FilterStuff);
	else if (Filter == "CtoK") NewFilter = (Classfilter*) new ClassCtoK(File, ToVar, FilterStuff);
	else if (Filter == "replace") NewFilter = (Classfilter*) new ClassReplace(File, ToVar, FilterStuff); //I have added this from the borland version.
	else {
		string Mess = "Filter: '" + Filter + "' name not recognised";
		/*    CRHMException Except(Mess.c_str(), ERR);
		Message(Mess.c_str(), "Error in filter in observation file", mbOK);
		LogError(Except);*/
		return;
	}

	if (!NewFilter->Error) FilterList->AddObject(Filter, (TObject*)NewFilter);
	else delete NewFilter;
}




//---------------------------------------------------------------------------
void Classmacro::execute(long Line) {

	for (int ii = 0; ii < FilterList->Count; ii++)
		((Classfilter*)FilterList->Objects[ii])->doFunctions(Line);
}

//---------------------------------------------------------------------------
void Classmacro::fixup(void) {

	for (int ii = 0; ii < FilterList->Count; ii++)
		((Classfilter*)FilterList->Objects[ii])->fixup();
}

//---------------------------------------------------------------------------
Classmacro::Classmacro(ClassData *File) : File(File), Interpolation(0) {

	FilterList = new TStringList;
	FilterList->Sorted = false;
}

//---------------------------------------------------------------------------
Classmacro::~Classmacro() { // calls all filters to finalise data memory addresses

	if (Interpolation > 0 && FilterList->Count > Interpolation)
		LogError("Filter execution occurs before interpolation -> values may be incorrect." + File->DataFileName, TExcept::WARNING);

	//for (int ii = 0; ii < FilterList->Count; ii++)
	//	delete (Classfilter*)FilterList->Objects[ii];
	delete FilterList;
}


//---------------------------------------------------------------------------
ClassForce::ClassForce(ClassData *MyObs, string ToVar, string args, string argtypes) :
	Classfilter(MyObs, ToVar, args, argtypes) {
	readargs();

	if (MyObs->FilterCnt > 0)
		error("ForceFilter filter must be the first filter.");

	if (Constants[0] > 288)
		error("ForceFilter filter interval cannot be less than 5 minutes ");

	if (Constants[0] < 1)
		error("ForceFilter filter interval cannot be greater than daily ");

	MyObs->ForceInterval = (long)Constants[0];
}


//---------------------------------------------------------------------------
void   Update_Main_DIM(TDim Dim, long dim) {
	//SendMessage(Global::crhmMain, WM_CRHM_Main_DIM, (unsigned int)Dim, (unsigned int)dim);
}

//---------------------------------------------------------------------------
void   Update_Main_Dirty(void) {
	//SendMessage(Global::crhmMain, WM_CRHM_Main_DIRTY, (unsigned int)0, (unsigned int)0);
}

//---------------------------------------------------------------------------

long getdim(TDim dimen) {
	MapDim::iterator itDim;
	string s;

	switch (dimen) {

	case TDim::BASIN:
		return ((int)TDim::ONE);

	case TDim::ONE:
	case TDim::TWO:
	case TDim::THREE:
	case TDim::FOUR:
	case TDim::FIVE:
	case TDim::SIX:
	case TDim::SEVEN:
	case TDim::EIGHT:
	case TDim::NINE:
	case TDim::TEN:
	case TDim::ELEVEN:
	case TDim::TWELVE:
		return ((int)dimen);

	case TDim::NHRU:
		return (Global::nhru);

	case TDim::NOBS:
		return (Global::nobs);

	case TDim::NLAY:
		return (Global::nlay);

	default:

		return(-1);
	}
}

//---------------------------------------------------------------------------
long getstep(void) { return Global::DTindx - Global::DTmin + 1; }

//---------------------------------------------------------------------------
bool laststep(void) { return (Global::DTindx == Global::DTmax - 1); }

//---------------------------------------------------------------------------
void setdim(TDim dimen, long dim) {
	MapDim::iterator itDim;
	string s;

	switch (dimen) {

	case TDim::NHRU: s = "nhru";
		Update_Main_DIM(TDim::NHRU, dim);
		break;

	case TDim::NOBS: s = "nobs";
		Update_Main_DIM(TDim::NOBS, dim);
		break;

	case TDim::NLAY: s = "nlay";
		Update_Main_DIM(TDim::NLAY, dim);
		break;

	default:

		return;
	}
}

//---------------------------------------------------------------------------
void   LogError(CRHMException Except) {
	//SendMessage(Global::crhmLog, WM_CRHM_LOG_EXCEPTION, (unsigned int)&Except, 0);
	if (Except.Kind == TExcept::TERMINATE)
		throw (Except);
}

//---------------------------------------------------------------------------
void   LogError(string S, TExcept Kind) {
	//SendMessage(Global::crhmLog, WM_CRHM_LOG_EXCEPTION1, (unsigned int)&S, (unsigned int)&Kind);
}

//---------------------------------------------------------------------------
void   LogMessage(const char *S, double V, const char *S2, TExtra Opt) {

	string SS = S + FloatToStrF(V, "ffFixed", 10, 4) + " " + S2;

	LogMessage(SS.c_str(), Opt);
}

//---------------------------------------------------------------------------
void   LogMessage(const char *S, double V, TExtra Opt) {

	string D, SS;
	switch (Opt) {
	case TExtra::BLANK:
		break;
	case TExtra::DD:
		D = FormatString(Global::DTnow, "yy mm dd ");
		break;
	case TExtra::DT:
		D = FormatString(Global::DTnow, "yy mm dd hh nn ");
		break;
	case TExtra::TT:
		D = FormatString(Global::DTnow, "hh nn ");
	default:
		break;
	}

	if (Opt == TExtra::BLANK)
		SS = S + FloatToStrF(V, "ffFixed", 10, 4);
	else
		SS = D + S + FloatToStrF(V, "ffFixed", 10, 4);

	//SendMessage(Global::crhmLog, WM_CRHM_LOG_DEBUG, (unsigned int)&SS, (unsigned int)0);
}

//---------------------------------------------------------------------------
void   LogMessage(const char *S, long V, TExtra Opt) {

	string D, SS;
	switch (Opt) {
	case TExtra::BLANK:
		break;
	case TExtra::DD:
		D = FormatString(Global::DTnow, "yy mm dd ");
		break;
	case TExtra::DT:
		D = FormatString(Global::DTnow, "yy mm dd hh nn ");
		break;
	case TExtra::TT:
		D = FormatString(Global::DTnow, "hh nn ");
	default:
		break;
	}

	if (Opt == TExtra::BLANK)
		SS = S + to_string(V);
	else
		SS = D + S + to_string(V);

	//SendMessage(Global::crhmLog, WM_CRHM_LOG_DEBUG, (unsigned int)&SS, (unsigned int)0);
}

//---------------------------------------------------------------------------
void   LogMessage(const char *S, TExtra Opt) {

	string D, SS;
	switch (Opt) {
	case TExtra::BLANK:
		break;
	case TExtra::DD:
		D = FormatString(Global::DTnow, "yy mm dd ");
		break;
	case TExtra::DT:
		D = FormatString(Global::DTnow, "yy mm dd hh nn ");
		break;
	case TExtra::TT:
		D = FormatString(Global::DTnow, "hh nn ");
	default:
		break;
	}

	if (Opt == TExtra::BLANK)
		SS = S;
	else
		SS = D + S;

	//SendMessage(Global::crhmLog, WM_CRHM_LOG_DEBUG, (unsigned int)&SS, (unsigned int)0);
}

//---------------------------------------------------------------------------
void   LogMessage(long hh, const char *S, double V, TExtra Opt) {

	string A = string("HRU " + to_string(hh + 1) + ": ");
	string D;
	switch (Opt) {
	case TExtra::BLANK:
		break;
	case TExtra::DD:
		D = FormatString(Global::DTnow, "yy mm dd ");
		break;
	case TExtra::DT:
		D = FormatString(Global::DTnow, "yy mm dd hh nn ");
		break;
	case TExtra::TT:
		D = FormatString(Global::DTnow, "hh nn ");
	default:
		break;
	}

	string SS = A + D + S + FloatToStrF(V, "ffFixed", 10, 4);
	//SendMessage(Global::crhmLog, WM_CRHM_LOG_DEBUG, (unsigned int)&SS, (unsigned int)0);
}

//---------------------------------------------------------------------------
void   LogMessage(long hh, const char *S, long V, TExtra Opt) {

	string A = string("HRU " + to_string(hh + 1) + ": ");
	string D;
	switch (Opt) {
	case TExtra::BLANK:
		break;
	case TExtra::DD:
		D = FormatString(Global::DTnow, "yy mm dd ");
		break;
	case TExtra::DT:
		D = FormatString(Global::DTnow, "yy mm dd hh nn ");
		break;
	case TExtra::TT:
		D = FormatString(Global::DTnow, "hh nn ");
	default:
		break;
	}

	string SS = A + D + S + to_string(V);
	//SendMessage(Global::crhmLog, WM_CRHM_LOG_DEBUG, (unsigned int)&SS, (unsigned int)0);
}

//---------------------------------------------------------------------------
void   LogMessage(long hh, const char *S, TExtra Opt) {

	string A = string("HRU " + to_string(hh + 1) + ": ");
	string D;
	switch (Opt) {
	case TExtra::BLANK:
		break;
	case TExtra::DD:
		D = FormatString(Global::DTnow, "yy mm dd ");
		break;
	case TExtra::DT:
		D = FormatString(Global::DTnow, "yy mm dd hh nn ");
		break;
	case TExtra::TT:
		D = FormatString(Global::DTnow, "hh nn ");
	default:
		break;
	}

	string SS = A + D + S;
	//SendMessage(Global::crhmLog, WM_CRHM_LOG_DEBUG, (unsigned int)&SS, (unsigned int)0);
}

//---------------------------------------------------------------------------
void   LogDebug(char *S) {

	string SS = S;
	//SendMessage(Global::crhmLog, WM_CRHM_LOG_DEBUG, (unsigned int)&SS, (unsigned int)0);
}

//---------------------------------------------------------------------------
void   LogDebug(long h, char *Text, double v) {

	string S = to_string(h) + to_string(1);
	S += Text + to_string(v);

	//SendMessage(Global::crhmLog, WM_CRHM_LOG_DEBUG, (unsigned int)&S, (unsigned int)0);
}

//---------------------------------------------------------------------------
void   LogDebugD(char *S) {

	string D = FormatString(Global::DTnow, "yy mm dd ");

	string SS = D + S;
	//SendMessage(Global::crhmLog, WM_CRHM_LOG_DEBUG, (unsigned int)&SS, (unsigned int)0);
}

//---------------------------------------------------------------------------
void   LogDebugT(string S) {

	string D = FormatString(Global::DTnow, "yy mm dd hh nn ");

	string SS = "yy mm dd hh mm - ";
	SS += D + " - " + S;
	//SendMessage(Global::crhmLog, WM_CRHM_LOG_DEBUG, (unsigned int)&SS, (unsigned int)0);
}

//---------------------------------------------------------------------------
void   Update_Main_Status(string S) {

	//SendMessage(Global::crhmMain, WM_CRHM_Main_STATUS, (unsigned int)&S, (unsigned int)0);
}

//---------------------------------------------------------------------------
void   LogMessage(long hh, const char *S, double V1, double V2, TExtra Opt) {

	string SS = S + FloatToStrF(V1, "ffFixed", 10, 4) + ' ' + FloatToStrF(V2, "ffFixed", 10, 4);
	LogMessage(hh, SS.c_str(), Opt);
}

//---------------------------------------------------------------------------
void   LogMessage(long hh, const char *S, double V1, double V2, double V3, TExtra Opt) {

	string SS = S + FloatToStrF(V1, "ffFixed", 10, 4) + " " + FloatToStrF(V2, "ffFixed", 10, 4) + " " + FloatToStrF(V3, "ffFixed", 10, 4);
	LogMessage(hh, SS.c_str(), Opt);
}
//---------------------------------------------------------------------------

void   LogMessageA(long hh, const char *S, double V1, TExtra Opt) {

	string SS = S + FloatToStrF(V1, "ffFixed", 10, 2);
	LogMessage(hh, SS.c_str(), Opt);
}

//---------------------------------------------------------------------------
void   LogMessageA(long hh, const char *S, double V1, const char *S2, TExtra Opt) {

	string SS = S + FloatToStrF(V1, "ffFixed", 10, 2) + " " + S2;
	LogMessage(hh, SS.c_str(), Opt);
}

void   LogMessageA(long hh, const char *S, double V1, const double HRU_area, const double Basin_area, TExtra Opt) {

	string SS = S + FloatToStrF(V1, "ffFixed", 10, 2) + " " +
		FloatToStrF(V1*HRU_area, "ffFixed", 10, 2) + " " +
		FloatToStrF(V1*HRU_area / Basin_area, "ffFixed", 10, 3);
	LogMessage(hh, SS.c_str(), Opt);
}

//---------------------------------------------------------------------------
void   LogMessageA(long hh, const char *S, double V1, const double HRU_area, const double Basin_area, const char *S2, TExtra Opt) {

	string SS = S + FloatToStrF(V1, "ffFixed", 10, 2) + " " +
		FloatToStrF(V1*HRU_area, "ffFixed", 10, 2) + " " +
		FloatToStrF(V1*HRU_area / Basin_area, "ffFixed", 10, 3) + " " + S2;
	LogMessage(hh, SS.c_str(), Opt);
}

//---------------------------------------------------------------------------
void   LogMessageA(long hh, const char *S, double V1, const double Basin_area, TExtra Opt) {

	string SS = S + FloatToStrF(V1, "ffFixed", 10, 2) + " " +
		FloatToStrF(V1 / Basin_area, "ffFixed", 10, 3);
	LogMessage(hh, SS.c_str(), Opt);
}
//---------------------------------------------------------------------------

void   LogMessageA(long hh, const char *S, double V1, const double Basin_area, const char *S2, TExtra Opt) {

	string SS = S + FloatToStrF(V1, "ffFixed", 10, 2) + " " +
		FloatToStrF(V1 / Basin_area, "ffFixed", 10, 3) + " " + S2;
	LogMessage(hh, SS.c_str(), Opt);
}
//---------------------------------------------------------------------------

long GetSharedUnit(string units) {
	const string delims("()/* \t,.-_");
	string::size_type begIdx, endIdx;
	begIdx = units.find_first_not_of(delims);

	while (begIdx != string::npos) {
		endIdx = units.find_first_of(delims, begIdx);
		if (endIdx == string::npos)
			endIdx = units.length();

		string name(units.substr(begIdx, endIdx - begIdx));

		if (name == "d")
			return Global::Freq;
		else if (name == "int")
			return 1;

		begIdx = units.find_first_not_of(delims, endIdx);
	}
	return 0; // was -1!!!!!!
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

Convert::Convert() {
	NumUnits = 0;
	NumBasicUnits = 0;
	NumMultipliers = 0;

	while (!(Infos[NumUnits].Abbr).empty()) {
		if ((Infos[NumUnits].Expr).empty()) {
			Infos[NumUnits].k = NumBasicUnits;
			++NumBasicUnits;
		}

		++NumUnits;
	}

	while (Multipliers[NumMultipliers].Abbr) {
		++NumMultipliers;
	}
}

Unit_Info Convert::Infos[] = {
	Unit_Info("1", "1", 1, "") ,
	Unit_Info("A", "Ampere" , 1, "") ,
	Unit_Info("Bq", "becquerel", 1, "l/s") ,
	Unit_Info("Btu", "InternationalTableBtu", 1055.05585262, "Kg*m^2/s^2") ,
	Unit_Info("C", "Coulomb", 1, "A*s") ,
	Unit_Info("Ci", "Curie", 3.7 * 1010, "l/s") ,
	Unit_Info("F", "Farad", 1, "A^2*s^4/Kg*m^2") ,
	Unit_Info("Fdy", "Faraday", 96487, "A*s") ,
	Unit_Info("Gy", "Gray", 1, "m^2/s^2") ,
	Unit_Info("H", "Henry", 1, "Kg*m^2/A^2*s^2") ,
	Unit_Info("Hz", "Hertz", 1, "s^-1") ,
	Unit_Info("J", "Joule", 1, "Kg*m^2/s^2") ,
	Unit_Info("K", "Kelvin" , 1, ""),
	Unit_Info("Knot", "NauticalMilesPerHour", 463.0 / 900.0, "m/s") ,
	Unit_Info("N", "Newton", 1, "Kg*m/s^2") ,
	Unit_Info("Ohm", "Ohm", 1, "Kg*m^2/A^2*s^3") ,
	Unit_Info("OzUK", "UKFluidOunce", 2.8413075e-5, "m^3") ,
	Unit_Info("P", "Poise", 0.1, "Kg/m*s") ,
	Unit_Info("Pa", "Pascal", 1, "Kg/m*s") ,
	Unit_Info("Pdl", "Poundal", 0.13825495376, "Kg*m/s^2") ,
	Unit_Info("Pica", "Pica", 1.0 / 72.0, "in") ,
	Unit_Info("R", "Roentgen", 0.000258, "A*s/Kg") ,
	Unit_Info("S", "Siemens", 1, "A^2*s^3/Kg*m^2") ,
	Unit_Info("St", "Stokes", 0.0001, "m^2/s") ,
	Unit_Info("Sv", "Sievert", 1, "m^2/s^2") ,
	Unit_Info("T", "Tesla", 1, "Kg/A*s^2") ,
	Unit_Info("U", "UnifiedAtomicMass", 1.6605402e-27, "Kg") ,
	Unit_Info("V", "Volt", 1, "Kg*m^2/A*s^2") ,
	Unit_Info("W", "Watt", 1, "Kg*m^2/s^3") ,
	Unit_Info("Wb", "Weber", 1, "Kg*m^2/A*s^2") ,
	Unit_Info("acre", "Acre", 4046.87260987, "m^2") ,
	Unit_Info("arcmin", "ArcMinute", 2.9088820866e-4, "r") ,
	Unit_Info("arcs", "ArcSecond", 4.848136111e-6, "r") ,
	Unit_Info("atm", "Attmosphere", 101325, "Kg/m*s^2") ,
	Unit_Info("au", "AstronomicalUnit", 1.495979 * 1011, "m") ,
	Unit_Info("b", "Barn", 1e-28, "m^2") ,
	Unit_Info("bar", "Bar", 100000, "Kg/m*s^2") ,
	Unit_Info("bbl", "Barrel", 0.158987294928, "m^3") ,
	Unit_Info("bu", "Bushel", 0.03523907, "m^3") ,
	Unit_Info("c", "LightSpeed", 299792458, "m/s") ,
	Unit_Info("cal", "Calorie", 4.1868, "Kg*m^2/s^2") ,
	Unit_Info("cd", "Candela" , 1, "") ,
	Unit_Info("chain", "Chain", 20.1168402337, "m") ,
	Unit_Info("ct", "Carat", 0.0002, "Kg") ,
	Unit_Info("cu", "USCup", 2.365882365e-4, "m^3") ,
	Unit_Info("d", "day", 86400, "s") ,
	Unit_Info("dyn", "Dyne", 0.00001, "Kg*m/s^2") ,
	Unit_Info("eV", "ElectronVolt", 1.60217733e-19, "Kg*m^2/s^2") ,
	Unit_Info("erg", "Erg", 0.0000001, "Kg*m^2/s^2") ,
	Unit_Info("fath", "Fathom", 1.82880365761, "m") ,
	Unit_Info("fbm", "BoardFoot", 0.002359737216, "m^3") ,
	Unit_Info("fc", "Footcandle", 10.7639104167, "cd*sr/m^2") ,
	Unit_Info("fermi", "Fermi", 1e-15, "m") ,
	Unit_Info("flam", "Footlambert", 3.42625909964, "cd/m^2") ,
	Unit_Info("ft", "InternationalFoot", 0.3048, "m") ,
	Unit_Info("ftUS", "SurveyFoot", 0.304800609601, "m") ,
	Unit_Info("g", "Gram" , 1, "") ,
	Unit_Info("ga", "StandardFreefall", 9.80665, "m/s^2") ,
	Unit_Info("gal", "USGallon", 0.003785411784, "m^3") ,
	Unit_Info("galC", "CanadianGallon", 0.00454609, "m^3") ,
	Unit_Info("galUK", "UKGallon", 0.004546092, "m^3") ,
	Unit_Info("gf", "GramForce", 0.00980665, "Kg*m/s^2") ,
	Unit_Info("grad", "Grade", 1.57079632679e-2, "r") ,
	Unit_Info("grain", "Grain", 0.00006479891, "Kg") ,
	Unit_Info("h", "Hour", 3600, "s") ,
	Unit_Info("ha", "Hectare", 10000, "m^2") ,
	Unit_Info("hp", "horsepower", 745.699871582, "Kg*m^2/s^2") ,
	Unit_Info("in", "Inch", 0.0254, "m") ,
	Unit_Info("inH2O", "InchesOfWater", 248.84, "Kg/m*s^2") ,
	Unit_Info("inHg", "InchesOfMercury", 3386.38815789, "Kg/m*s^2") ,
	Unit_Info("int", "CRHM time step", 3600, "s") ,
	Unit_Info("kip", "KilopoundForce", 4448.22161526, "Kg*m/s^2") ,
	Unit_Info("kph", "KilometersPerHour", 5.0 / 18.0, "m/s") ,
	Unit_Info("l", "Liter", 0.001, "m^3") ,
	Unit_Info("lam", "Lambert", 3183.09886184, "cd/m^2") ,
	Unit_Info("lb", "AvoirdupoisPound", 0.45359267, "Kg") ,
	Unit_Info("lbf", "PoundForce", 4.44822161526, "Kg*m/s^2") ,
	Unit_Info("lbt", "TroyPound", 0.3732417216, "Kg") ,
	Unit_Info("lm", "Lumen", 1, "cd*sr") ,
	Unit_Info("lx", "Lux", 1, "cd*sr/m^2") ,
	Unit_Info("lyr", "LightYear", 9.46052840488 * 1015, "m") ,
	Unit_Info("m", "Meter" , 1, "") ,
	Unit_Info("mho", "Mho", 1, "A^2*s^2/Kg*m^2") ,
	Unit_Info("mi", "InternationalMile", 1609.344, "m") ,
	Unit_Info("miUS", "USStatuteMile", 1609.34721869, "m") ,
	Unit_Info("mil", "Mil", 0.0000254, "m") ,
	Unit_Info("min", "Minute", 60, "s") ,
	Unit_Info("mmHg", "MilimeterOfMercury", 133.322368421, "Kg/m*s^2") ,
	Unit_Info("mol", "Mole" , 1, "") ,
	Unit_Info("mph", "MilesPerHour", 0.44704, "m/s") ,
	Unit_Info("nmi", "NauticalMile", 1852, "m") ,
	Unit_Info("oz", "Ounce", 0.028349523125, "Kg") ,
	Unit_Info("ozfl", "USFluidOunce", 2.95735295625e-5, "m^3") ,
	Unit_Info("ozt", "TroyOunce", 0.0311034768, "Kg") ,
	Unit_Info("pc", "Parsec", 3.08567818585106, "m") ,
	Unit_Info("ph", "Phot", 10000, "cd*sr/m^2") ,
	Unit_Info("pk", "Peck", 0.0088097675, "m^3") ,
	Unit_Info("psi", "PoundsPerSquareInch", 6894.75729317, "Kg/m*s^2") ,
	Unit_Info("pt", "Pint", 0.000473176473, "m^3") ,
	Unit_Info("qt", "Quart", 0.000946352946, "m^3") ,
	Unit_Info("r", "Radian" , 1, "") ,
	Unit_Info("rad", "Rad", 0.01, "m^2/s^2") ,
	Unit_Info("rd", "Rod", 5.02921005842, "m") ,
	Unit_Info("rem", "Rem", 0.01, "m^2/s^2") ,
	Unit_Info("s", "Second" , 1, "") ,
	Unit_Info("sb", "Stilb", 10000, "cd/m^2") ,
	Unit_Info("slug", "Slug", 14.5939029372, "Kg") ,
	Unit_Info("sr", "Steradian" , 1, "") ,
	Unit_Info("st", "Stere", 1, "m^3") ,
	Unit_Info("t", "MetricTon", 1000, "Kg") ,
	Unit_Info("tbsp", "Tablespoon", 1.47867647813e-5, "m^3") ,
	Unit_Info("therm", "EECTherm", 105506000, "Kg*m^2/s^2") ,
	Unit_Info("ton", "ShortTon", 907.18474, "Kg") ,
	Unit_Info("tonUK", "UKLongTon", 1016.0469088, "Kg") ,
	Unit_Info("torr", "Torr", 133.322368421, "Kg/m^2") ,
	Unit_Info("tsp", "Teaspoon", 4.92892159375e-6, "m^3") ,
	Unit_Info("yd", "InternationalYard", 0.9144, "m") ,
	Unit_Info("yr", "Year", 31556925.9747, "s") ,
	//Unit_Info("\B0", "Degree", 1.74532925199e-2, "r") ,
	//Unit_Info("\B0C", "DegreeCelsius", 1.0, "K"),
	//Unit_Info("\B0F", "DegreesFahrenheit", 1.0 / 1.8, "K"),
	//Unit_Info("\B0R", "DegreesRankine", 1.0 / 1.8, "K"),
	//Unit_Info("\B5", "Micron", 1e-6, "m"),
	//Unit_Info("\C5", "Angstrom", 1e-10, "m"),
	Unit_Info("\0xB", "Degree", 1.74532925199e-2, "r"),
	Unit_Info("\0xB0C", "DegreeCelsius", 1.0, "K") ,
	Unit_Info("\0xB0F", "DegreesFahrenheit", 1.0 / 1.8, "K") ,
	Unit_Info("\0xB0R", "DegreesRankine", 1.0 / 1.8, "K") ,
	Unit_Info("\0xB5", "Micron", 1e-6, "m") ,
	Unit_Info("\0xC5", "Angstrom", 1e-10, "m") ,
	Unit_Info("", "", 0, "") };


Multiplier_Info Convert::Multipliers[] = {
	Multiplier_Info('Y',	"Yotta",   1e+24) ,
	Multiplier_Info('Z',	"Zetta",   1e+21) ,
	Multiplier_Info('E',	"Exa",     1e+18) ,
	Multiplier_Info('P',	"Peta",    1e+15) ,
	Multiplier_Info('T',	"Tera",    1e+12) ,
	Multiplier_Info('G',	"Giga",    1e+9) ,
	Multiplier_Info('M',	"Mega",    1e+6) ,
	Multiplier_Info('k',        "kilo",    1e+3) ,
	Multiplier_Info('K',        "Kilo",    1e+3) ,
	Multiplier_Info('h',        "hecto",   1e+2) ,
	Multiplier_Info('H',        "Hecto",   1e+2) ,
	Multiplier_Info('D',	"Deka",    1e+1) ,
	Multiplier_Info('d',	"deci",    1e-1) ,
	Multiplier_Info('c',	"cent",    1e-2) ,
	Multiplier_Info('m',	"mili",    1e-3) ,
	Multiplier_Info('u',	"micro",   1e-6) ,
	//Multiplier_Info('\B5',        "micro",   1e-6) ,
	Multiplier_Info('n',	"nano",    1e-9) ,
	Multiplier_Info('p',	"pico",    1e-12) ,
	Multiplier_Info('f',	"femto",   1e-15) ,
	Multiplier_Info('a',	"atto",    1e-18) ,
	Multiplier_Info('z',	"zepto",   1e-21) ,
	Multiplier_Info('y',	"yocto",   1e-24) ,
	Multiplier_Info('\0',	"",          0.0) };

string tcsinc(string pszIn) {

	return pszIn.substr(1);
}

string tcsSkipSpaces(string pszIn) {
	while (pszIn[0] == ' ')
		pszIn = pszIn.substr(1);
	return pszIn;
}

string tcsSqueeze(string pszIn) {
	string::iterator pos;

	string::size_type Idx;
	//  char const Misc[] = {"( )%\BA\B0\t"};
	//char const Misc[] = { "( )%\BA\t" };
	char const Misc[] = { "( )%\0xBA\t" };

	Idx = pszIn.find_first_of(Misc);
	while (Idx != string::npos) {
		pszIn = pszIn.erase(Idx, 1);
		Idx = pszIn.find_first_of(Misc);
	}

	return pszIn;
}

// Main Functions

// Converts the <dValue> from the <pszSrcUnit> to the <pszDstUnit>

bool Convert::ConvertUnit(
	double& dValue,     // [in] value to convert, [out] converted value
	string pszSrcUnit,  // [in] source unit
	string pszDstUnit   // [in] destination unit
) {

	pszSrcUnit = tcsSkipSpaces(pszSrcUnit);
	pszDstUnit = tcsSkipSpaces(pszDstUnit);

	dValue = 1.0;

	if (pszSrcUnit.compare(pszDstUnit) == 0)
		return true;

	Tresult uSrcUnit(NumBasicUnits);
	try 
	{
		pszSrcUnit = ParseDivUnitExpr(pszSrcUnit, uSrcUnit);
	}
	catch (int i) 
	{
		if (i == 3) 
		{
			return false;
		}
	}

	Tresult uDstUnit(NumBasicUnits);
	try 
	{
		pszDstUnit = ParseDivUnitExpr(pszDstUnit, uDstUnit);
	}
	catch (int i) 
	{
		if (i == 3) 
		{
			return false;
		}
	}

	for (int i = 0; i < NumBasicUnits; ++i)
		if (uSrcUnit.aExp[i] != uDstUnit.aExp[i]) {
			dValue = -999;
			return false;
		}

	dValue *= uSrcUnit.k;
	dValue /= uDstUnit.k;

	return true;
}

double Convert::quickPow(double k, int nPow)
{
	bool bNegative = (nPow < 0);
	if (bNegative)
		nPow = -nPow;

	double r = 1.0;

	while (nPow != 0)
	{
		if (nPow & 1)
			r *= k;

		k *= k;
		nPow >>= 1;
	}

	if (bNegative)
		r = 1.0 / r;

	return r;
}

int Convert::LookupUnit(string pszIn)
{
	int iFirst = 0;
	int iLast = NumUnits;
	int i;

	string::size_type Idx;

	Idx = pszIn.find_first_of("*/^");
	if (Idx == string::npos)
		Idx = pszIn.size();

	while (iLast >= iFirst)
	{
		i = (iLast + iFirst) / 2;

		int nCmp = pszIn.compare(0, Idx, Infos[i].Abbr);

		if (nCmp == 0)
			return i;  // match

		if (nCmp < 0)
			iLast = i - 1;
		else
			iFirst = i + 1;
	}

	return -1;  // no match
}

void Convert::DivUnit(Tresult& u1, const Tresult& u2)
{
	u1.k /= u2.k;

	for (int i = 0; i < NumBasicUnits; ++i)
		u1.aExp[i] -= u2.aExp[i];
};

void Convert::MulUnit(Tresult& u1, const Tresult& u2)
{
	u1.k *= u2.k;

	for (int i = 0; i < NumBasicUnits; ++i)
		u1.aExp[i] += u2.aExp[i];
};

void Convert::MulUnitK(Tresult& u, double k)
{
	u.k *= k;
};

void Convert::PowUnit(Tresult& u, char nPow)
{
	for (int i = 0; i < NumBasicUnits; ++i)
		u.aExp[i] *= nPow;

	u.k = quickPow(u.k, nPow);
};

void Convert::SetBasicUnit(Tresult& u, double k, int iUnit)
{
	u.k = k;

	int iBasicUnit = (int)(Infos[iUnit].k);
	if (0 <= iBasicUnit)
		u.aExp[iBasicUnit] = 1;
}

void Convert::ExpandUnitExpr(Tresult& u, double k, string pszExpr)
{
	pszExpr = ParseDivUnitExpr(pszExpr, u);

	u.k *= k;
}

// Unit interpretation: template declarations

string Convert::ParseDivUnitExpr(string pszIn, Tresult& u)
{
	pszIn = ParseMulUnitExpr(pszIn, u);

	if (pszIn.empty()) 
	{
		return pszIn;
	}
		

	string pszMark = pszIn;
	pszIn = tcsSkipSpaces(pszIn);
	if (pszIn[0] != '/')
	{
		throw 3;
	}
		
	pszIn = tcsinc(pszIn);
	pszIn = tcsSkipSpaces(pszIn);

	Tresult uAux(NumBasicUnits);
	pszIn = ParseMulUnitExpr(pszIn, uAux);

	DivUnit(u, uAux);

	pszMark = pszIn;
	return pszMark;
}

string Convert::ParseMulUnitExpr(string pszIn, Tresult& u)
{
	pszIn = ParsePowUnitExpr(pszIn, u);

	if (pszIn.empty())
		return pszIn;

	string pszMark = pszIn;

	while (pszIn = tcsSkipSpaces(pszIn), pszIn[0] == '*')
	{
		pszIn = tcsinc(pszIn);

		Tresult uAux(NumBasicUnits);
		pszIn = ParsePowUnitExpr(pszIn, uAux);
		if (pszIn == "error")
			return pszMark;

		MulUnit(u, uAux);

		pszMark = pszIn;
	}

	return pszIn;
}

string Convert::ParsePowUnitExpr(string pszIn, Tresult& u)
{
	pszIn = ParseUnitPhase1(pszIn, u);

	if (pszIn.empty())
		return pszIn;

	string pszMark = pszIn;

	pszIn = tcsSkipSpaces(pszIn);
	if (pszIn[0] != '^')
		return pszMark;

	pszIn = tcsinc(pszIn);
	pszIn = tcsSkipSpaces(pszIn);

	bool bNegative = (pszIn[0] == '-');

	if (pszIn[0] == '+' || pszIn[0] == '-')
		pszIn = tcsinc(pszIn);

	//if (!_istdigit(pszIn[0]))
	if (iswdigit(pszIn[0]))
		return pszMark;

	char nPow = 0;

	//while (_istdigit(pszIn[0]))
	while (iswdigit(pszIn[0]))
	{
		char nPow2 = nPow * 10 + (pszIn[0] - '0');
		if (nPow2 < nPow)
			break;

		nPow = nPow2;
		pszIn = tcsinc(pszIn);
	}

	if (bNegative)
		nPow = -nPow;

	PowUnit(u, nPow);

	return pszIn;
}

string Convert::ParseUnitPhase1(string pszIn, Tresult& u)
{
	string pszMark = pszIn;

	pszIn = ParseUnitPhase2(pszIn, u);
	if (pszIn != "????")
		return pszIn;

	pszIn = pszMark;

	double k;
	int ii;

	for (ii = 0; ii < NumMultipliers; ++ii)
		if (pszIn[0] == Multipliers[ii].Abbr) {
			k = Multipliers[ii].k;
			break;
		}

	if (ii >= NumMultipliers) {

		bool bNegative = (pszIn[0] == '-');

		if (pszIn[0] == '+' || pszIn[0] == '-')
			pszIn = tcsinc(pszIn);

		//if (!_istdigit(pszIn[0]))
		if (!iswdigit(pszIn[0]))

			throw 1; // cause fault
		else {

			k = 0.0;

			//while (_istdigit(pszIn[0]))
			while (iswdigit(pszIn[0]))
			{
				double nPow2 = (k * 10.0) + ((long long)pszIn[0] - (long long)'0');
				if (nPow2 < k)
					break;

				k = nPow2;
				pszIn = tcsinc(pszIn);
			}

			if (bNegative)
				k = -k;

			if (pszIn[0] == '*') // allow '*'
				pszIn = tcsinc(pszIn);
		}
	}
	else
		pszIn = tcsinc(pszIn);

	pszIn = ParseUnitPhase2(pszIn, u);

	MulUnitK(u, k);

	return pszIn;
}

string Convert::ParseUnitPhase2(string pszIn, Tresult& u)
{
	int iUnit = LookupUnit(pszIn);

	if (iUnit < 0)
		return "????";

	if (Infos[iUnit].Expr.empty())
		SetBasicUnit(u, 1.0, iUnit);
	else
		ExpandUnitExpr(u, Infos[iUnit].k, Infos[iUnit].Expr);

	pszIn = pszIn.substr(Infos[iUnit].Abbr.size());

	return pszIn;
}

bool Convert::CheckUnitsTable()
{
	bool bSuccess = true;

	for (int i = 0; i < NumUnits - 1; ++i)
	{
		const Unit_Info info1 = Infos[i];
		const Unit_Info info2 = Infos[i + 1];

		if (info1.Abbr.compare(info2.Abbr) >= 0)
		{
			bSuccess = false;
		}
	}
	return bSuccess;
}

void Convert::CheckUnitsString(string Name, string variable, string units)
{
	//string::size_type Idx;
	string temp(units);

	units = tcsSqueeze(units);

	if (units.empty()) return;

	Tresult uSrcUnit(NumBasicUnits);

	try {
		units = ParseDivUnitExpr(units, uSrcUnit);
		if (!units.empty()) {
			units = temp;
			throw 2;
		}
	}
	catch (int i) {
		if (i == 3) 
		{
			CRHMException Except("Units error ParseDivUnitExpr threw error code 3: '" + units + "' in " + Name + ' ' + variable, TExcept::WARNING);
			LogError(Except);
		}
		else 
		{
			CRHMException Except("Units error ParseDivUnitExpr returned empty: '" + units + "' in " + Name + ' ' + variable, TExcept::WARNING);
			LogError(Except);
		}
		
		
	}
}

void Convert::CheckUnitsObs(string Vunits, string Munits, string declaration) { // check original observation units
	//string::size_type Idx;
	double d;

	if (!Vunits.empty()) {
		if (!ConvertUnit(d, tcsSqueeze(Vunits), tcsSqueeze(Munits)) || d != 1.0) {
			CRHMException Except("Units error: '" + Vunits + " instead of " + Munits + "' in " + declaration, TExcept::WARNING);
			LogError(Except);
		}
	}
}

//---------------------------------------------------------------------------
long julian_month(long Year, long julian) {
	int Y, Month, Day, H, Min;
	double DT = StandardConverterUtility::EncodeDateTime((Word)Year - 1, (Word)12, (Word)31, 0, 0);

	DT += (double)julian;
	StandardConverterUtility::DecodeDateTime(DT, &Y, &Month, &Day, &H, &Min);
	return Month;
}

//---------------------------------------------------------------------------
string extract_units(string& Comment)
{
	string Lunits;
	string::size_type Idx, Idx2;

	Idx = Comment.find_first_of("(");
	if (Idx != string::npos) {
		Idx2 = Comment.find_first_of(")");
		if (Idx != string::npos) {
			Lunits = Comment.substr(Idx, Idx2 - Idx + 1);
			Comment = Comment.substr(Idx2 + 1);
			Idx = Comment.find_first_not_of(" \t");
			if (Idx != string::npos)
				Comment = Comment.substr(Idx);
			else
				Comment = "";
		}
	}
	return Lunits;
}



void   LogDebug(const char* S) {

	string SS = S;
	//SendMessage(Global::crhmLog, WM_CRHM_LOG_DEBUG, (unsigned int)&SS, (unsigned int)0);
}
void   LogDebug(const string S) {

	string SS = S;
	//SendMessage(Global::crhmLog, WM_CRHM_LOG_DEBUG, (unsigned int)&SS, (unsigned int)0);
}
