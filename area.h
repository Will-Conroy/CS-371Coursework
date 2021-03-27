#ifndef AREA_H_
#define AREA_H_
/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: 976789

  This file contains the Area class declaration. Area objects contain all the
  Measure objects for a given local area, along with names for that area and a
  unique authority code.

  functions and member variables you need to declare in this class.
 */

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include "measure.h"
#include "lib_json.hpp"

/*
  An Area object consists of a unique authority code, a container for names
  for the area in any number of different languages, and a container for the
  Measures objects.
*/
class Area {

private:
    //unique code identifying the area
    std::string localAuthorityCode;

    //key = IOS code for language | Value = name for that area in that language
    std::map<std::string, std::string> names;

    //Key = short code representing what data is stored |
    // Value = Measure object with all reading for that key
    std::map<std::string, Measure> measures;

public:
    /*----Constructors----*/
    Area() = default;
    Area(const std::string& localAuthorityCode);

    /*----Getters----*/
    std::string getLocalAuthorityCode() const;
    std::string getName(const std::string lang) const;
    Measure& getMeasure(const std::string key);

    /*----Setters---*/
    void setName(std::string lang, std::string name);
    void setMeasure(std::string codename, Measure measure);

    /*----Miscellaneous---*/
    unsigned int size() const;
    std::string toJSON() const;
    void merge(Area areaNew);

    /*----Overrides----*/
    friend bool operator==(const Area& lhs, const Area& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Area& area);


};

#endif // AREA_H_