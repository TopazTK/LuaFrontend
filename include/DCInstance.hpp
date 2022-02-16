#pragma once

#include <chrono>
#include <Windows.h>
#include <discord_rpc.h> 
#include <discord_register.h>

using namespace std;

class DCInstance {
public:
	static inline DiscordRichPresence Presence;

	static void InitializeRPC(const char* applicationID)
	{
		Discord_Initialize(applicationID, NULL, 1, NULL); 

		memset(&DCInstance::Presence, 0, sizeof(DCInstance::Presence));

		auto _currTime = chrono::system_clock::now().time_since_epoch();
		auto _secondCast = chrono::duration_cast<chrono::seconds>(_currTime);

		DCInstance::Presence.startTimestamp = _secondCast.count();
		Discord_UpdatePresence(&DCInstance::Presence);
	}

	static void UpdateDetails(const char* input)
	{
		DCInstance::Presence.details = input;
		Discord_UpdatePresence(&DCInstance::Presence);
	}

	static void UpdateState(const char* input)
	{
		DCInstance::Presence.state = input;
		Discord_UpdatePresence(&DCInstance::Presence);
	}

	static void UpdateLImage(const char* key, const char* text)
	{
		DCInstance::Presence.largeImageKey = key;
		DCInstance::Presence.largeImageText = text;
		Discord_UpdatePresence(&DCInstance::Presence);
	}

	static void UpdateSImage(const char* key, const char* text)
	{
		DCInstance::Presence.smallImageKey = key;
		DCInstance::Presence.smallImageText = text;
		Discord_UpdatePresence(&DCInstance::Presence);
	}
};