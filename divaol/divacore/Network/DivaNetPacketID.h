/*
 *  DivaNetPacketID.h
 *
 *  Created by Hyf042 on 3/4/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_NET_PACKET_ID
#define DIVA_NET_PACKET_ID

namespace divacore
{
	namespace network
	{
		enum{
			//CTS - client to server
			//STC - server to client
			DIVA_NET_CTS_LOGIN,
			DIVA_NET_CTS_READY,
			DIVA_NET_CTS_UPDATE_INFO,
			DIVA_NET_CTS_FAILURE,
			DIVA_NET_CTS_EVAL_INFO,
			DIVA_NET_STC_FULL,
			DIVA_NET_STC_START,
			DIVA_NET_STC_PLAYER_INFO,
			DIVA_NET_STC_UPDATE_INFO,
			DIVA_NET_STC_FAILURE,
			DIVA_NET_STC_UPDATE_EVAL,
			//for relay game
			DIVA_NET_CTS_RELAY_WANT_TO_CHANGE,
			DIVA_NET_CTS_RELAY_WANT_TO_PLAY,
			DIVA_NET_CTS_RELAY_I_AM_PLAYING,
			DIVA_NET_CTS_RELAY_GIVE_UP,
			DIVA_NET_CTS_RELAY_RENEW,
			DIVA_NET_STC_RELAY_RENEW,
			DIVA_NET_STC_RELAY_TIME_UP,
			DIVA_NET_STC_RELAY_CHANCE,
			DIVA_NET_STC_RELAY_NEW_PLAYER,
			DIVA_NET_STC_RELAY_YOU_CAN_PLAY,
			DIVA_NET_STC_PLAYER_LEFT,
			DIVA_NET_STC_RELAY_VOID_PERIOD,

			DIVA_NET_CTS_LOBBY_LOGIN, //登录，参数: id(int), name(string)
			DIVA_NET_CTS_LOBBY_LOGOUT, //登出
			DIVA_NET_CTS_LOBBY_READY, //准备
			DIVA_NET_CTS_LOBBY_UNREADY, //取消准备
			DIVA_NET_CTS_LOBBY_START, //游戏开始,参数: mode(1,2,3), songID(id)
			DIVA_NET_CTS_LOBBY_READY_START, //准备开始
			DIVA_NET_CTS_LOBBY_GAME_OVER, //游戏结束，返回房间
			DIVA_NET_STC_LOBBY_START_REPLY, //回馈给房主是否能开始游戏
			DIVA_NET_STC_LOBBY_FULL, //房间已满
			DIVA_NET_STC_LOBBY_YOUR_INFO, //你的info, 参数：位置(int)
			DIVA_NET_STC_LOBBY_ALL_INFO, //所有人的info, 参数：人数(const 4), (每人) bVoid,bReady,bPlaying,slot,id,name 
			DIVA_NET_STC_LOBBY_LOGIN, //有人登录，参数:位置，id,name
			DIVA_NET_STC_LOBBY_LOGOUT, //有人登出，参数:位置
			//DIVA_NET_STC_LOBBY_READY,
			//DIVA_NET_STC_LOBBY_UNREADY,
			DIVA_NET_STC_LOBBY_START, //准备开始，参数: mode, 游戏中位置, songid
			DIVA_NET_STC_LOBBY_GAME_ON, //游戏正式开始
			DIVA_NET_STC_LOBBY_UPDATE_SINGLE //更新某个人的信息,位置，bReady,bPlaying
		};
	}
}

#endif