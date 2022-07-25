//
// Created by huxy on 23/02/2020.
//

#ifndef GAMELIB_GCNETSERVER_HPP
#define GAMELIB_GCNETSERVER_HPP

#include <atomic>
#include <kissnet.hpp>
#include <list>

#include "Packet.hpp"
#include "../ASGEServer.hpp"

class GCNetServer
{
public:
	GCNetServer(ASGEServer* param);
	~GCNetServer();
	void update(double dt);
	void start();

	GCNetServer(const GCNetServer&) = delete;
	GCNetServer& operator=(const GCNetServer&) = delete;

	void send(const Packet& packet);

private:
	ASGEServer* asgeServer;
	kissnet::tcp_socket server;
	std::list<kissnet::tcp_socket> connections;
	std::atomic<bool> accept_connections = false;
};

#endif // GAMELIB_GCNETSERVER_HPP
