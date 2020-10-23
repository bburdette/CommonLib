#ifndef StringUtils_h
#define StringUtils_h

#include <string>

// replace all instances of 'from' string in 's' with 'to' string.
std::string::size_type replace_all(std::string& s,
							const std::string& from,
							const std::string& to);
							
inline std::string& trim(std::string& str)
{
  std::string::size_type pos = str.find_last_not_of(' ');
  if(pos != std::string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(' ');
    if(pos != std::string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
  
  return str;
}					
		
inline std::string trim(const std::string& str)
{
	std::string lS(str);
	return trim(lS);	
}

#endif
