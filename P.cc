// Name: Maxwell You
// Date: 2017-04-29
// Purpose: Overload operators

#include "P.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Default ctor
P::P() { /* Do nothing */ }

// Copy ctor
P::P(const P &rhs) : propsMap(rhs.propsMap), countMap(rhs.countMap),
					 propsSet(rhs.propsSet),  propOpened(rhs.propOpened) { }	//copy rhs's class members to obj being constructed currently

// String ctor
P::P(string filename) {
	readfile(filename);
}

const P & P::operator=(const P &rhs) {
	if (this != &rhs) {				// alias test, so we obj doesn't copy to itself
		propsMap = rhs.propsMap;
		countMap = rhs.countMap;
		propsSet = rhs.propsSet;
		propOpened = rhs.propOpened;
	}
	return *this;					// return the object this
}

// Dtor
P::~P() { }

set<string> P::props() const {
	return propsSet;
}

// Parse and store properties from a properties file
void P::readfile(string filename) {
	if (propOpened) {
			ostringstream oss;
			oss << "Cannot call propfile() more than once";
			throw oss.str();
	}   
	
	ifstream in(filename);
	
	if (!in) {
		ostringstream oss;
		oss << "Unable to open properties file: " << filename;
		throw oss.str();
	}   

	propOpened = true;

	int pos;
	string temp;
	string delim = ";";
	int uni;
	string prop;

	while (getline(in, temp)) {

		pos = temp.find(delim);
		string uniS = temp.substr(0, pos);
		if (uniS.length() < 4 || uniS.length() > 6) {
			ostringstream oss;
			oss << filename << " is an invalid properties file";
			throw oss.str();
		}   
	
		uni = stoi(temp.substr(0, pos), nullptr, 16);
		temp.erase(0, pos + delim.length());

		pos = temp.find(delim);
		temp.erase(0, pos + delim.length());

		pos = temp.find(delim);
		prop = temp.substr(0, pos);

		if ( prop.length() != 2 || (!(prop[0] == toupper(prop[0]))) || (!(prop[1] == tolower(prop[1]))) ) { 
			ostringstream oss;
			oss << filename << " is an invalid properties file!\n";
			throw oss.str();
		}   

		propsMap[uni] = prop;	//	
		propsSet.insert(prop);	// add property to the props set
	}

	for (auto &s : propsSet)	// initialize countMap with the unique properties and set their count to 0
		countMap[s] = 0;

	in.close();
}

// Count occurences of a codepoint's property
void P::count(int codepoint) {
	if (propsMap.count(codepoint) == 0)	// codepoint is not in propsMap, so no need to look through it
		return;

	string prop = propsMap.at(codepoint);	// get prop assoc with codepoint

	++countMap[prop];	// increment count of prop
}

// Count occurences of times that characters with prop have been encountered
int P::count(string prop) const {
	if (countMap.count(prop) == 0)	// if property is not in the map (invalid property), return a count of 0
		return 0;
	return countMap.at(prop);
}

// Returns number of unique properties read
int P::size() const {
   return propsSet.size();
}

// Returns true if no property names
bool P::empty() const {
	return propsMap.empty();
}

// Removes all data from the obj
void P::clear() {
	propsMap.clear();
	countMap.clear();
	propsSet.clear();
	propOpened = false;
}

// Send a P to the output stream
ostream & operator<<(ostream & out, const P & rhs) {
	int count = 0;		// will track the last element
	if (rhs.empty())	// if no props, return empty map
		out << "{}";
	else {
		out << "{";		// begin print with opening brace
		for (auto & prop : rhs.countMap) {
			if (count == rhs.size() - 1)							// if last element in map
				out << prop.first << ": " << prop.second << "}";	// add closing brace instead of comma
			else {													// else concatenate the elements normally
				out << prop.first << ": " << prop.second << ", ";
				++count;											// incr count
			}
		}
	}
	return out;	// return the modified stream
}

// Boolean evaluation
P::operator bool() const {
	return !(empty());	// p is false if it is empty, p is true when not empty
}
