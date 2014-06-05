#include "KeyMatch.h"

namespace controller
{
    KeyMatch::KeyMatch(const string& firstFile, const string& secondFile, int firstKey, int secondKey)
    {
        this->fileController = new FileController();
        this->firstTable = new ValueTable();
        this->secondTable = new ValueTable();

        this->fileController->insertValues(firstFile, this->firstTable, firstKey);
        this->fileController->insertValues(secondFile, this->secondTable, secondKey);
        
        this->matchFiles();
    }

    KeyMatch::~KeyMatch()
    {
        //TODO: add complete destructors
    }

    void KeyMatch::matchFiles()
    {
        for (map<string, vector<string> >::const_iterator it = this->firstTable->getValues()->begin(); it != this->firstTable->getValues()->end(); ++it)
        {
            string key = it->first;
            vector<string> values = it->second;
            map<string, vector<string> >::iterator it2 = this->secondTable->getValues()->find(key);
            
            if (it2 != this->secondTable->getValues()->end())
            {
                //MATCH FOUND
                vector<string> values2 = it2->second;
                map<string, size_t> match = map<string, size_t>();
                this->join(values, match);
                this->join(values2, match);
                this->writeMatch(match);
            }
            else
            {
                //NO MATCH
                this->writeNoMatch(values);
            }
        }
    }

    void KeyMatch::join(const vector<string>& vector1, map<string, size_t>& results)
    {
        for (vector<string>::const_iterator it = vector1.begin(); it != vector1.end(); ++it)
        {
            string key = *it; 
            ++results[key];
        }
    }

    void KeyMatch::printMatch(map<string, size_t> match)
    {
        for (map<string, size_t>::const_iterator it = match.begin(); it != match.end(); ++it)
        {
            string value = it->first;
            cout << value << ", ";
        }
        cout << endl;
    }

    void KeyMatch::writeMatch(map<string, size_t> match)
    {
        string matchString = "";
        for (map<string, size_t>::const_iterator it = match.begin(); it != match.end(); ++it)
        {
            string value = it->first;
            matchString += value;
            matchString += ",";
        }
        matchString = matchString.substr(0, matchString.size()-1);
        this->fileController->writeValue("output.csv", matchString);
    }

    void KeyMatch::writeNoMatch(vector<string> row)
    {
        string noMatchString = "";
        for (vector<string>::const_iterator it = row.begin(); it != row.end(); ++it)
        {
            string value = *it;
            noMatchString += value;
            noMatchString += ",";
        }
        noMatchString = noMatchString.substr(0, noMatchString.size()-1);
        this->fileController->writeValue("notfound.csv", noMatchString);
    }

}
