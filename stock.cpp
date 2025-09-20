#include <iostream>
#include <string> 
#include <vector>

class Stock {
	private:
		std::string stockID;
		std::string name;
		double price;
	public:
		double getPrice() {
			return price;
		};

		void updatePrice() {
			//
		};
};

struct PortItem {
	std::string name;
	int quantity;
};

class Port {
	private:
		double balance;
		std::vector <PortItem> holdings;

	public:
		void buyStock(std::string stockID, int quantity) {
			//
		};

		void sellStock(std::string stockID, int quantity){
			//
		};

		std::vector <Stock> dispayPort() {
			//
		};

		std::vector<double> getPortVal() {
			//
		};

		Stock findStock(std::string stockID) {
			//
		};




};