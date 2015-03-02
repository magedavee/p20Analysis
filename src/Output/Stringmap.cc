#include "Stringmap.hh"

#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include "strutils.hh"
#include "PathUtils.hh"
#include "SMExcept.hh"

Stringmap::Stringmap(const string& str) {
    vector<string> pairs = split(str,"\t");
    for(vector<string>::const_iterator it = pairs.begin(); it!=pairs.end(); it++) {
        vector<string> keyval = split(*it,"=");
        if(keyval.size() != 2)
            continue;
        dat.insert(std::make_pair(strip(keyval[0]),strip(keyval[1])));
    }
}

Stringmap::Stringmap(const Stringmap& m) {
    for(multimap< string, string >::const_iterator it = m.dat.begin(); it!=m.dat.end(); it++)
        dat.insert(std::make_pair(it->first,it->second));
}

void Stringmap::insert(const string& s, const string& v) {
    dat.insert(std::make_pair(s,v));
}

void Stringmap::insert(const string& s, double d) {
    insert(s,to_str(d));
}

void Stringmap::erase(const string& s) { dat.erase(s); }

vector<string> Stringmap::retrieve(const string& s) const {
    vector<string> v;
    for(multimap<string,string>::const_iterator it = dat.lower_bound(s); it != dat.upper_bound(s); it++)
        v.push_back(it->second);
    return v;
}

string Stringmap::getDefault(const string& s, const string& d) const {
    multimap<string,string>::const_iterator it = dat.find(s);
    if(it == dat.end())
        return d;
    return it->second;
}

string Stringmap::toString() const {
    string s;
    for(multimap<string,string>::const_iterator it = dat.begin(); it != dat.end(); it++)
        s += "\t" + it->first + " = " + it->second;
    return s;
}

void Stringmap::display(string linepfx) const {
    for(multimap<string,string>::const_iterator it = dat.begin(); it != dat.end(); it++)
        std::cout << linepfx <<	it->first << ": " << it->second << "\n";
}


double Stringmap::getDefault(const string& k, double d) const {
    string s = getDefault(k,"");
    if(!s.size())
        return d;
    std::stringstream ss(s);
    ss >> d;
    return d;
}

vector<double> Stringmap::retrieveDouble(const string& k) const {
    vector<string> vs = retrieve(k);
    vector<double> v;
    double d;
    for(vector<string>::const_iterator it = vs.begin(); it != vs.end(); it++) {
        std::stringstream s(*it);
        s >> d;
        v.push_back(d);
    }
    return v;
}

void Stringmap::mergeInto(Stringmap& S) const {
    for(multimap<string,string>::const_iterator it = dat.begin(); it != dat.end(); it++)
        S.insert(it->first,it->second);	
}

//-------------------------------------------------

SMFile::SMFile(const std::string& fname, bool readit) {
    name = fname;
    if(!readit || name=="")
        return;
    if(!fileExists(fname)) {
        SMExcept e("fileUnreadable");
        e.insert("filename",fname);
        throw(e);
    }
    std::ifstream fin(fname.c_str());
    string s;
    while (fin.good()) {
        std::getline(fin,s);
        s = strip(s);
        size_t n = s.find(':');
        if(n==std::string::npos || s[0]=='#') continue;
        string key = s.substr(0,n);
        string vals = s.substr(n+1);
        vals=strip(vals);
        while(vals.size() && vals[vals.size()-1]=='\\') {
            vals.erase(vals.size()-1);
            std::getline(fin,s);
            s = strip(s);
            vals += '\t';
        vals += s;
        }
        insert(key,Stringmap(vals));
    }
    fin.close();
}

void SMFile::insert(const std::string& s, const Stringmap& v) {
    dat.insert(std::make_pair(s,v));
}

void SMFile::erase(const std::string& s) { dat.erase(s); }

vector<Stringmap> SMFile::retrieve(const std::string& s) const {
    vector<Stringmap> v;
    for(multimap<std::string,Stringmap>::const_iterator it = dat.lower_bound(s); it != dat.upper_bound(s); it++)
        v.push_back(it->second);
    return v;
}

void SMFile::transfer(const SMFile& Q, const std::string& k) {
    vector<Stringmap> v = Q.retrieve(k);
    for(auto it = v.begin(); it != v.end(); it++)
        insert(k,*it);
}

void SMFile::display() const {
    for(multimap<std::string, Stringmap>::const_iterator it = dat.begin(); it != dat.end(); it++) {
        std::cout << "--- " << it->first << " ---:\n";
        it->second.display();
    }
}

void SMFile::commit(string outname) const {
    if(outname=="")
        outname = name;
    makePath(outname,true);
    std::ofstream fout(outname.c_str());
    if(!fout.good()) {
        SMExcept e("fileUnwriteable");
        e.insert("filename",outname);
        throw(e);
    }
    printf("Writing File '%s'.\n",outname.c_str());
    for(multimap<std::string, Stringmap>::const_iterator it = dat.begin(); it != dat.end(); it++)
        fout << it->first << ":\t" << it->second.toString() << "\n";
    fout.close();
}

vector<string> SMFile::retrieve(const std::string& k1, const std::string& k2) const {
    vector<string> v1;
    for(multimap<std::string,Stringmap>::const_iterator it = dat.lower_bound(k1); it != dat.upper_bound(k1); it++) {
        vector<string> v2 = it->second.retrieve(k2);
        for(vector<string>::const_iterator it2 = v2.begin(); it2 != v2.end(); it2++)
            v1.push_back(*it2);
    }
    return v1;
}

vector<double> SMFile::retrieveDouble(const std::string& k1, const std::string& k2) const {
    vector<double> v1;
    for(multimap<std::string,Stringmap>::const_iterator it = dat.lower_bound(k1); it != dat.upper_bound(k1); it++) {
        vector<double> v2 = it->second.retrieveDouble(k2);
        for(vector<double>::const_iterator it2 = v2.begin(); it2 != v2.end(); it2++)
            v1.push_back(*it2);
    }
    return v1;
}

string SMFile::getDefault(const std::string& k1, const std::string& k2, const std::string& d) const {
    for(multimap<std::string,Stringmap>::const_iterator it = dat.lower_bound(k1); it != dat.upper_bound(k1); it++) {
        vector<string> v2 = it->second.retrieve(k2);
        if(v2.size())
            return v2[0];
    }
    return d;
}

double SMFile::getDefault(const std::string& k1, const std::string& k2, double d) const {
    for(multimap<std::string,Stringmap>::const_iterator it = dat.lower_bound(k1); it != dat.upper_bound(k1); it++) {
        vector<double> v2 = it->second.retrieveDouble(k2);
        if(v2.size())
            return v2[0];
    }
    return d;
}

Stringmap SMFile::getFirst(const std::string& s, const Stringmap& dflt) const {
    multimap<std::string,Stringmap>::const_iterator it = dat.find(s);
    if(it == dat.end())
        return dflt;
    return it->second;
}

