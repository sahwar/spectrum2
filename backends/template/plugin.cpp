#include "plugin.h"
// Transport includes
#include "transport/Config.h"
#include "transport/NetworkPlugin.h"
#include "transport/Logging.h"

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;
using namespace boost::program_options;
using namespace Transport;

DEFINE_LOGGER(logger, "Backend Template");

Plugin::Plugin(Config *config, const std::string &host, int port) : NetworkPlugin() {
	this->config = config;
	LOG4CXX_INFO(logger, "Starting the plugin.");
	connect(host, std::to_string(port));
}

void Plugin::handleLoginRequest(const std::string &user, const std::string &legacyName, const std::string &password) {
	handleConnected(user);
	LOG4CXX_INFO(logger, user << ": Added buddy - Echo.");
	handleBuddyChanged(user, "echo", "Echo", std::vector<std::string>(), pbnetwork::STATUS_ONLINE);
}

void Plugin::handleLogoutRequest(const std::string &user, const std::string &legacyName) {
}

void Plugin::handleMessageSendRequest(const std::string &user, const std::string &legacyName, const std::string &message, const std::string &xhtml, const std::string &id) {
	LOG4CXX_INFO(logger, "Sending message from " << user << " to " << legacyName << ".");
	if (legacyName == "echo") {
		handleMessage(user, legacyName, message);
	}
}

void Plugin::handleBuddyUpdatedRequest(const std::string &user, const std::string &buddyName, const std::string &alias, const std::vector<std::string> &groups) {
	LOG4CXX_INFO(logger, user << ": Added buddy " << buddyName << ".");
	handleBuddyChanged(user, buddyName, alias, groups, pbnetwork::STATUS_ONLINE);
}

void Plugin::handleBuddyRemovedRequest(const std::string &user, const std::string &buddyName, const std::vector<std::string> &groups) {

}
