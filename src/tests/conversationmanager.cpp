#include "transport/userregistry.h"
#include "transport/config.h"
#include "transport/storagebackend.h"
#include "transport/user.h"
#include "transport/transport.h"
#include "transport/conversation.h"
#include "transport/usermanager.h"
#include "transport/conversationmanager.h"
#include "transport/localbuddy.h"
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <Swiften/Swiften.h>
#include <Swiften/EventLoop/DummyEventLoop.h>
#include <Swiften/Server/Server.h>
#include <Swiften/Network/DummyNetworkFactories.h>
#include <Swiften/Network/DummyConnectionServer.h>
#include "Swiften/Server/ServerStanzaChannel.h"
#include "Swiften/Server/ServerFromClientSession.h"
#include "Swiften/Parser/PayloadParsers/FullPayloadParserFactoryCollection.h"
#include "basictest.h"

using namespace Transport;

class ConversationManagerTest : public CPPUNIT_NS :: TestFixture, public BasicTest {
	CPPUNIT_TEST_SUITE(ConversationManagerTest);
	CPPUNIT_TEST(handleNormalMessages);
	CPPUNIT_TEST(handleGroupchatMessages);
	CPPUNIT_TEST(handleParticipantChanged);
	CPPUNIT_TEST(handlePMFromXMPP);
	CPPUNIT_TEST_SUITE_END();

	public:
		TestingConversation *m_conv;
		boost::shared_ptr<Swift::Message> m_msg;

		void setUp (void) {
			m_conv = NULL;
			m_msg.reset();
			setMeUp();
			connectUser();
			add2Buddies();
			factory->onMessageToSend.connect(boost::bind(&ConversationManagerTest::handleMessageReceived, this, _1, _2));
			received.clear();
		}

		void tearDown (void) {
			received.clear();
			disconnectUser();
			factory->onMessageToSend.disconnect(boost::bind(&ConversationManagerTest::handleMessageReceived, this, _1, _2));
			tearMeDown();
		}

	void handleMessageReceived(TestingConversation *_conv, boost::shared_ptr<Swift::Message> &_msg) {
		m_conv = _conv;
		m_msg = _msg;
	}

	void handleNormalMessages() {
		User *user = userManager->getUser("user@localhost");

		TestingConversation *conv = new TestingConversation(user->getConversationManager(), "buddy1");
		user->getConversationManager()->addConversation(conv);
		conv->onMessageToSend.connect(boost::bind(&ConversationManagerTest::handleMessageReceived, this, _1, _2));

		boost::shared_ptr<Swift::Message> msg(new Swift::Message());
		msg->setBody("hi there!");

		// Forward it
		conv->handleMessage(msg);
		loop->processEvents();
		
		CPPUNIT_ASSERT_EQUAL(1, (int) received.size());
		CPPUNIT_ASSERT(dynamic_cast<Swift::Message *>(getStanza(received[0])));
		CPPUNIT_ASSERT_EQUAL(std::string("hi there!"), dynamic_cast<Swift::Message *>(getStanza(received[0]))->getBody());
		CPPUNIT_ASSERT_EQUAL(std::string("user@localhost"), dynamic_cast<Swift::Message *>(getStanza(received[0]))->getTo().toString());
		CPPUNIT_ASSERT_EQUAL(std::string("buddy1@localhost/bot"), dynamic_cast<Swift::Message *>(getStanza(received[0]))->getFrom().toString());
		
		received.clear();

		// send response
		msg->setFrom("user@localhost/resource");
		msg->setTo("buddy1@localhost/bot");
		msg->setBody("response!");
		injectMessage(msg);
		loop->processEvents();
		
		CPPUNIT_ASSERT_EQUAL(0, (int) received.size());
		CPPUNIT_ASSERT(m_msg);
		CPPUNIT_ASSERT_EQUAL(std::string("response!"), m_msg->getBody());

		// send another message from legacy network, should be sent to user@localhost/resource now
		boost::shared_ptr<Swift::Message> msg2(new Swift::Message());
		msg2->setBody("hi there!");

		// Forward it
		conv->handleMessage(msg2);
		loop->processEvents();

		CPPUNIT_ASSERT_EQUAL(1, (int) received.size());
		CPPUNIT_ASSERT(dynamic_cast<Swift::Message *>(getStanza(received[0])));
		CPPUNIT_ASSERT_EQUAL(std::string("hi there!"), dynamic_cast<Swift::Message *>(getStanza(received[0]))->getBody());
		CPPUNIT_ASSERT_EQUAL(std::string("user@localhost/resource"), dynamic_cast<Swift::Message *>(getStanza(received[0]))->getTo().toString());
		CPPUNIT_ASSERT_EQUAL(std::string("buddy1@localhost/bot"), dynamic_cast<Swift::Message *>(getStanza(received[0]))->getFrom().toString());
		
		received.clear();

		// and now to bare JID again...
		user->getConversationManager()->resetResources();
		conv->handleMessage(msg2);
		loop->processEvents();
		
		CPPUNIT_ASSERT_EQUAL(1, (int) received.size());
		CPPUNIT_ASSERT(dynamic_cast<Swift::Message *>(getStanza(received[0])));
		CPPUNIT_ASSERT_EQUAL(std::string("hi there!"), dynamic_cast<Swift::Message *>(getStanza(received[0]))->getBody());
		CPPUNIT_ASSERT_EQUAL(std::string("user@localhost"), dynamic_cast<Swift::Message *>(getStanza(received[0]))->getTo().toString());
		CPPUNIT_ASSERT_EQUAL(std::string("buddy1@localhost/bot"), dynamic_cast<Swift::Message *>(getStanza(received[0]))->getFrom().toString());
		
		received.clear();
	}

