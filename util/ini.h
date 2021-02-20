#pragma once

#ifndef _INI_H_
#define _INI_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <map>


typedef std::map<std::string, std::string> mnode;
typedef std::map<std::string, mnode> mnodes;

typedef std::pair<std::string, std::string> pnode;
typedef std::pair<std::string, mnode> pnodes;

namespace ini 
{
    std::string& trim(std::string&);

    int read(std::string, mnodes&);
    int write(std::string, mnodes);

    std::string get_value(mnodes, std::string, std::string);
    int set_value(mnodes&, std::string, std::string, std::string);
}


#endif // _INI_H_
