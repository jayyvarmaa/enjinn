#pragma once
#include <string>
#include <chrono>
#include <deque>

namespace enjinn
{
	enum: int
	{
		logNormal = 0,
		logWarning,
		logError
	};

	struct Notification
	{
		std::string content = "";
		std::chrono::steady_clock::time_point startTime = {};
		int notificationType = enjinn::logNormal;

		Notification(std::string content, std::chrono::steady_clock::time_point startTime, int notificationType):
			content(content), startTime(startTime), notificationType(notificationType)
		{
		};

		Notification() {};
	};


	struct PushNotificationManager
	{

		void init();

		void update(bool &open);

		void pushNotification(const char *content, int logType = enjinn::logNormal);

		std::deque<Notification> notificationQue;
	};




};