#include <string>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using std::string;
using namespace testing;

class StockBrocker {
public:
	virtual bool login(std::string id, std::string password) = 0;
	virtual void selectStockBrocker(string name) = 0;
};

class MockStockBroker : public StockBrocker {
public:
	MockStockBroker() {}
	MOCK_METHOD(void, selectStockBrocker, (string name), (override));
	MOCK_METHOD(bool, login, (string id, string password), (override));
};

TEST(StockBrockerTest, ChoiceBrocker) {
	MockStockBroker mock;
//	EXPECT_CALL(mock, selectStockBrocker("K")).Times(1).WillOnce();

	mock.selectStockBrocker("K");
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
