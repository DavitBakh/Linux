#pragma once

#include <vector>

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
private:
  int size;
  Bill bills[];

public:
  Bank(int n);
  void printBalance(int id);
  void printMinBalance(int id);
  void printMaxBalance(int id);

  void froze_defroze(int id);

  void transfer(int from_id, int to_id, int sum);
  void creditToAll(int sum);
  void writeOffFromAll(int sum);

  void setMinBalance(int id, int sum);
  void setMaxBalance(int id, int sum);
  void print();
};
