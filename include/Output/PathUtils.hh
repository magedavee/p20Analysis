/// \file PathUtils.hh \brief File/path utilities.
#ifndef PATHUTILS_HH
#define PATHUTILS_HH

#include <string>
#include <vector>

using std::string;
using std::vector;

/// check if file exists
bool fileExists(string f);
/// check if directory exists
bool dirExists(string d);
/// make sure the specified path exists (if not, create it); optionally, exclude last item on path (filename)
void makePath(string p, bool forFile = false);
/// list directory contents
vector<string> listdir(const string& dir, bool includeHidden = false);
/// get time since last file modification (s)
double fileAge(const string& fname);
/// get environment variable, with default or fail if missing
string getEnvSafe(const string& v, const string& dflt = "FAIL_IF_MISSING");

#endif
