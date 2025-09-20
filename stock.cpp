#include <iostream>
#include <string> 
#include <vector>

void main() {
	int choice;
	do {
		std::cout << " //Welcome to Stock Sim!\\ \n";
		std::cout << "1. View Stock Market";
		std::cout << "2. Buy Stocks";
		std::cout << "3. Sell Stocks";
		std::cout << "4. Update Prices (Next Day)";
		std::cout << "5. View Portfolio";
		std::cout << "6. Load Portfolio";
		std::cout << "7. Save & Exit";
		std::cin >> choice;

		// switch(choice){
		// 	case 1: 
		// 	case 2: 
		// 	case 3: 
		// 	case 4:
		// 	case 5:
		// 	case 6:
		// 	case 7:
		// }
	}
	while (choice != 7);
};

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

		void depositCash(double amount) {
			//
		};

		void withdrawCash(double amount) {
			//
		};

		void saveFile() {

		};

		void loadFile() {

		};



};