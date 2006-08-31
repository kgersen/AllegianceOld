/*-------------------------------------------------------------------------
 * \src\Lobby\MessagesLS.h
 * 
 * Interface between Lobby and AllSrv
 * 
 * Owner: 
 * 
 * Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _MESSAGES_LS_H_
#define _MESSAGES_LS_H_

#include "MessageCore.h"

#define LOBBYVER_LS 5 // If you change this file, increment this


/*
  *************************************************
                 MESSAGES START HERE  

         Messages range for this file is 201 - 250

  *************************************************

  Message prefixes:
    L_  lobby to server
    S_  server to lobby
    LS_ both ways
*/

// when AllSrv announces to lobby that it's there. This does not necessarily 
// mean that the server will have games created on it, since the Lobby has a list of creatable servers.
DEFINE_FEDMSG(S, LOGON_LOBBY, 201) 
  short verLobby;
  DWORD dwPort;		// mdvalley: Server passes port to lobby. As good a place as any to do it. 
END_FEDMSG

DEFINE_FEDMSG(S, LOGOFF_LOBBY, 202) 
END_FEDMSG

DEFINE_FEDMSG(L, CREATE_MISSION_REQ, 203)
    FM_VAR_ITEM(NameCreator);
    DWORD dwCookie; // server will return this when mission is created so we know which is which
                    // this is only guaranteed to be unique among currently running missions
                    // Really a CFLServer*
    // The lobby expects a lobbymissioninfo in response to this.
END_FEDMSG

DEFINE_FEDMSG(S, NEW_MISSION, 204) // tells the lobby that we created a mission without being told to.
                                   // We need to get this cookie translated into a lobby cookie--see below
    DWORD dwIGCMissionID; // Aka old cookie
END_FEDMSG

DEFINE_FEDMSG(L, NEW_MISSION_ACK, 205) // Sent for missions created by servers that were not player-created.
                                       // E.g., object model-created. no one will be able to join until the server gets this
    DWORD dwIGCMissionID; // this is what was passed up in FM_S_NEW_MISSION
    DWORD dwCookie; // pointer to a lobby mission
    // The lobby expects a lobbymissioninfo in response to this.
END_FEDMSG

DEFINE_FEDMSG(S, HEARTBEAT, 206)
END_FEDMSG
    
DEFINE_FEDMSG(L, QUIT, 207) // the lobby is going away, so the server shold do likewise after finishing the current game
END_FEDMSG

DEFINE_FEDMSG(L, TOKEN, 208) // what token to use to authorize against
  FM_VAR_ITEM(Token);
END_FEDMSG

DEFINE_FEDMSG(S, PLAYER_JOINED, 209) // a new player has joined - tell the lobby.
  FM_VAR_ITEM(szCharacterName); 
  FM_VAR_ITEM(szCDKey); // Scrambled using CharacterName
  DWORD dwMissionCookie;
END_FEDMSG

DEFINE_FEDMSG(S, PLAYER_QUIT, 210) // a player quit the game
  FM_VAR_ITEM(szCharacterName); 
  DWORD dwMissionCookie;
END_FEDMSG

enum RemovePlayerReason
{
    RPR_duplicateName,
    RPR_duplicateCDKey
};

DEFINE_FEDMSG(L, REMOVE_PLAYER, 211) // this player is joining another game - boot the old copy
  FM_VAR_ITEM(szCharacterName); 
  FM_VAR_ITEM(szMessageParam); 
  DWORD dwMissionCookie;
  RemovePlayerReason reason;
END_FEDMSG

DEFINE_FEDMSG(S, PAUSE, 212) // server's intent to pause/unpause
  bool fPause;
END_FEDMSG

DEFINE_FEDMSG(L, LOGON_SERVER_NACK, 213)  // tells game server that they can't join the lobby.
  FM_VAR_ITEM(Reason);
END_FEDMSG

#endif // _MESSAGES_LS_H_

