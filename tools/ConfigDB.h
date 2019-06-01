#pragma once
///////////////////////////////////////
// C S Cowden
// Class to help connect to the sqlite3
// configuration database.
///////////////////////////////////////

#include <string>
#include <cassert>

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
			static ConfigDB inst;
			return &inst;
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

		///
		/// try a better query method
		inline std::vector<std::vector<char *> > query(int & nrows, const char * qry) {
			char * zErrMsg = 0;

			callback cb;
			int rc = sqlite3_exec(m_db, qry, cb.functor, static_cast<void*>(&cb), &zErrMsg);

			nrows = cb.nrows_;
			return cb.res_;
		}


	private:

		inline ConfigDB() {
			char * tdir = std::getenv("TURKDIR");
			std::string baseDir(tdir);
			std::string configName("\\TurkConfiguration.sqlite");
			int rc = sqlite3_open(std::string(baseDir + configName).c_str(), &m_db);
		}

		sqlite3 * m_db;

		class callback {
		public:
			std::vector<std::vector<char *> > res_;
			int nrows_;
			int ncols_;



		public:

			inline callback() :nrows_(0), ncols_(0) { }

			static inline int functor (void *obj, int argc, char **argv, char **azColName) {
				callback * cobj = static_cast<callback*>(obj);

				if (cobj->nrows_ == 0) {
					cobj->ncols_ = argc;
					cobj->res_.resize(cobj->ncols_);
				}

				assert(cobj->ncols_ == argc);

				for (int i = 0; i < argc; i++) {
					cobj->res_[i].push_back(argv[i]);
				}

				cobj->nrows_++;

				return 0;
			}
		};
	};
}