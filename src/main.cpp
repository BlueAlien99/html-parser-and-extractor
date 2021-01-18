#include <iostream>

#include "conf_parser.hpp"
#include "extractor.hpp"
#include "html_parser.hpp"
#include "node.hpp"
#include "sources.hpp"

int main() {
    std::string url_start = "https://www.";
    std::string url, conf;
    std::cout << "URL: " << url_start;
    std::cin >> url;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Conf: ";
    std::getline(std::cin, conf);

    SourceFromUrl src(url_start + url);
    HtmlParser parser(src);
    std::unique_ptr<Node> dom = parser.parseSafe(src);
    if (dom == nullptr) {
        return 1;
    }
    std::cout << std::endl;
    std::cout << dom->getAllText() << std::endl;

    if (conf == "") {
        return 0;
    }

    SourceFromString conf_src(conf);
    auto extracted = Extractor::extract(dom->clone(), conf_src, true);
    if (extracted == nullptr) {
        return 2;
    }

    std::cout << std::endl;
    std::cout << ">>> Extracted content:" << std::endl;

    auto nodes = extracted->getHtmlNodes();
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        std::cout << ">>> " << nodes[i]->getAllText() << std::endl;
    }

    return 0;
}
