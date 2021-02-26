#include "ini.h"


std::string& ini::trim(std::string& str)
{
    std::string::size_type pos = 0;
    while (str.npos != (pos = str.find(" ")))
        str = str.replace(pos, pos + 1, "");

    return str;
}

int ini::read(std::string path, mnodes& nodes)
{
    std::ifstream config_file(path.c_str());
    if (!config_file) return -1;
    nodes.clear();

    std::string str_line = "";
    std::string str_root = "";
    mnode mn;
    
    while (getline(config_file, str_line))
    {
        std::string::size_type left_pos = 0;
        std::string::size_type right_pos = 0;
        std::string::size_type equal_pos = 0;
        std::string now_root = "";
        std::string str_key = "";
        std::string str_val = "";


        if ((str_line.npos != (left_pos = str_line.find("["))) && (str_line.npos != (right_pos = str_line.find("]"))))
        {
            now_root = str_line.substr(left_pos + 1, right_pos - 1);
            now_root = trim(now_root);

            if (str_root.compare("") == 0)
            {
                str_root = now_root;
                mn.clear();
                continue;
            }

            if (now_root.compare(str_root) != 0)
            {
                nodes.insert(pnodes(str_root, mn));
                str_root = now_root;
                mn.clear();
                continue;
            }
        }

        if (str_line.npos != (equal_pos = str_line.find("=")))
        {
            str_key = str_line.substr(0, equal_pos);
            str_val = str_line.substr(equal_pos + 1, str_line.size() - 1);

            str_key = trim(str_key);
            str_val = trim(str_val);

            mn.insert(pnode(str_key, str_val));
            continue;
        }
    }

    nodes.insert(pnodes(str_root, mn));
    mn.clear();

    config_file.close();
    config_file.clear();
    return 0;
}


int ini::write(std::string path, mnodes nodes)
{
    std::ofstream config_file(path.c_str());
    if (!config_file) return -1;

    for (mnodes::iterator itr = nodes.begin(); itr != nodes.end(); itr++)
    {
        config_file << "[" << itr->first << "]" << std::endl;
        for (mnode::iterator s_itr = itr->second.begin(); s_itr != itr->second.end(); ++s_itr)
        {
            config_file << s_itr->first << "=" << s_itr->second << std::endl;
        }
    }

    config_file.close();
    config_file.clear();
    return 0;
}

std::string ini::get_value(mnodes nodes, std::string root, std::string key)
{
    mnodes::iterator itr = nodes.find(root);
    if (nodes.end() == itr)
        return "";
    
    mnode::iterator s_itr = itr->second.find(key);
    if (itr->second.end() == s_itr 
        || (s_itr->second).empty())
        return "";

    return s_itr->second;
}

int ini::set_value(mnodes &nodes, std::string root, std::string key, std::string value)
{
    mnodes::iterator itr = nodes.find(root);
    if (nodes.end() != itr)
        itr->second.insert(pnode(key, value));
    else
    {
        mnode mn;
        mn.insert(pnode(key, value));
        nodes.insert(pnodes(root, mn));
    }

    return 0;
}
