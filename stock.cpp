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

		void setPrice(double p) {
			price = p;
		}
};
struct PortItem {
	Stock* stockItem;
	int quantity;

	PortItem(Stock* item, int q) 
		: stockItem(item), quantity(q) {}
};

class Port {
	private:
		double balance;
		vector <PortItem> holdings;

	public:
		Port(double bal, vector <PortItem> hold)
			: balance(bal), holdings(hold) {}
		
		int buyStock(vector<Stock>& market) {
			string id;
			int quantity;
			cout << "Enter the ID of the stock you wish to purchase: \n";
			 cin >> id;
			cout << "Enter the quantity of stock you wish to purchase: \n";
			cin >> quantity;
			for (auto& stock : market) {
				if (stock.getID() == id) { 
					double new_balance = balance - (stock.getPrice() * quantity);
					if (new_balance < 0) {
						cerr << "Insufficient funds \n";
					} else {
						balance = new_balance;
						holdings.emplace_back(&stock, quantity);
						cout << "Stock Bought \n";
						cout << "New Balance: " << balance << "\n";
					}
					return 0;
				}
			}
			cerr << "Error: Invalid stock ID";
			return 0;
		}

		int sellStock(vector<Stock>& market) {
			string id;
			int quantity;
			cout << "Enter the ID of stock you wish to sell \n";
			cin >> id;
			cout << "Enter the quantity of stock you wish to sell \n";
			cin >> quantity;

			for (auto& stock : holdings) {
				if ((stock.stockItem)->getID() == id) {
					if (quantity > stock.quantity) {
						cerr << "Error: You own " << stock.quantity << " shares of " << stock.stockItem->getName() << "\n";
					} else {
						stock.quantity -= quantity;
						cout << "Stock sold \n";
						balance = balance + ((stock.stockItem)->getPrice() * quantity);
						cout << "New Balance: " << balance << "\n";
					}
					return 0;
				}
			}
			cerr << "Error: The stock ID " << id << " does not exist";
			return 0;
		}
		double generateFlux() {
				random_device rd;
				mt19937 gen(rd());
				uniform_real_distribution<> dist(-0.05, 0.05);
				double rnum = dist(gen);
				return rnum;
		}
		int nextDay(vector<Stock>& market) {
			cout << "Fast forwarding to next day... \n \n";
			for (auto& stock : market) {
				double new_price = stock.getPrice() * (1 + generateFlux());
				cout << "New Price: " << new_price << "\n";
				stock.setPrice(new_price);
			}
			return 0;
		}

		void viewPort() {
			double total = 0.0;
			for (auto& stock : holdings) {
				cout << (stock.stockItem)->getID() << " | " << (stock.stockItem)->getName() << " | " << (stock.stockItem)->getPrice() << " | " << stock.quantity << " shares\n";
				total += (stock.stockItem)->getPrice() * stock.quantity;
			}
			cout << "Total Portfolio Value: " << total << "\n";
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
void checkExit() {
	string ex;
	do {
	cout << "\nEnter 'E' to exit to Main Menu \n";
	cin >> ex;
	transform(ex.begin(), ex.end(), ex.begin(), ::toupper);
	} while (ex != "E");
	cout << "\n";
}
int displayMarket(vector<Stock>& market) {
	for (auto& stock : market) {
		cout << stock.getID() << " | " << stock.getName() << " | " << stock.getPrice() << "\n";
	}
	checkExit();
	return 0;

}


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
				displayMarket(market);
				break;

			case 2:
				acc1.buyStock(market);
				break;
			
			case 3:
				acc1.sellStock(market);
				break;
			case 4:
				acc1.nextDay(market);
				break;

			case 5:
				acc1.viewPort();
				break;
		}
	}
	while (choice != 7);

	return 0;
}