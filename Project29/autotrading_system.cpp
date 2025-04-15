
#include "stockbrocker.cpp"

using std::string;
using std::vector;
using namespace testing;

class AutoTradingSystem {
public:
	AutoTradingSystem(StockBrocker* stockBrocker) : stockBrocker{ stockBrocker } {}

	void selectStockBrocker(string name) {
		if (name == "kiwer") stockBrocker = new KiwerDriver();
		if (name == "nemo") stockBrocker = new NemoDriver();
		if (name == "mock") stockBrocker = new MockDriver();
		return;
	}

	bool buyNiceTiming(std::string stockCode, int price) {

		vector<int> prices = getRecentPrices(stockCode);

		if (prices[2] > prices[1] && prices[1] > prices[0]) {
			int currentPrice = getStockPrice(stockCode, 0);
			buyStock(stockCode, static_cast<int>((float)price / (float)currentPrice), currentPrice);
			return true;
		}

		return false;
	}

	bool sellNiceTiming(std::string stockCode, int count) {
		vector<int> prices = getRecentPrices(stockCode);

		if (prices[2] < prices[1] && prices[1] < prices[0]) {
			int currentPrice = getStockPrice(stockCode, 0);
			sellStock(stockCode, count, currentPrice);
			return true;
		}

		return false;
	}

	void loginSystem(std::string ID, std::string password) {
		stockBrocker->loginSystem(ID, password);
	}

	void buyStock(std::string stockCode, int count, int price) {
		stockBrocker->buyStock(stockCode, count, price);
	}

	void sellStock(std::string stockCode, int count, int price) {
		stockBrocker->sellStock(stockCode, count, price);
	}

	int getStockPrice(std::string stockCode, int minute) {
		return stockBrocker->getStockPrice(stockCode);
	}

	vector <int> getRecentPrices(string stockCode) {
		return stockBrocker->getRecentPrices(stockCode);
	}

private:
	StockBrocker* stockBrocker;
};
