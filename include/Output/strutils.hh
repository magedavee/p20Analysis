/// \file Output/strutils.hh \brief String manipulation utilities.
#ifndef STRUTILS_HH
#define STRUTILS_HH

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>

using std::string;
using std::vector;
using std::ifstream;

/// utility function for converting to string
template<typename T>
string to_str(T x) {
    std::stringstream ss;
    ss << x;
    return ss.str();
}

/// integer to roman numerals string
string itosRN(int i);

/// convert an array to a string list
template<typename T>
string vtos(const T* st, const T* en, string sep = ",") {
    string ss = "";
    if(st==en)
        return ss;
    ss = to_str(*st);
    for(const T* it = st+1; it != en; it++)
        ss += sep + to_str(*it);
    return ss;
}

/// convert a vector to a string list
template<typename T>
string vtos(const vector<T>& ds, string sep = ",") {  return vtos(&*ds.begin(),&*ds.end(),sep); }

/// split a string into a vector of doubles
vector<double> sToDoubles(const string& str, const string splitchars = ", \t\r\n");
/// split a string into a vector of ints
vector<int> sToInts(const string& str, const string splitchars = ", \t\r\n");
/// read in an array from a file
vector< vector<double> > readArray(ifstream& fin, unsigned int minitems = 1, const string splitchars = ", \t\r\n");

/// convert a char to a string
string ctos(char c);
/// convert a string to lowercase
string lower(string s);
/// convert a string to uppercase
string upper(string s);
/// replace all of one character in a string with another
string replace(string str, char o, char n);
/// check whether string a begins with string b
bool startsWith(const string& a, const string& b);
/// split a string into substrings on given split characters
vector<string> split(const string& str, const string splitchars = " \t\r\n");
/// join a list of strings into a single string
string join(const vector<string>& ss, const string& sep = " ");
/// strip junk chars off start and end of string
string strip(const string& str, const string stripchars = " \t\r\n");

#endif
