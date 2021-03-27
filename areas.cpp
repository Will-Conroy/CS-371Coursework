/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: 976789

  The file contains the Areas class implementation. Areas are the top
  level of the data structure in Beth Yw? for now.

  Areas is also responsible for importing data from a stream (using the
  various populate() functions) and creating the Area and Measure objects.
*/

#include <stdexcept>
#include <string>
#include <stdexcept>
#include <tuple>
#include <unordered_set>


#include "datasets.h"
#include "areas.h"
#include "measure.h"
#include "datasets.h"
#include "bethyw.h"
#include "lib_json.hpp"
/*
  An alias for the imported JSON parsing library.
*/
using json = nlohmann::json;

/*
  Constructor for an Areas object.

  @example
    Areas data = Areas();
*/
Areas::Areas() {}

/*
  Add a particular Area to the Areas object.

  If an Area already exists with the same local authority code, overwrite all
  data contained within the existing Area with those in the new
  Area (i.e. they should be combined, but the new Area's data should take
  precedence, e.g. replace a name with the same language identifier).

  @param localAuthorityCode
    The local authority code of the Area

  @param area
    The Area object that will contain the Measure objects

  @return
    void

  @example
    Areas data = Areas();
    std::string localAuthorityCode = "W06000023";
    Area area(localAuthorityCode);
    data.setArea(localAuthorityCode, area);
*/
void Areas::setArea(std::string localAuthorityCode, Area area) {

    if(areas.find(localAuthorityCode) == areas.end()){
        areas.insert(std::pair<std::string, Area>(localAuthorityCode, area));

    }else{
        area.merge(areas.at(localAuthorityCode));
        areas.at(localAuthorityCode) = area;
    }
}

/*
  Retrieve an Area instance with a given local authority code.

  @param localAuthorityCode
    The local authority code to find the Area instance of

  @return
    An Area object

  @throws
    std::out_of_range if an Area with the set local authority code does not
    exist in this Areas instance

  @example
    Areas data = Areas();
    std::string localAuthorityCode = "W06000023";
    Area area(localAuthorityCode);
    data.setArea(localAuthorityCode, area);
    ...
    Area area2 = areas.getArea("W06000023");
*/
Area& Areas::getArea(std::string localAuthorityCode){
    if(areas.find(localAuthorityCode) == areas.end())
        throw std::out_of_range("No area found matching " + localAuthorityCode);

    return areas.at(localAuthorityCode);
}

/*
  Retrieve the number of Areas within the container. This function should be 
  callable from a constant context, not modify the state of the instance, and
  must promise not throw an exception.

  @return
    The number of Area instances

  @example
    Areas data = Areas();
    std::string localAuthorityCode = "W06000023";
    Area area(localAuthorityCode);
    data.setArea(localAuthorityCode, area);
    
    auto size = areas.size(); // returns 1
*/
unsigned int Areas::size() const{
    return areas.size();
}