	void handleGroupchatMessages() {
		User *user = userManager->getUser("user@localhost");
		TestingConversation *conv = new TestingConversation(user->getConversationManager(), "#room", true);
		user->getConversationManager()->addConversation(conv);
		conv->onMessageToSend.connect(boost::bind(&ConversationManagerTest::handleMessageReceived, this, _1, _2));
		conv->setNickname("nickname");
		conv->setJID("user@localhost/resource");

		// reset resources should not touch this resource
		user->getConversationManager()->resetResources();

		boost::shared_ptr<Swift::Message> msg(new Swift::Message());
		msg->setBody("hi there!");

		// Forward it
		conv->handleMessage(msg, "anotheruser");

		loop->processEvents();
		CPPUNIT_ASSERT_EQUAL(1, (int) received.size());
		CPPUNIT_ASSERT(dynamic_cast<Swift::Message *>(getStanza(received[0])));
		CPPUNIT_ASSERT_EQUAL(std::string("hi there!"), dynamic_cast<Swift::Message *>(getStanza(received[0]))->getBody());
		CPPUNIT_ASSERT_EQUAL(std::string("user@localhost/resource"), dynamic_cast<Swift::Message *>(getStanza(received[0]))->getTo().toString());
		CPPUNIT_ASSERT_EQUAL(std::string("#room@localhost/anotheruser"), dynamic_cast<Swift::Message *>(getStanza(received[0]))->getFrom().toString());

		received.clear();

		// send response
		msg->setFrom("user@localhost/resource");
		msg->setTo("#room@localhost");
		msg->setBody("response!");
		msg->setType(Swift::Message::Groupchat);
		injectMessage(msg);
		loop->processEvents();
		
		CPPUNIT_ASSERT_EQUAL(0, (int) received.size());
		CPPUNIT_ASSERT(m_msg);
		CPPUNIT_ASSERT_EQUAL(std::string("response!"), m_msg->getBody());
	}

