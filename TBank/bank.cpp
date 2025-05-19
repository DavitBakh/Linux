#include "includeAll.h"
#include "bank.h"

using namespace std;

Bank::Bank(int n) : size(n)
{
	sem_init(&sem, 1, 1);

	sem_wait(&sem);
	for (int ind = 0; ind < size; ind++)
	{
		bills[ind].currBalance = 0;
		bills[ind].minBalance = INT_MIN;
		bills[ind].maxBalance = INT_MAX;
		bills[ind].is_frozen = false;
		bills[ind].ID = ind;
	}
	sem_post(&sem);
}

string Bank::GetCommandsList()
{
	return commandsList;
}

string Bank::GetAll()
{
	string res = "";
	sem_wait(&sem);

	res += size + "\n";
	for (int i = 0; i < size; i++)
	{
		res += "ID: " + to_string(bills[i].ID) + "\n\t curr balance: "
			   + to_string(bills[i].currBalance) + "\n\t min balance: "
			   + to_string(bills[i].minBalance) + "\n\t max balance: "
			   + to_string(bills[i].maxBalance) + "\n\t is forzen: "
			   + to_string(bills[i].is_frozen) + "\n";
	}
	res += "----------------------------------------\n";
	sem_post(&sem);

	return res;
}

int Bank::GetBalance(int id)
{
	sem_wait(&sem);

	if (id < 0 || id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		sem_post(&sem);
		return INT_MIN;
	}

	int res = bills[id].currBalance;
	sem_post(&sem);
	return res;
}

int Bank::GetMinBalance(int id)
{
	sem_wait(&sem);

	if (id < 0 || id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		sem_post(&sem);
		return INT_MIN;
	}

	int res = bills[id].minBalance;
	sem_post(&sem);
	return res;
}

int Bank::GetMaxBalance(int id)
{
	sem_wait(&sem);

	if (id < 0 || id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		sem_post(&sem);
		return INT_MIN;
	}

	int res = bills[id].maxBalance;
	sem_post(&sem);
	return res;
}

bool Bank::froze_defroze(int id)
{
	sem_wait(&sem);

	if (id < 0 || id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		sem_post(&sem);
		return false;
	}
	bills[id].is_frozen = !bills[id].is_frozen;

	cout << "Account: " << id << " was ";
	if (bills[id].is_frozen)
		cout << "frozen" << endl;
	else
		cout << "defrozen" << endl;

	std::cout << "----------------------------------------" << std::endl;

	sem_post(&sem);
	return true;
}

bool Bank::isFrozen(int id)
{
	sem_wait(&sem);

	if (id < 0 || id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		sem_post(&sem);
		return false;
	}

	bool res = bills[id].is_frozen;
	sem_post(&sem);

	return res;
}

bool Bank::transfer(int from_id, int to_id, int sum)
{
	if (sum <= 0)
	{
		std::cout << "Sum can't be negative or zero" << std::endl;
		return false;
	}

	if (from_id == to_id)
	{
		std::cout << "Can't transfer money to the same account" << std::endl;
		return false;
	}

	sem_wait(&sem);
	if (from_id < 0 || from_id >= size || to_id < 0 || to_id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		sem_post(&sem);
		return false;
	}

	if (bills[from_id].is_frozen || bills[to_id].is_frozen)
	{
		std::cout << "One of the accounts is frozen" << std::endl;
		sem_post(&sem);
		return false;
	}

	if (bills[from_id].currBalance - sum < bills[from_id].minBalance)
	{
		std::cout << "Not enough money on the account" << std::endl;
		sem_post(&sem);
		return false;
	}

	if (bills[to_id].currBalance + sum > bills[to_id].maxBalance)
	{
		std::cout << "Too much money on the account" << std::endl;
		sem_post(&sem);
		return false;
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

	sem_post(&sem);
	return true;
}

bool Bank::creditToAll(int sum)
{
	sem_wait(&sem);

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

	sem_post(&sem);
	return true;
}

bool Bank::writeOffFromAll(int sum)
{
	sem_wait(&sem);

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

	sem_post(&sem);
	return true;
}

bool Bank::setMinBalance(int id, int sum)
{
	sem_wait(&sem);

	if (id < 0 || id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		sem_post(&sem);
		return false;
	}
	if (sum > bills[id].currBalance)
	{
		std::cout << "Min balance can't be more than current balance" << std::endl;
		sem_post(&sem);
		return false;
	}
	if (sum > bills[id].maxBalance)
	{
		std::cout << "Min balance can't be more than max balance" << std::endl;
		sem_post(&sem);
		return false;
	}

	bills[id].minBalance = sum;

	std::cout << "Min balance of account ID: " << id << " is set to " << sum << std::endl;
	std::cout << "----------------------------------------" << std::endl;

	sem_post(&sem);
	return true;
}

bool Bank::setMaxBalance(int id, int sum)
{
	sem_wait(&sem);

	if (id < 0 || id >= size)
	{
		std::cout << "ID is out of range" << std::endl;
		sem_post(&sem);
		return false;
	}
	if (sum < bills[id].currBalance)
	{
		std::cout << "Max balance can't be less than current balance" << std::endl;
		sem_post(&sem);
		return false;
	}

	if (sum < bills[id].minBalance)
	{
		std::cout << "Max balance can't be less than min balance" << std::endl;
		sem_post(&sem);
		return false;
	}

	bills[id].maxBalance = sum;

	std::cout << "Max balance of account ID: " << id << " is set to " << sum << std::endl;
	std::cout << "----------------------------------------" << std::endl;

	sem_post(&sem);
	return true;
}