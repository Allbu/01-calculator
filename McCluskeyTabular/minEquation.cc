#include <iostream>
#include <bitset>
#include <sstream>
#include <map>
#include <fstream>



uint32_t verifyTotalOfOnes(std::stringstream& input){
    uint32_t nOnes{0};
    std::string bit;
    while(std::getline(input,bit)){
        for(char c : bit){
            if(c == '1'){
                nOnes++;
            };
        }
    }
    return nOnes;
}

void run(std::string& path){
    std::ifstream input(path);
    std::string bits;
    std::string m_bits[1024][10];
    uint32_t line_1, line_2, line_3, line_4, line_5, 
             line_6, line_7, line_8, line_9;
    while(std::getline(input,bits)){
        std::stringstream currentBits(bits);
        uint32_t n_bits = verifyTotalOfOnes(currentBits);
        switch (n_bits)
        {
        case 2:
            m_bits[line_1][1] = std::bitset <10>(bits).to_string();
            line_1++;
            break;
        case 3:
            m_bits[line_2][2] = std::bitset <10>(bits).to_string();
            line_2++;
            break;
        case 4:
            m_bits[line_3][3] = std::bitset <10>(bits).to_string();
            line_3++;
            break;
        case 5:
            m_bits[line_4][4] = std::bitset <10>(bits).to_string();
            line_4++;
            break;
        case 6:
            m_bits[line_5][0] = std::bitset <10>(bits).to_string();
            line_5++;
            break;
        case 7:
            m_bits[line_6][0] = std::bitset <10>(bits).to_string();
            line_6++;
            break;
        case 8:
            m_bits[line_7][0] = std::bitset <10>(bits).to_string();
            line_7++;
            break;
        case 9:
            m_bits[line_8][0] = std::bitset <10>(bits).to_string();
            line_8++;
            break;

        }
    }
    
}
int main(){

std::string path{"../Results/IndividualBinaries/Signal.csv"};
run(path);

}