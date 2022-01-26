#pragma once

#include "transport/Config.h"
#include "transport/NetworkPlugin.h"

class Plugin : public Transport::NetworkPlugin {
	public:
		Plugin(Transport::Config *config, const std::string &host, int port);

		// NetworkPlugin uses this method to send the data to networkplugin server
		void sendData(const std::string &string);

		void handleLoginRequest(const std::string &user, const std::string &legacyName, const std::string &password);

		void handleLogoutRequest(const std::string &user, const std::string &legacyName);

		void handleMessageSendRequest(const std::string &user, const std::string &legacyName, const std::string &message, const std::string &xhtml = "", const std::string &id = "");

		void handleBuddyUpdatedRequest(const std::string &user, const std::string &buddyName, const std::string &alias, const std::vector<std::string> &groups);

		void handleBuddyRemovedRequest(const std::string &user, const std::string &buddyName, const std::vector<std::string> &groups);

	private:
		Transport::Config *config;
};