	void handleParticipantChanged() {
		User *user = userManager->getUser("user@localhost");
		TestingConversation *conv = new TestingConversation(user->getConversationManager(), "#room", true);
		
		conv->onMessageToSend.connect(boost::bind(&ConversationManagerTest::handleMessageReceived, this, _1, _2));
		conv->setNickname("nickname");
		conv->setJID("user@localhost/resource");

		// normal presence
		conv->handleParticipantChanged("anotheruser", 0, Swift::StatusShow::Away, "my status message");
		loop->processEvents();

		CPPUNIT_ASSERT_EQUAL(1, (int) received.size());
		CPPUNIT_ASSERT(dynamic_cast<Swift::Presence *>(getStanza(received[0])));
		CPPUNIT_ASSERT_EQUAL(Swift::StatusShow::Away, dynamic_cast<Swift::Presence *>(getStanza(received[0]))->getShow());
		CPPUNIT_ASSERT_EQUAL(std::string("user@localhost/resource"), dynamic_cast<Swift::Presence *>(getStanza(received[0]))->getTo().toString());
		CPPUNIT_ASSERT_EQUAL(std::string("#room@localhost/anotheruser"), dynamic_cast<Swift::Presence *>(getStanza(received[0]))->getFrom().toString());
		CPPUNIT_ASSERT(getStanza(received[0])->getPayload<Swift::MUCUserPayload>());
		CPPUNIT_ASSERT_EQUAL(Swift::MUCOccupant::Member, *getStanza(received[0])->getPayload<Swift::MUCUserPayload>()->getItems()[0].affiliation);
		CPPUNIT_ASSERT_EQUAL(Swift::MUCOccupant::Participant, *getStanza(received[0])->getPayload<Swift::MUCUserPayload>()->getItems()[0].role);

		received.clear();

		// this user presence - status code 110
		conv->handleParticipantChanged("nickname", 1, Swift::StatusShow::Away, "my status message");
		loop->processEvents();

		CPPUNIT_ASSERT_EQUAL(1, (int) received.size());
		CPPUNIT_ASSERT(dynamic_cast<Swift::Presence *>(getStanza(received[0])));
		CPPUNIT_ASSERT_EQUAL(Swift::StatusShow::Away, dynamic_cast<Swift::Presence *>(getStanza(received[0]))->getShow());
		CPPUNIT_ASSERT_EQUAL(std::string("user@localhost/resource"), dynamic_cast<Swift::Presence *>(getStanza(received[0]))->getTo().toString());
		CPPUNIT_ASSERT_EQUAL(std::string("#room@localhost/nickname"), dynamic_cast<Swift::Presence *>(getStanza(received[0]))->getFrom().toString());
		CPPUNIT_ASSERT(getStanza(received[0])->getPayload<Swift::MUCUserPayload>());
		CPPUNIT_ASSERT_EQUAL(Swift::MUCOccupant::Admin, *getStanza(received[0])->getPayload<Swift::MUCUserPayload>()->getItems()[0].affiliation);
		CPPUNIT_ASSERT_EQUAL(Swift::MUCOccupant::Moderator, *getStanza(received[0])->getPayload<Swift::MUCUserPayload>()->getItems()[0].role);
		CPPUNIT_ASSERT_EQUAL(110, getStanza(received[0])->getPayload<Swift::MUCUserPayload>()->getStatusCodes()[0].code);

		received.clear();

		// renamed - status code 303
		conv->handleParticipantChanged("anotheruser", 1, Swift::StatusShow::Away, "my status message", "hanzz");
		loop->processEvents();

		CPPUNIT_ASSERT_EQUAL(2, (int) received.size());
		CPPUNIT_ASSERT(dynamic_cast<Swift::Presence *>(getStanza(received[0])));
		CPPUNIT_ASSERT_EQUAL(Swift::Presence::Unavailable, dynamic_cast<Swift::Presence *>(getStanza(received[0]))->getType());
		CPPUNIT_ASSERT_EQUAL(std::string("user@localhost/resource"), dynamic_cast<Swift::Presence *>(getStanza(received[0]))->getTo().toString());
		CPPUNIT_ASSERT_EQUAL(std::string("#room@localhost/anotheruser"), dynamic_cast<Swift::Presence *>(getStanza(received[0]))->getFrom().toString());
		CPPUNIT_ASSERT(getStanza(received[0])->getPayload<Swift::MUCUserPayload>());
		CPPUNIT_ASSERT_EQUAL(Swift::MUCOccupant::Admin, *getStanza(received[0])->getPayload<Swift::MUCUserPayload>()->getItems()[0].affiliation);
		CPPUNIT_ASSERT_EQUAL(Swift::MUCOccupant::Moderator, *getStanza(received[0])->getPayload<Swift::MUCUserPayload>()->getItems()[0].role);
		CPPUNIT_ASSERT_EQUAL(std::string("hanzz"), *getStanza(received[0])->getPayload<Swift::MUCUserPayload>()->getItems()[0].nick);
		CPPUNIT_ASSERT_EQUAL(303, getStanza(received[0])->getPayload<Swift::MUCUserPayload>()->getStatusCodes()[0].code);
	}

	void handlePMFromXMPP() {
		User *user = userManager->getUser("user@localhost");
		TestingConversation *conv = new TestingConversation(user->getConversationManager(), "#room", true);
		user->getConversationManager()->addConversation(conv);
		conv->onMessageToSend.connect(boost::bind(&ConversationManagerTest::handleMessageReceived, this, _1, _2));
		conv->setNickname("nickname");
		conv->setJID("user@localhost/resource");

		conv->handleParticipantChanged("anotheruser", 0, Swift::StatusShow::Away, "my status message");
		loop->processEvents();

		received.clear();
		boost::shared_ptr<Swift::Message> msg(new Swift::Message());
		msg->setBody("hi there!");
		msg->setFrom("user@localhost/resource");
		msg->setTo("#room@localhost/anotheruser");
		msg->setBody("hi there!");
		injectMessage(msg);
		loop->processEvents();

		CPPUNIT_ASSERT_EQUAL(0, (int) received.size());
		CPPUNIT_ASSERT(m_msg);
		CPPUNIT_ASSERT_EQUAL(std::string("hi there!"), m_msg->getBody());
		CPPUNIT_ASSERT_EQUAL(std::string("#room/anotheruser"), m_conv->getLegacyName());

		Conversation *pmconv = user->getConversationManager()->getConversation("#room/anotheruser");

		boost::shared_ptr<Swift::Message> msg2(new Swift::Message());
		msg2->setBody("response!");

		pmconv->handleMessage(msg2);
		dumpReceived();
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION (ConversationManagerTest);