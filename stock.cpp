#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <string> 
#include <vector>

class Stock {
	private:
		std::string stockID;
		std::string name;
		double price;
	public:
		Stock (std::string id, std::string name, double price)
			: stockID(id), name(name), price(price) {}

		std::string getID() const{
			return stockID;
		}

		std::string getName() const{
			return name;
		}
	
		double getPrice() const{
			return price;
		}
};

std::vector<Stock> initialiseStocks() {
	std::fstream inputFile("market.txt");
	std::vector<Stock> new_market;
	
	if (!inputFile) {
		std::cerr << "Error, file cannot load \n";
		return new_market;
	}

	std::string line;
	while (std::getline(inputFile, line)) {
		if (line.empty()) continue;
		std::stringstream ss(line);
		std::string stockAttr;
		std::vector<std::string> stockDetails;
		while (std::getline(ss, stockAttr, '|')) {
			stockDetails.push_back(stockAttr);
		}
		if (stockDetails.size() != 3) {
			std::cerr << "Incorrectly formatted line, skipping to next..." << line << std::endl;
			continue;
		}
		double price = 0.0;
        try {
            price = std::stod(stockDetails[2]);
        } catch (const std::exception& e) {
            std::cerr << "Invalid price in line: " << line << std::endl;
            continue;
        }
		new_market.emplace_back(stockDetails[0], stockDetails[1], price);
	}
	return new_market;
}

int displayMarket() {
	std::string ex;
	std::fstream inputFile("market.txt");

	if (!inputFile) {
		std::cerr << "Error, file cannot load \n";
		return 1;
	}

	std::string line;
	while (std::getline(inputFile, line)) {
		std::cout << line << std::endl;
	}

	inputFile.close();
	do {
	std::cout << "\nEnter 'E' to exit to Main Menu \n";
	std::cin >> ex;
	std::transform(ex.begin(), ex.end(), ex.begin(), ::toupper);
	} while (ex != "E");
	std::cout << "\n";
	return 0;

}

struct PortItem {
	const Stock stockItem;
	int quantity;

	PortItem(Stock item, int q) 
		: stockItem(item), quantity(q) {}
};

class Port {
	private:
		double balance;
		std::vector <PortItem> holdings;

	public:
		Port(double bal, std::vector <PortItem> hold)
			: balance(bal), holdings(hold) {}
		
		int buyStock(std::vector<Stock> market) {
			std::string id;
			int quantity;
			std::cout << "Enter the ID of the stock you wish to purchase: \n";
			std:: cin >> id;
			std::cout << "Enter the quantity of stock you wish to purchase: \n";
			std::cin >> quantity;
			for (const auto& stock:market) {
				if (stock.getID() == id) { 
					double new_balance = balance - (stock.getPrice() * quantity);
					if (new_balance < 0) {
						std::cerr << "Insufficient funds \n";
					} else {
						balance = new_balance;
						holdings.emplace_back(stock, quantity);
						std::cout << "Stock Bought \n";
						std::cout << "New Balance: " << balance << "\n";
					}
					return 0;
				}
			}
			std::cerr << "Error: Invalid stock ID";
			return 0;
		}

		int sellStock(std::vector<Stock> market) {
			std::string id;
			int quantity;
			std::cout << "Enter the ID of stock you wish to sell \n";
			std::cin >> id;
			std::cout << "Enter the quantity of stock you wish to sell \n";
			std::cin >> quantity;

			for (auto& stock : holdings ) {
				if ((stock.stockItem).getID() == id) {
					if (quantity > stock.quantity) {
						std::cerr << "Error: You own " << stock.quantity << " shares of " << stock.stockItem.getName() << "\n";
					} else {
						stock.quantity -= quantity;
						std::cout << "Stock sold \n";
						balance = balance + ((stock.stockItem).getPrice() * quantity);
						std::cout << "New Balance: " << balance << "\n";
					}
					return 0;
				}
			}
			std::cerr << "Error: The stock ID " << id << " does not exist";
			return 0;
		}
};

int main() {
	std::cout << std::fixed << std::setprecision(2);
	Port acc1(10000000.0, std::vector<PortItem>{});
	std::vector<Stock> market;
	market = initialiseStocks();
	int choice;
	do {
		std::cout << " //Welcome to Stock Sim!\\ \n";
		std::cout << "\n";
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

			case 2:
				acc1.buyStock(market);
				break;
			
			case 3:
				acc1.sellStock(market);
				break;
		}
	}
	while (choice != 7);

	return 0;
}