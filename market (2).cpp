#include "market.h"

market::market(int argc, char** argv)
{
	
}

void market::start()
{
	std::string convert;
    std::fstream file("output.txt", std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
    }

    std::vector<Tradeorder> tradeorders;
    std::string line;
    const int maxEntities = 10000000;
    std::vector<CusPriorityQueue> buyOrders(maxEntities);
    std::vector<CustomPriorityQueue> sellOrders(maxEntities);

    int i  =  1;
    int no_of_trades = 0;
    int money_transfered = 0;
    int shares_traded = 0;
    std::vector<company>companies_vector;

    while (std::getline(file, line)) {
        if ( line == "TL" || line == "!@") {
            continue;
        }
        if(line == "@!" ){
            break;
        }
        // std::cout << line << std::endl;
        Tradeorder tradeOrder;

        std::istringstream ss(line);
        ss >> tradeOrder.timestamp.first >> tradeOrder.entity >>tradeOrder.action ;

        Stock stock;
        std::string combined_token;
        while (ss >> stock.ticker) {
            // Check if the next token is an integer
            if (isDollar(stock.ticker)) {
                // If it's an integer, it's the second timestamp
                break;
            }
            combined_token +=  stock.ticker + " ";
        }       
                combined_token.pop_back();
                convert      = stock.ticker;
                if(combined_token.find(' ') != std::string::npos){ //space is there
                    if(changeorder(combined_token) != "wrong_input"){
                        stock.ticker = changeorder(combined_token);
                    }
                    else{
                        continue;
                    }
                }
                else{
                    stock.ticker = combined_token;
                }
                stock.price = extractAmount(convert);
                ss >> convert;
                stock.quantity = extractAmount(convert);
                ss >> tradeOrder.timestamp.second;
                tradeOrder.stocks.push_back(stock);
        if(tradeOrder.action == "BUY"){
            size_t hashValue = customHash(tradeOrder.stocks[0].ticker);
            int entityIndex = hashValue % maxEntities;
            Tradeorder topmost;
            if(sellOrders[entityIndex].size()!=0){
                while(sellOrders[entityIndex].size()!=0){
                    topmost = sellOrders[entityIndex].top();
                    if((topmost.timestamp.second == -1) || ((tradeOrder.timestamp.first - topmost.timestamp.first) <= topmost.timestamp.second)){
                        if(topmost.stocks[0].price<=tradeOrder.stocks[0].price){
                            if(topmost.stocks[0].quantity>tradeOrder.stocks[0].quantity){
                                    if(tradeOrder.stocks[0].quantity!=0){
                                        std::cout<<tradeOrder.entity<<" purchased "<<tradeOrder.stocks[0].quantity<<" share of "<<tradeOrder.stocks[0].ticker<<" from "<<topmost.entity<<" for "<<"$"<<topmost.stocks[0].price<<"/share"<<std::endl;
                                        no_of_trades++;
                                    }
                                    
                                    shares_traded += tradeOrder.stocks[0].quantity;
                                    money_transfered += (tradeOrder.stocks[0].quantity)*(topmost.stocks[0].price);

                                    int p = 0;
                                    int q = 0;
                                    for(int j = 0;j < companies_vector.size();j++){
                                        if(companies_vector[j].company_name == tradeOrder.entity && p==0){
                                            p++;
                                            companies_vector[j].bought += tradeOrder.stocks[0].quantity;
                                            companies_vector[j].transfer_amount -= (tradeOrder.stocks[0].quantity)*(topmost.stocks[0].price);
                                        }
                                        if(companies_vector[j].company_name == topmost.entity && q==0){
                                            q++;
                                            companies_vector[j].sold += tradeOrder.stocks[0].quantity;
                                            companies_vector[j].transfer_amount += (tradeOrder.stocks[0].quantity)*(topmost.stocks[0].price);
                                        }
                                        if(p==1 && q==1){
                                            break;
                                        }
                                    }
                                    if(p==0 && tradeOrder.entity != topmost.entity){
                                        company sample;
                                        sample.bought = tradeOrder.stocks[0].quantity;
                                        sample.sold   = 0;
                                        sample.transfer_amount = -((tradeOrder.stocks[0].quantity)*(topmost.stocks[0].price));
                                        sample.company_name = tradeOrder.entity;
                                        companies_vector.push_back(sample);
                                    }
                                    if(q==0 && tradeOrder.entity != topmost.entity){
                                        company sampleq;
                                        sampleq.bought = 0;
                                        sampleq.sold   = tradeOrder.stocks[0].quantity;
                                        sampleq.transfer_amount = (tradeOrder.stocks[0].quantity)*(topmost.stocks[0].price);
                                        sampleq.company_name = topmost.entity;
                                        companies_vector.push_back(sampleq);
                                    }
                                    if(p==0 && q==0 && tradeOrder.entity == topmost.entity){
                                        company sample;
                                        sample.bought  = tradeOrder.stocks[0].quantity;
                                        sample.sold    = tradeOrder.stocks[0].quantity;
                                        sample.transfer_amount = 0;
                                        sample.company_name = tradeOrder.entity;
                                        companies_vector.push_back(sample);
                                    }
                                    topmost.stocks[0].quantity -=  tradeOrder.stocks[0].quantity;
                                    sellOrders[entityIndex].pop();
                                    sellOrders[entityIndex].push(topmost);
                                    break;
                            }   
                            else{
                                if(topmost.stocks[0].quantity!=0){
                                    std::cout<<tradeOrder.entity<<" purchased "<<topmost.stocks[0].quantity<<" share of "<<tradeOrder.stocks[0].ticker<<" from "<<topmost.entity<<" for "<<"$"<<topmost.stocks[0].price<<"/share"<<std::endl;
                                    no_of_trades++;
                                }
                                
                                shares_traded += topmost.stocks[0].quantity;
                                money_transfered += (topmost.stocks[0].quantity)*(topmost.stocks[0].price);


                                    int p = 0;
                                    int q = 0;
                                    for(int j = 0;j < companies_vector.size();j++){
                                        if(companies_vector[j].company_name == tradeOrder.entity && p==0){
                                            p++;
                                            companies_vector[j].bought += topmost.stocks[0].quantity;
                                            companies_vector[j].transfer_amount -= (topmost.stocks[0].quantity)*(topmost.stocks[0].price);
                                        }
                                        if(companies_vector[j].company_name == topmost.entity && q==0){
                                            q++;
                                            companies_vector[j].sold += topmost.stocks[0].quantity;
                                            companies_vector[j].transfer_amount += (topmost.stocks[0].quantity)*(topmost.stocks[0].price);
                                        }
                                        if(p==1 && q==1){
                                            break;
                                        }
                                    }
                                    if(p==0 && tradeOrder.entity != topmost.entity){
                                        company sample;
                                        sample.bought = topmost.stocks[0].quantity;
                                        sample.sold   = 0;
                                        sample.transfer_amount = -((topmost.stocks[0].quantity)*(topmost.stocks[0].price));
                                        sample.company_name = tradeOrder.entity;
                                        companies_vector.push_back(sample);
                                    }
                                    if(q==0 && tradeOrder.entity != topmost.entity){
                                        company sampleq;
                                        sampleq.bought = 0;
                                        sampleq.sold   = topmost.stocks[0].quantity;
                                        sampleq.transfer_amount = (topmost.stocks[0].quantity)*(topmost.stocks[0].price);
                                        sampleq.company_name = topmost.entity;
                                        companies_vector.push_back(sampleq);
                                    }
                                    if(p==0 && q==0 && tradeOrder.entity == topmost.entity){
                                        company sample;
                                        sample.bought = topmost.stocks[0].quantity;
                                        sample.sold   = topmost.stocks[0].quantity;
                                        sample.transfer_amount = 0;
                                        sample.company_name = topmost.entity;
                                        companies_vector.push_back(sample);
                                    }

                                tradeOrder.stocks[0].quantity = tradeOrder.stocks[0].quantity - topmost.stocks[0].quantity;
                                sellOrders[entityIndex].pop();
                                if(sellOrders[entityIndex].size()==0){
                                    buyOrders[entityIndex].push(tradeOrder);
                                    break;
                                }
                                continue;
                            }
                        }
                        else{
                            buyOrders[entityIndex].push(tradeOrder);
                            break;
                        }
                    }
                    else{
                        sellOrders[entityIndex].pop();
                        if(sellOrders[entityIndex].size()==0){
                            buyOrders[entityIndex].push(tradeOrder);
                            break;
                        }
                        continue;
                    }
                }
            }
            else{
                buyOrders[entityIndex].push(tradeOrder);
            }
        }

        else if(tradeOrder.action == "SELL"){
            size_t hashValue = customHash(tradeOrder.stocks[0].ticker);
            int entityIndex = hashValue % maxEntities;
            Tradeorder buymost;
            if(buyOrders[entityIndex].size()!=0){
                while (buyOrders[entityIndex].size()!=0){
                    buymost = buyOrders[entityIndex].top();
                    if((buymost.timestamp.second == -1) || ((tradeOrder.timestamp.first - buymost.timestamp.first) <= buymost.timestamp.second)){
                        if(buymost.stocks[0].price>=tradeOrder.stocks[0].price){
                            if(buymost.stocks[0].quantity>tradeOrder.stocks[0].quantity){
                                    buymost.stocks[0].quantity -=  tradeOrder.stocks[0].quantity;
                                    buyOrders[entityIndex].pop();
                                    buyOrders[entityIndex].push(buymost);
                                    if(tradeOrder.stocks[0].quantity!=0){
                                        std::cout<<buymost.entity<<" purchased "<<tradeOrder.stocks[0].quantity<<" share of "<<tradeOrder.stocks[0].ticker<<" from "<<tradeOrder.entity<<" for "<<"$"<<buymost.stocks[0].price<<"/share"<<std::endl;
                                        no_of_trades++;
                                    }
                                    
                                    shares_traded += tradeOrder.stocks[0].quantity;

                                    
                                    int p = 0;
                                    int q = 0;
                                    for(int j = 0;j < companies_vector.size();j++){
                                        if(companies_vector[j].company_name == buymost.entity && p==0){
                                            p++;
                                            companies_vector[j].bought += tradeOrder.stocks[0].quantity;
                                            companies_vector[j].transfer_amount -= (tradeOrder.stocks[0].quantity)*(buymost.stocks[0].price);
                                        }
                                        if(companies_vector[j].company_name == tradeOrder.entity && q==0){
                                            q++;
                                            companies_vector[j].sold += tradeOrder.stocks[0].quantity;
                                            companies_vector[j].transfer_amount += (tradeOrder.stocks[0].quantity)*(buymost.stocks[0].price);
                                        }
                                        if(p==1 && q==1){
                                            break;
                                        }
                                    }
                                    if(p==0 && buymost.entity != tradeOrder.entity){
                                        company sample;
                                        sample.bought = tradeOrder.stocks[0].quantity;
                                        sample.sold   = 0;
                                        sample.transfer_amount = -((tradeOrder.stocks[0].quantity)*(buymost.stocks[0].price));
                                        sample.company_name = buymost.entity;
                                        companies_vector.push_back(sample);
                                    }
                                    if(q==0 && buymost.entity != tradeOrder.entity){
                                        company sampleq;
                                        sampleq.bought = 0;
                                        sampleq.sold   = tradeOrder.stocks[0].quantity;
                                        sampleq.transfer_amount = ((tradeOrder.stocks[0].quantity)*(buymost.stocks[0].price));
                                        sampleq.company_name = tradeOrder.entity;
                                        companies_vector.push_back(sampleq);
                                    }
                                    if(p == 0 && q ==0 && buymost.entity == tradeOrder.entity){
                                        company sample;
                                        sample.bought = tradeOrder.stocks[0].quantity;
                                        sample.sold   = tradeOrder.stocks[0].quantity;
                                        sample.transfer_amount = 0;
                                        sample.company_name = buymost.entity;
                                         companies_vector.push_back(sample);
                                    }


                                    money_transfered += (tradeOrder.stocks[0].quantity)*(buymost.stocks[0].price);
                                    break;
                            }
                            else{
                                if(buymost.stocks[0].quantity!=0){
                                    std::cout<<buymost.entity<<" purchased "<<buymost.stocks[0].quantity<<" share of "<<tradeOrder.stocks[0].ticker<<" from "<<tradeOrder.entity<<" for "<<"$"<<buymost.stocks[0].price<<"/share"<<std::endl;
                                    no_of_trades++;
                                }
                                

                                                                    
                                    int p = 0;
                                    int q = 0;
                                    for(int j = 0;j < companies_vector.size();j++){
                                        if(companies_vector[j].company_name == buymost.entity && p==0){
                                            p++;
                                            companies_vector[j].bought += buymost.stocks[0].quantity;
                                            companies_vector[j].transfer_amount -= (buymost.stocks[0].quantity)*(buymost.stocks[0].price);
                                        }
                                        if(companies_vector[j].company_name == tradeOrder.entity && q==0){
                                            q++;
                                            companies_vector[j].sold += buymost.stocks[0].quantity;
                                            companies_vector[j].transfer_amount += (buymost.stocks[0].quantity)*(buymost.stocks[0].price);
                                        }
                                        if(p==1 && q==1){
                                            break;
                                        }
                                    }
                                    if(p==0 && buymost.entity!=tradeOrder.entity){
                                        company sample;
                                        sample.bought = buymost.stocks[0].quantity;
                                        sample.sold   = 0;
                                        sample.transfer_amount = -((buymost.stocks[0].quantity)*(buymost.stocks[0].price));
                                        sample.company_name = buymost.entity;
                                        companies_vector.push_back(sample);
                                    }
                                    if(q==0 && buymost.entity!=tradeOrder.entity){
                                        company sampleq;
                                        sampleq.bought = 0;
                                        sampleq.sold   = buymost.stocks[0].quantity;
                                        sampleq.transfer_amount = ((buymost.stocks[0].quantity)*(buymost.stocks[0].price));
                                        sampleq.company_name = tradeOrder.entity;
                                        companies_vector.push_back(sampleq);
                                    }
                                    if(p==0 && q ==0 && buymost.entity==tradeOrder.entity){
                                        company sample;
                                        sample.bought = buymost.stocks[0].quantity;
                                        sample.sold   = buymost.stocks[0].quantity;
                                        sample.company_name = buymost.entity;
                                        sample.transfer_amount = 0;
                                        companies_vector.push_back(sample);
                                    }

                                shares_traded += buymost.stocks[0].quantity;
                                money_transfered += (buymost.stocks[0].quantity)*(buymost.stocks[0].price);
                                tradeOrder.stocks[0].quantity -=  buymost.stocks[0].quantity;
                                buyOrders[entityIndex].pop();
                                if(buyOrders[entityIndex].size()==0){
                                    sellOrders[entityIndex].push(tradeOrder);
                                    break;
                                }
                                continue;
                            }
                        }
                        else{
                            sellOrders[entityIndex].push(tradeOrder);
                            break;
                        }
                    }
                    else{
                        buyOrders[entityIndex].pop();
                        if(buyOrders[entityIndex].size()==0){
                            sellOrders[entityIndex].push(tradeOrder);
                            break;
                        }
                        continue;
                    }
                }
            }
            else{
                sellOrders[entityIndex].push(tradeOrder);
            }
        }
        //printHeaps(buyOrders,sellOrders);
    }
    std::cout<<std::endl;
    std::cout<<"---End of Day---"<<std::endl;
    std::cout<<"Total Amount of Money Transferred: "<<"$"<<money_transfered<<std::endl;
    std::cout<<"Number of Completed Trades: "<<no_of_trades<<std::endl;
    std::cout<<"Number of Shares Traded: "<<shares_traded<<std::endl;
    for(int iter = companies_vector.size()-1; iter >= 0;iter -- ){
        std::cout<<companies_vector[iter].company_name<<" bought "<<companies_vector[iter].bought
        <<" and sold "<<companies_vector[iter].sold<<" for a net transfer of $"<<companies_vector[iter].transfer_amount<<std::endl;
    }
}
