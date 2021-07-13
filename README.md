# Beth Yw ? Stats Wales Data Analysis Tool
*🏴 󠁢󠁷󠁬󠁳󠁿C++ tool to read and filter Walsh data stored in JSON and CSV formats 🏴󠁧󠁢󠁷󠁬󠁳󠁿*
***
##🖊️ Authors
###Structure and Original Code:
1. Dr Martin Porcheron (https://www.swansea.ac.uk/staff/science/compsci/porcheron-m-a-w/)
###Course Work Contribution:
1. 976789 🥰
***
Now that code is '*finished*' all of Dr Porcheron's comment have been edited to reflex this.
Meaning they don't contain any advice on implantation, just a description of what they do, 
and all instance of "this function should" changed to "this function is".
Comments in the same style have been added to any new functions.
***
##bethYw.cpp
#### Added functions
*(I added a lot of helper function that all other classes use as I didn't know where else to add them)*
- **BethYw::insensitiveEquals(a, b)** | does what it says on the tin compares to string as if they are both lower case
- **BethYw::convertToLower(string)** | when storing data I converted to lower in most case to stay constant so this code 
  is reused over though out the code 
- **BethYw::validateYear(string)** | Both turns a string into a unsigned int and does simple validation added to help 
  readability and add abstraction
***
##area.cpp
#### Added functions
- **Area::getJSONString()** | Added so that Areas::ToJSON could call when making JSON string
- **Area::merge(area)** | Made sense to me to have a function with in area it self to merge the data correctly this calls
Measure::merge(measure) to merge all data correctly
####💾 Stored Data
*(localAuthorityCode is also stored as a string)*
- **names** map is perfect for that as the ISO code maps perfectly to correct name
- **measures** was a undored set with a tuple inside but after seeing how easy maps where I am never going back
***
##areas.cpp
#### Added functions
-**Areas::getVariableCSV(line)** | This return the first variable in a CSV Line while deleting that variable for that line.
this means you can just keep calling this to get the next variable. I found implication very useful as importing CSV is 
very linner
-**Areas::filterContains(filter, string)** | This used to be in BethYw however once i had finished i found i only used 
it in Areas. Found my self rewriting this code again and again so added a function to help abstions.
This doesn't work for the yearsFilter as it is a differently type of contanor and only used once.  
-**Areas::isFilterEmpty(filter)** | Once I made filerContains it was only natural to add this function, again it doesn't 
work for the years filter. I thought about adding a 3rd function that mixed to to all filterShouldAdd but this seemed 
like over kill and I thought it would reduce readability.
####💾  Stored Data
-**AreasContainer** | I set Dr Porcheron's AreaContainer to be a map, as maps make fining a area give a Authority code 
very easy and that they would be ordered if I wanted to print them
***
##measure.cpp
#### Added functions
- **merge(measureNew)** | Added so that Area::Merge(area) could call this to merge all data correctly they work they 
  same way
- **getJSONString()** | Same idea of merge area call thi when creating it own JSONString.
####💾 Stored Data
*(code and label are also stored as a string)*  
-**readings** | What can I say map sexy. Again a year mapping to one piece of data this is what maps where made for.
***
##input.cpp 
This is here too, however there is nothing to note about my implantation.
***
