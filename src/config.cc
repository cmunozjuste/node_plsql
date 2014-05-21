#include "global.h"
#include "config.h"

///////////////////////////////////////////////////////////////////////////
std::string Config::asString() const
{
	std::ostringstream s;
	s	<< "Con Min: " << m_conMin << std::endl
		<< "Con Max: " << m_conMax << std::endl
		<< "Con Inc: " << m_conIncr << std::endl
		<< "Username: " << m_username << std::endl
		<< "Password: " << m_password << std::endl
		<< "Database: " << m_database << std::endl
		<< "Connection pool: " << m_conPool << std::endl
		<< "Debug: " << m_debug << std::endl;

	return s.str();
}
