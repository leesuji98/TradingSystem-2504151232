#include <string>
#include <vector>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "kiwer_api.cpp"
#include "nemo_api.cpp"

using std::string;
using std::vector;
using namespace testing;

#define interface struct

interface StockBrocker {
public:
	StockBrocker() {}
	virtual bool login(std::string id, std::string password) = 0;
	virtual void buy(std::string stockCode, int price, int quantity) = 0;
	virtual void sell(std::string stockCode, int price, int quantity) = 0;
	virtual int getPrice(std::string stockCode) = 0;
};

class MockStockBroker : public StockBrocker {
public:
	MOCK_METHOD(bool, login, (string id, string password), (override));
	MOCK_METHOD(void, buy, (std::string stockCode, int price, int quantity), (override));
	MOCK_METHOD(void, sell, (std::string stockCode, int price, int quantity), (override));
	MOCK_METHOD(int, getPrice, (std::string stockCode), (override));

	StockBrocker selectStockBrocker(string name) {
		if (name == "KIWER") {
			return KiwerDriver();
		}
		else if (name == "NEMO") {
			return NemoDriver();
		}
		else {
			throw std::runtime_error("지원하지 않는 증권사 이름입니다: " + name);
		}
	}
};

class KiwerDriver : public StockBrocker {
public:
	KiwerDriver() {}
	bool login(string id, string password) {
		return kiwer.login(id, password);
	}

	void buy(string stockCode, int price, int quantity) {
		kiwer.buy(stockCode, price, quantity);
	}

	void sell(string stockCode, int price, int quantity) {
		kiwer.sell(stockCode, price, quantity);
	}
	int getPrice(string stockCode) {
		return kiwer.currentPrice(stockCode);
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
	bool login(string id, string password) {
		return nemo.certification(id, password);
	}

	void buy(string stockCode, int price, int quantity) {
		nemo.purchasingStock(stockCode, price, quantity);
	}

	void sell(string stockCode, int price, int quantity) {
		nemo.sellingStock(stockCode, price, quantity);
	}
	int getPrice(string stockCode) {
		return nemo.getMarketPrice(stockCode, 0);
	}

	string getName() {
		return "NEMO";
	}

private:
	NemoAPI nemo;
};

TEST(StockBrockerTest, ChoiceBrocker) {
	MockStockBroker mock;

	EXPECT_EQ("nemo", mock.selectStockBrocker("nemo"));
}

TEST(StockBrockerTest, LoginSuccess) {
	MockStockBroker mock;
	EXPECT_CALL(mock, login("user", "pass")).WillOnce(Return(true));

	EXPECT_TRUE(mock.login("user", "pass"));
}


int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
