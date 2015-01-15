#ifndef __RAKNET_GLOBALS__
#define __RAKNET_GLOBALS__

#include "RakPeerInterface.h"
#include "BitStream.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"

#define SERVER_PORT 60000
#define CLIENT_PORT 49999
#define MAX_CLIENTS 1

extern RakNet::RakPeerInterface* peer;
extern RakNet::SystemAddress otherPlayer;
extern bool isServer;
extern bool isClient;
extern bool isConnected;
extern bool otherGameOver;

extern int currentLevel;
extern int p1Score;
extern int p2Score;
extern float p1BulletX;
extern float p1BulletY;
extern float p2BulletX;
extern float p2BulletY;
extern int timeStamp;
extern int p1Time;
extern int p2Time;
extern int isP1;
extern int isP2;

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM+1,
	ID_GAME_START,
	ID_GAME_PLAYER_LOST,
	ID_GAME_NEW_POINTS,
	ID_GAME_SCORE,
	ID_GAME_BULLET,
	ID_GAME_ENEMY_POS,
	ID_GAME_INIT_SWAP,
	ID_GAME_NEXT_LEVEL,
	ID_GAME_TIME_STAMP,
	ID_GAME_OVER_SERVER,
	ID_GAME_OVER_CLIENT
};

#endif // __RAKNET_GLOBALS__