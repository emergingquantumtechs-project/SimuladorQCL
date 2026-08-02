#ifndef CSVREADER_HPP
#define CSVREADER_HPP

#include <string>
#include <vector>

class CSVReader
{
public:
    std::vector<std::vector<std::string>> read(const std::string& filename);
};

#endif