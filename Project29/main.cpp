#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "kiwer_api.cpp"
#include "nemo_api.cpp"
#include "autotrading_system.cpp"

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

TEST(AutoTradingBotTest, SellCalledOnDownTrend2) {
	MockDriver mock;

	EXPECT_CALL(mock, getRecentPrices("XYZ"))
		.WillOnce(Return(vector<int>{1500, 1400, 1300}));
	EXPECT_CALL(mock, getStockPrice("XYZ")).WillOnce(Return(1300));
	EXPECT_CALL(mock, sellStock("XYZ", 2, 1300)).Times(1);

	AutoTradingSystem bot(&mock);
	EXPECT_TRUE(bot.sellNiceTiming("XYZ", 2));

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
