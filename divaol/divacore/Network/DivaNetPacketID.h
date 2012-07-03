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

			DIVA_NET_CTS_LOBBY_LOGIN, //��¼������: id(int), name(string)
			DIVA_NET_CTS_LOBBY_LOGOUT, //�ǳ�
			DIVA_NET_CTS_LOBBY_READY, //׼��
			DIVA_NET_CTS_LOBBY_UNREADY, //ȡ��׼��
			DIVA_NET_CTS_LOBBY_START, //��Ϸ��ʼ,����: mode(1,2,3), songID(id)
			DIVA_NET_CTS_LOBBY_READY_START, //׼����ʼ
			DIVA_NET_CTS_LOBBY_GAME_OVER, //��Ϸ���������ط���
			DIVA_NET_STC_LOBBY_START_REPLY, //�����������Ƿ��ܿ�ʼ��Ϸ
			DIVA_NET_STC_LOBBY_FULL, //��������
			DIVA_NET_STC_LOBBY_YOUR_INFO, //���info, ������λ��(int)
			DIVA_NET_STC_LOBBY_ALL_INFO, //�����˵�info, ����������(const 4), (ÿ��) bVoid,bReady,bPlaying,slot,id,name 
			DIVA_NET_STC_LOBBY_LOGIN, //���˵�¼������:λ�ã�id,name
			DIVA_NET_STC_LOBBY_LOGOUT, //���˵ǳ�������:λ��
			//DIVA_NET_STC_LOBBY_READY,
			//DIVA_NET_STC_LOBBY_UNREADY,
			DIVA_NET_STC_LOBBY_START, //׼����ʼ������: mode, ��Ϸ��λ��, songid
			DIVA_NET_STC_LOBBY_GAME_ON, //��Ϸ��ʽ��ʼ
			DIVA_NET_STC_LOBBY_UPDATE_SINGLE //����ĳ���˵���Ϣ,λ�ã�bReady,bPlaying
		};
	}
}

#endif