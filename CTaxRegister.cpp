#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cmath>
#include <compare>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#endif /* __PROGTEST__ */

struct Citizen {
  std::string name;
  std::string addr;
  std::string account;
  int total_income = 0;
  int total_expense = 0;

  Citizen() = default;

  Citizen(const std::string &name, const std::string &addr,
          const std::string &account) {
    this->name = name;
    this->addr = addr;
    this->account = account;
  }
};

class CIterator {
 public:
  bool atEnd() const;
  void next();
  const std::string &name() const;
  const std::string &addr() const;
  const std::string &account() const;

  CIterator() = default;
  CIterator(const std::vector<Citizen> &_iterator, unsigned int start_index)
      : iterator(_iterator), index(start_index) {
    size = _iterator.size();
    index = 0;
  }

 private:
  const std::vector<Citizen> iterator;
  unsigned int index;
  unsigned int size;

  // Define an empty static string constant for handling situations when Iteraor
  // does not find a record. In this case we have to return empty string,
  // however, since method returns a ref. value, we cannot return an empty
  // string itself as it will be destroyed after function call;

  static const std::string EMPTY_STR;
};

const std::string CIterator::EMPTY_STR = "";

bool CIterator::atEnd() const {
  if (iterator.empty()) return true;
  if (index >= size) return true;
  return false;
}

void CIterator::next() {
  if (!atEnd()) index++;
  return;
}

const std::string &CIterator::name() const {
  if (index < iterator.size() && !iterator.empty()) return iterator[index].name;
  return EMPTY_STR;
}

const std::string &CIterator::addr() const {
  if (index < iterator.size() && !iterator.empty()) return iterator[index].addr;
  return EMPTY_STR;
}

const std::string &CIterator::account() const {
  if (index < iterator.size() && !iterator.empty())
    return iterator[index].account;
  return EMPTY_STR;
}

class CTaxRegister {
 public:
  bool birth(const std::string &name, const std::string &addr,
             const std::string &account);
  bool death(const std::string &name, const std::string &addr);
  bool income(const std::string &account, int amount);
  bool income(const std::string &name, const std::string &addr, int amount);
  bool expense(const std::string &account, int amount);
  bool expense(const std::string &name, const std::string &addr, int amount);
  bool audit(const std::string &name, const std::string &addr,
             std::string &account, int &sumIncome, int &sumExpense) const;
  CIterator listByName() const;

 private:
  std::vector<Citizen> Accounts;
  std::vector<Citizen> NameAddr;
};

CIterator CTaxRegister::listByName() const {
  if (NameAddr.empty()) {
    std::vector<Citizen> empty;
    CIterator empty_list(empty, 0);
    return empty_list;
  }
  CIterator listed(NameAddr, 0);
  return listed;
}

// Custom comparators for binary search algorithm

bool Find_Account_by_RefName(const Citizen &first, const Citizen &second) {
  if (first.name != second.name) return first.name < second.name;
  return first.addr < second.addr;
}

bool Find_Account_by_RefID(const Citizen &first, const Citizen &second) {
  return first.account < second.account;
}

bool CTaxRegister::birth(const std::string &name, const std::string &addr,
                         const std::string &account) {
  Citizen tmp(name, addr, account);
  auto iterator_Name = std::lower_bound(NameAddr.begin(), NameAddr.end(), tmp,
                                        Find_Account_by_RefName);

  if (iterator_Name != NameAddr.end() && iterator_Name->name == name &&
      iterator_Name->addr == addr)
    return false;  // Duplicate name+address detected

  NameAddr.insert(iterator_Name, tmp);

  auto iterator_Account = std::lower_bound(Accounts.begin(), Accounts.end(),
                                           tmp, Find_Account_by_RefID);

  // After adding in NameAddr, we shall look for same occurance in Accounts
  // If detected -> erase both, as we might assume that record was created
  // before

  if ((iterator_Account != Accounts.end() && iterator_Account->name == name &&
       iterator_Account->addr == addr) ||
      (iterator_Account != Accounts.end() &&
       iterator_Account->account == account)) {
    iterator_Name = std::lower_bound(NameAddr.begin(), NameAddr.end(), tmp,
                                     Find_Account_by_RefName);
    NameAddr.erase(iterator_Name);
    return false;
  }

  Accounts.insert(iterator_Account, tmp);

  return true;
}

