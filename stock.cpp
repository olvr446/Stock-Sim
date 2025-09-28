#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <string> 
#include <vector>
#include <cstdlib>

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
		string id;
		double balance;
		vector <PortItem> holdings;

		void checkExit() {
			string ex;
			do {
				cout << "\nEnter 'E' to exit to Main Menu \n";
				cin >> ex;
				transform(ex.begin(), ex.end(), ex.begin(), ::toupper);
			} while (ex != "E");
			cout << "\n";
		}

		double generateFlux() {
			random_device rd;
			mt19937 gen(rd());
			uniform_real_distribution<> dist(-0.05, 0.05);
			double rnum = dist(gen);
			return rnum;
		}

	public:
		Port() : id(""), balance(0.0), holdings({}) {}
		Port(string id, double bal, vector <PortItem> hold)
			: id(id), balance(bal), holdings(hold) {}

		int displayMarket(vector<Stock>& market) {
			for (auto& stock : market) {
				cout << stock.getID() << " | " << stock.getName() << " | " << stock.getPrice() << "\n";
			}
			checkExit();
			return 0;
		}
		
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

		int saveExit() {
			bool uniqueID = true;
			string newID = "";
			fstream openFile("portfolios.txt");
				if (!openFile) {
					cerr << "Error, file cannot load \n";
					return 0;
				}
			do {
				uniqueID = true;
				openFile.clear();
				openFile.seekg(0, ios::beg);

				cout << "Create an account ID \n";
				cin >> newID;
				string line;
				while (getline(openFile, line)) {
					if (line.empty()) continue;
					stringstream ss(line);
					string accDetails;
					vector<string> acc;
					while (getline(ss, accDetails, '|')) {
						acc.push_back(accDetails);
					}
					if (acc[0] == newID) {
						cerr << "Error: ID already exists \n";
						uniqueID = false;
						break;
					}
				}
			} while (uniqueID != true);
			openFile.clear();
			openFile.seekp(0, ios::end);
			cout << "Saving & Exiting...";
			id = newID;
			string stockList = "";
			for (const auto& stock : holdings) {
				stockList += stock.stockItem->getID() + ":" + to_string(stock.quantity) + ",";
			}
			openFile << id << "|" << balance << "|" << stockList << "\n";
			openFile.close();
			exit(0);
		}
};

vector<Stock> initialiseStocks() {
	ifstream inputFile("market.txt");
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
			cerr << "Incorrectly formatted line, skipping to next line..." << line << endl;
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
	inputFile.close();
	return new_market;
}

void getAllAccountDetails(ifstream& file, vector<string>& accList, vector<vector<string>>& detailsList) {
	file.clear();
	file.seekg(0, ios::beg);
	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;
		stringstream ss(line);
		string info;
		vector<string> accountDetails;
		while (getline(ss, info, '|')) {
			accountDetails.push_back(info);
		}
		if (accountDetails.size() != 3) {
			cerr << "Incorrectly formatted line, skipping to next line... \n";
			continue;
		}
		detailsList.emplace_back(accountDetails);
		accList.push_back(accountDetails[0]);
	}
}

void loadAccounts(Port& account, vector<Stock>& market){
	bool validUsername = false;
	ifstream portFile("portfolios.txt");
	string line;
	while (getline(portFile, line)) {
		if (line.empty()) continue;
		cout << line << endl;
	}
	string acc = "";
	vector<vector<string>> accDetailsList;
	do {
		cout << "Enter the username of the account you wish to load: \n";
		cin >> acc;
		vector<string> accList;
		getAllAccountDetails(portFile, accList, accDetailsList);
		if (find(accList.begin(), accList.end(), acc) != accList.end()) {
			validUsername = true;
			cout << "Account Found... \n";
		} else {
			cout << "This username does not exist yet, please try again. \n";
		}
	} while (!validUsername);
	for (auto accDetails : accDetailsList) {
		if (acc == accDetails[0]) {
			vector<PortItem> stocks;
			stringstream ss(accDetails[2]);
			vector<vector<string>> stockInfo;
			string part;
			while (getline(ss, part, ',')) {
				stringstream stockSS(part);
				string stockID, stockQuant;
				getline(stockSS, stockID, ':');
				getline(stockSS ,stockQuant, ':');
				
				stockInfo.push_back({stockID, stockQuant});
			}
			for (auto& stock : market) {
				for (auto shares : stockInfo) {
					if (shares[0] == stock.getID()) {
						stocks.push_back(PortItem(&stock, stoi(shares[1])));
					}
				}

			}
			account = Port (accDetails[0], stod(accDetails[1]), stocks);

		}
	}
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

bool emptyFile(ifstream& iFile) {
	return iFile.peek() == fstream::traits_type::eof();
}

int main() {
	cout << fixed << setprecision(2);
	vector<Stock> market;
	market = initialiseStocks();
	Port acc;

	ifstream portFile("portfolios.txt");
	portFile.seekg(0, ios::beg);
	if (emptyFile(portFile) == true) {
		cout << "Creating new account... \n \n";
		acc =  Port("", 10000.0, vector<PortItem>{});
	} else {
		int newOrLoad;
		bool accLoaded = false;
		do {
			cout << "Select an Option: \n";
			cout << "1. Create New Account \n";
			cout << "2. Load An Existing Account";
			cin >> newOrLoad;

			switch (newOrLoad) {
				case 1: {
					cout << "Creating new account... \n \n";
					acc = Port ("", 10000.0, vector<PortItem>{});
					accLoaded = true;
					break;
				}

				case 2:
					loadAccounts(acc, market);
					accLoaded = true;
					break;
			}
		} while (accLoaded == false);
	}
	portFile.close();

	int choice;
	do {
		cout << " //Welcome to Stock Sim!\\ \n";
		cout << "\n";
		cout << "1. View Stock Market \n";
		cout << "2. Buy Stocks \n";
		cout << "3. Sell Stocks \n";
		cout << "4. Update Prices (Next Day) \n";
		cout << "5. View Portfolio \n";
		cout << "6. Save & Exit \n";
		cin >> choice;

		switch(choice){
			case 1: 
				acc.displayMarket(market);
				break;

			case 2:
				acc.buyStock(market);
				break;
			
			case 3:
				acc.sellStock(market);
				break;
			case 4:
				acc.nextDay(market);
				break;

			case 5:
				acc.viewPort();
				break;

			case 6:
				acc.saveExit();
				break;
		}
	}
	while (choice != 6);
	return 0;
}