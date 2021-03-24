//**************************program indentification***************
//*                                                              *
//* Program AUTHOR: Michael Nunes                                *
//*                                                              *
//* course #: CSC 30500 21                                       *
//*                                                              *
//* due date: 11/25/2019                                         *
//****************************************************************

//****************************PROGRAM DESCRIPTION*********************************
//*                                                                              *
//* Process: The user inputs two variables and with these varibles the code will *
//* exicute the desried task. The first letter read from the user can be  a,l,f, *
//* d,s and q.a is adding, l is listing, f is Find, d is delete and q is quit    *
//* S is summarize.                                                              *
//* Some of them has a sub category like a = c,m,d. l = c,d. F= m,z. D= c,d      *
//* The second variable read from the user will determine what the code will do  *
//*                                                                              *
//*                                                                              *
//*                                                                              *
//*                                                                              *
//* USER DEFINED                                                                 *
//* FUNCTIONS: main() - create the tables and go to choose function              *  
//*            choose() - to choose what the user wants to do                    *
//*            creatingcar() -if the second variable is c add a car              *
//*   creatingDealer - if the second variable is d add a dealer                  *
//*   creatinmanu() -  if the second variable is m add a manufacturer            *
//*   listingcar() - if the second variable is c list all cars                   *
//*   listingDealer() - if the second variable is d list all dealers             *
//*   findmanufacturer() - if the second variable is m find all cars with the    *
//*   same manufacturer                                                          *
//*   findZicode() - if second variable is z find all cars with the              *
//*   same zip                                                                   *
//*   deletecar() - this function delete car from Carstables where the vin match *
//*   summarize() -this function summarize print out one line for each           *
//*   manufacturer that includes the average price of the cars made by           *
//*   that manufacturer                                                          *
//*   deletedealer() -this function delete car where dealershipdelete match and  *
//*   delete dealer where dealerName match                                       *
//*   quitProgram() - function to quit program                                   *
//********************************************************************************

/* Standard C++ includes */
//#include <cstdlib>
#include <iostream>
#include <iomanip> // include the format ouput

/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <conio.h>


using namespace std; // it is the standard namespace. cout,cin and a lot of things are defined in it 



void creatingcar();// this function add cars to the CarsTables.Also, this function avoid duplicate vin and if the manufacturer and the dealer name
// is not created yet, we cannot add the car to the CarsTables.Also, insert the values to the CarsTables
void creatingDealer(); //this function add dealers to the DealerTables. Also, this function reads the DealerTable and store the DealerName
// so we can compare dealername with dealernameDoble to avoid duplicate data
void creatingManu(); // this function read in two string from the user one of them is the abbreviation for a manufacturer and 
//the another one is for actual name of the manufacturerand store it in manuTables.
// Also, this function reads the manuTablesand store the abbvmanu for every line
// so we can compare abbvdoble with abbvmanu to avoid duplicate data
void listingcar(); // this function real all the information which is in Carstables and print all the information to the user
void listingDealer(); //this function real all the information which is in DealerTables and print all the information to the user
void choose(); // this function print >> > and read the firstand second letter from the inputand call functions
void findmanufacturer(); // read in from the user strings and then open the views from every table, store and compare things from them. Also, print it
void findZicode(); // read in from the user strings and then open the views from every table, store and compare things from them. Also, print it
void quitProgram(); // function to quit program 
void summarize(); //this function summarize print out one line for each manufacturer that includes the average price of the cars made by that manufacturer
void deletedealer(); //this function delete car where dealershipdelete match and delete dealer where dealerName match 
void deletecar(); //this function delete car from Carstables where the vin match 



sql::Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::ResultSet* res;
sql::Statement* stmt1;
sql::ResultSet* res1;
sql::Statement* stmt2;
sql::ResultSet* res2;

// reads in a password without echoing it to the screen
//    WARNING: Does not work in VS2015. 
string myget_passwd()
{
	string passwd;

	for (;;)
	{
		char ch;
		ch = _getch();           // get char WITHIOUT echo!
		if (ch == 13 || ch == 10) // if done ...
			break;           //  stop reading chars!
		cout << '*';  // dump * to screen
		passwd += ch;   // addd char to password
	}
	cin.sync(); // flush anything else in the buffer (remaining newline)
	string dummy;
	getline(cin, dummy);
	cout << endl;  // simulate the enter that the user pressed

	return passwd;
}



