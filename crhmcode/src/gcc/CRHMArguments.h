#ifndef CRHM_ARGUMENTS
#define CRHM_ARGUMENTS

#include "../core/Common/Common.h"

enum class OUTPUT_FORMAT {STD, OBS};

const string USE_MESSAGE = "\ncrhm [options] PROJECT_FILE\n"
	"\n"
	"\t-h - Display this message.\n"
	"\n"
	"\t-t TIME_FORMAT - Select the format for date time outputs.\n"
	"\t\tValid formats are:\n"
	"\t\tMS - Microsoft Excel floating point.\n"
	"\t\tYYYYMMDD - year-month-dayTtime.\n"
	"\t\tMMDDYYYY - month/day/year time.\n"
	"\n"
	"\t-f OUTPUT_FORMAT - Select the file format for the output.\n"
	"\t\tValid formats are:\n"
	"\t\tSTD - Standard output format. Sutable for a spreadsheet.\n"
	"\t\tOBS - Observation file .obs format. Sutable for reading with CRHM GUI.\n"
	"\n";

struct crhm_arguments
{
	std::string project_name;
	TIMEFORMAT time_format;
	OUTPUT_FORMAT output_format;
};

#endif // !CRHM_ARGUMENTS


