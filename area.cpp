/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: 976789

  This file contains the implementation for the Area class. Area is a relatively
  simple class that contains a local authority code, a container of names in
  different languages (perhaps stored in an associative container?) and a series
  of Measure objects (also in some form of container).

  This file contains numerous functions you must implement. Each function you
  must implement has a
*/
#include <stdexcept>
#include "bethyw.h"
#include "area.h"
#include "lib_json.hpp"

/*
  An alias for the imported JSON parsing library.
*/
using json = nlohmann::json;
/*
  Construct an Area with a given local authority code.

  @param localAuthorityCode
    The local authority code of the Area

  @example
    Area("W06000023");
*/
Area::Area(const std::string& localAuthorityCode): localAuthorityCode(localAuthorityCode) {}

/*
  Retrieve the local authority code for this Area. This function should be 
  callable from a constant context and not modify the state of the instance.
  
  @return
    The Area's local authority code

  @example
    Area area("W06000023");
    ...
    auto authCode = area.getLocalAuthorityCode();
*/
std::string Area::getLocalAuthorityCode() const {
    return this->localAuthorityCode;
}

/*
  Get a name for the Area in a specific language.  This function is
  callable from a constant context and not modify the state of the instance.

  @param lang
    A three-letter language code in ISO 639-3 format, e.g. cym or eng

  @return
    The name for the area in the given language

  @throws
    std::out_of_range if lang does not correspond to a language of a name stored
    inside the Area instance

  @example
    Area area("W06000023");
    std::string langCode  = "eng";
    std::string langValue = "Powys";
    area.setName(langCode, langValue);
    ...
    auto name = area.getName(langCode);
*/
 std::string Area::getName(const std::string lang) const{

    std::string lower = BethYw::convertToLower(lang);

    if(names.find(lang) == names.end())
        throw (std::out_of_range("No known lang"));

    return names.find(lang)->second;
}

/*
  Set a name for the Area in a specific language.

  @param lang
    A three-letter (alphabetical) language code in ISO 639-3 format,
    e.g. cym or eng, which should be converted to lowercase

  @param name
    The name of the Area in `lang`

  @throws
    std::invalid_argument if lang is not a three letter alphabetic code

  @example
    Area area("W06000023");
    std::string langCodeEnglish  = "eng";
    std::string langValueEnglish = "Powys";
    area.setName(langCodeEnglish, langValueEnglish);

    std::string langCodeWelsh  = "cym";
    std::string langValueWelsh = "Powys";
    area.setName(langCodeWelsh, langValueWelsh);
*/
void Area::setName(std::string lang, std::string name){
    if(lang.size() != 3){
        throw std::invalid_argument("Area::setName: Language code must be three alphabetical letters only");
    }
    for(char& c : lang) {
        if(!isalpha(c))
            throw std::invalid_argument("Area::setName: Language code must be three alphabetical letters only");
    }

    this->names.insert( std::pair<std::string, std::string>(BethYw::convertToLower(lang),name));
}

/*
  Retrieve a Measure object, given its codename. This function is case
  insensitive when searching for a measure.

  @param key
    The codename for the measure you want to retrieve

  @return
    A Measure object

  @throws
    std::out_of_range if there is no measure with the given code, throwing
    the message:
    No measure found matching <codename>

  @example
    Area area("W06000023");
    Measure measure("Pop", "Population");
    area.setMeasure("Pop", measure);
    ...
    auto measure2 = area.getMeasure("pop");
*/
Measure& Area::getMeasure(const std::string key) {

    if(measures.find(BethYw::convertToLower(key)) == measures.end())
        throw std::out_of_range("No measure found matching " + key);

    return measures.at(key);
}

/*
  Add a particular Measure to this Area object.

  If a Measure already exists with the same codename in this Area, overwrite any
  values contained within the existing Measure with those in the new Measure
  passed into this function. The resulting Measure stored inside the Area
  instance should be a combination of the two Measures instances.

  Note that the Measure's codename are be converted to lowercase.

  @param codename
    The codename for the Measure

  @param measure
    The Measure object

  @return
    void

  @example
    Area area("W06000023");

    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    double value = 12345678.9;
    measure.setValue(1999, value);

    area.setMeasure(codename, measure);
*/
void Area::setMeasure(std::string codename, Measure measure){
    std::string codenameLower = BethYw::convertToLower(codename);
    if(this->measures.find(codenameLower) == this->measures.end()) {
        this->measures.insert(std::pair<std::string, Measure>(codenameLower, measure));
    }else{
        measure.merge(measures.at(codenameLower));
        measures.at(codenameLower) = measure;
    }
}

/*
  Retrieve the number of Measures we have for this Area. This function is
  callable from a constant context, not modify the state of the instance, and
  must promise not throw an exception.

  @return
    The size of the Area (i.e., the number of Measures)

  @example
    Area area("W06000023");
    std::string langCode  = "eng";
    std::string langValue = "Powys";
    area.setName(langCode, langValue);

    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    area.setMeasure(code, measure);
    auto size = area.size();
*/
unsigned int Area::size() const{
    return this->measures.size();
}

/*
  Overload the stream output operator as a free/global function.

  Output the name of the Area in English and Welsh, followed by the local
  authority code. Then output all the measures for the area (see the coursework
  worksheet for specific formatting).

  If the Area only has only one name, output this. If the area has no names,
  output the name "Unnamed".

  Measures are ordered by their Measure codename. If there are no measures
  output the line "<no measures>" after you have output the area names.

  @param os
    The output stream to write to

  @param area
    Area to write to the output stream

  @return
    Reference to the output stream

  @example
    Area area("W06000023");
    area.setName("eng", "Powys");
    std::cout << area << std::endl;
*/
std::ostream &operator<<(std::ostream &os, const Area &area) {

    os << area.getName("eng") << " / " << area.getName("cym") << '('
       << area.getLocalAuthorityCode() << ')' << std::endl;

    if(area.measures.empty())
        os << "<no measures>" << std::endl << std::endl;

    for(auto const& measure : area.measures)
        os << measure.second;

    return os;
}

/*
  Overload the == operator for two Area objects as a global/free function. Two
  Area objects are only equal when their local authority code, all names, and
  all data are equal.

  @param lhs
    An Area object

  @param rhs
    A second Area object

  @return
    true if both Area instanes have the same local authority code, names
    and data; false otherwise.

  @example
    Area area1("MYCODE1");
    Area area2("MYCODE1");

    bool eq = area1 == area2;
*/
bool operator==(const Area& lhs, const Area& rhs){
    if(lhs.getLocalAuthorityCode() != rhs.getLocalAuthorityCode())
        return false;

    if(lhs.names != rhs.names)
        return false;

    return lhs.measures == rhs.measures;
}

/*
 * Combinds two areas. New data replace any data in the old area but data not contained in the new area but in the old
 * area is kept.

  @param areaNew
    An Area object

  @return
   void

  @example
    Area area1("MYCODE1");
    Area area2("MYCODE1");
    area1.merge(area2);
 *
 * */
void Area::merge(Area areaNew){
    measures.insert(areaNew.measures.begin(), areaNew.measures.end());
    names.insert(areaNew.names.begin(), areaNew.names.end());
}
/*
  Convert this Area object, and the Measure instances within those, to a JSON string.
  (https://github.com/nlohmann/json) for more info

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

std::string Area::toJSON() const {
    json j;

    for (auto const& name : names)
        j["names"][name.first] = name.second;

    for (auto const& measure : measures)
        j["measures"][measure.first] = json::parse(measure.second.toJSON());

    return j.dump();
}


