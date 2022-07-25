//
// Created by huxy on 23/02/2020.
//

#ifndef NETGAME_GCNETCLIENT_HPP
#define NETGAME_GCNETCLIENT_HPP
#include <kissnet.hpp>
#include <string>
#include "../ASGEGame.hpp"
#include "Packet.hpp"

class GCNetClient
{
public:



	GCNetClient(ASGENetGame* param);
	~GCNetClient();
	void connect(const std::string& server_ip, unsigned short server_port);
	void update(double dt);

	void send(const Packet& packet);

	GCNetClient(const GCNetClient&) = delete;
	GCNetClient& operator=(const GCNetClient&) = delete;
		

private:
	ASGENetGame* asgeNetGame;

	kissnet::tcp_socket socket;
};



#endif // NETGAME_GCNETCLIENT_HPP
