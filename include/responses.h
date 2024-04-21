#pragma once 


#include <httplib.h>
#include <inja/inja.hpp>
#include <json/json.h>

#include <iostream>
#include <fstream>
#include <string>



// Something like enum class for c-strings
struct Pages {

    static const char *main() { return "/"; }
    static const char *about() { return "/about/"; }
    static const char *contacts() { return "/contacts/"; }
    static const char *projects() {return "/projects/"; }
    static const char *nowhere() {return "/nowhere/"; }

};


class ResponseProcessor {
public:

    void GetMain(httplib::Server *server) {
        std::cout << "[*] main" << std::endl;
        // Set test page
        server->Get(
            Pages::main(),           // address on site
            [&](const httplib::Request &req, httplib::Response &res) {
                // Load template
                inja::Template tmpt = env.parse_template(lang + "index.html");

                // Build data
                page_data["nothing"] = "nothing";
                page_data["lang"] = lang;

                // render content   
                res.set_content(env.render(tmpt, page_data).c_str(), "text/html");
            }
        );
    }


    // About page
    void GetAbout(httplib::Server *server) {
        std::cout << "[*] about" << std::endl;
        server->Get(
            Pages::about(),           // address on site
            [&](const httplib::Request &req, httplib::Response &res) {
                // Load template
                inja::Template tmpt = env.parse_template(lang + "about.html");

                // Build data
                page_data["nothing"] = "nothing";
                page_data["lang"] = lang;

                // render content   
                res.set_content(env.render(tmpt, page_data).c_str(), "text/html");
            }
        );
    }


    // Contacts page
    void GetContacts(httplib::Server *server) {
        std::cout << "[*] contacts" << std::endl;
        server->Get(
            Pages::contacts(),           // address on site
            [&](const httplib::Request &req, httplib::Response &res) {
                // Load template
                inja::Template tmpt = env.parse_template(lang + "contacts.html");

                // Build data
                page_data["nothing"] = "nothing";
                page_data["lang"] = lang;

                // render content   
                res.set_content(env.render(tmpt, page_data).c_str(), "text/html");
            }
        );
    }


    // Projects page
    void GetProjects(httplib::Server *server) {
        std::cout << "[*] projects" << std::endl;
        // Set about page
        server->Get(
            Pages::projects(),           // address on site
            [&](const httplib::Request &req, httplib::Response &res) {
                // Load template
                inja::Template tmpt = env.parse_template(lang + "projects.html");

                // Build data
                page_data["nothing"] = "nothing";
                page_data["lang"] = lang;

                // render content   
                res.set_content(env.render(tmpt, page_data).c_str(), "text/html");
            }
        );
    }


    // Projects page
    void GetNowhere(httplib::Server *server) {
        std::cout << "[*] nowhere" << std::endl;
        // Set about page
        server->Get(
            Pages::nowhere(),           // address on site
            [&](const httplib::Request &req, httplib::Response &res) {
                // Load template
                inja::Template tmpt = env.parse_template(lang + "about.html");

                // Build data
                page_data["nothing"] = "nothing";
                page_data["lang"] = lang;

                // render content   
                res.set_content(env.render(tmpt, page_data).c_str(), "text/html");
            }
        );
    }


    // Change language     inja::Environment env_ru{"./app/html/ru/"};
    void ChangeLangRU(httplib::Server *server) {
        server->Get(
            "/ru/", 
            [&](const httplib::Request &req, httplib::Response &res) {
                lang = "ru/";
                res.set_redirect(Pages::main());
            }
        );
    }
    // Change language     inja::Environment env_ru{"./app/html/ru/"};
    void ChangeLangEN(httplib::Server *server) {
        server->Get(
            "/en/", 
            [&](const httplib::Request &req, httplib::Response &res) {
                lang = "";
                res.set_redirect(Pages::main());
            }
        );
    }


private:

    inja::Environment env{"./app/html/"};
    inja::json page_data;

    std::string lang = "";

};
