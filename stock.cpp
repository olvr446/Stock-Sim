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
		}
};

void displayMarket() {
	printf("hello");
}

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
		}

		void sellStock(std::string stockID, int quantity){
			//
		}

		std::vector <Stock> dispayPort() {
			//
		}

		std::vector<double> getPortVal() {
			//
		}

		Stock findStock(std::string stockID) {
			//
		}

		void depositCash(double amount) {
			//
		}

		void withdrawCash(double amount) {
			//
		}

		void saveFile() {

		}

		void loadFile() {

		}
};

int main() {
	int choice;
	do {
		std::cout << " //Welcome to Stock Sim!\\ \n";
		std::cout << "1. View Stock Market \n";
		std::cout << "2. Buy Stocks \n";
		std::cout << "3. Sell Stocks \n";
		std::cout << "4. Update Prices (Next Day) \n";
		std::cout << "5. View Portfolio \n";
		std::cout << "6. Load Portfolio \n";
		std::cout << "7. Save & Exit \n";
		std::cin >> choice;

		switch(choice){
			case 1: 
				displayMarket();
				break;
		}
	}
	while (choice != 7);

	return 0;
}