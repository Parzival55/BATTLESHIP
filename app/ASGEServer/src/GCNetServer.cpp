//
// Created by huxy on 23/02/2020.
//

#include "GCNetServer.hpp"
#include <iostream>
#include <kissnet.hpp>
#include <thread>
#include <vector>

namespace kn = kissnet;


namespace
{
	kissnet::tcp_socket bind(const std::string& ip_address, kissnet::port_t port)
	{
		// create endpoint and assigning to a socket
		kissnet::tcp_socket server(kissnet::endpoint(ip_address, port));

		// bind and listen on
		server.bind();
		server.listen();

		// return it
		return server;
	}
} // namespace

GCNetServer::GCNetServer(ASGEServer* param) :asgeServer(param) {

}

GCNetServer::~GCNetServer()
{
	accept_connections = false;
	server.close();
}

void GCNetServer::start()
{
	server = bind("0.0.0.0", 31276);
	accept_connections = true;

	std::thread thread(
		[&]()
		{
			while (accept_connections)
			{
				auto& socket = connections.emplace_back(server.accept());
				std::cout << "connection received from: " << socket.get_recv_endpoint().address << ":"
					<< socket.get_recv_endpoint().port << std::endl;

				//========================================
				while (true) {
					//=== recieve ===//
					kn::buffer<1024> buff;
					const auto [size, status] = socket.recv(buff);
					if (size < buff.size()) buff[size] = std::byte{ 0 };

					auto ptr = reinterpret_cast<std::byte*>(&buff);
					auto buffer = std::vector<std::byte>(ptr, ptr + sizeof(buff));
					auto packet = reinterpret_cast<Packet*>(&buffer[0]);

					asgeServer->handlePacket(*packet);
				}
			}
		});

	thread.detach();
}


void GCNetServer::send(const Packet & packet) {
	auto& socket = connections.back();
	socket.send(reinterpret_cast<const std::byte*>(&packet), sizeof(packet));
}


void GCNetServer::update(double /*dt*/) {}
