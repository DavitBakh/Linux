#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include "bank.h"

using namespace std;

void PrintCommands()
{
  cout << "\n----------------------------------------" << endl;
  cout << "Welcome to our transparent bank!" << endl;
  cout << "----------------------------------------\n"
       << endl;

  cout << "There is avilable commands:" << endl;
  cout << "----------------------------------------" << endl;
  cout << "\t 1. Get Balance [id]" << endl;
  cout << "\t 2. Get Min Balance [id]" << endl;
  cout << "\t 3. Get Max Balance [id]" << endl;

  cout << "\t 4. Freeze [id]" << endl;
  cout << "\t 5. Defreeze [id]" << endl;

  cout << "\t 6. Transfer [from_id] [to_id] [sum]" << endl;
  cout << "\t 7. Write off from all [sum]" << endl;
  cout << "\t 8. Credit to all [sum]" << endl;

  cout << "\t 9. Set Min Balance [id] [sum]" << endl;
  cout << "\t10. Set Max Balance [id] [sum]" << endl;

  cout << "\t11. Print all accounts" << endl;
  cout << "\t 0. Exit" << endl;
  cout << "\t-1. Commands list" << endl;
  cout << "----------------------------------------\n"
       << endl;
}

int main(int argc, char **argv)
{

  int shm_fd = shm_open("/TBank", O_RDWR, 0666);
  struct stat sb;
  if (fstat(shm_fd, &sb) == -1)
  {
    perror("lstat");
    exit(EXIT_FAILURE);
  }

  int shm_size = sb.st_size;
  void *shm_ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  Bank *bank = (Bank *)shm_ptr;

  PrintCommands();
  cout << "Enter command: ";
  int command;
  cin >> command;

  while (command)
  {
    switch (command)
    {
    case -1:
      PrintCommands();
      break;
    case 1:
      int id;
      cin >> id;
      bank->printBalance(id);
      break;
    case 2:
      cin >> id;
      bank->printMinBalance(id);
      break;
    case 3:
      cin >> id;
      bank->printMaxBalance(id);
      break;

    case 4:
      cin >> id;

      if (bank->isFrozen(id))
      {
        cout << "Account: " << id << " is already frozen" << endl;
        break;
      }

      bank->froze_defroze(id);
      break;

    case 5:
      cin >> id;
      if (!bank->isFrozen(id))
      {
        cout << "Account: " << id << " is already defrozen" << endl;
        break;
      }
      bank->froze_defroze(id);
      break;

    case 6:
      int from_id, to_id, sum;
      cin >> from_id >> to_id >> sum;
      bank->transfer(from_id, to_id, sum);
      break;

    case 7:
      cin >> sum;
      bank->writeOffFromAll(sum);
      break;

    case 8:
      cin >> sum;
      bank->creditToAll(sum);
      break;

    case 9:
      cin >> id >> sum;
      bank->setMinBalance(id, sum);
      break;

    case 10:
      cin >> id >> sum;
      bank->setMaxBalance(id, sum);
      break;

    case 11:
      bank->print();
      break;

    default:
      cout << "Unknown command" << endl;
    }
    cout << "Enter command: ";
    cin >> command;
  }

  munmap(shm_ptr, shm_size);
  close(shm_fd);
}
