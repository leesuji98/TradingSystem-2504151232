#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "kiwer_api.cpp"
#include "nemo_api.cpp"

using std::string;
using std::vector;
using namespace testing;

class StockBrocker {
public:
	StockBrocker() {}
	virtual void loginSystem(std::string ID, std::string password) = 0;
	virtual void buyStock(std::string stockCode, int count, int price) = 0;
	virtual void sellStock(std::string stockCode, int count, int price) = 0;
	virtual int getStockPrice(std::string stockCode) = 0;
	virtual vector <int> getRecentPrices(string stockCode) = 0;
};

class MockDriver : public StockBrocker {
public:
	MOCK_METHOD(void, loginSystem, (std::string ID, std::string password), (override));
	MOCK_METHOD(void, buyStock, (std::string stockCode, int count, int price), (override));
	MOCK_METHOD(void, sellStock, (std::string stockCode, int count, int price), (override));
	MOCK_METHOD(int, getStockPrice, (std::string stockCode), (override));
	MOCK_METHOD(vector<int>, getRecentPrices, (string stockCode), (override));
};

class KiwerDriver : public StockBrocker {
public:


	void loginSystem(string id, string password) override {
		kiwer.login(id, password);
		return;
	}

	void buyStock(string stockCode, int price, int quantity) override {
		kiwer.buy(stockCode, price, quantity);
	}

	void sellStock(string stockCode, int price, int quantity) override {
		kiwer.sell(stockCode, price, quantity);
	}
	int getStockPrice(string stockCode) override {
		return kiwer.currentPrice(stockCode);
	}
	vector <int> getRecentPrices(string stockCode) override {
		//return kiwer.getRecentPrices(stockCode);
	}

	string getName() {
		return "KIWER";
	}

private:
	KiwerAPI kiwer;
};


class NemoDriver : public StockBrocker {
public:
	NemoDriver() {}
	void loginSystem(string id, string password) override {
		nemo.certification(id, password);
		return;
	}

	void buyStock(string stockCode, int price, int quantity) override {
		nemo.purchasingStock(stockCode, price, quantity);
	}

	void sellStock(string stockCode, int price, int quantity) override {
		nemo.sellingStock(stockCode, price, quantity);
	}
	int getStockPrice(string stockCode) override {

		return nemo.getMarketPrice(stockCode, 0);
	}

	string getName() {
		return "NEMO";
	}

	vector <int> getRecentPrices(string stockCode) override {
		//return nemo.getRecentPrices(stockCode);
	}


private:
	NemoAPI nemo;
};