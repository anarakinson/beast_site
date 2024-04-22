#pragma once


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iostream>
#include <string>


namespace pt = boost::property_tree;


class JsonParser {
public:

    JsonParser() = delete;
    JsonParser(const std::string &filename) {

        // Create a root
        pt::ptree root;

        // Load the json file in this ptree
        pt::read_json(filename, root);
        
        // parse data
        m_server_ip = root.get<std::string>("server_ip", "0.0.0.0");
        m_server_port = root.get<int>("server_port", 9999);

        // Configuring SSL key and cert
        m_ssl = root.get<bool>("ssl", false);
        m_ssl_key = root.get<std::string>("ssl_key", "");
        m_ssl_cert = root.get<std::string>("ssl_cert", "");

        // return;

    }

    ~JsonParser() = default;


    JsonParser(const JsonParser &other) :
        m_server_ip{other.m_server_ip},
        m_server_port{other.m_server_port},
        m_ssl{other.m_ssl},
        m_ssl_key{other.m_ssl_cert},
        m_ssl_cert{other.m_ssl_key} 
    {}
    JsonParser &operator = (const JsonParser &other) {
        m_server_ip = other.m_server_ip;
        m_server_port = other.m_server_port;
        m_ssl = other.m_ssl;
        m_ssl_key = other.m_ssl_cert;
        m_ssl_cert = other.m_ssl_key;

        return *this;
    }
    

    JsonParser(JsonParser &&other) :
        m_server_ip{other.m_server_ip},
        m_server_port{other.m_server_port},
        m_ssl{other.m_ssl},
        m_ssl_key{other.m_ssl_cert},
        m_ssl_cert{other.m_ssl_key} 
    {}
    JsonParser &operator = (JsonParser &&other) {
        std::swap(m_server_ip, other.m_server_ip);
        std::swap(m_server_port, other.m_server_port);
        std::swap(m_ssl, other.m_ssl);
        std::swap(m_ssl_key, other.m_ssl_cert);
        std::swap(m_ssl_cert, other.m_ssl_key);

        return *this;
    }
    

    // getters
    std::string server_ip() { return m_server_ip; }
    int server_port() { return m_server_port; }
    bool ssl() { return m_ssl; }
    std::string ssl_key() { return m_ssl_key; }
    std::string ssl_cert() { return m_ssl_cert; }


private:

        std::string m_server_ip;
        int m_server_port;
        bool m_ssl;
        std::string m_ssl_key;
        std::string m_ssl_cert;
  
};