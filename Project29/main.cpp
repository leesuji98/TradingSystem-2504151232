#include "gmock/gmock.h"

enum Stock
{
	Stock_Kiwer,
	Stock_Nemo,
};

int main()
{
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}

class StockBroker		
{
public:
	void SelectStockDriver(Stock eStock)
	{
		switch(eStock)
		{
		case Stock_Kiwer:
			pstDriver = KiwerDriver::GetInstance();
			break;
		case Stock_Nemo:
			pstDriver = NemoDriver::GetInstance();
			break;
		}
	}

	void buy()
	{
		pstDriver->buy();
	}
private:
	IDriver *pstDriver;
};

class IDriver
{
public:
	virtual bool login(std::string id, std::string password) = 0;
	virtual void buy() = 0;
    virtual void sell() = 0;
    virtual void selectStockBrocker(string name) = 0;
};

class NemoDriver : public IDriver
{
public:
	void buy()
	{
		stNemoApi.sellingStock();
	}
private:
	NemoAPI stNemoAPI;
};

class KiwerDriver : public IDriver
{
public:
	void buy()
	{
		stNemoApi.sellingStock();
	}
private:
	KiwerAPI stNemoAPI;
};