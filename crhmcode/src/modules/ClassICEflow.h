//created by Manishankar Mondal

#include "../core/ClassModule.h"

class ClassICEflow : public ClassModule {
public:

ClassICEflow(string Name, string Version = "undefined", LMODULE Lvl = LMODULE::PROTO) : ClassModule(Name, Version, Lvl, 1001, "") {};

// declared variables
double *Ux{ NULL };
double *Ub{ NULL };
double *Utot{ NULL };
double *Ice_in{ NULL };
double *Ice_out{ NULL };
double *cumIce_in{ NULL };
double *cumIce_out{ NULL };
double *cumulative_net_rain{ NULL };

// declared parameters
const double *distrib{ NULL };
const double *radius_glacier{ NULL };
const double *Arrhenius_const{ NULL };
const double *slip_c{ NULL };
const double *slip_m{ NULL };
const double *slip_sf{ NULL };
const double *slip_fs{ NULL };
const double **distrib_hru{ NULL };
const double *basin_area{ NULL };   // [BASIN]
const double *hru_area{ NULL }; // [nhru]
const double *test_option{ NULL }; // [nhru]
const long  *SWE_to_firn_Julian{ NULL };
const long  *order{ NULL }; // [nhru]
const long *Disable_ICEflow{ NULL }; // [nhru]
const long  *channel_option{ NULL }; // [nhru]
const double *basal_option{ NULL }; // [nhru]

// variable get inputs
const double *firn{ NULL };
const double *net_rain{ NULL };
const double *hru_GSL{ NULL };

// variable get/put inputs
double *ice{ NULL };

void decl(void);
void init(void);
void run(void);
void finish(bool good);

ClassICEflow* klone(string name) const;
};
