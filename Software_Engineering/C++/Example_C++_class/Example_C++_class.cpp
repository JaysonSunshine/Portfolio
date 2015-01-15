#include <map>
#include <vector>
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

struct price_size{
	double price;
	int size;
	bool operator==(const price_size &c1){
		return (price == c1.price);
	}
	bool operator!=(const price_size &c1){
		return (price != c1.price);
	}
	bool operator<(const price_size &c1){
		return (price < c1.price);
	}
};

class OrderBook{
	
	public:
		void add(int order_id,char side,double price,int size);
		void modify(int order_id,int new_size);
		void remove(int order_id);
		double get_price(char side,int level);
		int get_size(char side,int level);
	private:
		map<int, order> orders;
		vector<price_size> buy_aggr;
		vector<price_size> sell_aggr;
};

double OrderBook::get_price(char side,int level){
	if(level < 1)
		level = 1; //could alternatively fail with message to user
	int loc = 1;
	vector<price_size>::iterator ps;
	if(side == 'B' || side == 'b'){
		ps = buy_aggr.end() - 1;
		if(level >= buy_aggr.size()){
			ps = buy_aggr.begin();
			return (*ps).price;
		}
		while(loc < level && ps != buy_aggr.begin()){
			if(*ps != *(ps - 1)){
				loc++;
			}
			ps--;
		}
		return (*ps).price;
	}
	else{
		ps = sell_aggr.begin();
		if(level >= sell_aggr.size())
			return (*(sell_aggr.end() - 1)).price;
		while(loc < level && ps < sell_aggr.end()){
			if(*ps != *(ps + 1)){
				loc++;
			}
			ps++;
		}
		if(ps == sell_aggr.end())
			ps--;
		return (*ps).price;
	}
}

int OrderBook::get_size(char side,int level){
	if(level < 1)
		level = 1; //could alternatively fail with message to user
	int loc = 1;
	int sum = 0;
	vector<price_size>::iterator it;
	if(side == 'B' || side == 'b'){
		if(level >= buy_aggr.size())
			level = buy_aggr.size();
		it = buy_aggr.end() - 1;
		while(loc < level && it > buy_aggr.begin()){
			if(*it != *(it - 1)){
				loc++;
			}
			it--;
		}
		sum = (*it).size;
		while(it > buy_aggr.begin() && *it == *(it - 1)){
			it--;
			sum += (*it).size;
			
		}
		return sum;
	}
	else{
		it = sell_aggr.begin();
		while(loc < level && it < sell_aggr.end()){
			if(*it != *(it + 1)){
				loc++;
			}
			it++;
		}
		sum = (*it).size;
		while(*it == *(it + 1) && it < sell_aggr.end()){
			it++;
			sum += (*it).size;
		}
		return sum;
	}
}

void OrderBook::add(int order_id,char side,double price,int size){
	order new_order;
	new_order.side = side;
	new_order.price = price;
	new_order.size = size;
	map<int, order>::iterator it;
	it = orders.find(order_id);
	if(it == orders.end()){
		struct price_size ps;
		ps.price = price;
		ps.size = size;
		orders.insert(pair<int, order>(order_id, new_order));
		if(side == 'B' || side == 'b'){
			auto lower = lower_bound(buy_aggr.begin(), buy_aggr.end(), ps);
			buy_aggr.insert(lower, ps);
		}
		else{
			auto lower = lower_bound(sell_aggr.begin(), sell_aggr.end(), ps);			
			sell_aggr.insert(lower, ps);
		}
	}
}

void OrderBook::modify(int order_id,int new_size){
	map<int, order>::iterator it;
	vector<price_size>::iterator it_ps;
	it = orders.find(order_id);
	price_size old;
	if(it == orders.end())
		cout << "ID " << order_id << " not found--nothing to modify\n";
	else{
		old.price = it->second.price;
		old.size = it->second.size;
		if(it->second.side == 'B' || it->second.side == 'b'){
			it_ps = find(buy_aggr.begin(), buy_aggr.end(), old);
			old.size = new_size;
			*it_ps = old;
		}
		else{
			it_ps = find(sell_aggr.begin(), sell_aggr.end(), old);
			old.size = new_size;
			*it_ps = old;
		}
		it->second.size = new_size;
	}
}

void OrderBook::remove(int order_id){
	map<int, order>::iterator it;
	it = orders.find(order_id);
	vector<price_size>::iterator it_ps;
	price_size old;
	if(it == orders.end())
		cout << "Order " << order_id << " doesn't exist--nothing to erase\n";
	else{
		old.price = it->second.price;
		old.size = it->second.size;
		if(it->second.side == 'B' || it->second.side == 'b'){
			it_ps = find(buy_aggr.begin(), buy_aggr.end(), old);
			buy_aggr.erase(it_ps);
		}
		else{
			it_ps = find(sell_aggr.begin(), sell_aggr.end(), old);
			sell_aggr.erase(it_ps);	
		}
		orders.erase(it);
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