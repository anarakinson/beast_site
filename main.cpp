

#include <json_parser.hpp>
#include <html_parser.hpp>
#include <boost_server.hpp>
// #include <server.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>


#include <iostream>
#include <memory>
#include <string>
#include <thread>

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


        HTMLParser htmlp{"./app/html/"};

        // auto page = htmlp.get_page("about");

        // std::cout << page << std::endl;


        // std::string path = "D:/Coding/Atom/cpp/website_cv_beast/app/html/test.html";
       

        // /*-------------------------------------------------------------------*/


        int result = run_server(argc, argv);

        // std::cout << result << std::endl;

        std::cout << "Stop server" << std::endl;


    } catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;

}


