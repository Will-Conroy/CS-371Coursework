


/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: 976789

  This file contains the implementation of the Measure class. Measure is a
  very simple class that needs to contain a few member variables for its name,
  codename, and a Standard Library container for data. The data you need to 
  store is values, organised by year. I'd recommend storing the values as 
  doubles.
*/

#include <stdexcept>
#include <string>
#include <numeric>

#include "measure.h"
#include "bethyw.h"

/*
  Construct a single Measure, that has values across many years.

  All StatsWales JSON files have a codename for measures. You should convert 
  all codenames to lowercase.

  @param codename
    The codename for the measure

  @param label
    Human-readable (i.e. nice/explanatory) label for the measure

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);
*/
Measure::Measure(std::string codename, const std::string &label) {
    std::string temp;
    for(char& c : codename) {
       temp += std::tolower(c);
    }
    this->codename = codename;
    this->label = label;
}

/*

  Retrieve the code for the Measure. This function should be callable from a 
  constant context and must promise to not modify the state of the instance or 
  throw an exception.

  @return
    The codename for the Measure

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    ...
    auto codename2 = measure.getCodename();
*/
const std::string Measure::getCodename() const{
    return this->codename;
}


/*
  Retrieve the human-friendly label for the Measure. This function should be 
  callable from a constant context and must promise to not modify the state of 
  the instance and to not throw an exception.

  @return
    The human-friendly label for the Measure

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    ...
    auto label = measure.getLabel();
*/

std::string Measure::getLabel() const {
    return this->label;
}


/*
  Change the label for the Measure.

  @param label
    The new label for the Measure

  @example
    Measure measure("pop", "Population");
    measure.setValue(1999, 12345678.9);
    ...
    measure.setLabel("New Population");
*/
void Measure::setLabel(std::string label) {
    this->label = label;
}


/*
  Retrieve a Measure's value for a given year.

  @param key
    The year to find the value for

  @return
    The value stored for the given year

  @throws
    std::out_of_range if year does not exist in Measure with the message
    No value found for year <year>

  @return
    The value

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    ...
    auto value = measure.getValue(1999); // returns 12345678.9
*/
float Measure::getValue(unsigned int key){
    if(this->readings.find(key) == this->readings.end())
        throw std::out_of_range("Unknown year");
    return this->readings.find(key)->second;
}


/*
  Add a particular year's value to the Measure object. If a value already
  exists for the year, replace it.

  @param key
    The year to insert a value at

  @param value
    The value for the given year

  @return
    void

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
*/
void Measure::setValue(unsigned int key, float value){
    if(this->readings.find(key) != this->readings.end())
        this->readings.find(key)->second = value;
    this->readings.insert(std::pair<unsigned int, float>(key,value));
}


/*

  Retrieve the number of years data we have for this measure. This function
  should be callable from a constant context and must promise to not change
  the state of the instance or throw an exception.

  @return
    The size of the measure

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    auto size = measure.size(); // returns 1
*/
unsigned int Measure::size() const{
    return readings.size();
}

/*

  Calculate the difference between the first and last year imported. This
  function should be callable from a constant context and must promise to not
  change the state of the instance or throw an exception.

  @return
    The difference/change in value from the first to the last year, or 0 if it
    cannot be calculated

  @example
    Measure measure("pop", "Population");
    measure.setValue(1999, 12345678.9);
    measure.setValue(2001, 12345679.9);
    auto diff = measure.getDifference(); // returns 1.0
*/
float Measure::getDifference() const{
    return  readings.end()->second - readings.begin()->second;
}


/*

  Calculate the difference between the first and last year imported as a 
  percentage. This function should be callable from a constant context and
  must promise to not change the state of the instance or throw an exception.

  @return
    The difference/change in value from the first to the last year as a decminal
    value, or 0 if it cannot be calculated

  @example
    Measure measure("pop", "Population");
    measure.setValue(1990, 12345678.9);
    measure.setValue(2010, 12345679.9);
    auto diff = measure.getDifferenceAsPercentage();
*/
float Measure::getDifferenceAsPercentage() const{
    if(getDifference() == 0)
        return 0;
    return (getDifference()/readings.begin()->second) * 100;
}


/*
  Calculate the average/mean value for all the values. This function should be
  callable from a constant context and must promise to not change the state of 
  the instance or throw an exception.

  @return
    The average value for all the years, or 0 if it cannot be calculated

  @example
    Measure measure("pop", "Population");
    measure.setValue(1999, 12345678.9);
    measure.setValue(2001, 12345679.9);
    auto diff = measure.getAverage(); // returns 12345678.4
*/

float Measure::getAverage() const{

    if(readings.size() == 0)
        return 0;

    float sum = 0;
    for (auto const& reading : readings)
        sum += reading.second;

    return sum/readings.size();
}


/*
  TODO: operator<<(os, measure)

  Overload the << operator to print all of the Measure's imported data.

  We align the year and value outputs by padding the outputs with spaces,
  i.e. the year and values should be right-aligned to each other so they
  can be read as a table of numerical values.

  Years should be printed in chronological order. Three additional columns
  should be included at the end of the output, correspodning to the average
  value across the years, the difference between the first and last year,
  and the percentage difference between the first and last year.

  If there is no data in this measure, print the name and code, and 
  on the next line print: <no data>

  See the coursework specification for more information.

  @param os
    The output stream to write to

  @param measure
    The Measure to write to the output stream

  @return
    Reference to the output stream

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    std::cout << measure << std::end;
    Measure 2 name> (<Measure 2 codename>)
     <year 1>  <year 2> <year 3> ...  <year n>
    <value 1>  <year 2> <year 3> ... <value n> <mean 2> <diff 2> <diffp 2>
*/
std::ostream &operator<<(std::ostream &os, const Measure &measure) {
    os << measure.label << '(' << measure.codename << ')' << std::endl;
    for (auto const &reading : measure.readings)
        os << reading.second;
    os << "Average Diff. % Diff." << std::endl << measure.getAverage()
       << measure.getDifference() << measure.getDifferenceAsPercentage() << std::endl;
    return os;
}
/*
  Overload the == operator for two Measure objects. Two Measure objects
  are only equal when their codename, label and data are all equal.

  @param lhs
    A Measure object

  @param rhs
    A second Measure object

  @return
    true if both Measure objects have the same codename, label and data; false
    otherwise
*/

bool operator==(const Measure& lhs, const Measure& rhs){
    if(lhs.codename != rhs.codename)
        return false;

    if(lhs.label != rhs.label)
        return false;

    return lhs.readings == rhs.readings;
}

/*TODO: write comment*/

void Measure::merge(Measure measureNew){
    readings.insert(measureNew.readings.begin(), measureNew.readings.end());
}

/*
 * "pop":{"2015":242316.0,"2016":244462.0,"2017":245480.0,"2018":246466.0},*/
std::string Measure::getJSONString() const{
    if(readings.size() == 0)
        return "{}";

    std::string out = "\"" + codename + "\" : {";

    for (auto const& reading : readings){

        out += "\"" + reading.first;
        out += "\" :";
        out += reading.second;
        out += ", ";
    }
    //removes the last ,
    out = out.substr(0, out.size()-1);
    out += "}";
    return out;
}

