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

sf::Image
TransposeImage(const sf::Image &img) {
    auto [width, height] = img.getSize();
    sf::Image result{};
    result.create(height, width);
    for (size_t x = 0; x < width; ++x) {
    for (size_t y = 0; y < height; ++y) {
        auto color = img.getPixel(x, y);
        result.setPixel(y, x, color);
    }
    }
    return result;
}

sf::Image
FlipHorisontally(const sf::Image &img) {
    auto [width, height] = img.getSize();
    sf::Image result{img};
    for (size_t x = 0; x < width; ++x) {
    for (size_t y = 0; y < height / 2; ++y) {
        auto firstColor = img.getPixel(x, y);
        auto secondColor = img.getPixel(x, height - 1 - y);
        result.setPixel(x, y, secondColor);
        result.setPixel(x, height - 1 -y, firstColor);
    }
    }
    return result;
}

sf::Image
GetHalftoneImage(const sf::Image &img) {
    auto [width, height] = img.getSize();
    sf::Image result{img};
    for (size_t x = 0; x < width; ++x) {
    for (size_t y = 0; y < height; ++y) {
        auto color = img.getPixel(x, y);
        sf::Uint8 tone = (color.r + color.g + color.b) / 3.0;
        sf::Color toneColor{tone, tone, tone, color.a};
        result.setPixel(x, y, toneColor);
    }
    }
    return result;
}

bool
IsInCircle(
    const sf::Vector2<size_t>& coord,
    const sf::Vector2<double>& center,
    double radius
    ) {
    return sqrt( (center.x - coord.x) * (center.x - coord.x)
               + (center.y - coord.y) * (center.y - coord.y) ) <= radius;
}

sf::Image
CropCircle(const sf::Image &img) {
    auto [width, height] = img.getSize();
    sf::Image result{img};
    sf::Vector2<double> center = {width / 2.0, height / 2.0};
    uint radius = std::min(width / 2, height / 2);

    for (size_t x = 0; x < width; ++x) {
    for (size_t y = 0; y < height; ++y) {
        auto color = img.getPixel(x, y);
        if (IsInCircle({x, y}, center, radius))
            result.setPixel(x, y, color);
        else
            result.setPixel(x, y, sf::Color::White);
    }
    }
    return result;
} 

sf::Image
CombineThreeImages(
    sf::Image &_firstImage,
    sf::Image &_secondImage,
    sf::Image &_thirdImage
    ) {
    auto [width, height] = _firstImage.getSize();
    std::cout << width << ' ' << height << std::endl;

    sf::Image output{_firstImage};

    for (size_t x = 0; x < width; ++x) {
    for (size_t y = 0; y < height; ++y) {
        sf::Color first = output.getPixel(x, y);
        sf::Color second = _secondImage.getPixel(x, y);
        sf::Color third = _thirdImage.getPixel(x, y);

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
    return output;
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
    sf::Image _first;
    sf::Image _second;
    sf::Image _third;

    if (!_first.loadFromFile(vmap["f"].as<std::string>())) {
        throw std::runtime_error("wrong first file");
    }
    if (!_second.loadFromFile(vmap["s"].as<std::string>())) {
        throw std::runtime_error("wrong second file");
    }
    if (!_third.loadFromFile(vmap["a"].as<std::string>())) {
        throw std::runtime_error("wrong third file");
    }

    auto output = CombineThreeImages(_first, _second, _third);

    if (!CropCircle(output).saveToFile("out.png")) {
        throw std::runtime_error("something went wrong during saving");
    }
    return 0;
}
