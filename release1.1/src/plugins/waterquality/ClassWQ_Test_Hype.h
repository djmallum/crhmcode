#pragma once

#ifndef CLASSWQ_TEST_HYPE
#define CLASSWQ_TEST_HYPE

#include "WQ_CRHM.h"

class ClassWQ_Test_Hype : public ClassModule {
public:

	ClassWQ_Test_Hype(string Name, string Version = "undefined", LMODULE Lvl = LMODULE::PROTO) : ClassModule(Name, Version, Lvl) {};

	// declared variables
	double* infil{ NULL };
	double* snowinfil{ NULL };
	double* runoff{ NULL };
	double* meltrunoff{ NULL };
	double* hru_evap{ NULL };
	double* hru_cum_evap{ NULL };
	double* hru_cum_actet{ NULL };
	double* hru_actet{ NULL };
	double* net_rain{ NULL };
	double* SWE{ NULL };
	double* SWE_max{ NULL };
	double* hru_t{ NULL };
	double* SWE_conc{ NULL };
	double** SWE_conc_lay{ NULL };

	double* soil_moist{ NULL };
	double* soil_rechr{ NULL };

	// declared parameters
	const double* infil_0{ NULL };
	const double* snowinfil_0{ NULL };
	const double* runoff_0{ NULL };
	const double* meltrunoff_0{ NULL };
	const double* hru_evap_0{ NULL };
	const double* hru_actet_0{ NULL };
	const double* hru_cum_evap_0{ NULL };
	const double* hru_cum_actet_0{ NULL };
	const double* net_rain_0{ NULL };
	const double* SWE_0{ NULL };
	const double* hru_t_0{ NULL };
	const double* SWE_conc_0{ NULL };
	const double** SWE_conc_lay_0{ NULL };

	const double* soil_rechr_0{ NULL };
	const double* soil_moist_0{ NULL };
	const double* soil_rechr_max{ NULL };
	const double* soil_moist_max{ NULL };

	const long* Julian_start{ NULL };
	const long* Julian_end{ NULL };

	void decl(void);
	void init(void);
	void run(void);
	void finish(bool good);

	ClassWQ_Test_Hype* klone(string name) const;
};


#endif // !CLASSWQ_TEST_HYPE
