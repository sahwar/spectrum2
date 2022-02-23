### Version 2.1.3 (2022-02-18)
#### General
* minor code refactoring/cleanup, see Git log for details
#### XMPP frontend
* map attachment descriptions to `{jabber:x:oob}desc` element
* do not send carbon copy to the originated entity
#### libpurple backend
* forward Xfer remote filename as attachment description
___
### Version 2.1.2 (2022-02-03)
#### libpurple backend
* Fix issues with filetransfers: always use unique filename for incoming file
___
### Version 2.1.1 (2022-02-03)
#### General
* Fix MySQL 8.x compatibility
* Various minor code improvements, see commit log
#### libpurple backend
* Fix issues with filetransfers: files automatically downloaded to `web.directory`
* Add support for the purple-gowhatsapp QR code auth
* Fix crash when processing `[purple]` boolean config options
#### Docker
* Updated base image to Debian 11
* Added example `docker-compose` configuration
#### Web interface
* Fix security issues found by CodeQL scans
* Use third-party scripts from CDN
___
### Version 2.1.0 (2021-10-15)
#### General
* Swiften 4.0 is now minimum supported Swiften version
* CMake 3.7 is now minimum supported CMake version (which is present in Debian oldoldstable)
* improve compatibility with latest build tools and dependencies
* Fix service discovery compatibility with some clients (Gajim)
#### libpurple backend
* Improved support for passwordless backends
___
### Version 2.0.13 (2020-07-13)
#### General
* improve compatibility with latest build tools
#### libpurple backend
* fix random crashes
* telegram-tdlib support
___
### Version 2.0.12 (2019-07-15)
#### libpurple backend
* revert changes which causes crash on start
___
### Version 2.0.11 (2019-07-12)
#### General
* Minor compatibility fixes with latest packaging tools
* 2FA support for Steam libpurple plugin
* Improved image and carbons forwarding
___
### Version 2.0.10 (2019-01-25)
#### General
* Compatibility with latest compilers and libraries 
#### Libpurple backend
* 2FA support for Discord plugin
* Compatibility with non-protocol plugins (nss-prefs)
* Improved room list forwarding
* Improved image forwarding
___
### Version 2.0.9 (2018-09-22)
#### General
* Finally fix MySQL storage backend issues
___
### Version 2.0.8 (2018-09-17)
#### General
* Fix some MySQL storage backend issues
* Appveyor build integration
#### Libpurple backend
* Fix Steam Guard compatibility in Steam Mobile plugin
* Fix VCard forwarding and room joining issues
#### Twitter backend
* 280 characters support
* Support for Direct Messages is disabled until switching to new Direct Message API
#### Libcommuni backend
* split multiline messages into series of PRIVMSG commands
___
### Version 2.0.7 (2018-04-27)
#### General
* Fix Web interface and Twitter backend compatibilty with Swiften 4 by switching to C++11-compatible JSON library
* Minor changes in packaging and build environment
#### XMPP
* Fix XEP-0085 compatibility with some clients
___
### Version 2.0.6 (2018-02-07)
#### General
* Add support for forwading own messages sent from different legacy network client.
#### XMPP frontend
* Forward own messages as XEP-280 Carbons. Requires XEP-0356 capable server and transport should be configured as privileged entity for outgoing messages.
* Fixed compatibility with XEP-0356: now it can be used to provide remote roster management support.
* Forward images using XEP-0066 attachments. May not be compatible with all backends.
#### Libpurple backend
* Forward own messages to frontend. Tested with prpl-skypeweb.
* Fixed compatibility with latest prpl-steam-mobile
#### Minor bugfixes
* packaging updates and improvements
* better compatibility with newest compilers and libraries
___
### Version 2.0.5 (2017-08-09)
#### Minor bugfixes
* fix purple-hangouts login
* fix Debian packages dependencies
___
### Version 2.0.4 (2017-08-03)
#### General
* Add support for Swiften 4.0rc2. Swiften 3.x and 2.x are still supported.
* Add support for passwordless libpurple backends (e.g. purple-hangouts).
* Partial support for groupchats with case-sensitive names (e.g. purple-telegram).
* Various small bugfixes, see Git commit descriptions for the full list of changes.
___	
### Version 2.0.3 (2016-02-29)
#### General
* Swift 3.0rc2 is now minimum Swift 3.x version needed to compile Spectrum2, Swith 2.x remains supported.
* Rename user when he tries to connect to room from the second client with different nickname than he used with the first client.
* Send self presence as the first presence from the MUC room.
* Send the icon hash to the backend with other buddies information.
* Send unavailable presence as a response to presence for exiting the room. Fixes room rejoin in Pidgin.
* Fix memory leak in HTTPRequest - currently used only in Slack frontend.
#### Slack frontend
* For IRC allow registering without providing account name/password.
* Do not reconnect to Slack RTM when no URL is available after disconnect.
* Handle "account_inactive" Slack error.

