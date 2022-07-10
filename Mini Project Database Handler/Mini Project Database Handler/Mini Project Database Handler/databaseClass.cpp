#include "databaseClass.h"

#include<vector>
#include<string>
#include<sstream>
#include<iomanip>

databaseClass::databaseClass(int evmID)
{
	this->host = DBHOST;
	this->user = USER;
	this->pass = PASSWORD;
	this->db = DATABASE;
	this->table = TABLE;

	this->sessionID = 0;
	this->evmID = evmID;

	std::cout << "EVM ID : " << evmID << std::endl;

	driver = get_driver_instance();
	con = driver->connect(host, user, pass);
	cout << "Setting connection to Database : " << db << endl;
	con->setSchema(db);
	con->setAutoCommit(true);
	cout << "\nDatabase connection\'s autocommit mode = " << con->getAutoCommit() << endl;
	retrieve_dbmetadata_and_print(con);
}

databaseClass::~databaseClass()
{
	cout << "Cleaning up the resources .." << endl;

	delete res;
	delete stmt;
	delete prep_stmt;
	con->close();
	delete con;
}

void databaseClass::retrieve_dbmetadata_and_print(Connection* dbcon)
{
	if (dbcon->isClosed()) {
		throw runtime_error("DatabaseMetaData FAILURE - database connection closed");
	}

	cout << "\nDatabase Metadata" << endl;
	cout << "-----------------" << endl;

	cout << boolalpha;

	DatabaseMetaData* dbcon_meta = dbcon->getMetaData();

	cout << "Database Product Name: " << dbcon_meta->getDatabaseProductName() << endl;
	cout << "Database Product Version: " << dbcon_meta->getDatabaseProductVersion() << endl;
	cout << "Database User Name: " << dbcon_meta->getUserName() << endl << endl;

	cout << "Driver name: " << dbcon_meta->getDriverName() << endl;
	cout << "Driver version: " << dbcon_meta->getDriverVersion() << endl << endl;

	cout << "Database in Read-Only Mode?: " << dbcon_meta->isReadOnly() << endl;
	cout << "Supports Transactions?: " << dbcon_meta->supportsTransactions() << endl;
	cout << "Supports DML Transactions only?: " << dbcon_meta->supportsDataManipulationTransactionsOnly() << endl;
	cout << "Supports Batch Updates?: " << dbcon_meta->supportsBatchUpdates() << endl;
	cout << "Supports Outer Joins?: " << dbcon_meta->supportsOuterJoins() << endl;
	cout << "Supports Multiple Transactions?: " << dbcon_meta->supportsMultipleTransactions() << endl;
	cout << "Supports Named Parameters?: " << dbcon_meta->supportsNamedParameters() << endl;
	cout << "Supports Statement Pooling?: " << dbcon_meta->supportsStatementPooling() << endl;
	cout << "Supports Stored Procedures?: " << dbcon_meta->supportsStoredProcedures() << endl;
	cout << "Supports Union?: " << dbcon_meta->supportsUnion() << endl << endl;

	cout << "Maximum Connections: " << dbcon_meta->getMaxConnections() << endl;
	cout << "Maximum Columns per Table: " << dbcon_meta->getMaxColumnsInTable() << endl;
	cout << "Maximum Columns per Index: " << dbcon_meta->getMaxColumnsInIndex() << endl;
	cout << "Maximum Row Size per Table: " << dbcon_meta->getMaxRowSize() << " bytes" << endl;

	cout << "\nDatabase schemas: " << endl;

	unique_ptr <ResultSet> rs(dbcon_meta->getSchemas());

	cout << "\nTotal number of schemas = " << rs->rowsCount() << endl;
	cout << endl;

	int row = 1;

	while (rs->next()) {
		cout << "\t" << row << ". " << rs->getString("TABLE_SCHEM") << endl;
		++row;
	}

	cout << endl;
}

void databaseClass::retrieve_rsmetadata_and_print(ResultSet* rs)
{
	if (rs->rowsCount() == 0) {
		throw runtime_error("ResultSetMetaData FAILURE - no records in the result set");
	}

	cout << "ResultSet Metadata" << endl;
	cout << "------------------" << endl;
	ResultSetMetaData* res_meta = rs->getMetaData();

	int numcols = res_meta->getColumnCount();
	cout << "\nNumber of columns in the result set = " << numcols << endl << endl;

	cout << setw(20) << left << "Column Name/Label";
	cout << setw(20) << left << "Column Type";
	cout << setw(20) << left << "Column Size" << endl;

	for (int i = 0; i < numcols; ++i) {
		cout << setw(20) << left << res_meta->getColumnLabel(i + 1);
		cout << setw(20) << left << res_meta->getColumnTypeName(i + 1);
		cout << setw(20) << left << res_meta->getColumnDisplaySize(i + 1) << endl;
	}
	for (int i = 1; i <= numcols; i++)
	{
		cout << "\nColumn \"" << res_meta->getColumnLabel(i);
		cout << "\" belongs to the Table: \"" << res_meta->getTableName(i);
		cout << "\" which belongs to the Schema: \"" << res_meta->getSchemaName(i) << "\"";
	}
	cout << endl;
}

void databaseClass::retrieve_data_and_print(ResultSet* rs)
{
	cout << "\nRetrieved " << rs->rowsCount() << " row(s)." << endl << endl;

	ResultSetMetaData* res_meta = rs->getMetaData();
	int numcols = res_meta->getColumnCount();

	for (int i = 1; i <= numcols; i++)
		cout << setw(20) << left << res_meta->getColumnLabel(i);
	cout << endl;

	for (int i = 1; i <= numcols; i++)
		cout << setw(20) << left << "----------";
	cout << endl;
	while (rs->next()) {
		for (int i = 1; i <= numcols; i++)
			cout << setw(20) << left << rs->getString(i);
		cout << endl;
	}

	cout << endl;
}

void databaseClass::print_all_from_table()
{
	stmt = con->createStatement();
	string query = "select * from ";
	query = query + table;
	cout << "Executing the Query : "<< query << endl;
	res = stmt->executeQuery(query);

	cout << "Retrieving the result set .." << endl;
	if (res->rowsCount() != 0)
	{
		//retrieve_rsmetadata_and_print(res);
		retrieve_data_and_print(res);
	}
	else
	{
		cout << "Noting in the Table : " << this->table << endl;
	}
}

void databaseClass::insert_into_table(char c)
{
	this->sessionID++;

	stmt = con->createStatement();

	stringstream query;
	query << "insert into "<< this->table<<"(Session_ID, EVM_ID, timestamp, party)";
	query << " values(" << this->sessionID << "," << this->evmID << ",now(),'" << c << "');";
	std::cout << "Executing the Query : " << query.str() << std::endl;
	
	bool res = stmt->execute(query.str());
	if (res)
	{
		cout << "Can't add to database" << endl;
	}
}

void databaseClass::show_party_count()
{
	stringstream query;
	query << "select party, count(party) from " << this->table << " group by party order by party;";

	stmt = con->createStatement();

	std::cout << "Executing the Query : " << query.str() << std::endl;

	res = stmt->executeQuery(query.str());

	cout << "Retrieving the result set .." << endl;
	if (res->rowsCount() != 0)
	{
		//retrieve_rsmetadata_and_print(res);
		retrieve_data_and_print(res);
	}
	else
	{
		cout << "Noting in the Table : " << this->table << endl;
	}
}