bool CTaxRegister::death(const std::string &name, const std::string &addr) {
  Citizen tmp_by_Name(name, addr, "");

  if (NameAddr.empty()) return false;

  auto iterator_Name = std::lower_bound(NameAddr.begin(), NameAddr.end(),
                                        tmp_by_Name, Find_Account_by_RefName);
  if (iterator_Name == NameAddr.end()) return false;

  tmp_by_Name.account = iterator_Name->account;
  NameAddr.erase(iterator_Name);

  Citizen tmp_by_Account("", "", tmp_by_Name.account);
  auto iterator_Account = std::lower_bound(
      Accounts.begin(), Accounts.end(), tmp_by_Account, Find_Account_by_RefID);

  if (iterator_Account == Accounts.end()) return false;

  tmp_by_Account.name = iterator_Account->name;
  tmp_by_Account.addr = iterator_Account->addr;
  Accounts.erase(iterator_Account);

  return true;
}

bool CTaxRegister::income(const std::string &account, int amount) {
  if (Accounts.empty() || NameAddr.empty()) return false;

  Citizen tmp_by_Account("", "", account);

  auto iterator_Account = std::lower_bound(
      Accounts.begin(), Accounts.end(), tmp_by_Account, Find_Account_by_RefID);
  if (iterator_Account == Accounts.end() ||
      iterator_Account->account != account)
    return false;

  iterator_Account->total_income += amount;

  Citizen tmp_by_Name(iterator_Account->name, iterator_Account->addr, "");

  auto iterator_Name = std::lower_bound(NameAddr.begin(), NameAddr.end(),
                                        tmp_by_Name, Find_Account_by_RefName);
  if (iterator_Name == NameAddr.end() || iterator_Name->account != account)
    return false;

  iterator_Name->total_income += amount;

  return true;
}

bool CTaxRegister::income(const std::string &name, const std::string &addr,
                          int amount) {
  if (Accounts.empty() || NameAddr.empty()) return false;

  Citizen tmp_by_Name(name, addr, "");

  auto iterator_Name = std::lower_bound(NameAddr.begin(), NameAddr.end(),
                                        tmp_by_Name, Find_Account_by_RefName);
  if (iterator_Name == NameAddr.end() || iterator_Name->name != name ||
      iterator_Name->addr != addr)
    return false;

  iterator_Name->total_income += amount;

  Citizen tmp_by_Account("", "", iterator_Name->account);

  auto iterator_Account = std::lower_bound(
      Accounts.begin(), Accounts.end(), tmp_by_Account, Find_Account_by_RefID);
  if (iterator_Account == Accounts.end() || iterator_Name->name != name ||
      iterator_Name->addr != addr)
    return false;

  iterator_Account->total_income += amount;

  return true;
}

bool CTaxRegister::expense(const std::string &account, int amount) {
  if (Accounts.empty() || NameAddr.empty()) return false;

  Citizen tmp_by_Account("", "", account);

  auto iterator_Account = std::lower_bound(
      Accounts.begin(), Accounts.end(), tmp_by_Account, Find_Account_by_RefID);
  if (iterator_Account == Accounts.end() ||
      iterator_Account->account != account)
    return false;

  iterator_Account->total_expense += amount;

  Citizen tmp_by_Name(iterator_Account->name, iterator_Account->addr, "");

  auto iterator_Name = std::lower_bound(NameAddr.begin(), NameAddr.end(),
                                        tmp_by_Name, Find_Account_by_RefName);
  if (iterator_Name == NameAddr.end() || iterator_Name->account != account)
    return false;

  iterator_Name->total_expense += amount;

  return true;
}

bool CTaxRegister::expense(const std::string &name, const std::string &addr,
                           int amount) {
  if (Accounts.empty() || NameAddr.empty()) return false;

  Citizen tmp_by_Name(name, addr, "");

  auto iterator_Name = std::lower_bound(NameAddr.begin(), NameAddr.end(),
                                        tmp_by_Name, Find_Account_by_RefName);
  if (iterator_Name == NameAddr.end() || iterator_Name->name != name ||
      iterator_Name->addr != addr)
    return false;

  iterator_Name->total_expense += amount;

  Citizen tmp_by_Account("", "", iterator_Name->account);

  auto iterator_Account = std::lower_bound(
      Accounts.begin(), Accounts.end(), tmp_by_Account, Find_Account_by_RefID);
  if (iterator_Account == Accounts.end() || iterator_Account->name != name ||
      iterator_Account->addr != addr)
    return false;

  iterator_Account->total_expense += amount;

  return true;
}