#### Web interface
* Show warning when trying to register already registered username.
* Allow registering with empty password for networks like IRC where password is not needed.
* Allow showing examples of username or server in registration form. This is configured in config file.
* Fix `service.base_location` setting for register.shtml page.

#### Libpurple backend
* Make the conversation active according to chatstates notifications. Should fix the issue with repeating messages with prpl-facebook.
* When `service.web_directory` and `service.web_url` is set, images sent in messages are stored to `service.web_directory` and link is forwarded to the user. Tested only with prpl-facebook for now.
* Treat empty password as fatal error - do no try to reconnect automatically in that case.

#### Libcommuni backend
* Latest libcommuni 3.4.0 is now needed.
* Libcommuni backend has been improved to use the newest libcommuni features.
* Ports can now be set in `service.irc_server` option and in the JID. When '+' character is used before the port number, SSL is used automatically. When port 6697 is used, SSL is used automatically too.
#### Swiften backend
* Respect the resource when connecting to 3rd-party XMPP network.
___
### Version 2.0.2 (2016-02-09)
#### General
* Fix joining the room when using JID Escaping even for '@' character.
* Fix Raw XML mode - fixes the Swiften backend.
* Allow backends to set avatars to buddies in room - currently no backend uses it.
* Set the room list per frontend user - this fixes possible information for networks like Skype or Facebook where the room list is not global per network.
* Support usage of aliases in the rooms, so backends can use the network-based UID for the buddies in the room while still setting user-friendly aliases to them.
* Fix leaving the room when user is connected to transport using two clients, but only one of them joined the room and that one disconnects the transport without leaving the room (For example when he just quits the client).
#### XMPP frontend
* Handle the XMPP VCard changes - updating avatar should work now for backends supporting it.
#### Slack frontend
* Slack frontend is now completely managed by the web interface, old way of managing (".spectrum2 ..." commands sent to Spectrum 2 bot) are not supported.
* Slack channels are now created automatically on Slack when you join them using web-interface. The same applies also to the Main channel.
* Logging has been improved to show the name of Slack team for each log line.
#### Libpurple backend
* Forward the room topic to frontend.
* Fix joining the rooms on networks when the nickname in the room cannot be changed (Skype or Facebook). Libpurple backend changes your nickname to the right one in that case.
* Show the buddies aliases in the Facebook rooms instead of Facebook buddies numbers.
#### Twitter backend
* Fix compilation with newer gcc.
#### Swiften backend
* Swiften backend works as before the frontends introduction.
#### Skype backend
* The old skype backend has been removed. Use purple-skypeweb instead.
#### Web interface
* Allow setting "port" and "base_location" in the config file.
* Allow maintaining joined rooms in the Web interface for the Slack frontend.
* "[identity] name" config variables is now used as a name of the transport.
___
### Version 2.0.1 (2016-01-25)
#### General
* Fix wrong nickname when joining room with some backends.
* Leave room when disconnected from 3rd-party network because of an error. Fixes a bug when users were not able to rejoin the room after Spectrum 2 disconnected them from 3rd-party network.
* Fix building on Windows.
* Support case-sensitive buddy names transparently - boost_locale is now needed as a new dependency.
#### Spectrum2_manager
* Add simple web interface, can be tested using `spectrum2_manager server`.
#### XMPP frontend
* Show MUC feature in room disco#info response.
#### Slack frontend
* Slack frontend is now working as Slack app, so you need to obtain the client_id and client_secret from Slack to setup your own server and to do the OAuth2 flow, you have to setup the Spectrum2_manager Server. Read the documentation for more info.
* Fix OAuth2 registration flow.
* Fix multiple crashes when disconnected from Slack network or from 3rd-party network.
* Slack frontend should still be used only for testing, it is till under the development.
#### Libpurple backend
* Forward room list to frontend.
* Fix compatibility with latest telegram-purple - You mean need to unregister the transport and register it again if you experience compatibility problems.
#### Twitter backend
* Fix default value of fetch timer.
* Disable useless Curl verbose output.
* Fix crash when populating roster when using Swiften 3.
* Do not set status message for buddies not in Friends list - fixes a bug when buddies who are not in Friends list have been added to roster.
___
### Version 2.0.0 (2015-12-29)
#### General
* Fix compilation with new versions of Spectrum 2 dependencies.
* Added support for multiple frontends - Slack frontend is the first one.
* Log errors related to backend spawning (Show proper log message for example when path to backend binary is not found).
* Set SQLite3 as default database backend.
* Fixed disconnecting from server caused by sending VCard response with bad "from" attribute.
* Added Munin plugin (Thanks to Askovpen).
* Added support for more admin_jid JIDs (Thanks to Askovpen).
* Fixed allowed_servers option.
* Options from config file can now be set also using command line like `--service.jid=domain.tld`.
* Do not send password in IQ-get registration response.
* Added support for AdHoc commands.
* Do not store buddies with empty name in database.
* Improved MySQL storage backend performance.
* Fix crash caused by two XMPP users using single PurpleAccount instance.
* Support for [registration] allowed_usernames.
#### Spectrum2_manager
* Rewritten to provide more features. Check the documentation.
#### Libpurple
* prpl-gg: Fetch the contact list properly (#252).
* Added support for prpl-novell as it was in Spectrum 1.
#### Twitter
* Added Twitter support using Twitter backend. Thanks to Sarang and Google Summer of Code.
#### Skype
* Skype backend is now deprecated, use libpurple purple-skypeweb plugin as a replacement.
#### Backend API
* Added Python NetworkPlugin class, so it is now easier to write backends in Python (Thanks to Sarang).
___
### Version 2.0.0-beta2 (2012-03-28)
#### General
* Fixed bug when Roster Item Exchange and subscribe stanzas were sent repeatedly.
* Backends related logs now contain the backend PID.
* Fixed username_mask setting.
* Added new fields into statistics (backends_crashed, messages related stats).
* Chatstates are now not counted as incoming messages in stats.
* Log4cxx is now optional dependency. Without Log4cxx, Spectrum 2 logs to standard output.
* Fixed crash when Log4cxx configuration file didn't exist.
* Admin can now see "Admin" contact in server-mode.
#### libpurple
* Added initial support for MUC for prpl-jabber protocol.
#### LibCommuni IRC backend
* Fixed sending/receiving UTF8 messages.
* Using the [registration] auto_register=1 config option, users don't have to register manually when connecting IRC network.
#### Skype
* Memory usage statistic now includes the Skype client.
* Fixed logging issue when the logs were not stored in the proper instance directory.
* Skype backend includes also Skype client memory usage into the account.
* Working buddies adding/removing.
* Information about missed call is now forwarded to XMPP user.
* Fixed bug when Skype client instance wasn't killed by backend.
___
### Version 2.0.0-beta (2012-02-28)
#### General
* Added PostreSQL support (thanks to Jadestorm).
* Added XEP-0100 (Gateway interaction) support.
* Send presences only "from" bare JID (fixed bug with buddies appearing twice in the roster and potential unregistering issues).
* Fixed potential MySQL/SQLite3 deadlocks.
* Fixed disconnecting in server-mode when client does not send unavailable presence before disconnection.
* Fixed crash in server-mode when client send its custom jabber:iq:storage payload.
* Fixed registration from Pidgin.
* Unsubscribe presence sent to some buddy doesn't disconnect the account.
* Remote Roster requests are not sent to resources, but to bare JID.
* Added automatic reconnection in case of non-fatal error.
* Added more error messages.
#### Skype
* Initial support for Skype added, read more on
	  http://spectrum.im/projects/spectrum/wiki/Spectrum_2_Admin_-_Skype_backend

#### SMSTools3
* Initial support for SMSTools3, read more on
	http://spectrum.im/projects/spectrum/wiki/Spectrum_2_Admin_-_SMSTools3_backend
___
### Version 2.0.0 alpha (2011-12-06)
#### General
* First Spectrum 2.0.0 alpha release, check more on http://spectrum.im/projects/spectrum/wiki/Spectrum_200_alpha
___
