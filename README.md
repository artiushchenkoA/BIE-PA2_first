# BIE-PA2_first
### First homework on BIE-PA2 course at FIT CTU

Your task is to implement class CTaxRegister, which implements a simple tax revenue database.

The database is very simplified. We assume the database holds information about all citizens from the moment they born until they die. Each citizen is identified by his/her account. The accounts are strings and are unique in the database. Second, citizens may be positively identified by their names and addresses. We assume the pair (name, address) is unique in the database. That is, there may exist many citizens with the same name (e.g. "John Smith"), there may be many citizens registered with the same address (e.g. "Main Street 10"), however, there may be at most one "John Smith" registered on "Main Street 10".

The class shall implement the following public interface:

A constructor (without parameters). This constructor initializes a new empty database.
Destructor -- it shall free all resources used by the instance.
Method birth (name, addr, acct ) which adds a new record to the database. The parameters are name, address and account id of the person being registered. The method returns true if it succeeds, or false if it fails (the pair name, addr is already present in the database, or the acct is not unique in the database).
Method death (name, addr) removes the corresponding record from the database, the record is identified by the name and address. The method returns true if it succeeds, or false if it fails (the corresponding record was not present).
Methods income add amount to the income of some account. There are two variants of the method: the account is either identified directly by account id, or the account is identified by the owner name and address. The methods return true if they succeed, or false if they fail (the corresponding person was not found).
Methods expense add amount to the expenses of some account. There are two variants of the method: the account is either identified directly by account id, or the account is identified by the owner name and address. The methods return true if they succeed, or false if they fail (the corresponding person was not found).
Method audit ( name, addr, acct, sumIncome, sumExpense) searches the database for the record identified by owner name and address. The method fills the acct, sumIncome, and sumExpense output parameters accordingly. The method returns true if it succeeds, or false if it fails (the corresponding person was not found). Moreover, if the method fails, the output parameters shall be left unmodified.
Method listByName creates and returns a new instance of class CIterator. The exact interface of the returned object is listed below, the object is used to access the records in the tax register. The records are visited in a sorted way, the order will be based on the person names (i.e., the person with the lexicographically smallest name is visited first). If the tax register contains two records with the same name, then the record with lexicographically smaller address goes first.
The public interface of CIterator class is the following:

Method atEnd returns a boolean true if the end of the register was reached (the iterator does not refer to any valid record) or boolean false if the iterator did not reach the end of the register (it refers to a valid record).
Method next which advances the iterator further.
Method name to retrieve the name of a person at the current iterator position.
Method addr to retrieve the address of a person at the current iterator position.
Method account to retrieve the account id of a person at the current iterator position.
The testing environment does not perform any changes to the register when the iterator created for the register is used. In particular, methods CTaxRegister::birth and CTaxRegister::death are not called.
Submit a source file with your implementation of classes CTaxRegister and CIterator. The class must follow the required public interface. If there is a mismatch in the interface, the compilation will fail. You may extend the interface and add you auxiliary methods and member variables (both public and private, although private are preferred). The submitted file must include both declarations as well as implementation of the class (the methods may be implemented inline but do not have to). The submitted file shall not contain anything except the required classes. In particular, if the file contains main, your tests, or any #include definitions, please, keep them in a conditional compile block. Use the attached template as a basis for your implementation. If the preprocessor definitions are preserved, the file may be submitted to Progtest.

The class is tested in a limited environment -- both memory and running time is limited. The available memory is big enough to store the records. The class does not have to implement a copy constructor or an overloaded operator =. This functionality is not tested in this homework.

Your implementation must be both time and space efficient. A simple linear-time solution will not succeed (it takes more than 5 minutes for the test data). You may assume Birth and Death calls are rare compared to the other methods. Thus, a binary searching in a sorted array with logarithmic time complexity is recommended. The add and delete operations may be implemented using binary searching (to find the index) and linear copying of the array elements.

Either STL container or a dynamic array allocation is required to implement the database. If implemented using the dynamically allocated array, set the initial size of the array to some reasonably small value (e.g. one hundred elements). When the array is full, do not increase the size by just one element. The overhead of the resizing would be enormous. Instead, increase the size by e.g. a thousand elements or use a quotient ranging from 1.5 to 2 (better solution).

If STL is used, your implementation does not have to care about the allocation. Caution: STL container map is disabled in this homework.

