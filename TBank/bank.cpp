#include <iostream>
#include <unistd.h>
#include "bank.h"
#include <climits>

using namespace std;

Bank::Bank(int n) : size(n)
{
	sem_init(&sem, 1, 1);

	for (int ind = 0; ind < size; ind++)
	{
		bills[ind].currBalance = 0;
		bills[ind].minBalance = INT_MIN;
		bills[ind].maxBalance = INT_MAX;
		bills[ind].is_frozen = false;
		bills[ind].ID = ind;
	}
}

void Bank::print()
{
	std::cout << size << std::endl;
	for (int i = 0; i < size; i++)
	{
		std::cout << "ID: " << bills[i].ID << "\n\t curr balance: "
				  << bills[i].currBalance << "\n\t min balance: "
				  << bills[i].minBalance << "\n\t max balance: "
				  << bills[i].maxBalance << "\n\t is forzen: "
				  << bills[i].is_frozen << std::endl;
	}

	std::cout << "----------------------------------------" << std::endl;
}

void Bank::printBalance(int id)
{
	sem_wait(&sem);
	sleep(5);
	if (id < 0 || id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		return;
	}

	cout << "ID: " << id << " Balance: " << bills[id].currBalance << std::endl;
	std::cout << "----------------------------------------" << std::endl;

	sem_post(&sem);
}

void Bank::printMinBalance(int id)
{
	if (id < 0 || id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		return;
	}
	cout << "ID: " << id << " Min balance: " << bills[id].minBalance << std::endl;
	std::cout << "----------------------------------------" << std::endl;
}

void Bank::printMaxBalance(int id)
{
	if (id < 0 || id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		return;
	}
	cout << "ID: " << id << " Max balance: " << bills[id].maxBalance;
	std::cout << "----------------------------------------" << std::endl;
}

void Bank::froze_defroze(int id)
{
	if (id < 0 || id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		return;
	}
	bills[id].is_frozen = !bills[id].is_frozen;

	cout << "Account: " << id << " was ";
	if(bills[id].is_frozen)
		cout << "frozen" << endl;
	else
		cout << "defrozen" << endl;

	std::cout << "----------------------------------------" << std::endl;
}

bool Bank::isFrozen(int id)
{
	if (id < 0 || id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		return false;
	}
	return bills[id].is_frozen;
}

void Bank::transfer(int from_id, int to_id, int sum)
{
	if (sum <= 0)
	{
		std::cout << "Sum can't be negative or zero" << std::endl;
		return;
	}

	if (from_id == to_id)
	{
		std::cout << "Can't transfer money to the same account" << std::endl;
		return;
	}

	if (from_id < 0 || from_id >= size || to_id < 0 || to_id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		return;
	}

	if (bills[from_id].is_frozen || bills[to_id].is_frozen)
	{
		std::cout << "One of the accounts is frozen" << std::endl;
		return;
	}

	if (bills[from_id].currBalance - sum < bills[from_id].minBalance)
	{
		std::cout << "Not enough money on the account" << std::endl;
		return;
	}

	if (bills[to_id].currBalance + sum > bills[to_id].maxBalance)
	{
		std::cout << "Too much money on the account" << std::endl;
		return;
	}

	bills[from_id].currBalance -= sum;
	bills[to_id].currBalance += sum;

	std::cout << "Transfered " << sum << " from account ID: " << from_id
			  << " to account ID: " << to_id << std::endl;

	std::cout << "New balance of account ID: " << from_id << " is "
			  << bills[from_id].currBalance << std::endl;

	std::cout << "New balance of account ID: " << to_id << " is "
			  << bills[to_id].currBalance << std::endl;

	std::cout << "Transfer completed" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
}

void Bank::creditToAll(int sum)
{
	for (int i = 0; i < size; i++)
	{
		if (bills[i].is_frozen)
		{
			std::cout << "Accounts ID: " << i << " is frozen" << std::endl;
			continue;
		}
		if (bills[i].currBalance + sum > bills[i].maxBalance)
		{
			std::cout << "Too much money on the account ID: " << i << std::endl;
			continue;
		}

		bills[i].currBalance += sum;
	}

	std::cout << "Credited " << sum << " to all accounts" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
}

void Bank::writeOffFromAll(int sum)
{
	for (int i = 0; i < size; i++)
	{
		if (bills[i].is_frozen)
		{
			std::cout << "Accounts ID: " << i << " is frozen" << std::endl;
			continue;
		}
		if (bills[i].currBalance - sum > bills[i].minBalance)
		{
			std::cout << "Not enough money on the account ID: " << i << std::endl;
			continue;
		}

		bills[i].currBalance -= sum;
	}

	std::cout << "Wrote off " << sum << " from all accounts" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
}

void Bank::setMinBalance(int id, int sum)
{
	if (id < 0 || id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		return;
	}
	if (sum > bills[id].currBalance)
	{
		std::cout << "Min balance can't be more than current balance" << std::endl;
		return;
	}
	if (sum > bills[id].maxBalance)
	{
		std::cout << "Min balance can't be more than max balance" << std::endl;
		return;
	}
	
	bills[id].minBalance = sum;

	std::cout << "Min balance of account ID: " << id << " is set to " << sum << std::endl;
	std::cout << "----------------------------------------" << std::endl;
}

void Bank::setMaxBalance(int id, int sum)
{
	if (id < 0 || id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		return;
	}
	if (sum < bills[id].currBalance)
	{
		std::cout << "Max balance can't be less than current balance" << std::endl;
		return;
	}

	if (sum < bills[id].minBalance)
	{
		std::cout << "Max balance can't be less than min balance" << std::endl;
		return;
	}

	bills[id].maxBalance = sum;

	std::cout << "Max balance of account ID: " << id << " is set to " << sum << std::endl;
	std::cout << "----------------------------------------" << std::endl;
}