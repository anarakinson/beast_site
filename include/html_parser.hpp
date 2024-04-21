#pragma once

#include <inja/inja.hpp>
#include <fstream>


class HTMLParser {
public:
    HTMLParser() = default;
    explicit HTMLParser(const std::string &path) : env{path} {
        set_css("./app/css/main.css");
    }

    std::string get_page(const std::string &path) {

        std::cout << m_lang << " " << path << " " << std::endl;
        inja::Template tmpt = env.parse_template(m_lang + path + ".html");

        // Build data
        page_data["nothing"] = "nothing";
        page_data["lang"] = m_lang;
        page_data["css_path"] = "/app/css/main.css";
        page_data["css_body"] = m_css_body;

        // render content   
        return env.render(tmpt, page_data);
    }

    void set_css(const std::string &path) {
        
        std::ifstream file(path);
        if (!file) {
            std::cerr << "Cannot open file\n";
            return;
        }
        std::string str; 
        while (std::getline(file, str)) {
            m_css_body += str + '\n';
        }

    } 

    void set_language(const std::string &lang) {
        if (lang == "ru") {
            m_lang = "/ru/";
        } else if (lang == "en") {
            m_lang = "";
        } else {
            return;
        }
    }

private:

    inja::Environment env;
    inja::json page_data;

    std::string m_lang = "";
    std::string m_css_body = "";

};
