#pragma once 


#include <html_parser.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <boost/asio.hpp>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace asio = boost::asio;           // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>



// Return a reasonable mime type based on the extension of a file.
beast::string_view mime_type(beast::string_view path) {

    using beast::iequals;

    auto const ext = [&path] {
        auto const pos = path.rfind(".");
        if(pos == beast::string_view::npos)
            return beast::string_view{};
        return path.substr(pos);
    }();

    if(iequals(ext, ".htm"))  return "text/html";
    if(iequals(ext, ".html")) return "text/html";
    if(iequals(ext, ".php"))  return "text/html";
    if(iequals(ext, ".css"))  return "text/css";
    if(iequals(ext, ".txt"))  return "text/plain";
    if(iequals(ext, ".js"))   return "application/javascript";
    if(iequals(ext, ".json")) return "application/json";
    if(iequals(ext, ".xml"))  return "application/xml";
    if(iequals(ext, ".swf"))  return "application/x-shockwave-flash";
    if(iequals(ext, ".flv"))  return "video/x-flv";
    if(iequals(ext, ".png"))  return "image/png";
    if(iequals(ext, ".jpe"))  return "image/jpeg";
    if(iequals(ext, ".jpeg")) return "image/jpeg";
    if(iequals(ext, ".jpg"))  return "image/jpeg";
    if(iequals(ext, ".gif"))  return "image/gif";
    if(iequals(ext, ".bmp"))  return "image/bmp";
    if(iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
    if(iequals(ext, ".tiff")) return "image/tiff";
    if(iequals(ext, ".tif"))  return "image/tiff";
    if(iequals(ext, ".svg"))  return "image/svg+xml";
    if(iequals(ext, ".svgz")) return "image/svg+xml";

    return "application/text";

}



// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
std::string path_cat(
    beast::string_view base,
    beast::string_view path
)
{
    if (base.empty()) { return std::string(path); }
    
    std::string result(base);
#ifdef BOOST_MSVC
    char constexpr path_separator = '\\';
    if (result.back() == path_separator) { result.resize(result.size() - 1); }
    result.append(path.data(), path.size());
    
    for (auto& c : result) {
        if (c == '/') { c = path_separator; }
    }
#else
    char constexpr path_separator = '/';
    if (result.back() == path_separator) { result.resize(result.size() - 1); }
    result.append(path.data(), path.size());
#endif
    return result;
}



// Report a failure
void fail(beast::error_code ec, char const* what) {
    std::cerr << what << ": " << ec.message() << "\n";
}


// Return a response for the given request.
//
// The concrete type of the response message (which depends on the
// request), is type-erased in message_generator.
template <typename Body, typename Allocator>
static http::message_generator handle_request(
    const std::string &content,
    http::request<Body, http::basic_fields<Allocator>> &&request
) {

    // Returns a bad request response
    const auto bad_request = [&request] (beast::string_view why) {
        http::response<http::string_body> response{http::status::bad_request, request.version()};
        response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        response.set(http::field::content_type, "text/html");
        response.keep_alive(request.keep_alive());
        response.body() = std::string(why);
        response.prepare_payload();
        return response;
    }; 


    // Returns a not found response
    const auto not_found = [&request] (beast::string_view target) {
        http::response<http::string_body> response{http::status::not_found, request.version()};
        response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        response.set(http::field::content_type, "text/html");
        response.keep_alive(request.keep_alive());
        response.body() = "The resource '" + std::string(target) + "' was not found.";
        response.prepare_payload();
        return response;
    };


    // Returns a server error response
    auto const server_error = [&request] (beast::string_view what) {
        http::response<http::string_body> response{http::status::internal_server_error, request.version()};
        response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        response.set(http::field::content_type, "text/html");
        response.keep_alive(request.keep_alive());
        response.body() = "An error occurred: '" + std::string(what) + "'";
        response.prepare_payload();
        return response;
    };

    /*-------------------------------------------------------------------*/

    // Make sure we can handle the method
    if (request.method() != http::verb::get &&
        request.method() != http::verb::head) {
        return bad_request("Unknown HTTP-method");
    }

    // Request path must be absolute and not contain "..".
    if (request.target().empty() ||
        request.target()[0] != '/' ||
        request.target().find("..") != beast::string_view::npos) {
        return bad_request("Illegal request-target");
    }


    // Build the path to the requested file
    std::string path = path_cat(".", request.target());
    if (request.target().back() == '/') { 
        path.append("index.html");
    }

    HTMLParser parser{"./app/html/"};

    // Respond to GET request
    http::response<http::string_body> response{http::status::ok, request.version()};
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_type, "text/html");
    response.keep_alive(request.keep_alive());
    // boost::string_view sv = request.target();
    // std::string str_target{sv.begin(), sv.end()};
    // response.body() = m_parser.get_page(str_target);
    response.body() = parser.get_page(path);
    response.prepare_payload();
    return response;
}

    
// Handles an HTTP server connection
class session : public std::enable_shared_from_this<session>
{
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    std::string doc_root_;
    http::request<http::string_body> req_;
    

public:
    // Take ownership of the stream
    session(
        tcp::socket&& socket,
        std::string doc_root)
        : stream_(std::move(socket))
        , doc_root_(doc_root)
    {}

