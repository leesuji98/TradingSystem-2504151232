#include "gmock/gmock.h"

#include <string>
#include "kiwer_api.cpp"
#include "nemo_api.cpp"

using std::string;
using namespace testing;

#define interface struct

interface StockBrocker {
public:
	virtual void login(std::string id, std::string password) = 0;
	virtual void buy(string stockCode, int price, int count) = 0;
	virtual void sell(string stockCode, int price, int count) = 0;
};

class MockDriver : public StockBrocker {
public:
	MOCK_METHOD(void, login, (string id, string password), (override));
	MOCK_METHOD(void, buy, (string stockCode, int price, int count), (override));
	MOCK_METHOD(void, sell, (string stockCode, int price, int count), (override));
};

class KiwerDriver : public StockBrocker {
public:
	void login(string ID, string password) {
		api.login(ID, password);
	}
	void buy(string stockCode, int price, int count) override {
		api.buy(stockCode, count, price);
	}
	void sell(string stockCode, int price, int count) override {
		api.sell(stockCode, count, price);
	}
private:
	KiwerAPI api;
};

TEST(StockBrockerTest, LoginSuccess) {
	MockDriver mockDriver;
	StockBrocker* brocker = &mockDriver;

	EXPECT_CALL(mockDriver, login);

	brocker->login("KFCID", "KFCPW");
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
