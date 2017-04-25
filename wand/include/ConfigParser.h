# ifndef __CONFIGPARSER_H__
# define __CONFIGPARSER_H__

# include <vector>
# include <map>
# include <iostream>
# include <fstream>
# include <string>

# include "Color.h"

using namespace std;


namespace cfg
{
    typedef map<string, float> FloatValueMap;
    typedef map<string, lux::Color> ColorValueMap;

    FloatValueMap floatValueParser(string cfgFileName)
    {
        ifstream cfgFile(cfgFileName, std::ios::in);
        if (!cfgFile.is_open())
        {
            cout << "Could not read file " << cfgFileName << ". File does not exist." << endl;
            exit(0);
        }

        string line = "";
        FloatValueMap floatValueMap;
        cout << "========================" << endl;
        cout << "***** Config Value *****" << endl;
        while (getline(cfgFile, line))
        {
            istringstream iss(line);
            string key;
            float value;
            if (iss >> key >> value)
            {
                if (key.at(0) != '#')   // look for comments
                {
                    floatValueMap[key] = value;
                    cout << key << ": " << value << endl;
                }
            }
        }
        cout << "========================" << endl;
        cfgFile.close();

        return floatValueMap;
    }


    ColorValueMap colorValueParser(string cfgFileName)
    {
        ifstream cfgFile(cfgFileName, std::ios::in);
        if (!cfgFile.is_open())
        {
            cout << "Could not read file " << cfgFileName << ". File does not exist." << endl;
            exit(0);
        }

        string line = "";
        ColorValueMap colorValueMap;
        cout << "========================" << endl;
        cout << "***** Config Value *****" << endl;
        while (getline(cfgFile, line))
        {
            istringstream iss(line);
            string key;
            float r, g, b, a;
            if (iss >> key >> r >> g >> b >> a)
            {
                if (key.at(0) != '#')   // look for comments
                {
                    lux::Color value = lux::Color(r, g, b, a);
                    colorValueMap[key] = value;
                    cout << key << ": " << "(" << r << ", " << g << ", " << b << ", " << a << ")" << endl;
                }
            }
        }
        cout << "========================" << endl;
        cfgFile.close();

        return colorValueMap;
    }

}

# endif
