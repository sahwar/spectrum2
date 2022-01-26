#pragma once

#include "transport/Config.h"
#include "transport/NetworkPlugin.h"
#include "transport/Logging.h"
#include "transport/SQLite3Backend.h"
#include "transport/MySQLBackend.h"
#include "transport/PQXXBackend.h"
#include "transport/StorageBackend.h"

#ifndef _WIN32
#include "unistd.h"
#include "signal.h"
#include "sys/wait.h"
#include "sys/signal.h"
#endif
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "twitcurl.h"
#include "TwitterResponseParser.h"

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <cstdio>

using namespace boost::filesystem;
using namespace boost::program_options;
using namespace Transport;

#define STR(x) (std::string("(") + x.from + ", " + x.to + ", " + x.message + ")")

class TwitterPlugin;
extern TwitterPlugin *np;


class TwitterPlugin : public NetworkPlugin {
	public:
		std::unique_ptr<boost::asio::deadline_timer> tweet_timer;
		std::unique_ptr<boost::asio::deadline_timer> message_timer;
		std::unique_ptr<boost::asio::io_service> io;
		StorageBackend *storagebackend;

		TwitterPlugin(Config *config, StorageBackend *storagebackend, const std::string &host, int port);
		~TwitterPlugin();

		// Send data to NetworkPlugin server
		void sendData(const std::string &string);
	
		// User trying to login into his twitter account
		void handleLoginRequest(const std::string &user, const std::string &legacyName, const std::string &password);
		
		// User logging out
		void handleLogoutRequest(const std::string &user, const std::string &legacyName);
		
		void handleJoinRoomRequest(const std::string &/*user*/, const std::string &/*room*/, const std::string &/*nickname*/, const std::string &/*pasword*/);

		void handleLeaveRoomRequest(const std::string &/*user*/, const std::string &/*room*/);

		void handleMessageSendRequest(const std::string &user, const std::string &legacyName, const std::string &message, const std::string &xhtml = "", const std::string &id = "");

		void handleBuddyUpdatedRequest(const std::string &user, const std::string &buddyName, const std::string &alias, const std::vector<std::string> &groups);

		void handleBuddyRemovedRequest(const std::string &user, const std::string &buddyName, const std::vector<std::string> &groups);
		
		void handleVCardRequest(const std::string &/*user*/, const std::string &/*legacyName*/, unsigned int /*id*/);
		
		void pollForTweets();

		void pollForDirectMessages();
		
		bool getUserOAuthKeyAndSecret(const std::string user, std::string &key, std::string &secret);
		
		bool checkSpectrum1User(const std::string user);
		
		bool storeUserOAuthKeyAndSecret(const std::string user, const std::string OAuthKey, const std::string OAuthSecret);
		
		void initUserSession(const std::string user, const std::string legacyName, const std::string password);
		
		void OAuthFlowComplete(const std::string user, twitCurl *obj);
		
		void pinExchangeComplete(const std::string user, const std::string OAuthAccessTokenKey, const std::string OAuthAccessTokenSecret);
		
		void updateLastTweetID(const std::string user, const std::string ID);

		std::string getMostRecentTweetID(const std::string user);

		void updateLastDMID(const std::string user, const std::string ID);
		
		std::string getMostRecentDMID(const std::string user);

		void clearRoster(const std::string user);

		int getTwitterMode(const std::string user);

		bool setTwitterMode(const std::string user, int m);

		/****************** Twitter response handlers **************************************/
		void statusUpdateResponse(std::string &user, Error &errMsg);
		
		void helpMessageResponse(std::string &user, std::string &msg);
		
		void populateRoster(std::string &user, std::vector<User> &friends, std::vector<std::string> &friendAvatars, Error &errMsg);
		
		void displayFriendlist(std::string &user, std::vector<User> &friends, std::vector<std::string> &friendAvatars, Error &errMsg);
		
		void displayTweets(std::string &user, std::string &userRequested, std::vector<Status> &tweets , Error &errMsg);
		
		void directMessageResponse(std::string &user, std::string &username, std::vector<DirectMessage> &messages, Error &errMsg);
		
		void createFriendResponse(std::string &user, User &frnd, std::string &img, Error &errMsg);
		
		void deleteFriendResponse(std::string &user, User &frnd, Error &errMsg);
		
		void RetweetResponse(std::string &user, Error &errMsg);
		
		void profileImageResponse(std::string &user, std::string &buddy, std::string &img, unsigned int reqID, Error &errMsg);
		/***********************************************************************************/

	private:
		std::string getMostRecentTweetIDUnsafe(const std::string user);
		std::string getMostRecentDMIDUnsafe(const std::string user);

		enum status {NEW, WAITING_FOR_PIN, CONNECTED, DISCONNECTED};
		enum mode {SINGLECONTACT, MULTIPLECONTACT, CHATROOM};

		Config *config;
		std::string adminLegacyName;
		std::string adminChatRoom;
		std::string adminNickName;
		std::string adminAlias;

		std::string consumerKey;
		std::string consumerSecret;
		std::string OAUTH_KEY;
		std::string OAUTH_SECRET;
		std::string MODE;

		boost::mutex dblock, userlock;

		std::set<std::string> onlineUsers;
		struct UserData
		{
			std::string legacyName;
			bool spectrum1User; //Legacy support
			User userTwitterObj;
			std::string userImg;
			twitCurl* sessions;		
			status connectionState;
			std::string mostRecentTweetID;
			std::string mostRecentDirectMessageID;
			std::string nickName;
			std::set<std::string> buddies;
			std::map<std::string, User> buddiesInfo;
			std::map<std::string, std::string> buddiesImgs;
			mode twitterMode;

			UserData() { sessions = NULL; }
		};
		std::map<std::string, UserData> userdb;
		bool m_firstPing;
};
