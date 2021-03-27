#ifndef MEASURE_H_
#define MEASURE_H_

/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: 976789

  This file contains the decalaration of the Measure class.

  functions and member variables you need to declare in this class.
 */

#include <string>
#include <map>
#include <iostream>

/*
  The Measure class contains a measure code, label, and a container for readings
  from across a number of years.

  or functions you implement here, and perhaps additional operators you may wish
  to overload.
*/
class Measure {
private:
    //code idefing the data
    std::string codename;

    //Readable label discriabing the data
    std::string label;

    //Key = the year for the data | Value = the data
    std::map<unsigned int, double> readings;

public:
  /*----Constructor----*/
  Measure() = default;
  Measure(std::string code, const std::string &label);

  /*----Setters----*/
  void setLabel(std::string label);
  void setValue(unsigned int key, double value);

  /*----Getters----*/
  double getValue(unsigned int key);
  std::string getLabel() const;
  const std::string getCodename() const;
  double getDifference() const;
  double getDifferenceAsPercentage() const;
  double getAverage() const;

  /*----Miscellaneous----*/
  unsigned int size() const;
  void merge(Measure measureNew);
  std::string toJSON() const;

  /*----Overrides----*/
  friend bool operator==(const Measure& lhs, const Measure& rhs);
  friend std::ostream& operator<<(std::ostream& os, const Measure& measure);
};

#endif // MEASURE_H_