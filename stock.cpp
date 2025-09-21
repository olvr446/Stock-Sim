#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <string> 
#include <vector>

using namespace std;

class Stock {
	private:
		string stockID;
		string name;
		double price;
	public:
		Stock (string id, string name, double price)
			: stockID(id), name(name), price(price) {}

		string getID() const{
			return stockID;
		}

		string getName() const{
			return name;
		}
	
		double getPrice() const{
			return price;
		}
};

vector<Stock> initialiseStocks() {
	fstream inputFile("market.txt");
	vector<Stock> new_market;
	
	if (!inputFile) {
		cerr << "Error, file cannot load \n";
		return new_market;
	}

	string line;
	while (getline(inputFile, line)) {
		if (line.empty()) continue;
		stringstream ss(line);
		string stockAttr;
		vector<string> stockDetails;
		while (getline(ss, stockAttr, '|')) {
			stockDetails.push_back(stockAttr);
		}
		if (stockDetails.size() != 3) {
			cerr << "Incorrectly formatted line, skipping to next..." << line << endl;
			continue;
		}
		double price = 0.0;
        try {
            price = stod(stockDetails[2]);
        } catch (const exception& e) {
            cerr << "Invalid price in line: " << line << endl;
            continue;
        }
		new_market.emplace_back(stockDetails[0], stockDetails[1], price);
	}
	return new_market;
}

int displayMarket() {
	string ex;
	fstream inputFile("market.txt");

	if (!inputFile) {
		cerr << "Error, file cannot load \n";
		return 1;
	}

	string line;
	while (getline(inputFile, line)) {
		cout << line << endl;
	}

	inputFile.close();
	do {
	cout << "\nEnter 'E' to exit to Main Menu \n";
	cin >> ex;
	transform(ex.begin(), ex.end(), ex.begin(), ::toupper);
	} while (ex != "E");
	cout << "\n";
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
		vector <PortItem> holdings;

	public:
		Port(double bal, vector <PortItem> hold)
			: balance(bal), holdings(hold) {}
		
		int buyStock(vector<Stock> market) {
			string id;
			int quantity;
			cout << "Enter the ID of the stock you wish to purchase: \n";
			 cin >> id;
			cout << "Enter the quantity of stock you wish to purchase: \n";
			cin >> quantity;
			for (const auto& stock:market) {
				if (stock.getID() == id) { 
					double new_balance = balance - (stock.getPrice() * quantity);
					if (new_balance < 0) {
						cerr << "Insufficient funds \n";
					} else {
						balance = new_balance;
						holdings.emplace_back(stock, quantity);
						cout << "Stock Bought \n";
						cout << "New Balance: " << balance << "\n";
					}
					return 0;
				}
			}
			cerr << "Error: Invalid stock ID";
			return 0;
		}

		int sellStock(vector<Stock> market) {
			string id;
			int quantity;
			cout << "Enter the ID of stock you wish to sell \n";
			cin >> id;
			cout << "Enter the quantity of stock you wish to sell \n";
			cin >> quantity;

			for (auto& stock : holdings ) {
				if ((stock.stockItem).getID() == id) {
					if (quantity > stock.quantity) {
						cerr << "Error: You own " << stock.quantity << " shares of " << stock.stockItem.getName() << "\n";
					} else {
						stock.quantity -= quantity;
						cout << "Stock sold \n";
						balance = balance + ((stock.stockItem).getPrice() * quantity);
						cout << "New Balance: " << balance << "\n";
					}
					return 0;
				}
			}
			cerr << "Error: The stock ID " << id << " does not exist";
			return 0;
		}

		int nextDay() {


			return 0;
		}
};

int main() {
	cout << fixed << setprecision(2);
	Port acc1(10000000.0, vector<PortItem>{});
	vector<Stock> market;
	market = initialiseStocks();
	int choice;
	do {
		cout << " //Welcome to Stock Sim!\\ \n";
		cout << "\n";
		cout << "1. View Stock Market \n";
		cout << "2. Buy Stocks \n";
		cout << "3. Sell Stocks \n";
		cout << "4. Update Prices (Next Day) \n";
		cout << "5. View Portfolio \n";
		cout << "6. Load Portfolio \n";
		cout << "7. Save & Exit \n";
		cin >> choice;

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