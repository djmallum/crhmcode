#pragma once

#ifndef CRHM_NAMESPACE
#define CRHM_NAMESPACE

namespace CRHM {
	enum TFitType { POLY, FOURIER, POWER, EXPO, LOGARITHM, MLR };
	enum TAgainst { myTIME, VARIABLE };
	enum TDsply { FRACT, OBS, BOTH, MDY, XY };
	enum TTiming { IDLE, EARLY, MELT, MELT2, MATURE, HOLD };
	enum TValue { PSI, KSAT, WILT, FCAP, PORG, PORE, AIRENT, PORESZ, AVAIL };
	enum TFrozen { PREMELT, RESTRICTED, LIMITED, UNLIMITED, SATURATED };
	enum TSANDCLAY { LOAM1, LOAM2, LOAM3, SAND, CLAY };
	enum { NOTUSED, DRIFT, HUMMOCK };
	enum TCONDITION { FROZEN, ICEWATER, WATER, DRAINING };
	enum CONTROLSTATES { SAVE, LOAD };
	enum HRUOBSTYPES { HRU_OBS_t_rh_ea = 1, HRU_OBS_p_ppt, HRU_OBS_u, HRU_OBS_Q, HRU_OBS_misc };
}

#endif // !CRHM_NAMESPACE
