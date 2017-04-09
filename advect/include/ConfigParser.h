# ifndef __CONFIGPARSER_H__
# define __CONFIGPARSER_H__

# include <vector>
# include <map>
# include <iostream>
# include <fstream>
# include <string>

using namespace std;


namespace cfg
{
    typedef map<string, float> FloatValueMap;

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
                floatValueMap[key] = value;
                cout << key << ": " << value << endl;
            }
        }
        cout << "========================" << endl;
        cfgFile.close();

        return floatValueMap;
    }

}

# endif
