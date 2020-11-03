//created by Manishankar Mondal

#include "ClassModule.h"

class ClassSoil : public ClassModule {
public:

ClassSoil(string Name, string Version = "undefined", LMODULE Lvl = LMODULE::PROTO) : ClassModule(Name, Version, Lvl) {};

long snowinfilDiv{0};
long meltrunoffDiv{0};
long runoffDiv{0};
long evapDiv{0};

// declared observation variables

// declared variables
long *current_getstep{ NULL };
double *Sd{ NULL };
double *gw{ NULL };
double *soil_rechr{ NULL };
double *soil_moist{ NULL };
double *soil_gw{ NULL };
double *Sd_gw{ NULL };
double *gw_flow{ NULL };
double *soil_ssr{ NULL };
double *rechr_ssr{ NULL };
double *soil_runoff{ NULL };
double *cum_soil_ssr{ NULL };
double *cum_rechr_ssr{ NULL };
double *cum_soil_runoff{ NULL };
double *cum_runoff_to_Sd{ NULL };
double *cum_redirected_residual{ NULL };
double *soil_gw_D{ NULL };
double *soil_ssr_D{ NULL };
double *gw_flow_D{ NULL };
double *soil_runoff_D{ NULL };
double *HK_V{ NULL };
double *HK_A{ NULL };
double *culvert_Q{ NULL };
double *culvert_water_H{ NULL };
double *culvert_water_A{ NULL };
double *culvert_water_V{ NULL };
double *culvert_over_Q{ NULL };
double *culvert_evap{ NULL };
double *cum_culvert{ NULL };
double *cum_culvert_over{ NULL };
double *HD{ NULL };
double *redirected_residual{ NULL };
double *infil_act{ NULL };
double *cum_infil_act{ NULL };
double *infil_act_D{ NULL };

double *soil_runoff_stream{ NULL };
double *weir_outflow{ NULL };
double *weir_outflow_m3s{ NULL };
double *weir_water_A{ NULL };
double *weir_water_V{ NULL };
double *weir_water_h{ NULL };
double *weir_evap{ NULL };
double *cum_weir_over_m3{ NULL };
double *weir_crest_volume{ NULL };
double *stream{ NULL };
double *stream_to_Sd{ NULL };
double *cum_stream{ NULL };
double *stream_Grp_in{ NULL };
double *stream_Grp_loss{ NULL };
double *cum_stream_Grp_loss{ NULL };

// local variables
double *cum_soil_gw{ NULL };
double *cum_Sd_gw{ NULL };
double *cum_gw_flow{ NULL };
double *cum_hru_condense{ NULL };
double *cum_Sd_evap{ NULL };
double *soil_moist_Init{ NULL };
double *soil_rechr_Init{ NULL };
double *Sd_Init{ NULL };
double *gw_Init{ NULL };
double *SWE_Init{ NULL };

double *snowinfil_buf{ NULL };
double *runoff_buf{ NULL };
double *meltrunoff_buf{ NULL };
double *hru_evap_buf{ NULL };

// declared parameters
const double *Sdmax{ NULL };
const double *Sdinit{ NULL };
const double *gw_max{ NULL };
const double *gw_init{ NULL };
const double *gw_K{ NULL };
const double *basin_area{ NULL };   // [BASIN]
const double *hru_area{ NULL };
const double *soil_rechr_max{ NULL };
const double *soil_rechr_init{ NULL };
const double *soil_moist_max{ NULL };
const double *soil_moist_init{ NULL };
const double *soil_gw_K{ NULL };
const double *rechr_ssr_K{ NULL };
const double *lower_ssr_K{ NULL };
const double *Sd_ssr_K{ NULL };
const double *Sd_gw_K{ NULL };
const double *channel_slope{ NULL };
const double *side_slope{ NULL };
const double *culvert_diam{ NULL };
const double *culvert_water_Dmax{ NULL };
const double *number_culverts{ NULL };

const double *weir_crest{ NULL };
const double *weir_coefficient{ NULL };
const double *weir_mult{ NULL };
const double *weir_b{ NULL };
const double *lake_mult{ NULL };
const double *lake_power{ NULL };
const double *lake_p{ NULL };
const double *lake_S{ NULL };
const double *runoff_stream{ NULL };
const double *stream_runoff{ NULL };
const double *stream_Sd{ NULL };
const double *Wetlands_scaling_factor{ NULL };    // temporary modification

const long *culvert_type{ NULL };
const long *cov_type{ NULL };
const long *transp_limited{ NULL };
const long *soil_ssr_runoff{ NULL };
const long *inhibit_evap{ NULL };
const long *soil_withdrawal{ NULL };
const long **soil_withdrawal_Tables{ NULL };

// variable inputs
const double *infil{ NULL };
const double *snowinfil{ NULL };
const double *meltrunoff{ NULL }; // frozen melt runoff
const double *runoff{ NULL };     // liquid runoff
const double *hru_evap{ NULL };
const double *SD{ NULL };

// variable put inputs
double *hru_actet{ NULL };      // [nhru]
double *hru_cum_actet{ NULL };


// local allocated arrays

void decl(void);
void init(void);
void run(void);
void finish(bool good);

ClassSoil* klone(string name) const;
};