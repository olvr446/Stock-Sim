#include <iostream>
#include <fstream>
#include <sstream>
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
            continue;  // skip invalid price
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
	std::cout << "\nEnter 'E' to exit to Main Menu\n";
	std::cin >> ex;
	std::transform(ex.begin(), ex.end(), ex.begin(), ::toupper);
	} while (ex != "E");
	std::cout << "\n";
	return 0;

}

struct PortItem {
	std::string name;
	int quantity;

	PortItem(const std::string& n, int q) 
		: name(n), quantity(q) {}
};

class Port {
	private:
		double balance;
		std::vector <PortItem> holdings;

	public:
		Port(double bal, std::vector <PortItem> hold)
			: balance(bal), holdings(hold) {}
		
		int buyStock(std::vector<Stock> market, std::string stockID, int quantity) {
			for (const auto& stock: market) {
				if (stock.getID() == stockID) {
					holdings.emplace_back(stock.getName(), quantity);
					// balance -= (stock.getPrice() * quantity);
					std::cout << "Stock Bought /n";
					// Add balance checks
					return 0;
				}
			}
			return 0;
		}
};

int main() {
	Port acc1(1000000, std::vector<PortItem>{});
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
				std::string id;
				int amount;
				std::cout << "Enter the ID of the stock you wish to purchase: ";
				std:: cin >> id;
				std::cout << "Enter the amount of stock you wish to purchase: ";
				std::cin >> amount;	
				acc1.buyStock(market, id, amount);
		}
	}
	while (choice != 7);

	return 0;
}