    // Start the asynchronous operation
    void run() {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this session. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
        asio::dispatch(stream_.get_executor(),
                    beast::bind_front_handler(
                        &session::do_read,
                        shared_from_this()));
    }

    void do_read() {
        // Make the request empty before reading,
        // otherwise the operation behavior is undefined.
        req_ = {};

        // Set the timeout.
        stream_.expires_after(std::chrono::seconds(30));

        // Read a request
        http::async_read(stream_, buffer_, req_,
            beast::bind_front_handler(
                &session::on_read,
                shared_from_this()));
    }

    void on_read(
        beast::error_code ec,
        std::size_t bytes_transferred
    ) {
        boost::ignore_unused(bytes_transferred);

        // This means they closed the connection
        if (ec == http::error::end_of_stream) { return do_close(); }
        if (ec) { return fail(ec, "read"); }

        // Send the response
        send_response(
            handle_request(doc_root_, std::move(req_)));
    }

    void send_response(http::message_generator&& msg) {
        bool keep_alive = msg.keep_alive();

        // Write the response
        beast::async_write(
            stream_,
            std::move(msg),
            beast::bind_front_handler(
                &session::on_write, shared_from_this(), keep_alive
            )
        );
    }

    void on_write(
        bool keep_alive,
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec){ return fail(ec, "write"); }

        if(! keep_alive) {
            // This means we should close the connection, usually because
            // the response indicated the "Connection: close" semantic.
            return do_close();
        }

        // Read another request
        do_read();
    }

    void do_close() {
        // Send a TCP shutdown
        beast::error_code ec;
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

        // At this point the connection is closed gracefully
    }
};




class Server : public std::enable_shared_from_this<Server> {
public:

    Server() = delete;
    Server(
        const std::string &path,
        asio::io_context& ioc,
        tcp::endpoint endpoint,
        std::string doc_root 
    ) : m_io_context(ioc), 
        m_acceptor(asio::make_strand(ioc)), 
        m_doc_root(doc_root)
    {
        beast::error_code ec;

        // Open the acceptor
        m_acceptor.open(endpoint.protocol(), ec);
        if(ec) {
            fail(ec, "open");
            return;
        }

        // Allow address reuse
        m_acceptor.set_option(asio::socket_base::reuse_address(true), ec);
        if(ec) {
            fail(ec, "set_option");
            return;
        }

        // Bind to the server address
        m_acceptor.bind(endpoint, ec);
        if(ec) {
            fail(ec, "bind");
            return;
        }

        // Start listening for connections
        m_acceptor.listen(
            asio::socket_base::max_listen_connections, ec);
        if(ec)
        {
            fail(ec, "listen");
            return;
        }
    }


    void run() { do_accept(); }

private:

    std::string m_doc_root;
    asio::io_context& m_io_context;
    tcp::acceptor m_acceptor;


    void do_accept() {
        // The new connection gets its own strand
        m_acceptor.async_accept(
            asio::make_strand(m_io_context),
            beast::bind_front_handler(
                &Server::on_accept,
                shared_from_this()));
    }

    void on_accept(beast::error_code ec, tcp::socket socket) {
        if(ec) {
            fail(ec, "accept");
            return; // To avoid infinite loop
        }
        else {
            // Create the session and run it
            std::make_shared<session>(
                std::move(socket),
                m_doc_root
            )->run();
        }

        // Accept another connection
        do_accept();
    }


};





//------------------------------------------------------------------------------

int run_server(int argc, char* argv[])
{
    // Check command line arguments.
    if (argc != 5)
    {
        std::cerr <<
            "Usage: http-server-async <address> <port> <doc_root> <threads>\n" <<
            "Example:\n" <<
            "    http-server-async 0.0.0.0 8080 . 1\n";
        return EXIT_FAILURE;
    }
    auto const address = asio::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const doc_root = std::string(argv[3]);
    auto const threads = std::max<int>(1, std::atoi(argv[4]));

    // The io_context is required for all I/O
    asio::io_context ioc{threads};

    // Create and launch a listening port
    std::make_shared<Server>(
        "./app/html/",
        ioc,
        tcp::endpoint{address, port},
        doc_root)->run();

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
        v.emplace_back(
        [&ioc]
        {
            ioc.run();
        });
    ioc.run();

    return EXIT_SUCCESS;
}