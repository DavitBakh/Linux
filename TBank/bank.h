#pragma once

#include <vector>

class Bill {
public:
  int cur_balance;
  int min_pos_balance;
  int max_pos_balance;
  bool is_frozen;
  int ID;
};


class Bank {
private:
  
  std::vector <Bill> bills;

  
public:
  Bank(int n);
  int print_balance(int id);
  void froze_defroze(int id);
  void transfer(int from_id, int to_id, int sum);
  void transfer_to_all(int sum);
  void set_balance(int id, int sum);
  void set_min_balance(int id, int sum);
  void set_max_balance(int id, int sum);
  void print();

};
