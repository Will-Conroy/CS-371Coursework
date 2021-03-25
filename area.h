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


  or functions you implement here, and perhaps additional operators you may wish
  to overload.
*/
class Area {
private:
    std::string localAuthorityCode;
    std::map<std::string, std::string> names;
    std::map<std::string, Measure> measures;

public:
    Area() = default;
    Area(const std::string& localAuthorityCode);
    std::string getLocalAuthorityCode() const;
    std::string getName(const std::string lang) const;
    void setName(std::string lang, std::string name);
    Measure& getMeasure(const std::string key);
    void setMeasure(std::string codename, Measure measure);
    unsigned int size() const;
    std::string getJSONString() const;


    friend bool operator==(const Area& lhs, const Area& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Area& area);
    void merge(Area areaNew);


};

#endif // AREA_H_