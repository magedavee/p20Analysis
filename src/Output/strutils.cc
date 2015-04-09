#include "strutils.hh"
#include <stdlib.h>

ostream& operator<<(ostream& os, const map<int,int>& mp) {
    os << "(";
    for(auto it = mp.begin(); it != mp.end(); it++) {
        os << " " << it->first;
        if(it->second > 1) os << "x" << it->second;
    }
    os << " )";
    return os;
}

string itosRN(int i) {
    if(!i) return "0";
    string s;
    if(i<0) { s += "-"; i = -i; }
    while(i>=1000) { s += "M"; i-=1000; }
    while(i>=900) { s += "CM"; i-=900; }
    while(i>=500) { s += "D"; i-=500; }
    while(i>=400) { s += "CD"; i-=400; }
    while(i>=100) { s += "C"; i-=100; }
    while(i>=90) { s += "XC"; i-=90; }
    while(i>=50) { s += "L"; i-=50; }
    while(i>=40) { s += "XL"; i-=40; }
    while(i>=10) { s += "X"; i-=10; }
    while(i>=9) { s += "IX"; i-=9; }
    while(i>=5) { s += "V"; i-=5; }
    while(i>=4) { s += "IV"; i-=4; }
    while(i>0) { s += "I"; i--; }
    return s;
}

string vtos(const double* st, const double* en, string sep) {
    string s = "";
    if(st==en)
        return s;
    s = to_str(*st);
    for(const double* it = st+1; it != en; it++)
        s += sep+to_str(*it);
    return s;
}

string vtos(const vector<double>& ds,string sep) { return vtos(&*ds.begin(),&*ds.end(),sep); }

string vtos(const float* st, const float* en, string sep) {
    string s = "";
    if(st==en)
        return s;
    s = to_str(*st);
    for(const float* it = st+1; it != en; it++)
        s += sep+to_str(*it);
    return s;
}

string vtos(const vector<float>& ds,string sep) { return vtos(&*ds.begin(),&*ds.end(),sep); }

string vtos(const int* st, const int* en, string sep) {
    string s = "";
    if(st==en)
        return s;
    s = to_str(*st);
    for(const int* it = st+1; it != en; it++)
        s += sep+to_str(*it);
    return s;
}

string vtos(const vector<int>& ds,string sep) { return vtos(&*ds.begin(),&*ds.end(),sep); }

string ctos(char c) {
    char ch[3];
    sprintf(ch,"%c",c);
    return string(ch);         
}

string lower(string s) {
    std::transform(s.begin(), s.end(), s.begin(), (int(*)(int))tolower);
    return s;
}

string upper(string s) {
    std::transform(s.begin(), s.end(), s.begin(), (int(*)(int))toupper);
    return s;
}

string replace(string s, char o, char n) {
    string::size_type found = s.find_first_of(o);
    while( found != string::npos ) {
        s[found] = n;
        found = s.find_first_of(o,found+1);
    }
    return s;
}

bool startsWith(const string& a, const string& b) { return a.substr(0,b.size()) == b; }

vector<string> split(const string& s, const string splitchars) {
    vector<string> v;
    size_t p = 0;
    while(p<s.size()) {
        size_t wstart = s.find_first_not_of(splitchars,p);
        if(wstart == string::npos)
            break;
        p = s.find_first_of(splitchars,wstart);
        if(p == string::npos)
            p = s.size();
        v.push_back(s.substr(wstart,p-wstart));
    }
    return v;
}

string join(const vector<string>& ss, const string& sep) {
    string s = "";
    if(!ss.size())
        return s;
    s = ss[0];
    for(vector<string>::const_iterator it = ss.begin()+1; it < ss.end(); it++)
        s += sep + *it;
    return s;
}

string strip(const string& s, const string stripchars) {
    size_t wstart = s.find_first_not_of(stripchars);
    if(wstart == string::npos)
        return "";
    size_t wend = s.find_last_not_of(stripchars);
    return s.substr(wstart,wend-wstart+1);
}

vector<double> sToDoubles(const string& s, const string splitchars) {
    vector<double> v;
    vector<string> words = split(s,splitchars);
    for(unsigned int i=0; i<words.size(); i++)
        v.push_back(atof(words[i].c_str()));
    return v;
}

vector<float> sToFloats(const string& s, const string splitchars) {
    vector<float> v;
    vector<string> words = split(s,splitchars);
    for(unsigned int i=0; i<words.size(); i++)
        v.push_back(atof(words[i].c_str()));
    return v;
}

vector<int> sToInts(const string& s, const string splitchars) {
    vector<int> v;
    vector<string> words = split(s,splitchars);
    for(unsigned int i=0; i<words.size(); i++)
        v.push_back(atoi(words[i].c_str()));
    return v;
}

vector< vector<float> > readArray(std::ifstream& fin, unsigned int minitems, const string splitchars) {
    vector< vector<float> > a;
    string s;
    while (fin.good()) {
        std::getline(fin,s);
        vector<float> v = sToFloats(s,splitchars);
        if(v.size() >= minitems)
            a.push_back(v);
    }
    return a;
}
