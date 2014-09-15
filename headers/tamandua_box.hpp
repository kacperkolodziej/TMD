#ifndef TAMANDUA_BOX_HPP
#define TAMANDUA_BOX_HPP
#include "tamandua/tamandua.hpp"
#include "debug_gui.hpp"
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

struct tamandua_box
{
	std::thread reader_thread;
	std::mutex running_lock;
	boost::asio::ssl::context context;
	tamandua::client client;
	bool running;

	tamandua_box() :
		context(boost::asio::ssl::context::sslv23),
		client(context),
		running(true)
	{
		context.load_verify_file("ssl/TamanduaCA.pem");
		client.get_socket().set_verify_mode(boost::asio::ssl::verify_peer);
	}

	void turn_off()
	{
		std::lock_guard<std::mutex> lock(running_lock);
		running = false;
	}
};

#endif
