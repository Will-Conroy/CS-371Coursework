#ifndef MEASURE_H_
#define MEASURE_H_

/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: 976789

  This file contains the decalaration of the Measure class.

  TODO: Read the block comments with TODO in measure.cpp to know which 
  functions and member variables you need to declare in this class.
 */

#include <string>
#include <map>

/*
  The Measure class contains a measure code, label, and a container for readings
  from across a number of years.

  TODO: Based on your implementation, there may be additional constructors
  or functions you implement here, and perhaps additional operators you may wish
  to overload.
*/
class Measure {
private:
    std::string codename;
    std::string label;
    std::map<unsigned int, float> readings;

public:
  Measure() = default;
  Measure(std::string code, const std::string &label);
  const std::string getCodename();
  std::string getLabel();
  void setLabel(std::string label);
  float getValue(unsigned int key);
  void setValue(unsigned int key, float value);
  unsigned int size();
  void merge(Measure measureNew);
  friend bool operator==(const Measure& lhs, const Measure& rhs);
};

#endif // MEASURE_H_