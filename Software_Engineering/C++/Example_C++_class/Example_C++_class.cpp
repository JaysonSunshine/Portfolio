#include <map>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct order{
	char side;
	double price;
	int size;
};

class OrderBook{
	
	public:
		void add(int order_id,char side,double price,int size);
		void modify(int order_id,int new_size);
		void remove(int order_id);
		double get_price(char side,int level);
		int get_size(char side,int level);
	private:
		map<double, int> buy_aggr;
		map<double, int> sell_aggr;
		unordered_map<int, order> orders;
};

double OrderBook::get_price(char side,int level){
	if(level < 1)
		level = 1; //could alternatively fail with message to user
	map<double, int>::iterator ps;
	if(side == 'B' || side == 'b'){ //what should we do if call get_price on empty orders?
		if(level >= buy_aggr.size()){ //could return with message to user
			return buy_aggr.begin()->first;
		}
		ps = buy_aggr.end();
		for(int i = 0; i < level; i++)
			ps--;
		return ps->first;
	}
	else{
		if(level >= sell_aggr.size()){
			ps = sell_aggr.end();
			ps--;
			return ps->first;
		}
		ps = sell_aggr.begin();
		for(int i = 1; i < level; i++){
			ps++;
		}
		return ps->first;
	}
}

int OrderBook::get_size(char side,int level){
	if(level < 1)
		level = 1; //could alternatively fail with message to user
	map<double, int>::iterator it;
	if(side == 'B' || side == 'b'){
		if(level >= buy_aggr.size()){
			it = buy_aggr.begin();
			return it->second;
		}
		it = buy_aggr.end();
		for(int i = 0; i < level; i++){
			it--;
		}
		return it->second;
	}
	else{
		if(level >= sell_aggr.size()){
			it = sell_aggr.end();
			it--;
			return it->second;
		}
		it = sell_aggr.begin();
		for(int i = 1; i < level; i++){
			it++;
		}
		return it->second;
	}
}

void OrderBook::add(int order_id,char side,double price,int size){
	unordered_map<int, order>::iterator it;
	it = orders.find(order_id);
	if(it == orders.end()){ //message to user on duplicate insert?
		order new_order;
		new_order.side = side;
		new_order.price = price;
		new_order.size = size;
		orders[order_id] = new_order;
		if(side == 'B' || side == 'b'){
			buy_aggr[price] += size;
		}
		else{
			sell_aggr[price] += size;
		}
	}
}

void OrderBook::modify(int order_id,int new_size){
	unordered_map<int, order>::iterator it;
	it = orders.find(order_id);
	if(it == orders.end())
		cout << "ID " << order_id << " not found--nothing to modify\n";
	else{
		int difference = new_size - it->second.size;
		if(it->second.side == 'B' || it->second.side == 'b'){
			buy_aggr[it->second.price] += difference;
		}
		else{
			sell_aggr[it->second.price] += difference;
		}
		it->second.size = new_size;
	}
}

void OrderBook::remove(int order_id){
	unordered_map<int, order>::iterator it;
	it = orders.find(order_id);
	if(it == orders.end())
		cout << "Order " << order_id << " doesn't exist--nothing to erase\n";
	else{
		if(it->second.side == 'B' || it->second.side == 'b'){
			buy_aggr[it->second.price] -= it->second.size;
		}
		else{
			sell_aggr[it->second.price] -= it->second.size;	
		}
		orders.erase(order_id);
	}
}

int main(int argc, char* argv[]){
	OrderBook test;
	ifstream myfile;
	myfile.open(argv[1]);
	string line;
	string word, word2;
	char side;
	int order_id, size;
	double price;
	while(getline(myfile,line))
	{
		istringstream iss(line);
		
		iss >> word;
		if(word == "add"){
			iss >> order_id;
			iss >> side;
			iss >> price;
			iss >> size;
			test.add(order_id, side, price, size);
		}
		else if(word == "modify"){
			iss >> order_id;
			iss >> size;
			test.modify(order_id, size);
		}
		else if(word == "get"){
			iss >> word2;
			if(word2 == "price"){
				iss >> side;
				iss >> price;
				cout << test.get_price(side, price) << "\n";
			}
			else{
				iss >> side;
				iss >> price;
				cout << test.get_size(side, price) << "\n";
			}
		}
		else if(word == "remove"){
			iss >> order_id;
			test.remove(order_id);
		}
	}
	myfile.close();
}