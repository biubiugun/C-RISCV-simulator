#include "CPUSimulator.h"
using namespace std;

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
