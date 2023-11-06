#include <iostream>
#include <bitset>
#include <fstream>

int main() {
    std::ifstream fileInputTable("Results/InputTable.csv"), fileInputValue("Results/ValueOutput.csv");
    std::ofstream outputInputTable("Results/Decimals/DecimalsInputTable.csv"), 
        outputInputValue("Results/Decimals/DecimalsValueOutput.csv");
    std::string line{};
    while (std::getline(fileInputTable,line))
    {
        char signal1;
        char signal2;

        if(line[0] == '1'){
            signal1 = '-';
        }else{
            signal1 = '+';
        };
        if(line[5] == '1'){
            signal2 = '-';
        }else{
            signal2 = '+';
        };
        std::string first_input = line.substr(1, line.length()/2);
        std::string second_input = line.substr(line.length()/2 + 1, line.length()); 

        outputInputTable << signal1 << std::bitset<5>(first_input).to_ulong() <<"\t" 
        << signal2 << std::bitset<5>(second_input).to_ulong() <<std::endl;
    }
    outputInputTable.close();
    while (std::getline(fileInputValue,line))
    {
        outputInputValue << std::bitset<5>(line).to_ulong() << std::endl;
    }
    outputInputValue.close();
    return 0;
}
