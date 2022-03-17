#pragma once
#include <vector>
#include <string>



class CSVimport
{
    std::string fileName;
    std::string delimeter;
public:
    CSVimport(std::string filename, std::string delm = ",") :
        fileName(filename), delimeter(delm)
    { }
    // Function to fetch data from a CSV File
    std::vector<std::vector<std::string> > getData();
};