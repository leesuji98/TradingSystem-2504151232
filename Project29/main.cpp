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
	vector <int> getRecentPrices(string stockCode) override{
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

	vector <int> getRecentPrices(string stockCode) override{
		//return nemo.getRecentPrices(stockCode);
	}


private:
	NemoAPI nemo;
};

class AutoTradingSystem {
public:
	AutoTradingSystem(StockBrocker* stockBrocker) : stockBrocker{ stockBrocker } {}

	void selectStockBrocker(string name) {
		if (name == "kiwer") stockBrocker = new KiwerDriver();
		if (name == "nemo") stockBrocker = new NemoDriver();
		if (name == "mock") stockBrocker = new MockDriver();
		return ;
	}

	bool buyNiceTiming(std::string stockCode, int price) {

		vector<int> prices = getRecentPrices(stockCode);

		if (prices[2] > prices[1] && prices[1] > prices[0]) {
			int currentPrice = getStockPrice(stockCode, 0);
			buyStock(stockCode, static_cast<int>( (float)price / (float)currentPrice), currentPrice);
			return true;
		}

		return false;
	}

	bool sellNiceTiming(std::string stockCode, int price) {
		vector<int> prices = getRecentPrices(stockCode);

		if (prices[2] < prices[1] && prices[1] < prices[0]) {
			int currentPrice = getStockPrice(stockCode, 0);
			sellStock(stockCode, 5, currentPrice);
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


TEST(AutoTradingBotTest, BuyCalledOnUpTrend) {
	MockDriver mock;

	EXPECT_CALL(mock, getRecentPrices("ABC"))
		.WillOnce(Return(vector<int>{1000, 1050, 1100}));
	EXPECT_CALL(mock, getStockPrice("ABC")).WillOnce(Return(1100));
	EXPECT_CALL(mock, buyStock("ABC", 9, 1100)).Times(1);

	AutoTradingSystem bot(&mock);
	EXPECT_TRUE(bot.buyNiceTiming("ABC", 10000));
}

TEST(AutoTradingBotTest, DoNotBuyOnDownTrend) {
	MockDriver mock;

	EXPECT_CALL(mock, getRecentPrices("ABC"))
		.WillOnce(Return(vector<int>{1100, 1050, 1000}));
	EXPECT_CALL(mock, buyStock).Times(0);

	AutoTradingSystem bot(&mock);
	EXPECT_FALSE(bot.buyNiceTiming("ABC", 10000));
}

TEST(AutoTradingBotTest, SellCalledOnDownTrend) {
	MockDriver mock;

	EXPECT_CALL(mock, getRecentPrices("XYZ"))
		.WillOnce(Return(vector<int>{1500, 1400, 1300}));
	EXPECT_CALL(mock, getStockPrice("XYZ")).WillOnce(Return(1300));
	EXPECT_CALL(mock, sellStock("XYZ", 5, 1300)).Times(1);

	AutoTradingSystem bot(&mock);
	EXPECT_TRUE(bot.sellNiceTiming("XYZ", 5));

}

TEST(AutoTradingBotTest, DoNotSellOnUpTrend) {
	MockDriver mock;

	EXPECT_CALL(mock, getRecentPrices("XYZ"))
		.WillOnce(Return(vector<int>{1300, 1400, 1500}));
	EXPECT_CALL(mock, sellStock).Times(0);

	AutoTradingSystem bot(&mock);
	EXPECT_FALSE(bot.sellNiceTiming("XYZ", 5));
}



int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