/*
  This function specifically parses the compiled areas.csv file of local 
  authority codes, and their names in English and Welsh.

  This is a simple dataset that is a comma-separated values file (CSV), where
  the first row gives the name of the columns, and then each row is a set of
  data.

  For this coursework, you can assume that areas.csv will always have the same
  three columns in the same order.

  Once the data is parsed, you need to create the appropriate Area objects and
  insert them in to a Standard Library container within Areas.

  @param is
    The input stream from InputSource

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @param areasFilter
    An umodifiable pointer to set of umodifiable strings for areas to import,
    or an empty set if all areas should be imported

  @return
    void

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variable areasFilter is created

  @example
    InputFile input("data/areas.csv");
    auto is = input.open();

    auto cols = InputFiles::AREAS.COLS;

    auto areasFilter = BethYw::parseAreasArg();

    Areas data = Areas();
    areas.populateFromAuthorityCodeCSV(is, cols, &areasFilter);

  @throws 
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file)
    std::std::out_of_range if there are not enough columns in cols if there are not enough columns in cols
*/
void Areas::populateFromAuthorityCodeCSV(
    std::istream &is,
    const BethYw::SourceColumnMapping &cols,
    const StringFilterSet * const areasFilter) {

    //TODO: add more error handling if you get time
    if(cols.size() < 3)
        throw std::out_of_range("Not enough columns");

    if(!(is.good()))
        throw (std::runtime_error("Failed to open file"));

    //reading first line which is just the name of cols
    //As coursework states that this should remain constant, throw away the line
    std::string line;
    std::getline(is,line);

    //check if areasFilter is give at all OR that it is just empty
    bool all = false;
    if(areasFilter == nullptr || areasFilter->empty())
        all = true;
    while (std::getline(is, line)) {
        std::string code = getVariableCSV(line);
        if( all || areasFilter->find(code) != areasFilter->end()){
            Area temp(code);
            temp.setName("eng", getVariableCSV(line));
            temp.setName("cym", getVariableCSV(line));
            this->setArea(code, temp);
        }
    }
}

/*
    Takes json dir and populated the area contaoner with that data which matches the filer.
    if a filter is missing or emtpy (<0,0> for yearsfiler) all data is imporated

  @param is
    The input stream from InputSource

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @param areasFilter
    An umodifiable pointer to set of umodifiable strings of areas to import,
    or an empty set if all areas should be imported

  @param measuresFilter
    An umodifiable pointer to set of umodifiable strings of measures to import,
    or an empty set if all measures should be imported

  @param yearsFilter
    An umodifiable pointer to an umodifiable tuple of two unsigned integers,
    where if both values are 0, then all years should be imported, otherwise
    they should be treated as the range of years to be imported (inclusively)

  @return
    void

  @throws 
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file)
    std::out_of_range if there are not enough columns in cols

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variable areasFilter is created

  @example
    InputFile input("data/popu1009.json");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["popden"].COLS;

    auto areasFilter = BethYw::parseAreasArg();
    auto measuresFilter = BethYw::parseMeasuresArg();
    auto yearsFilter = BethYw::parseMeasuresArg();

    Areas data = Areas();
    areas.populateFromWelshStatsJSON(
      is,
      cols,
      &areasFilter,
      &measuresFilter,
      &yearsFilter);
*/
void Areas::populateFromWelshStatsJSON(std::istream &is,
            const BethYw::SourceColumnMapping &cols,
            const StringFilterSet * const areasFilter,
            const StringFilterSet * const measuresFilter,
            const YearFilterTuple * const yearsFilter){

    //get years for readability
    unsigned int yearStart = std::get<0>(*yearsFilter);
    unsigned int yearEnd = std::get<1>(*yearsFilter);

    //moved nullptr to left for small efficacy
    bool allAreas = areasFilter == nullptr || areasFilter->empty();
    bool allMeasures = areasFilter == nullptr || measuresFilter->empty();
    bool allYears = yearsFilter == nullptr ||(yearStart == 0 && yearEnd == 0);

    json j;
    is >> j;

    for (auto& el : j["value"].items()) {
        auto &data = el.value();
        std::string localAuthorityCode = data[cols.at(BethYw::SourceColumn::AUTH_CODE)];

        //area in not already store and it in the filter or we are imporating them all
        if(allAreas || BethYw::filterContains(areasFilter, localAuthorityCode)){
            if(areas.find(localAuthorityCode) == areas.end()){
                Area temp = Area(localAuthorityCode);
                temp.setName("eng", data[cols.at(BethYw::SourceColumn::AUTH_NAME_ENG)]);
                areas.insert({localAuthorityCode, temp});
            }

            std::string lowerMeasureCode = BethYw::convertToLower(data[cols.at(BethYw::SourceColumn::MEASURE_CODE)]);
            if(allMeasures || BethYw::filterContains(measuresFilter, lowerMeasureCode)){
                std::string measureCode = data[cols.at(BethYw::SourceColumn::MEASURE_CODE)];
                double reading;
                try{
                    reading = data["Data"];
                }catch(const nlohmann::detail::type_error& error){
                    std::string temp = data["Data"];
                    reading = std::stod(temp);
                }

                Measure measure = Measure(measureCode, data[cols.at(BethYw::SourceColumn::MEASURE_NAME)]);

                //turns the year string into unsigned int and happened to do some small validation
                unsigned int year = BethYw::validateYear(data[cols.at(BethYw::SourceColumn::YEAR)]);

                if(allYears || (year >= yearStart && year <= yearEnd))
                    measure.setValue(year, reading);
                areas.at(localAuthorityCode).setMeasure(measureCode,measure);
            }
        }
    }
}

