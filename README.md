# Beth Yw ? Stats Wales DataAnalysis Tool
*C++ tool to read in Walesh data stored in JSON and CSV formates*
***
##Authors
###Structure and Original Code:
1. Dr Martin Porcheron (https://www.swansea.ac.uk/staff/science/compsci/porcheron-m-a-w/)

###Course Work Contribution:
1. 976789
***
##bethYw.cpp
#### Added functions
*(I added a lot of helper function that all other classes use as I didn't know where else to add them)*
- **BethYw::insensitiveEquals(a, b)** | does what it says on the tin compairs to string as if they are both lower case
- **BethYw::convertToLower(string)** | when storing data I converted to lower in most case to stay constate so this code is 
  reused over though out the code 
- **BethYw::validateYear(string)**| Both turns a string into a unsigned int and does simple validation added to help 
  reabilty and add abstration
- **BethYw::filterContains** | Found my self rewritting this code again and again so added a function to help abstions. 
  This doesn't work for the yearsFilter as it is a differently type of contanor and only used once.
***
##area.cpp
#### Added functions
- **Area::getJSONString()** | Added so that Areas::ToJSON could call when making JSON string
- **Area::merge(area)** | Made sence to me to have a function with in area it self to merge the data correctly this calls
Measure::merge(measure) to merge all data correctly
#### Stored Data
*(localAuthorityCode is also stored as a string)*
- **names** map is perfect for that as the ISO code maps perfecly to correct name
- **measures** was a undored set with a tuple inside but after seeing how easy maps where I am never going back

***
##areas.cpp
#### Added functions
-**Areas::getVariableCSV(line)** | This return the first verable in a CSV Line while deleteling that verabel for that line.
this means you can just keep calling this to get the next verable. I found impleation very useful as importing CSV is 
very linner
#### Stored Data

-**AreasContainer** | I set Dr Porcheron's AreaContainer to be a map, as maps make fining a area give a Authory code 
very easy and that they would be ordered if I wanted to print them
***
##measure.cpp
#### Added functions
- **merge(measureNew)** | Added so that Area::Merge(area) could call this to merge all data correcly they work they same
  way
- **getJSONString()** | Same idea of merge area call thi when creating it own JSONString.
#### Stored Data
*(code and label is also stored as a string)*  
-**readings** | What can I say map sexy. Again a year mapping to one peice of data this is what maps where made for.

***
##input.cpp
This is here too
***
#STEVEN IS A BIG WEEB