#include "receiver.h"
#include <vector>
class stockorder{
    public:
    std::string name;                       // name of the stock
    int stockvalue;                         // represents current stock_value
    int past_b_order;         // remaining ready to sell orders
    int past_s_order;         // remaining ready to buy orders
    bool b,s;
    stockorder();
    stockorder(std::string s,int value){
    name = s;
    stockvalue = value;
    past_b_order = 0;
    past_s_order = 0;
  }
};
// phase1 part1 functions
stockorder* find_stock(std:: string s,std:: vector<stockorder*> stock ){
    int size = stock.size();
    for(int i=0;i<size;i++){
        if(strcmp(s.c_str(),stock[i]->name.c_str())==0){
        return stock[i];
        }
    } 
    return NULL;
}
void find_partial_arbitrage(std::vector<std::vector<int>>* M,int line,std::vector<int>* path,std::vector<int>* data,std::vector<std::vector<int>>* found_path){ 
    std::vector<int>* new_path = new std::vector<int>(M->size()+1,0);
    for(int j = 1; j< path->size();j++){
        (*new_path)[j] = path->at(j);
    }
    for(int i = 0;i<= M->at(line)[1];i++){
        if(line == M->size()-1 && i == 0) continue;
        bool found_arbitrage = true;
        std::vector<int>* new_data = new std::vector<int>(M->at(0).size()-3,0);
        for(int j = 0; j< data->size(); j++){
            (*new_data)[j] = data->at(j) + M->at(line)[j+3]*i*M->at(line)[2];
            if(new_data->at(j) != 0) found_arbitrage = false;
        }
        (*new_path)[line+1] = i;
        (*new_path)[0] = path->at(0) + M->at(line)[0]*i*M->at(line)[2];
        if(new_path->at(0) <= 0) found_arbitrage = false;
        if(found_arbitrage == true) {
            std::vector<int> x(*new_path) ;
            found_path->push_back(x);
        }
        if(line - 1 >= 0){
            find_partial_arbitrage(M,line-1,new_path,new_data,found_path);
        }
        delete new_data;
    }
    delete new_path;
return  ;
}
int main(int argc,char* argv[]) {
    Receiver rcv;
    bool found_dollar = false;
    std::string message = rcv.readIML();
    while(!found_dollar){
        std::string message1 = rcv.readIML();
        message.append(message1);
        if(message.find('$') != std::string::npos){
            //std::message1 = rcv.readIML();
            found_dollar= true;
        }
    }
    if(argv[1][0] == '1'){
    std:: vector<stockorder*> stock ;
    int i=0,j=0,value;
    std::string s,s1,status;
    while(message[i] != '$'){
        if(message[i] == '#'){
             int k = 0;
             while(true){
                if(message[j+k]==' ') break;
                k++;
             }
             s = message.substr(j,k);
             int q = 0;
             while(true){
                if(message[j+k+q+1]==' ') break;
                q++;
             }
             s1 = message.substr(j+k+1,q);
             value = std::stoi(s1);
             status = message.substr(j+k+q+2,1);
             j=i+2;
             if( stock.empty() || find_stock(s,stock) == NULL){
                stockorder* x = new stockorder(s,value);
                stock.push_back(x);
                if(status=="s"){
                std::cout<<s<<" "<<s1<<" "<<'b'<<std::endl;
                }
                else{
                std::cout<<s<<" "<<s1<<" "<<'s'<<std::endl;
                }
             }
             else{
                stockorder* x = find_stock(s,stock);
                int y = x->stockvalue;
                if(status == "b"){
                   if(value > x->past_b_order && x->b == true){
                        x->past_b_order = value;
                        if(value == x->past_s_order && x->s == true){
                        std::cout<<"No Trade"<<std::endl;
                        x->b = false;
                        x->s = false;
                        }
                        else if(value > x->stockvalue){
                        std::cout<<s<<" "<<s1<<" "<<'s'<<std::endl;
                        x->stockvalue = value;
                        }
                        else{
                        std::cout<<"No Trade"<<std::endl;
                        }
                    }
                    else if(x->b == false){
                        if(value == x->past_s_order && x->s == true){
                        std::cout<<"No Trade"<<std::endl;
                        x->b = false;
                        x->s = false;
                        }
                        else if(value > x->stockvalue){
                        std::cout<<s<<" "<<s1<<" "<<'s'<<std::endl;
                        x->stockvalue = value;
                        }
                        else{
                        std::cout<<"No Trade"<<std::endl;
                        x->past_b_order = value;
                        x->b = true;
                        }
                    }
                    else{
                        std::cout<<"No Trade"<<std::endl;
                    }
                }
                if(status == "s"){
                   if(value < x->past_s_order && x->s == true){
                        x->past_s_order = value;
                        if(value == x->past_b_order && x->b == true){
                        std::cout<<"No Trade"<<std::endl;
                        x->b = false;
                        x->s = false;
                        }
                        else if(value < x->stockvalue){
                        std::cout<<s<<" "<<s1<<" "<<'b'<<std::endl;
                        x->stockvalue = value;
                        }
                        else{
                        std::cout<<"No Trade"<<std::endl;
                        }
                    }
                    else if(x->past_s_order == 0){
                        if(value == x->past_b_order && x->b == true){
                        std::cout<<"No Trade"<<std::endl;
                        x->b = false;
                        x->s = false;
                        }
                        else if(value < x->stockvalue){
                        std::cout<<s<<" "<<s1<<" "<<'b'<<std::endl;
                        x->stockvalue = value;
                        }
                        else{
                        std::cout<<"No Trade"<<std::endl;
                        x->past_s_order = value;
                        x->s = true;
                        }
                    }
                    else{
                        std::cout<<"No Trade"<<std::endl;
                    }
                }
             }
             
        }
        i++;
    }
    return 0;
    }
    else if(argv[1][0] == '2'){
        std:: vector <std::vector<int>>* M = new std:: vector<std::vector<int>>() ;
        std:: vector<std:: string> map_name;
        int netprofit = 0;
        map_name.push_back("/n");
        map_name.push_back("/n");
        map_name.push_back("/n");
        int i = 0,j = 0;
        while(message[i]!= '$'){
            if(message[i] == '#'){
                std::vector<std::vector<int>>* found_path = new std::vector<std::vector<int>>();
                int size = map_name.size();
                std:: vector<int> order(size,0);
                int k= j,prev_k = j,next_k = j,itr = j,x,z;
                std:: string s,p,t;
                t = message.substr(i-1,1);
                if(t == "b"){order[2] = 1;}
                if(t == "s"){order[2] = -1;}
                while(itr != i-2){
                    if(message[itr] == ' '){
                        if(prev_k == k ){
                            k = itr;
                            next_k = itr;
                        }
                        else if(k == next_k ){
                            next_k = itr;
                            s = message.substr(prev_k,k-prev_k);
                            p = message.substr(k+1,next_k-k-1);
                            z = std::stoi(p);
                            for(x = 3;x<map_name.size();x++ ){
                                if(strcmp(map_name[x].c_str(),s.c_str()) == 0){
                                    break;
                                }
                            }
                            if(x == map_name.size()){
                                map_name.push_back(s);
                                order.push_back(z);
                            }else{
                                order[x] = z;
                            }
                            prev_k = next_k+1;
                            k = next_k+1;
                            next_k++;
                        }
                    }itr++;
                        if(itr == i-2){
                            s = message.substr(k,itr-k);
                            z = std::stoi(s);
                            order[0] = z;
                            order[1] = 1;
                        }
                }
                j=i+2;
                if(map_name.size() != size){
                    int extra = map_name.size()-size;                    
                    for(int n=0;n<M->size();n++){
                        for(int k=0;k<extra;k++){
                            (*M)[n].push_back(0);
                        }
                    }
                    M->push_back(order);
                }
                else{
                    bool found = false;
                    bool found_opposite = false;
                    int temp0 = order[0];
                    order[0] = 0;
                    for(k = 0;k < M->size();k++ ){
                        int m_temp0 = (*M)[k][0];
                        (*M)[k][0] = 0;
                        (order == (*M)[k])? found = true: found = false;
                        (*M)[k][0] = m_temp0;
                        if(found == true) break;
                    }
                    order[2] = -1*order[2];
                    order[0] = temp0;
                    for(x = 0;x < M->size();x++ ){
                        (order == (*M)[x])? found_opposite = true: found_opposite = false;
                        if(found_opposite == true) break;
                    }
                    order[2] = -1*order[2];
                    if(found == false){
                        if(found_opposite == false){
                            M->push_back(order);
                            std::vector<int>* path = new std::vector<int>(M->size()+1,0);
                            std::vector<int>* data = new std::vector<int>((*M)[0].size()-3,0);
                            find_partial_arbitrage(M,M->size()-1,path,data,found_path);
                            delete path;
                            delete data;
                        }
                        else{
                        auto itr = M->begin();
                        for(int y = 0;y < x ;y++){
                            itr++;
                        }
                        M->erase(itr);
                        }
                    }else{
                        if(order[0]*order[2] > (*M)[k][0]*(*M)[k][2]){
                        if(found_opposite == false){
                            auto itr = M->begin();
                            for(int y = 0;y < k ;y++){
                                itr++;
                            }
                            M->erase(itr);
                            M->push_back(order);
                            std::vector<int>* path = new std::vector<int>(M->size()+1,0);
                            std::vector<int>* data = new std::vector<int>((*M)[0].size()-3,0);
                            find_partial_arbitrage(M,M->size()-1,path,data,found_path);
                            delete path;
                            delete data;
                        }
                        else{
                            if(x > k){
                                auto itr = M->begin();
                                for(int y = 0; y < k;y++) itr++;
                                M->erase(itr);
                                itr++;
                                for(int y = k ;y < x-1;y++) itr++;
                                M->erase(itr);
                            }
                            else if( x < k){
                                auto itr = M->begin();
                                for(int y = 0; y < x;y++) itr++;
                                M->erase(itr);
                                itr++;
                                for(int y = x ;y < k-1;y++) itr++;
                                M->erase(itr);
                            }
                        }
                        }            
                    }
                }
            
            int index = 0;
            if(found_path->size() != 0){
                int index = 0;
                for(int i = 0;i< found_path->size();i++){
                    if(found_path->at(i)[0] > found_path->at(index)[0]) index = i;
                }
                for(int i = found_path->at(index).size()-1;i> 0 ;i--){
                    if(found_path->at(index)[i] == 0) continue;
                    for(int j = 3; j < (*M)[i-1].size();j++){
                        if((*M)[i-1][j] != 0){
                        std::cout<<map_name[j]<<" "<<(*M)[i-1][j]<<" ";
                    }
                }
                std::cout<<(*M)[i-1][0] <<" ";
                if((*M)[i-1][2] == -1)std::cout<<"b"<<std::endl;
                if((*M)[i-1][2] == 1)std::cout<<"s"<<std::endl;
                (*M)[i-1][1] -= found_path->at(index)[i];        
            }
            int x =1;
            for(int i = 1;i< found_path->at(index).size();i++){
                if((*M)[i-x][1] == 0){
                    auto itr = M->begin();
                    for(int k = 0;k<i-x;k++){
                        itr++;
                    }
                    (*M)[i-x].clear();
                    M->erase(itr);
                    x++;
                }
            }
                netprofit += found_path->at(index)[0];
            }
            else{
                std::cout<<"No Trade"<<std::endl;
            }
            delete found_path;
            }
            i++;
        }
            std::cout<<netprofit<<std::endl;
    }else if(argv[1][0] == '3'){
        std:: vector <std::vector<int>>* M = new std::vector<std::vector<int>>();
        std:: vector<std:: string> map_name;
        int netprofit = 0;
        map_name.push_back("/n");
        map_name.push_back("/n");
        map_name.push_back("/n");
        int i = 0,j = 0;
        while(message[i]!= '$'){
            if(message[i] == '#'){
                std::vector<std::vector<int>>* found_path = new std::vector<std::vector<int>>();
                int size = map_name.size();
                std:: vector<int> order(size,0);
                int k= j,prev_k = j,next_k = j,itr = j,x,z;
                std:: string s,p,t;
                t = message.substr(i-1,1);
                if(t == "b"){order[2] = 1;}
                if(t == "s"){order[2] = -1;}
                while(itr != i-2){
                    if(message[itr] == ' '){
                        if(prev_k == k ){
                            k = itr;
                            next_k = itr;
                        }
                        else if(k == next_k ){
                            next_k = itr;
                            s = message.substr(prev_k,k-prev_k);
                            p = message.substr(k+1,next_k-k-1);
                            z = std::stoi(p);
                            for(x = 3;x<map_name.size();x++ ){
                                if(strcmp(map_name[x].c_str(),s.c_str()) == 0){
                                    break;
                                }
                            }
                            if(x == map_name.size()){
                                map_name.push_back(s);
                                order.push_back(z);
                            }else{
                                order[x] = z;
                            }
                            prev_k = next_k+1;
                            k = next_k+1;
                            next_k++;
                        }
                    }itr++;
                        if(itr == i-2){
                            next_k = itr;
                            s = message.substr(prev_k,k-prev_k);
                            p = message.substr(k+1,next_k-k-1);
                            z = std::stoi(s);
                            x = std::stoi(p);
                            order[0] = z;
                            order[1] = x;
                        }
                }
                j=i+2;
                if(map_name.size() != size){
                    int extra = map_name.size()-size;                    
                    for(int n=0;n<M->size();n++){
                        for(int k=0;k<extra;k++){
                            (*M)[n].push_back(0);
                        }
                    }
                    M->push_back(order);
                }else{
                    bool found = false;
                    for(k = 0;k < M->size();k++ ){
                        int temp1 = order[1];
                        int temp2 = order[2];
                        int m_temp1 = (*M)[k][1];
                        int m_temp2 = (*M)[k][2];
                        order[1] = 0;
                        order[2] = 0;
                        (*M)[k][1] = 0;
                        (*M)[k][2] = 0;
                        (order == (*M)[k])? found = true: found = false;
                        order[1] = temp1;
                        order[2] = temp2;
                        (*M)[k][1] = m_temp1;
                        (*M)[k][2] = m_temp2;
                        if(found == true) break;
                    }
                    // std::cout<<found<<std::endl;
                    // for(int x= 0;x<order.size();x++){
                    //     std::cout<<order[x]<<" ";
                    // }
                    // std::cout<<std::endl;
                    if(found == false){
                        M->push_back(order);
                        std::vector<int>* path = new std::vector<int>(M->size()+1,0);
                        std::vector<int>* data = new std::vector<int>((*M)[0].size()-3,0);
                        find_partial_arbitrage(M,M->size()-1,path,data,found_path);
                        delete path;
                        delete data;
                    }else{
                        int temp = order[1]*order[2];
                        int prev = M->at(k)[1]*M->at(k)[2];
                        int sum = temp + prev;
                        if(sum > 0){
                            order[2] = 1;
                            order[1] = sum;
                        }else if(sum < 0){
                            order[2] = -1;
                            order[1] = -1*sum;
                        }
                        auto itr = M->begin();
                        for( x = 0;x<k;x++){
                            itr++;
                        }
                        if(sum != 0 ){
                            M->erase(itr);
                            M->push_back(order);
                            std::vector<int>* path = new std::vector<int>(M->size()+1,0);
                            std::vector<int>* data = new std::vector<int>((*M)[0].size()-3,0);
                            find_partial_arbitrage(M,M->size()-1,path,data,found_path);
                            delete path;
                            delete data;
                        }else{
                            M->erase(itr);
                        }                     
                    }
                }
        int index = 0;
        if(found_path->size() != 0){
            int index = 0;
            for(int i = 0;i< found_path->size();i++){
                if(found_path->at(i)[0] > found_path->at(index)[0]) index = i;
            }
            for(int i = found_path->at(index).size()-1;i> 0 ;i--){
                if(found_path->at(index)[i] == 0) continue;
                for(int j = 3; j < (*M)[i-1].size();j++){
                    if((*M)[i-1][j] != 0){
                    std::cout<<map_name[j]<<" "<<(*M)[i-1][j]<<" ";
                }
            }
            std::cout<<(*M)[i-1][0] <<" " <<found_path->at(index)[i]<<" ";
            if((*M)[i-1][2] == -1)std::cout<<"b"<<std::endl;
            if((*M)[i-1][2] == 1)std::cout<<"s"<<std::endl;
            (*M)[i-1][1] -= found_path->at(index)[i];        
        }
        int x =1;
        for(int i = 1;i< found_path->at(index).size();i++){
            if((*M)[i-x][1] == 0){
                auto itr = M->begin();
                for(int k = 0;k<i-x;k++){
                    itr++;
                }
                (*M)[i-x].clear();
                M->erase(itr);
                x++;
            }
        }
            netprofit += found_path->at(index)[0];
        }
        else{
            std::cout<<"No Trade"<<std::endl;
        }
        delete found_path;

        // for(int i =0 ; i< M->size() ; i++){
        //     for(int j = 0 ;j< (*M)[i].size();j++){
        //         std::cout<<(*M)[i][j]<<" ";
        //     }
        //     std::cout<<std::endl;
        // }
        // std::cout<<std::endl;
            }
            i++;
        }
        std::cout<<netprofit<<std::endl;

    }
    return 0;
}