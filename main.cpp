

#include <json_parser.hpp>
#include <html_parser.hpp>
#include <boost_server.hpp>

#include <iostream>
#include <string>

#include <cstdlib>


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace asio = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


int main(int argc, char* argv[]) {

    std::cout << "Hello" << std::endl;

    try {

        JsonParser config("./configs/config.jsonc");

        std::cout << config.server_ip() << " " << config.server_port() << " " << config.ssl() << std::endl;

        int result = run_server(argc, argv);

        std::cout << "Stop server" << std::endl;

    } catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;

}


