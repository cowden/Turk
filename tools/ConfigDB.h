///////////////////////////////////////
// C S Cowden
// Class to help connect to the sqlite3
// configuration database.
///////////////////////////////////////

#include <string>

#include "sqlite3.h"

namespace Turk {
	class ConfigDB {
	public:
		///
		/// destructor ... be sure to close the db connection
		inline ~ConfigDB() {
			sqlite3_close(m_db);
		}

		///
		/// return a static reference 
		inline static ConfigDB * instance() {
			if (!m_ptr)
				m_ptr = new ConfigDB();
			return m_ptr;
		}

		///
		/// query the database
		inline std::vector<std::string> query(const char *qry) {

			std::vector<std::string> result;
			char msg[500];

			// execute the query
			sqlite3_stmt *stmt;
			const char * extra;
			int rc = sqlite3_prepare(m_db, qry, -1, &stmt, 0);
			while (rc == 0) {
				rc = sqlite3_step(stmt);
				if (rc != SQLITE_ROW) break;
				for (unsigned i = 0; i != 3; i++){
					const unsigned char * res = sqlite3_column_text(stmt, i);
					sprintf(msg, "%s", res);
					result.push_back(std::string(msg));
				}
			}

			return result;
		}


	private:

		inline ConfigDB() {
			char * tdir = std::getenv("TURKDIR");
			std::string baseDir(tdir);
			std::string configName("\\TurkConfiguration.sqlite");
			int rc = sqlite3_open(std::string(baseDir + configName).c_str(), &m_db);
		}

		sqlite3 * m_db;
		static ConfigDB * m_ptr;
	};
}