/*

  This function imports CSV files that contain a single measure. The 
  CSV file consists of columns containing the authority code and years.
  Each row contains an authority code and values for each year (or no value
  if the data doesn't exist).

  Note that these files do not include the names for areas, instead you 
  have to rely on the names already populated through 
  Areas::populateFromAuthorityCodeCSV();

  The datasets that will be parsed by this function are
   - complete-popu1009-area.csv
   - complete-popu1009-pop.csv
   - complete-popu1009-opden.csv

  @param is
    The input stream from InputSource

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @param areasFilter
    An umodifiable pointer to set of umodifiable strings for areas to import,
    or an empty set if all areas should be imported

  @param measuresFilter
    An umodifiable pointer to set of strings for measures to import, or an empty 
    set if all measures should be imported

  @param yearsFilter
    An umodifiable pointer to an umodifiable tuple of two unsigned integers,
    where if both values are 0, then all years should be imported, otherwise
    they should be treated as a the range of years to be imported

  @return
    void

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variable areasFilter is created

  @example
    InputFile input("data/complete-popu1009-pop.csv");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["complete-pop"].COLS;

    auto areasFilter = BethYw::parseAreasArg();
    auto yearsFilter = BethYw::parseYearsArg();

    Areas data = Areas();
    areas.populateFromAuthorityCodeCSV(is, cols, &areasFilter, &yearsFilter);

  @throws 
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file)
    std::out_of_range if there are not enough columns in cols
*/
void Areas::populateFromAuthorityByYearCSV(std::istream &is,
                                       const BethYw::SourceColumnMapping &cols,
                                       const StringFilterSet * const areasFilter,
                                       const StringFilterSet * const measuresFilter,
                                       const YearFilterTuple * const yearsFilter){

    auto dataCode = cols.at(BethYw::SourceColumn::SINGLE_MEASURE_CODE);
    auto dataName = cols.at(BethYw::SourceColumn::SINGLE_MEASURE_NAME);
    bool allMeasures = measuresFilter == nullptr || measuresFilter->empty();

    if(is.good() && (allMeasures || BethYw::filterContains(measuresFilter, dataCode))){
        //get years for readability
        unsigned int yearStart = std::get<0>(*yearsFilter);
        unsigned int yearEnd = std::get<1>(*yearsFilter);

        bool allAreas = areasFilter == nullptr || areasFilter->empty();
        bool allYears = yearsFilter == nullptr ||(yearStart == 0 && yearEnd == 0);


        //reading first verable which is just AuthorityCode
        std::string line;
        std::getline(is,line);
        getVariableCSV(line);
        std::vector<unsigned int> years;
        //gets all the years at the top
        while(!(line.empty())) {
            years.push_back(std::stol(getVariableCSV(line)));
        }

        while(std::getline(is, line)){
            std::string localAuthCode = getVariableCSV(line);
            if(allAreas || BethYw::filterContains(areasFilter, localAuthCode)){
                Measure measure(dataCode,dataName);
                for(auto const& year : years){
                    if(allYears || (year >= yearStart && year <= yearEnd)){
                        measure.setValue(year,std::stod(getVariableCSV(line)));
                    }else{
                        getVariableCSV(line);
                    }
                    Area tempArea(localAuthCode);
                    tempArea.setMeasure(dataCode, measure);
                    setArea(localAuthCode, tempArea);
                }
            }
        }

    }
}

