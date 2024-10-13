#pragma once
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

sf::Color operator & (const sf::Color &clr, int v) {
    return sf::Color(clr.r & v, clr.g & v, clr.b & v);
}

sf::Color operator * (const sf::Color &clr, double v) {
    return sf::Color(clr.r * v, clr.g * v, clr.b * v);
}

sf::Color operator / (const sf::Color &clr, double v) {
    return sf::Color(clr.r / v, clr.g / v, clr.b / v);
}

sf::Color RoundTo(const sf::Color &clr, double v) {
    return ( clr / v ) * v;
}

template<typename T>
class TMatrix {
public:
    TMatrix()
        : _data({{T(),},})
    {}

    TMatrix(size_t width, size_t height, T value = T())
        : _data(width, std::vector<T>(height, value))
    {}

    TMatrix(const std::vector<std::vector<T>> &mtrx)
        : _data(mtrx)
    {}

    TMatrix(std::initializer_list<std::vector<T>> l)
        : _data(l)
    {}

    auto& operator[](size_t i) {
        return _data[i];
    }
    const auto& operator[](size_t i) const {
        return _data[i];
    }

    size_t size() const noexcept {
        return _data.size();
    }

protected:
    std::vector<std::vector<T>> _data;
};
