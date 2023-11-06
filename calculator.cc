#include <iostream>
#include <fstream>
#include <bitset>
#include <sstream>

/**
 * @author João Albuquerque
 * @memberof LASSE and UFPA
*/

/**
 * \brief Generate binaries combinations and verify if the output is 0 or 1;
 * \param outFile output file 
 * \param bits number of input bits 
 * \param current current bit 
*/
void generateBinaryCombinations(std::ofstream& outFile, int bits, std::string current = "") {
    if (bits == 0) {
        outFile << current << std::endl;
        return;
    }
    generateBinaryCombinations(outFile, bits - 1, current + "0");
    generateBinaryCombinations(outFile, bits - 1, current + "1");
}

/**
 * \brief Verify if the numbers are equal
 * \param input1 First input
 * \param input2 Second input
*/
bool verifyEq(std::bitset<5>& input1, std::bitset<5>& input2){
    std::bitset<4> LSB1 = (input1.to_ulong() & 0b1111);
    std::bitset<4> LSB2 = (input2.to_ulong() & 0b1111);
    if (LSB1.to_ulong() >= LSB2.to_ulong() && input1[4] == 0){
        return 0;
    } else if (LSB1.to_ulong() < LSB2.to_ulong() && input1[4] == 0)
    {
        return 1;
    }else if(LSB1.to_ulong() > LSB2.to_ulong() && input1[4] == 1){
        return 1;
    }else{
        return 0;
    }
}

/**
 * \brief Verify signal 
 * \param signal1 Signal of the first input
 * \param signal2 Signal of the second input
*/
bool verifySignal(std::bitset<5>& input1, std::bitset<5>& input2){
    // Most significant bit is in the last position of the bit array
    bool signal1 = input1[4];
    bool signal2 = input2[4];
    std::bitset <4> valInput1 = (input1.to_ulong() & (0b1111));
    std::bitset <4> valInput2 = (input2.to_ulong() & (0b1111));
    if (signal1 == signal2)
    {
        if (signal1 == 0 ){
            // Positive signal
            return 0;
        }else{
            // Negative signal
            if(valInput1.any() || valInput2.any()){
                return 1;
            }
            return 0;
        }
    }else{
        // Different signals
        return verifyEq(input1, input2); 
    }
    
}

/**
 * \brief Sum the input values
 * \param input1 First input 
 * \param input2 Second input
*/
std::bitset<5> sum(std::bitset<4>& input1, std::bitset<4>& input2) {
    std::bitset<5> result; 

    bool carry = false;

    for (size_t i = 0; i < input1.size(); i++) {
        bool bitA = input1[i];
        bool bitB = input2[i];
        bool sum = bitA ^ bitB ^ carry; // XOR logic port
        result[i] = sum;
        carry = (bitA & bitB) | (bitA & carry) | (bitB & carry); // Carry to all bits
    }
    // carray out to the last result bit
    result[4] = carry;
    return result;
}
/**
 * \brief Sub the input values
 * \param input1 First input 
 * \param input2 Second input
*/
std::bitset <5> sub(std::bitset<5>& input1, std::bitset<5>& input2, int index){
    std::bitset<4> invertedInput;
    std::bitset <4> valInput1 = (input1.to_ulong() & (0b1111));
    std::bitset <4> valInput2 = (input2.to_ulong() & (0b1111));
    if (input1[4] == 1)
    {
        invertedInput = ~valInput1;
        // Two's complement
        std::bitset <4> twosComplement(invertedInput.to_ulong() + 1);
        if(valInput1.to_ulong() > valInput2.to_ulong()){
            std::bitset <5> sumsComplement = sum(twosComplement,valInput2);
            // Correct value
            std::bitset <5> current_value(((~sumsComplement).to_ulong())+1);
            // Discard carry out because if one number is - and the other is +
            // Never gonna be more than 15
            current_value[4] =false;
            return (current_value);
        }else{
            std::bitset <5> current_value = sum(twosComplement,valInput2);
            current_value[4] =false;
            return current_value;
        }
        
    }else{

        invertedInput = ~valInput2;
        // Two's complement
        std::bitset <4> twosComplement((invertedInput.to_ulong()) + 1);
        if(valInput2.to_ulong() > valInput1.to_ulong()){
            std::bitset <5> sumsComplement = sum(twosComplement, valInput1);
            // Correct value
            std::bitset <5> current_value(((~sumsComplement).to_ulong())+1);
            // Discard carry out because if one number is - and the other is +
            // Never gonna be more than 15
            current_value[4] =false;
            return (current_value);
        }else{
            std::bitset <5> current_value = sum(twosComplement,valInput1);
            current_value[4] =false;
            return current_value;
        }
    }
}

/**
 * \brief The current operation for the current input
 * \param input1 First input
 * \param input2 Second input
*/
std::bitset <5> operationBit(std::bitset<5>& input1, std::bitset<5>& input2, int& line_index){
    std::bitset <4> valInput1 = (input1.to_ulong() & (0b1111));
    std::bitset <4> valInput2 = (input2.to_ulong() & (0b1111));
    if (input1[4] == false && input2[4] == false){
        return sum(valInput1, valInput2);
    }else if (input1[4] == true && input2[4] == true)
    {
        return sum(valInput1, valInput2);
    }
    return sub(input1, input2, line_index);
    

}
/**
 * \brief Verify if the output is true
 * \param path Path to inputs
*/
void verifyIfTrue(std::string& path){
    std::ofstream signalOutput("Results/Binaries/SignalOutput.csv"), 
        ValueOutput("Results/Binaries/ValueOutput.csv"),TruthsTableOutput("Results/Binaries/TruthsTableOutput.csv");
    std::ifstream inputs(path);
    int line_index{0};
    std::string line{};
    TruthsTableOutput << "signal" << "\t"<< "x4x3x2x1 signal z4z3z2z1" << "\t" << "signal y5y4y3y2y1" << std::endl;
    while(std::getline(inputs, line)){
        size_t bit_size = line.length();
        std::bitset<5> input1 (line.substr(0,(bit_size/2)));
        std::bitset<5> input2 (line.substr((bit_size/2),bit_size));

        bool calculator_signal = verifySignal(input1, input2);
        std::bitset<5> valueResult = operationBit(input1, input2, line_index);
        ValueOutput << valueResult << std::endl;
        signalOutput << calculator_signal << std::endl;
        TruthsTableOutput << line[0] << "\t\t"  << input1.to_string().substr(1,5) << "\t " 
                          << line[5] <<  "\t\t" << input2.to_string().substr(1,5) << "\t\t " 
                          << calculator_signal << "\t\t " << valueResult.to_string() << std::endl;
        line_index++;
    }
    TruthsTableOutput.close();
    signalOutput.close();
    ValueOutput.close();
}


int main() {
    int bits = 10;
    std::string pathOutput{"Results/Binaries/InputTable.csv"};
    std::ofstream outFile(pathOutput);

    if (outFile.is_open()) {
        generateBinaryCombinations(outFile, bits);
        outFile.close();
    }else{std::cerr << "Error";}
    verifyIfTrue(pathOutput);

    return 0;
}
