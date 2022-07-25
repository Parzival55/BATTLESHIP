//
// Created by huxy on 23/02/2020.
//

#include "GCNetClient.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;
namespace kn = kissnet;

GCNetClient::GCNetClient(ASGENetGame* param): asgeNetGame(param) {

}

void GCNetClient::update(double /*dt*/) {	
}
void GCNetClient::send(const Packet& packet) {	
	socket.send(reinterpret_cast<const std::byte*>(&packet), sizeof(packet));
}


void GCNetClient::connect(const std::string& server_ip, unsigned short server_port)
{
	socket = (kissnet::endpoint{ server_ip, server_port });
	socket.connect();

	
	std::thread thread(
		[&]()
		{
			while (true)
			{
				//=== recieve ===//
				kn::buffer<1024> buff;
				const auto [size, status] = socket.recv(buff);
				if (size < buff.size()) buff[size] = std::byte{ 0 };

				auto ptr = reinterpret_cast<std::byte*>(&buff);
				auto buffer = std::vector<std::byte>(ptr, ptr + sizeof(buff));
				auto packet = reinterpret_cast<Packet*>(&buffer[0]);

				asgeNetGame->handlePacket(*packet);
			}
		});
	thread.detach();
	
}

GCNetClient::~GCNetClient()
{
	socket.close();
}