/*
  Parse data from an standard input stream `is`, that has data of a particular
  `type`, and with a given column mapping in `cols`.

  This function should look at the `type` and hand off to one of the three 
  functions populate………() functions.

  The function must check if the stream is in working order and has content.

  @param is
    The input stream from InputSource

  @param type
    A value from the BethYw::SourceDataType enum which states the underlying
    data file structure

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @return
    void

  @throws 
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file),
    the stream is not open/valid/has any contents, or an unexpected type
    is passed in.
    std::out_of_range if there are not enough columns in cols

  @see
    See datasets.h for details of the values variable type can have

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variable areasFilter is created

  @example
    InputFile input("data/popu1009.json");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["popden"].COLS;

    Areas data = Areas();
    areas.populate(
      is,
      DataType::WelshStatsJSON,
      cols);
*/
void Areas::populate(std::istream &is,
                     const BethYw::SourceDataType &type,
                     const BethYw::SourceColumnMapping &cols) {

  if (type == BethYw::AuthorityCodeCSV) {
      if(cols.size() < 3){
          throw std::out_of_range("Cols is out of range");
      }
    populateFromAuthorityCodeCSV(is, cols);
  } else {
    throw std::runtime_error("Areas::populate: Unexpected data type");
  }
}

/*
  Parse data from an standard input stream, that is of a particular type,
  and with a given column mapping, filtering for specific areas, measures,
  and years, and fill the container.

  This function should look at the `type` and hand off to one of the three 
  functions you've implemented above.

  The function must check if the stream is in working order and has content.

  This overloaded function includes pointers to the three filters for areas,
  measures, and years.

  @param is
    The input stream from InputSource

  @param type
    A value from the BethYw::SourceDataType enum which states the underlying
    data file structure

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @param areasFilter
    An umodifiable pointer to set of umodifiable strings for areas to import,
    or an empty set if all areas should be imported

  @param measuresFilter
    An umodifiable pointer to set of umodifiable strings for measures to import,
    or an empty set if all measures should be imported

  @param yearsFilter
    An umodifiable pointer to an umodifiable tuple of two unsigned integers,
    where if both values are 0, then all years should be imported, otherwise
    they should be treated as a the range of years to be imported

  @return
    void

  @throws 
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file),
    the stream is not open/valid/has any contents, or an unexpected type
    is passed in.
    std::out_of_range if there are not enough columns in cols

  @see
    See datasets.h for details of the values variable type can have

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variables areasFilter, measuresFilter,
    and yearsFilter are created

  @example
    InputFile input("data/popu1009.json");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["popden"].COLS;

    auto areasFilter = BethYw::parseAreasArg();
    auto measuresFilter = BethYw::parseMeasuresArg();
    auto yearsFilter = BethYw::parseMeasuresArg();

    Areas data = Areas();
    areas.populate(
      is,
      DataType::WelshStatsJSON,
      cols,
      &areasFilter,
      &measuresFilter,
      &yearsFilter);
*/
void Areas::populate(
    std::istream &is,
    const BethYw::SourceDataType &type,
    const BethYw::SourceColumnMapping &cols,
    const StringFilterSet * const areasFilter,
    const StringFilterSet * const measuresFilter,
    const YearFilterTuple * const yearsFilter){
  if (type == BethYw::AuthorityCodeCSV && !(cols.size() < 3)) {
      populateFromAuthorityCodeCSV(is, cols, areasFilter);

  } else if(type == BethYw::AuthorityByYearCSV && !(cols.size() < 3)){
      populateFromAuthorityByYearCSV(is, cols, areasFilter, measuresFilter, yearsFilter);

  } else if(type == BethYw::WelshStatsJSON && !(cols.size() < 6 )) {
      populateFromWelshStatsJSON(is, cols, areasFilter, measuresFilter, yearsFilter);

  }else{
    throw std::runtime_error("Areas::populate: Unexpected data type");
  }
}

