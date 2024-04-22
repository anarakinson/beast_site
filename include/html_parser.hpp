#pragma once

#include <inja/inja.hpp>
#include <fstream>


class HTMLParser {
public:
    HTMLParser() = default;
    explicit HTMLParser(const std::string &path) : env{path} {}

    std::string get_page(const std::string &path) {

        inja::Template tmpt = env.parse_template(path);

        // Build data
        page_data["nothing"] = "nothing";
        page_data["css_path"] = "./css/main.css";
        page_data["css_body"] = m_css_body;

        // render content   
        return env.render(tmpt, page_data);
    }

    void set_css(const std::string &path) {
        
        std::ifstream file(path);
        if (!file) {
            std::cerr << "Cannot open file: " << path << "\n";
            return;
        }
        std::string str; 
        while (std::getline(file, str)) {
            m_css_body += str + '\n';
        }

    } 

private:

    inja::Environment env;
    inja::json page_data;

    std::string m_css_body = "";

};