// At this point, we don't really care to check both arrays as we are sure they
// are identical.

bool CTaxRegister::audit(const std::string &name, const std::string &addr,
                         std::string &account, int &sumIncome,
                         int &sumExpense) const {
  Citizen tmp(name, addr, "");

  if (NameAddr.empty()) return false;

  auto iterator = std::lower_bound(NameAddr.begin(), NameAddr.end(), tmp,
                                   Find_Account_by_RefName);
  if (iterator == NameAddr.end() || iterator->name != name ||
      iterator->addr != addr)
    return false;

  account = iterator->account;
  sumIncome = iterator->total_income;
  sumExpense = iterator->total_expense;

  return true;
}

#ifndef __PROGTEST__
int main() {
  std::string acct;
  int sumIncome, sumExpense;
  CTaxRegister b0;

  assert(b0.birth("John Smith", "Oak Road 23", "123/456/789"));
  assert(b0.birth("Jane Hacker", "Main Street 17", "Xuj5#94"));
  assert(b0.birth("Peter Hacker", "Main Street 17", "634oddT"));
  assert(b0.birth("John Smith", "Main Street 17", "Z343rwZ"));
  assert(b0.income("Xuj5#94", 1000));
  assert(b0.income("634oddT", 2000));
  assert(b0.income("123/456/789", 3000));
  assert(b0.income("634oddT", 4000));
  assert(b0.income("Peter Hacker", "Main Street 17", 2000));
  assert(b0.expense("Jane Hacker", "Main Street 17", 2000));
  assert(b0.expense("John Smith", "Main Street 17", 500));
  assert(b0.expense("Jane Hacker", "Main Street 17", 1000));
  assert(b0.expense("Xuj5#94", 1300));
  assert(b0.audit("John Smith", "Oak Road 23", acct, sumIncome, sumExpense));
  assert(acct == "123/456/789");
  assert(sumIncome == 3000);
  assert(sumExpense == 0);
  assert(
      b0.audit("Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense));
  assert(acct == "Xuj5#94");
  assert(sumIncome == 1000);
  assert(sumExpense == 4300);
  assert(
      b0.audit("Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense));
  assert(acct == "634oddT");
  assert(sumIncome == 8000);
  assert(sumExpense == 0);
  assert(b0.audit("John Smith", "Main Street 17", acct, sumIncome, sumExpense));
  assert(acct == "Z343rwZ");
  assert(sumIncome == 0);
  assert(sumExpense == 500);
  CIterator it = b0.listByName();
  assert(!it.atEnd() && it.name() == "Jane Hacker" &&
         it.addr() == "Main Street 17" && it.account() == "Xuj5#94");
  it.next();
  assert(!it.atEnd() && it.name() == "John Smith" &&
         it.addr() == "Main Street 17" && it.account() == "Z343rwZ");
  it.next();
  assert(!it.atEnd() && it.name() == "John Smith" &&
         it.addr() == "Oak Road 23" && it.account() == "123/456/789");
  it.next();
  assert(!it.atEnd() && it.name() == "Peter Hacker" &&
         it.addr() == "Main Street 17" && it.account() == "634oddT");
  it.next();
  assert(it.atEnd());

  assert(b0.death("John Smith", "Main Street 17"));

  CTaxRegister b1;
  assert(b1.birth("John Smith", "Oak Road 23", "123/456/789"));
  assert(b1.birth("Jane Hacker", "Main Street 17", "Xuj5#94"));
  assert(!b1.income("634oddT", 4000));
  assert(!b1.expense("John Smith", "Main Street 18", 500));
  assert(
      !b1.audit("John Nowak", "Second Street 23", acct, sumIncome, sumExpense));
  assert(!b1.death("Peter Nowak", "5-th Avenue"));
  assert(!b1.birth("Jane Hacker", "Main Street 17", "4et689A"));
  assert(!b1.birth("Joe Hacker", "Elm Street 23", "Xuj5#94"));
  assert(b1.death("Jane Hacker", "Main Street 17"));
  assert(b1.birth("Joe Hacker", "Elm Street 23", "Xuj5#94"));
  assert(b1.audit("Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense));
  assert(acct == "Xuj5#94");
  assert(sumIncome == 0);
  assert(sumExpense == 0);
  assert(!b1.birth("Joe Hacker", "Elm Street 23", "AAj5#94"));
  std::cout << "Assert complete." << std::endl;

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
