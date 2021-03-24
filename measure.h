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

/*
  The Measure class contains a measure code, label, and a container for readings
  from across a number of years.

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

  void setLabel(std::string label);
  void setValue(unsigned int key, float value);

  float getValue(unsigned int key);
  std::string getLabel();
  const std::string getCodename();

  unsigned int size() const;
  float getDifference() const;
  float getDifferenceAsPercentage() const;
  float getAverage() const;
  void merge(Measure measureNew);


  friend bool operator==(const Measure& lhs, const Measure& rhs);
};

#endif // MEASURE_H_