/*
  Convert this Areas object, and all its containing Area instances, and
  the Measure instances within those, to values.

  Use the sample JSON library as above to create this. Construct a blank
  JSON object:
    json j;

  Convert this json object to a string:
    j.dump();

  You then need to loop through your areas, measures, and years/values
  adding this data to the JSON object.

  Read the documentation for how to convert your outcome code to JSON:
    https://github.com/nlohmann/json#arbitrary-types-conversions
  
  The JSON should be formatted as such:
    {
    "<localAuthorityCode1>" : {
                              "names": { "<languageCode1>": "<languageName1>",
                                         "<languageCode2>": "<languageName2>" 
                                         …
                                         "<languageCodeN>": "<languageNameN>" }, 
                              "measures" : { "<year1>": <value1>,
                                             "<year2>": <value2>,
                                             …
                                            "<yearN>": <valueN> }
                               },
    "<localAuthorityCode2>" : {
                              "names": { "<languageCode1>": "<languageName1>",
                                         "<languageCode2>": "<languageName2>" 
                                         …
                                         "<languageCodeN>": "<languageNameN>" }, 
                              "measures" : { "<year1>": <value1>,
                                             "<year2>": <value2>,
                                             …
                                            "<yearN>": <valueN> }
                               }
    ...
    "<localAuthorityCodeN>" : {
                              "names": { "<languageCode1>": "<languageName1>",
                                         "<languageCode2>": "<languageName2>" 
                                         …
                                         "<languageCodeN>": "<languageNameN>" }, 
                              "measures" : { "<year1>": <value1>,
                                             "<year2>": <value2>,
                                             …
                                            "<yearN>": <valueN> }
                               }
    }

  An empty JSON is "{}" (without the quotes), which you must return if your
  Areas object is empty.
  
  @return
    std::string of JSON

  @example
    InputFile input("data/popu1009.json");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["popden"].COLS;

    auto areasFilter = BethYw::parseAreasArg();
    auto measuresFilter = BethYw::parseMeasuresArg();
    auto yearsFilter = BethYw::parseMeasuresArg();

    Areas data = Areas();
    std::cout << data.toJSON();
*/
std::string Areas::toJSON() const {
  json j;

  for (auto const& area : areas)
      j[area.second.getLocalAuthorityCode()] = json::parse(area.second.toJSON());

  if(j.dump() == "null")
      return "{}";

  return j.dump();
}

/*
  Overload the << operator to print all of the imported data.

   Areas are printed, ordered alphabetically by their local authority code. Measures
  within each Area should be ordered alphabetically by their codename.

  @param os
    The output stream to write to

  @param areas
    The Areas object to write to the output stream

  @return
    Reference to the output stream

  @example
    Areas areas();
    std::cout << areas << std::end;
*/
std::ostream &operator<<(std::ostream &os, const Areas &areas){
    for(auto const& area : areas.areas)
        os << area.second;
    return os;
}

/*
  Retrieve string form the head of CSV line, and then delete it's form that
  line.

  @param line
    A refeance to a CSV line

  @return
    std::string

  @example
    std::string line = "give,me,100%,please";
    std::string give = area.getVariableCSV(line);
    //line = "me,100%,please"; - now
*/

std::string Areas::getVariableCSV(std::string& line){

    if(line.find(",") == std::string::npos){
        std::string out = line;
        line = "";
        return out;
    }



    std::size_t pos = line.find(",");
    std::string out = line.substr(0,pos);

    //deletes outputted variable
    line = line.erase (0,pos+1);

    return out;
}

