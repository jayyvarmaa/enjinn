#pragma once
#include <string>
#include <deque>
#include <sstream>
#include <pushNotification/pushNotification.h>

namespace enjinn
{


	struct LogManager
	{

		static constexpr const char *DefaultLogFile = ENJINN_RESOURCES_PATH "logs.txt";

		//a null name will just log to a internal structure
		void init(std::string name);

		
		//this will be dependent on the configuration of the project. 
		void log(const char *l, int type = enjinn::logNormal);
		

		std::string name = "";
		bool firstLog = 0;

		static constexpr int maxInternalLogCount = 200;
		std::deque<std::string> internalLogs; //used to print the logs in a internal console

		
		enjinn::PushNotificationManager *pushNotificationManager = 0;
	private:
		//used only interally.
		void logToFile(const char *l, int type = enjinn::logNormal);
		void logInternally(const char *l, int type = enjinn::logNormal);
		void logToPushNotification(const char *l, int type = enjinn::logNormal);

	};


	void logToFile(const char *fileName, const char *l, int type = enjinn::logNormal);


}