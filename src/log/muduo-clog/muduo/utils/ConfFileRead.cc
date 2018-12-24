#include "ConfFileRead.h"
#include <fstream>
#include <string>

#include <stdlib.h>

using namespace std;
using namespace muduo;

std::string& trim(std::string &s)
{
    if (s.empty())
    {
        return s;
    }

    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
	
	s.erase(0,s.find_first_not_of("\t"));
    s.erase(s.find_last_not_of("\t") + 1);
    return s;
}

ConfFileRead::ConfFileRead(const char * FilePath,int level, int LogFileSize)
		:level_(level),LogFileSize_(LogFileSize)
{
	std::ifstream rf(FilePath);
	
	if(!rf.is_open()){
		return;
	}
	
	string strLine;
	while(!rf.eof()){
		getline(rf, strLine);
		size_t pos = strLine.find('#');
		string fullLine = strLine.substr(0, pos);
        pos = fullLine.find('=');
        string key = fullLine.substr(0, pos);
		key = trim(key);
		pos++;
        if(key == "LOG_LEVEL"){
            string value = fullLine.substr(pos, -1);   
			value = trim(value);
			level_ = atoi(value.c_str());
			if(level_ < 0 || level_ > 2)	/* only 0,1,2,3 is legal */
				level_ = 3;
        }else if(key == "FILE_SIZE"){
			string value = fullLine.substr(pos, -1);   
			value = trim(value);
			LogFileSize_ = atoi(value.c_str());
			if(LogFileSize_ <= 8000000)	/* illegal value */
				LogFileSize_ = 524288000; 	/* 500M */
		}  
	}
}