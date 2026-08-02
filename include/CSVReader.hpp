#ifndef CSVREADER_HPP
#define CSVREADER_HPP

#include <string>
#include <vector>

using namespace std;

class CSVReader
{
public:
    vector<vector<string>> read(const string& filename);
};

#endif