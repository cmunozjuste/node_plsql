#ifndef OCIP_INTERFACE__H
#define OCIP_INTERFACE__H

#include "oci_interface.h"
#include "oracleError.h"

namespace ocip
{

///////////////////////////////////////////////////////////////////////////
enum DataType
{
	String	= SQLT_STR,
	Integer = SQLT_INT
};
enum Direction
{
	Input,
	Output
};

///////////////////////////////////////////////////////////////////////////
std::string getTypeName(DataType type);

///////////////////////////////////////////////////////////////////////////
class Parameter;
class ParameterArray;

///////////////////////////////////////////////////////////////////////////
class Environment
{
public:
	Environment(int mode = OCI_DEFAULT, bool debug = false);
	~Environment();

	OCIEnv* hEnv() const {return m_envhp;}

	static oracleError reportError(int oracleStatus, OCIError* errhp, const std::string& message, const std::string& file, int line);

	static void debug(bool debug) {m_debug = debug;}
	static bool debug() {return m_debug;}
	
private:
	void create();
	void destroy();

	sb4					m_mode;
	OCIEnv*				m_envhp;

	static bool			m_debug;

	// Disable copy/assign
	Environment(const Environment&);
	Environment &operator=(const Environment&);
};

///////////////////////////////////////////////////////////////////////////
class ConnectionPool
{
public:
	ConnectionPool(Environment* environment);
	~ConnectionPool();

	bool create(const std::string& username, const std::string& password, const std::string& database, int connMin, int connMax, int connIncr);
	bool destroy();

	OCIEnv* hEnv() const {return m_envhp;}
	OCIError* hError() const {return m_errhp;}
	OCICPool* poolhp() const {return m_poolhp;}
	OraText* poolName() const {return m_poolName;}
	sb4	poolNameLen() const {return m_poolNameLen;}

	bool hasPool() const {return m_hasPool;}

	oracleError reportError(const std::string& message, const std::string& file, int line)
	{
		return Environment::reportError(m_oracle_status, m_errhp, message, file, line);
	}

private:
	OCIEnv*				m_envhp;
	OCIError*			m_errhp;
	OCICPool*			m_poolhp;
	OraText*			m_poolName;
	sb4					m_poolNameLen;
	bool				m_hasPool;
	sword				m_oracle_status;

	// Disable copy/assign
	ConnectionPool(const ConnectionPool&);
	ConnectionPool &operator=(const ConnectionPool&);
};

///////////////////////////////////////////////////////////////////////////
class Connection
{
public:
	Connection(Environment* environment);
	Connection(ConnectionPool* connectionPool);
	~Connection();

	bool connect(const std::string& username, const std::string& password, const std::string& server);
	bool connect(const std::string& username, const std::string& password)
	{
		return connect(username, password, "");
	}
	bool disconnect();

	bool hasPool() const {return m_poolhp != 0;}
	bool isConnected() const {return m_isLoggedOn;}

	bool commit();

	OCIEnv* hEnv() const {return m_envhp;}
	OCIError* hError() const {return m_errhp;}
	OCISvcCtx* hSvcCtx() const {return m_svchp;}

	oracleError reportError(const std::string& message, const std::string& file, int line)
	{
		return Environment::reportError(m_oracle_status, m_errhp, message, file, line);
	}

private:
	OCIEnv*				m_envhp;
	OCIError*			m_errhp;
	OCICPool*			m_poolhp;
	OraText*			m_poolName;
	sb4					m_poolNameLen;
	bool				m_hasPool;
	OCISvcCtx*			m_svchp;
	bool				m_isLoggedOn;

	sword				m_oracle_status;

	// Disable copy/assign
	Connection(const Connection&);
	Connection &operator=(const Connection&);
};

///////////////////////////////////////////////////////////////////////////
class Statement
{
public:
	Statement(Connection* connection);
	~Statement();

	bool prepare(const std::string& sql);
	bool execute(int iterations);
	bool bind(OCIBind** bindpp, const std::string& placeholder, ub2 dty, dvoid* valuep, sb4 value_sz, sb2* indp = 0, ub4 maxarr_len = 0, ub4* curelen = 0);
	void addParameter(Parameter* parameter);

	OCIStmt* hStmt() const {return m_stmtp;}
	OCIError* hError() const {return m_connection->hError();}

	oracleError reportError(const std::string& message, const std::string& file, int line)
	{
		return Environment::reportError(m_oracle_status, m_connection->hError(), message, file, line);
	}

	bool openAndReadLOB(OCILobLocator* locp, std::wstring* lob);
	bool writeBLOB(OCILobLocator* locp, const std::vector<unsigned char>& lob);

private:
	Connection*			m_connection;
	OCIStmt*			m_stmtp;
	sword				m_oracle_status;

	typedef std::list<Parameter*> ParameterListType;
	typedef std::list<Parameter*>::iterator ParameterListIteratorType;
	ParameterListType	m_parameters;

	// Disable copy/assign
	Statement(const Statement&);
	Statement &operator=(const Statement&);
};

///////////////////////////////////////////////////////////////////////////
class Parameter
{
public:
	friend class Statement;

	Parameter(const std::string& placeholder, DataType type, Direction direction) :	m_statement(0), m_placeholder(placeholder), m_type(type), m_direction(direction), m_bindp(0) {}
	virtual ~Parameter() {}

protected:
	virtual sword bind(OCIStmt* stmtp, OCIError* errhp) = 0;

	Statement*			m_statement;
	std::string			m_placeholder;
	DataType			m_type;
	Direction			m_direction;

	OCIBind*			m_bindp;

private:
	// Disable copy/assign
	Parameter(const Parameter&);
	Parameter &operator=(const Parameter&);
};

///////////////////////////////////////////////////////////////////////////
class ParameterValue : public Parameter
{
public:
	friend class Statement;

	ParameterValue(const std::string& placeholder, DataType type, Direction direction);
	virtual ~ParameterValue();

	virtual void value(long value);
	virtual void value(const std::string& value);

private:
	virtual sword bind(OCIStmt* stmtp, OCIError* errhp);

	unsigned char		m_value[32767];
	sb4					m_value_sz;
	sb2					m_ind;

	// Disable copy/assign
	ParameterValue(const ParameterValue&);
	ParameterValue &operator=(const ParameterValue&);
};

///////////////////////////////////////////////////////////////////////////
class ParameterArray : public Parameter
{
public:
	friend class Statement;

	ParameterArray(const std::string& placeholder, DataType type, Direction direction);
	virtual ~ParameterArray();

	virtual void value(std::list<long> list);
	virtual void value(std::list<std::string> list);

private:
	virtual sword bind(OCIStmt* stmtp, OCIError* errhp);
	void allocate(size_t value_size, size_t array_size);

	dvoid*				m_valuep;
	sb4					m_value_sz;
	sb2*				m_indp;
	ub4					m_maxarr_len;
	ub4					m_curelen;

	// Disable copy/assign
	ParameterArray(const ParameterArray&);
	ParameterArray &operator=(const ParameterArray&);
};

} // namespace "ocip"

#endif // OCIP_INTERFACE__H
