#include <iostream>
#include "bank.h"
#include <climits>

Bank::Bank(int n)
{
	bills.resize(n);
	for (int ind = 0; ind < bills.size(); ind++)
	{
		bills[ind].cur_balance = 0;
		bills[ind].min_pos_balance = INT_MIN;
		bills[ind].max_pos_balance = INT_MAX;
		bills[ind].is_frozen = false;
		bills[ind].ID = ind;
	}
}

void Bank::print()
{
	std::cout << bills.size() << std::endl;
	std::cout << bills.front().ID << std::endl;
	for (int i = 0; i < bills.size(); i++)
	{
		std::cout << "ID: " << bills[i].ID << "\n\t curr balance: "
				  << bills[i].cur_balance << "\n\t min balance: "
				  << bills[i].min_pos_balance << "\n\t max balance: "
				  << bills[i].max_pos_balance << "\n\t is forzen: "
				  << bills[i].is_frozen << std::endl;
	}
	std::cout << 2 << std::endl;
}
