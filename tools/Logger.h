/////////////////////////////////////////////////////////
// C S Cowden
// Log file coordinator class.
/////////////////////////////////////////////////////////

////////////
/*
* Manage log files by adding a time stamp for unique file names.
* Pass messages from Turk classes to log file.  
* Place frame stamp and class name for each message.
*/


#include <string>
#include <fstream>
#include <time.h>

#include "BWAPI.h"

namespace Turk {

	class Logger {
	public:

		///
		/// destructor
		inline ~Logger() {
			m_file.close();
		}


		///
		/// Get static reference
		inline static Logger * instance() {
			if (!m_ptr)
				m_ptr = new Logger();
			return m_ptr;
		}

		///
		/// Push log message for the given class name
		inline void log(const char *name, const char *msg) {
			m_file << BWAPI::Broodwar->getFrameCount() << "| " << name << "| " << msg << std::endl;
		}

		///
		/// Push a generic log message
		inline void log(const char * msg) {
			m_file << BWAPI::Broodwar->getFrameCount() << "|  |" << msg << std::endl;
		}

		///
		/// Initiate a new log file connection
		inline void newLog() {
			std::string ts = getTimeStamp();
			std::string name = m_baseName + "_" + ts + ".txt";

			if (m_file.is_open()) m_file.close();

			m_file = std::fstream(name, std::fstream::out);
			m_file << "Starting log file " << ts << std::endl;
		}

		///
		/// Initiate a new log file along with a new base name
		inline void newLog(const char * base) {
			m_baseName = std::string(base);
			newLog();
		}


		///
		/// Close file
		inline void close() { m_file.close(); }

	private:

		inline Logger():m_baseName("TheTurk"){};

		// get a time stamp and return its string value
		inline std::string getTimeStamp() {
			time_t now;
			time(&now);
			char buf[250] = { '\0' };
			strftime(buf, sizeof(buf), "%Y%m%d-%H%M%S", localtime(&now));
			return std::string(buf);
		}

		// -- member data
		std::string m_baseName;

		std::fstream m_file;

		static Logger * m_ptr;


	};
}