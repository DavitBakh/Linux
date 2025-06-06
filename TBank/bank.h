#pragma once

#include "includeAll.h"
using namespace std;

class Bill
{
public:
  int currBalance;
  int minBalance;
  int maxBalance;
  bool is_frozen;
  int ID;
};

class Bank
{
public:
  char errorMessage[65] = "An error has occurred, please contact the bank's support service";
  char successMessage[8] = "Success";

private:
char commandsList[549] =
      "\n----------------------------------------\n"
      "Welcome to our transparent bank!\n"
      "----------------------------------------\n\n"

      "There is avilable commands:\n"
      "----------------------------------------\n"
      "\t 1. Get Balance [id]\n"
      "\t 2. Get Min Balance [id]\n"
      "\t 3. Get Max Balance [id]\n"
      "\t 4. Freeze [id]\n"
      "\t 5. Defreeze [id]\n"
      "\t 6. Transfer [from_id] [to_id] [sum]\n"
      "\t 7. Write off from all [sum]\n"
      "\t 8. Credit to all [sum]\n"
      "\t 9. Set Min Balance [id] [sum]\n"
      "\t10. Set Max Balance [id] [sum]\n"
      "\t11. Print all accounts\n"
      "\t 0. Exit\n"
      "\t-1. Commands list\n"
      "----------------------------------------\n\n";

  sem_t sem;
  int size;
  Bill bills[];

public:
  Bank(int n);
  string GetCommandsList();
  int GetBalance(int id);
  int GetMinBalance(int id);
  int GetMaxBalance(int id);

  bool froze_defroze(int id);
  bool isFrozen(int id);

  bool transfer(int from_id, int to_id, int sum);
  bool creditToAll(int sum);
  bool writeOffFromAll(int sum);

  bool setMinBalance(int id, int sum);
  bool setMaxBalance(int id, int sum);
  string GetAll();
};
