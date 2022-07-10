#pragma once

#include <driver.h>
#include <connection.h>
#include <statement.h>
#include <prepared_statement.h>
#include <resultset.h>
#include <metadata.h>
#include <resultset_metadata.h>
#include <exception.h>
#include <warning.h>

#include <sstream>
#include <memory>
#include <string>
#include <stdexcept>

using namespace std;
using namespace sql;

#define DBHOST "tcp://127.0.0.1:3306"
#define USER "root"
#define PASSWORD "29062002"
#define DATABASE "VotingData"
#define TABLE "Votes"

class databaseClass
{
private:
	Driver* driver;
	Connection* con;
	Statement* stmt;
	ResultSet* res;
	PreparedStatement* prep_stmt;
	Savepoint* savept;
	string host, user, pass, db, table;
	int sessionID, evmID;

public:
	databaseClass(int evmID);
	~databaseClass();
	void retrieve_dbmetadata_and_print(Connection* dbcon);
	void retrieve_rsmetadata_and_print(ResultSet* rs);
	void retrieve_data_and_print(ResultSet* rs);
	void print_all_from_table();
	void insert_into_table(char c);
	void show_party_count();
};
