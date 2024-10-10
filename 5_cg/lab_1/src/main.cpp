#include <iostream>
#include <boost/program_options.hpp>
#include <SFML/Graphics.hpp>

namespace po = boost::program_options;

po::variables_map
process_program_options(
        const int argc,
        const char *const argv[],
        const po::options_description desc
    ) {
    po::variables_map args;
    po::store(
        po::parse_command_line(argc, argv, desc),
        args
    );
    po::notify(args);
    return args;
}

int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("f", po::value<std::string>(), "first image to combine")
        ("s", po::value<std::string>(), "second image to combine")
        ("a", po::value<std::string>(), "third image to combine as alpha-channel")
        ("h", "prints help")
        ("help", "prints help")
    ;
    auto vmap = process_program_options(argc, argv, desc);
    if (vmap.contains("h") || vmap.contains("help")) {
        desc.print(std::cout);
    }
    else if (!vmap.contains("f") || !vmap.contains("s") || !vmap.contains("a")) {
        throw std::runtime_error("Too few arguments");
    }
    sf::Image _firstCombine;
    sf::Image _secondCombine;
    sf::Image _thirdAlpha;

    if (!_firstCombine.loadFromFile(vmap["f"].as<std::string>())) {
        throw std::runtime_error("wrong first file");
    }
    if (!_secondCombine.loadFromFile(vmap["s"].as<std::string>())) {
        throw std::runtime_error("wrong second file");
    }
    if (!_thirdAlpha.loadFromFile(vmap["a"].as<std::string>())) {
        throw std::runtime_error("wrong third file");
    }

    auto [width, height] = _firstCombine.getSize();
    std::cout << width << ' ' << height << std::endl;

    sf::Image output{_firstCombine};

    for (size_t x = 0; x < width; ++x) {
    for (size_t y = 0; y < height; ++y) {
        sf::Color first = output.getPixel(x, y);
        sf::Color second = _secondCombine.getPixel(x, y);
        sf::Color third = _thirdAlpha.getPixel(x, y);

        sf::Uint8 red =(static_cast<double>(first.r)
                      + static_cast<double>(second.r))
                      / 2.0;
        sf::Uint8 green =(static_cast<double>(first.g)
                        + static_cast<double>(second.g))
                        / 2.0;
        sf::Uint8 blue =(static_cast<double>(first.b)
                       + static_cast<double>(second.b))
                       / 2.0;

        sf::Uint8 alpha =(static_cast<double>(third.r)
                        + static_cast<double>(third.g)
                        + static_cast<double>(third.b)
                        + static_cast<double>(third.a))
                        / 4.0;
        sf::Color out {red, green, blue, alpha};
        output.setPixel(x, y, out);
    }
    }

    if (!output.saveToFile("out.png")) {
        throw std::runtime_error("something went wrong during saving");
    }
    return 0;
}
