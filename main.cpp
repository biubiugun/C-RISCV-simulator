#include "CPUSimulator.h"
using namespace std;

void read(u_int8_t *mem){
    int address;
    std::string data;
    while(1){
        std::cin >> data;
        if(std::cin.eof()){
            return;
        }
        if(data[0] == '@'){
            sscanf(data.c_str() + 1, "%x", &address);
        }
        else{
            int sum;
            sscanf(data.c_str(), "%x", &sum);
            for(int i = 1; i <= 3; ++i){
                int tmp_data;
                std::cin >> std::hex >> tmp_data;
                sum += (tmp_data << (i * 8));
            }
            memcpy(mem + address, &sum, sizeof(sum));
            address += 4;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    Simulator risc_v;
//    read(risc_v.memory);
    u_int32_t index = 0;
    while(true){
        string data;
        cin >> data;
        if(data == "end")break;
        if(cin.eof())break;
        if(data[0] == '@'){
            sscanf(data.c_str() + 1,"%x",&index);
        }else{
            int32_t order;
            sscanf(data.c_str(),"%x",&order);
            for(int i = 1;i <= 3; ++i){
                int32_t tmp;
                cin >> hex >> tmp;
                order += (tmp << (8 * i));
            }
            memcpy(risc_v.memory + index,&order,sizeof(u_int32_t));
            index += 4;
        }
    }
    risc_v.run();

    return 0;
}