int main(void)
{
	// database credentials
	string db_host, db_user, db_password, db_name;

	// get database credentials. 
	cout << "Enter hostname:";
	cin >> db_host;
	cout << "Enter username:";
	cin >> db_user;
	cout << "Enter password:";
	db_password = myget_passwd();


	db_name = db_user;



	try {


		// Create a connection */
		driver = get_driver_instance();
		con = driver->connect(db_host, db_user, db_password);

		// choose database
		con->setSchema(db_name);

		if (con == NULL)
		{
			cout << "Connection Failed!!" << endl;
			return 1;
		}
		// creating the manuTables if not exists with the abbvmanu character 3 and ACTmanu character 10 and the primary key is abbvmanu
		string myQuery = "create table if not exists manuTables ";
		myQuery += "(ABBVmanu char(3), ACTmanu char(10), ";
		myQuery += "primary key (ABBVmanu) )";
		//create statement and execute 
		stmt = con->createStatement();
		stmt->execute(myQuery);
		delete stmt; // delete statement 
		// creating the DealerTables if not exists with the dealer name character 30, zipcode int and phonenumber with char 10 and the primary key is DealerName
		myQuery = "create table if not exists DealerTables ";
		myQuery += "(DealerName char(30), Zipcode int, Phonenumber char(10), ";
		myQuery += "primary key (DealerName) )";
		//create statement and execute 
		stmt = con->createStatement();
		stmt->execute(myQuery);
		delete stmt; // delete statement 
		// creating CarsTables if not exists with VIN char(17), miles int, dealership char(20), price int and ABBVVIN char(3) and the primary key is VIN with a foreign key ABBVVin references to manuTables ABBVmanu
		myQuery = "create table if not exists CarsTables ";
		myQuery += "(VIN char(17), Miles int, Dealership char(20), Price int, ABBVVin char(3), ";
		myQuery += "primary key (VIN), foreign key (ABBVVin) references manuTables (ABBVmanu) )";
		//create statement and execute 
		stmt = con->createStatement();
		stmt->execute(myQuery);
		delete stmt; // delete statement 

		choose(); // return to the choose function 

	}
	catch (sql::SQLException & e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	cout << endl;

	return 0; // return 0
}

/*
 * this function print >>> and read the first and second letter from the input and call functions
 */
void choose()
{
	string principalLetter; // creating a string variable for the principal letter
	string secondLetter;   // creating a string variable for the second letter

	cout << ">>>";        // print to the user >>>
	cin >> principalLetter; // read from the input the first letter and store in the principal letter variable

	if (principalLetter == "a") // check is the first letter is equal to a
	{
		cin >> secondLetter;     // read from the input the second letter and store in the secondLetter variable
		if (secondLetter == "c") // check if the second letter is equal to c
			creatingcar();       //call the creatingcar function and do it
		if (secondLetter == "m") // check if the second letter is equal to m
			creatingManu();     //call the creatingManu function and do it
		if (secondLetter == "d") // check if the second letter is equal to d
			creatingDealer();   //call the creatingDealer function and do it
		
	}
	if (principalLetter == "l") // check if the first letter is equal to l
	{
		cin >> secondLetter;     // read from the input the second letter and store in the secondLetter variable
		if (secondLetter == "d") // check if the second letter is equal to d
			listingDealer();     // call the ListingDealer function and do it
		if (secondLetter == "c") // check if the second letter is equal to c
			listingcar();        //call the listingcar function and do it
		 
	}
	if (principalLetter == "f")  // check if the first letter is equal to f
	{
		cin >> secondLetter;     // read from the input the second letter and store in the secondLetter variable
		if (secondLetter == "m") // check if the second letter is equal to m
			findmanufacturer();  //call the findmanufacturer function and do it
		if (secondLetter == "z") // check if the second letter is equal to z
			findZicode();        //call the findZicode function and do it
	}
	if (principalLetter == "d") // check if the first letter is equal to d
	{
		cin >> secondLetter;   // read from the input the second letter and store in the secondLetter variable 
		if (secondLetter == "c") // check if the secondLetter is equal to c 
			deletecar();         // call the deletecar function and do it 
		if (secondLetter == "d") // check if the secondLetter is equal to d 
			deletedealer();      // call the deletedealer function and do it 
	}
	if (principalLetter == "s") // check if the first letter is equal to s
	{
		summarize(); // call the summarize function and do it 
	}
	if (principalLetter == "q") // check if the first letter is equal to q
	{
		quitProgram(); // call the quitProgram function and do it 

	}
}

/*
* this function add cars to the CarsTables. Also, this function avoid duplicate vin and if the manufacturer and the dealer name 
* is not created yet, we cannot add the car to the CarsTables. Also, insert the values to the CarsTables
*/
void creatingcar()
{
	string vinCar;     // creating a string variable for the vin of the car
	string milesCar;   // creating a string variable for the miles of the car
	string dealerCar; // creating a string variable for the dealer of the car
	string priceCar;  // creating a string variable for the price of the car
	string abbvvin;   // creating a string variable for the abbreviation of the vin 
	cin >> vinCar;    // read in the string variable from the user and store it in vincar 
	cin >> milesCar;  // read in the string variable from the user and store it in milesCars
	cin >> dealerCar; // read in the string variable from the user and store it in dealerCars
	cin >> priceCar; // read in the string variable from the user and store it in priceCar
	abbvvin = vinCar.substr(0, 3);  //create a substring to get the first 3 letters of vinCar and store in in abbvvin

	bool a = true; // create a boolean a to check if manufacturer and dealer name exist
	string notdealer;  // creating a string variable for not dealer created yet
	string notvinCar;  // creating a string variable for not vinCar created yet
	string vinCardoble; // creating a string variable for the vin of the cardoble to check for duplicate things

	string myQuery1 = "select * from CarsTables"; // create query to select everything from CarsTables
	//create statement, view and execute the query 
	stmt2 = con->createStatement();
	res2 = stmt2->executeQuery(myQuery1);

	while (res2->next()) { //from the view of the CarsTables read everything until there is not more information
		vinCardoble = res2->getString("VIN");  // get the string VIN from CarsTables and store it in vinCardoble 

		if (vinCardoble == vinCar) // check if vinCardoble is equal to vinCar
		{
			cout << "Vin already exist" << endl; // print to the user Vin already exist
			a = false; // make the boolean a to false
			choose(); // return to the function choose
		}
	}
	string myQuery = "select * from manuTables"; // create query to select everything from manuTables
	//create statement, view and execute the query 
	stmt = con->createStatement();
	res = stmt->executeQuery(myQuery);

	while (res->next()) {//from the view of the manuTables read everything until there is not more information
		notvinCar = res->getString("ABBVmanu"); // get the string ABBVmanu from manutables and store it in notvinCar 
		string myQuery = "select * from DealerTables"; // create query to select everything from DealerTables
		//create statement, view and execute the query 
		stmt1 = con->createStatement();
		res1 = stmt1->executeQuery(myQuery);
		while (res1->next()) //from the view of the DealerTables read everything until there is not more information
		{
			notdealer = res1->getString("DealerName"); // get the string DealerName from DealerTables and store it in notdealer
			// check if the first three letter notvincar and vinCar are equal and notdealer is equal to dealerCar
				if (notdealer == dealerCar && notvinCar.at(0) == vinCar.at(0) && notvinCar.at(1) == vinCar.at(1) && notvinCar.at(2) == vinCar.at(2))
				{// create query to inset into Carstables values vinCars, milescar,dealercar, pricecar and abbvvin
					string myQuery = "insert into CarsTables values (\"";
					myQuery += vinCar;
					myQuery += "\", \"";
					myQuery += milesCar;
					myQuery += "\", \"";
					myQuery += dealerCar;
					myQuery += "\", ";
					myQuery += priceCar;
					myQuery += ",\"";
					myQuery += abbvvin;
					myQuery += "\");";
					//create statement and execute the query 
					stmt = con->createStatement();
					stmt->execute(myQuery);
					delete stmt; // delete statement 
					a = false; // make the boolean a to false 
					choose(); // return to the choose function 
				}
				
			}
	}
	if (a) { 
		cout << "Invalid entry" << endl; // print to the user invalid entry
		choose(); // return to the choose function
	}
}

/*
* this function add dealers to the DealerTables. Also, this function reads the DealerTable and store the DealerName
* so we can compare dealername with dealernameDoble to avoid duplicate data
*/
void creatingDealer()
{
	string dealername; // creating a string variable for the dealername of the Dealer
	string zipcode; // creating a string variable for the zipcode of the dealer
	string phonenumber; // creating a string variable for the phonenumber of the dealer
	cin >> dealername; // read in the string variable from the user and store it in dealername
	cin >> zipcode; // read in the string variable from the user and store it in zipcode
	cin >> phonenumber; // read in the string variable from the user and store it in phonenumber

	int control = 0; // int control is set to 0, this will work like a boolean for checking if dealer name exist
	string dealernamedoble; // creating a string variable from the doble of dealername of the dealerTables

	string myQuery = "select * from DealerTables"; // create query to select everything from DealerTables
	//create statement, view and execute the query 
	stmt = con->createStatement();
	res = stmt->executeQuery(myQuery);

	while (res->next()) { //from the view of the DealerTables read everything until there is not more information
		dealernamedoble = res->getString("DealerName"); // get the string dealername from dealerTables and store it in dealernamedoble
		if (dealernamedoble == dealername) // check if the dealernamedoble is equal to dealername
		{
			cout << "Dealer name already exist" << endl; // print to the user Dealer name already exist
			control = 1; // make the int control to 1
			choose(); // return to the choose function 
		}

	}
	//if not dealername exist insert into DealerTables values the dealername, zipcode and phonenumer for the dealerTables
	if (control==0) { // check if control is equal to 0
		string myQuery = "insert into DealerTables values (\"";
		myQuery += dealername;
		myQuery += "\", \"";
		myQuery += zipcode;
		myQuery += "\", \"";
		myQuery += phonenumber;
		myQuery += "\")";

		//create statement and execute the query
		stmt = con->createStatement();
		stmt->execute(myQuery);
		delete stmt; // delete statement 
		choose(); // return to the choose function 
	}
	delete res;   // delete res
	delete stmt; //delete statement 
}

/*
* this function read in two string from the user one of them is the abbreviation for a manufacturer and 
* the another one is for actual name of the manufacturer and store it in manuTables.
* Also, this function reads the manuTables and store the abbvmanu for every line 
* so we can compare abbvdoble with abbvmanu to avoid duplicate data
*/
void creatingManu()
{
	string abbvmanu;  // creating a string variable for the abbreviation for a manufacturer
	string actmanu;   // creating a string variable for the actual name of the manufacturer
	cin >> abbvmanu; // read in the string variable from the user and store it in abbvmanu
	cin >> actmanu;  // read in the string variable from the user and store it in actmanu

	int control = 0; // int control is set to 0, this will work like a boolean for checking if manufacturer exist

	string ABBVdouble; // creating a string variable from the doble abbreviation for a manufacturer
	// create query to select everything from manuTables
	string myQuery = "select * from manuTables";
	//create statement, view and execute the query 
	stmt = con->createStatement();
	res = stmt->executeQuery(myQuery);

	while (res->next()) {  // from the view of the manuTables read everything until there is nothing left 
		ABBVdouble = res->getString("ABBVmanu"); // get the string ABBVmanu from manutables and store it in abbvdoble 
		if (ABBVdouble == abbvmanu) // check if ABBVdouble is equal to abbvmanu
		{
			cout << "Manufacturer already exist" << endl; // print to the user manufacturer already exist
			control = 1; // make the int control to 1
			choose(); // return to the choose function 
		}

	}
	//if not manufacturer exist insert into manuTables values the abbreviation for a manufacturer and actual name of the manufacturer
	if (control==0) {// check if control is equal to 0 
		string myQuery = "insert into manuTables values (\"";
		myQuery += abbvmanu;
		myQuery += "\", \"";
		myQuery += actmanu;
		myQuery += "\")";

		//create statement and execute the query
		stmt = con->createStatement();
		stmt->execute(myQuery); 
		delete stmt; //delete statement 
		choose();   // return to the choose function 
	}
	delete res; // delete res
	delete stmt; // delete statement
}
/*
* this function real all the information which is in Carstables and print all the information to the user
*/

void listingcar()
{
	// create query to select everything from Carstable and the result order by VIN 
	string myQuery = "select * from CarsTables order by VIN";
	//create statement, view and execute the query 
	stmt = con->createStatement();
	res = stmt->executeQuery(myQuery);

	// get a view of the CarsTables and get the first string (VIN), second string (miles), third (dealership) and fourth string (price) until there is not value in the table
    // and print them out with a with of 20 from the left and order by VIN
	while (res->next()) {
		cout << left << setw(20) << res->getString(1) << " ";
		cout << res->getString(2) << " ";
		cout << res->getString(3) << " ";
		cout << res->getString(4) << endl;
	}

	delete res; // delete res 
	delete stmt; //delete statement 
	choose(); // return to the choose function 
}
/*
* this function real all the information which is in DealerTables and print all the information to the user
*/

void listingDealer()
{
	// create query to select everything from DealerTables and the result order by zipcode and Dealername
	string myQuery = "select * from DealerTables order by Zipcode, DealerName";
	//create statement, view and execute the query 
	stmt = con->createStatement();
	res = stmt->executeQuery(myQuery);

	// get a view of the dealerTables and get the first (dealername), second (zipcode) and third(phonenumber) until there is not value in the table
	// and print them out with a with of 20 from the left and order by zipcode and Dealername
	while (res->next()) { 
		cout << left << setw(20) << res->getString(1) << " ";
		cout << res->getString(2) << " ";
		cout << res->getString(3) << endl;
	}

	delete res; // delete res
	delete stmt; // delete statement 
	choose(); // return to the choose function 
}

/*
* read in from the user strings and then open the views from every table, store and compare things from them. Also, print it
*/

void findmanufacturer()
{
	string namemanufacturerFind; // creating a string variable for the name of the manufacturer
	cin >> namemanufacturerFind; // read in from the user of the name of the manufacturer
	// create query to select everything from manuTables
	string myQuery = "select * from manuTables";
	string abbvmanufacturerFind; //creating a string varible for the abbreviation of the manufacturer
	string actualnameFind; // creating a string variable for the actual name of the manufacturer
	//create statement, view and execute the query 
	stmt = con->createStatement();
	res = stmt->executeQuery(myQuery);

	while (res->next()) { // from the view table read in every string and store it until there is nothing in the table 
		abbvmanufacturerFind = res->getString("ABBVmanu"); // get string ABBVmanu from the manuTables and store it in abbvmanufacturer find
		actualnameFind = res->getString("ACTmanu"); // get string ACTmanu from the manuTables and store it in actualnamefind
		if (namemanufacturerFind == actualnameFind) { // check if namemanufacturerfind is equal to actualnamefind
			//creating query to select * from CarsTables and order by Price (high to low), miles (low to high) and then dealership
			string myQuery = "select * from CarsTables order by Price DESC, Miles ASC, Dealership";
			string VINFind; // creating a string variable for the vin 
			string milesfind; // creating a string variable for the miles
			string dealershipFind; // creating a string variable for the dealership 
			string priceFind; // creating a string variable for the price
			//create statement, view and execute the query 
			stmt1 = con->createStatement();
			res1 = stmt1->executeQuery(myQuery);

			while (res1->next()) { // from the view table read in every string and store it until there is nothing in the table 
				VINFind = res1->getString("VIN"); // get string VIN from the CarsTables and store it in VINFIND
				milesfind = res1->getString("Miles"); // get string Miles from the CarsTables and store it in milesfind
				dealershipFind = res1->getString("Dealership"); // get string dealership from the CarsTables and store it in dealershipFind
				priceFind = res1->getString("Price"); // get string price from the CarsTables and store it in priceFind
				// check if the first three letters of the VIN and abbreviation are equals
				if (abbvmanufacturerFind.at(0) == VINFind.at(0) && abbvmanufacturerFind.at(1) == VINFind.at(1) && abbvmanufacturerFind.at(2) == VINFind.at(2)) {
					string myQuery = "select * from DealerTables"; // creating query to select * from DealerTables
					//create statement, view and execute the query 
					stmt2 = con->createStatement();
					res2 = stmt2->executeQuery(myQuery);
					string dealernameFind;  // creating a string variable for the dealernameFind
					string zipcodeFind; // creating a string variable for the zipcode
					string phonenumberFind;  // creating a string variable for the phonenumber
					while (res2->next()) { // from the view table read in every string and store it until there is nothing in the table 
						dealernameFind = res2->getString("DealerName"); // get string DealerName from the DealerTables and store it in dealernameFind
						zipcodeFind = res2->getString("Zipcode"); // get string zipcode from the DealerTables and store it in zipcodeFind
						phonenumberFind = res2->getString("Phonenumber"); // get string phonenumber from the DealerTables and store it in PhonenumberFind
						if (dealershipFind == dealernameFind) { //check if dealership Find is equal to dealernamefind
							cout << actualnameFind << ":" << milesfind << " miles, $" << priceFind << ": " << dealernameFind << "[("
								<< phonenumberFind.at(0) << phonenumberFind.at(1) << phonenumberFind.at(2) << ")" << phonenumberFind.at(3) << phonenumberFind.at(4) << phonenumberFind.at(5)
								<< "-" << phonenumberFind.at(6) << phonenumberFind.at(7) << phonenumberFind.at(8) << phonenumberFind.at(9) << "]" << endl; // print to the user the actualnamefind,miles,price dealername and phonenumber
						}
					}
				}
			}
		}

	}
	choose(); // return to the choose function 
}

/*
* read in from the user strings and then open the views from every table, store and compare things from them. Also, print it
*/

void findZicode()
{
	string zipcodeEnter; // creating an integer variable for the zip code
	cin >> zipcodeEnter; // read in from the user the zipcode enter 

	string myQuery = "select * from DealerTables"; // creating query to select * from DealerTables
	//create statement, view and execute the query 
	stmt2 = con->createStatement();
	res2 = stmt2->executeQuery(myQuery);
	string dealernameFind;  // creating a string variable for the dealernameFind
	string zipcodeFind; // creating a string variable for the zipcode
	string phonenumberFind;  // creating a string variable for the phonenumber
	while (res2->next()) { // from the view table read in every string and store it until there is nothing in the table 
		dealernameFind = res2->getString("DealerName"); // get string DealerName from the DealerTables and store it in dealernameFind
		zipcodeFind = res2->getString("Zipcode"); // get string zipcode from the DealerTables and store it in zipcodeFind
		phonenumberFind = res2->getString("Phonenumber"); // get string phonenumber from the DealerTables and store it in PhonenumberFind
		string myQuery = "select * from manuTables order by ACTmanu asc"; // creating query to select * from ACTmanu (low to high)
		string abbvmanufacturerFind; //creating a string varible for the abbreviation of the manufacturer
		string actualnameFind; // creating a string variable for the actual name of the manufacturer
		//create statement, view and execute the query 
		stmt = con->createStatement();
		res = stmt->executeQuery(myQuery);
		while (res->next()) { // from the view table read in every string and store it until there is nothing in the table 
			abbvmanufacturerFind = res->getString("ABBVmanu"); // get string ABBVmanu from the manuTables and store it in abbvmanufacturer find
			actualnameFind = res->getString("ACTmanu"); // get string actmanu from the manutables and store it in actualnamefind
			if (zipcodeEnter == zipcodeFind) { // check if zipcodeenter and zipcodeFind are equals
				//creating query to select * from CarsTables order by Price and then dealership
				string myQuery = "select * from CarsTables order by Price desc, Dealership";
				string VINFind; // creating a string variable for the vin 
				string milesfind; // creating a string variable for the miles
				string dealershipFind; // creating a string variable for the dealership 
				string priceFind; // creating a string variable for the price
				//create statement, view and execute the query 
				stmt1 = con->createStatement();
				res1 = stmt1->executeQuery(myQuery);

				while (res1->next()) { // from the view table read in every string and store it until there is nothing in the table 
					VINFind = res1->getString("VIN"); // get string VIN from the CarsTables and store it in VINFIND
					milesfind = res1->getString("Miles"); // get string Miles from the CarsTables and store it in milesfind
					dealershipFind = res1->getString("Dealership"); // get string dealership from the CarsTables and store it in dealershipFind
					priceFind = res1->getString("Price"); // get string price from the CarsTables and store it in priceFind
					if (dealershipFind == dealernameFind) {//check if dealership Find is equal to dealernamefind
						// check if the first three letters of the VIN and abbreviation are equals
						if (abbvmanufacturerFind.at(0) == VINFind.at(0) && abbvmanufacturerFind.at(1) == VINFind.at(1) && abbvmanufacturerFind.at(2) == VINFind.at(2)) {
							cout << actualnameFind << ":" << milesfind << " miles, $" << priceFind << ": " << dealernameFind << "[("
								<< phonenumberFind.at(0) << phonenumberFind.at(1) << phonenumberFind.at(2) << ")" << phonenumberFind.at(3) << phonenumberFind.at(4) << phonenumberFind.at(5)
								<< "-" << phonenumberFind.at(6) << phonenumberFind.at(7) << phonenumberFind.at(8) << phonenumberFind.at(9) << "]" << endl; // print to the user the actualname of the car, miles, price, dealername and phonenumber
			
						}
					}
				}
			}
		}

	}
	choose(); // return to the choose function 
}

/*
* this function summarize print out one line for each manufacturer that includes the average price of the cars made by that manufacturer
*/
void summarize()
{
	// create a query to select actual name of the manufacturer and the average of the price where abbreviation of the CarsTables and manuTables match 
	// and group by the actual name of the manufacturer
	string myQuery = "select ACTmanu, avg(Price) from CarsTables, manuTables  where manuTables.ABBVmanu = CarsTables.ABBVVin group by manuTables.ACTmanu  ";
	//create statement, view and execute the query 
	stmt = con->createStatement();
	res = stmt->executeQuery(myQuery);

	while (res->next()) { //from the manuTables get the actual name of the manufacturer(ACTmanu) and from the CarsTables get the avg price where abbreviation of the carstables and manuTables match
		                  // and continue do it until there is not more information  
		cout << res->getString("ACTmanu") << " "; // get the actual name of the manufacutrer and print to the user 
		cout << res->getString(2) << endl; // get the average price and print to the user 
	}


	delete res; //delete res
	delete stmt; // delete statement 
	choose(); // return to the choose function 
}

/*
* this function delete car from Carstables where the vin match 
*/
void deletecar()
{
	string vinDelete; // creating a string variable for the vinDelete
	cin >> vinDelete; // read in the string variable from the user and store it in vinDelete
	// create query to delete car where vin match
	string myQuery = "delete from CarsTables where VIN =\"";
	myQuery += vinDelete;
	myQuery += "\"";
	//create statement and execute, delete car with vin match
	stmt = con->createStatement();
	stmt->execute(myQuery);
	
	delete stmt; // delete statement 
	choose(); // return to the choose function 
}

/*
* this function delete car where dealershipdelete match and delete dealer where dealerName match 
*/	
void deletedealer()
{
	string dealershipdelete;       // creating a string variable for the dealershipdelete
	cin >> dealershipdelete;      // read in the string variable from the user and store it in dealershipdelete

	string myQuery = "delete from CarsTables where Dealership =\""; // create query to delete car where deleshipdelete match 
	myQuery += dealershipdelete + "\"";
	//create statement and execute, delete car where dealership match
	stmt = con->createStatement();
	stmt->execute(myQuery);
	delete stmt; // delete statement 

	// create query to delete dealer where dealerName match
	myQuery = "delete from DealerTables where DealerName  =\""; 
	myQuery += dealershipdelete + "\"";
	// create statement and execute, delete dealer where dealerName match
	stmt = con->createStatement();
	stmt->execute(myQuery);
	delete stmt;  //delete statement
	choose();     // return to the choose function 
}

/*
* this function quit the program and print thanks for using this datebase system
*/
void quitProgram()
{
	cout << "thanks for using this datebase system\n"; // print to the user thanks for using this datebase system
	delete con; // delete connection 
}
