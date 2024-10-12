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

class TDitherMatrix : public TMatrix<double> {
public:
    TDitherMatrix() = delete;

    TDitherMatrix(const TMatrix<double>& mtrx, const sf::Vector2i &center, double coeff = 1.0)
        : TMatrix<double>(mtrx), _coeff(coeff)
        , width(mtrx.size()), height(mtrx[0].size())
        , center(center)
    {}

    TMatrix<sf::Color> applyTo(sf::Color& pixel, int n) const {
        TMatrix<sf::Color> tmp{
            static_cast<size_t>(width),
            static_cast<size_t>(height)
        };

        sf::Color newColor = RoundTo(pixel, n);
        sf::Color quantErr = pixel - newColor;

        for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            tmp[x][y] = quantErr * (this->_data[x][y] / _coeff);
        }
        }
        tmp[center.x][center.y] = newColor;
        return tmp;
    }

    sf::Vector2i center;
    int width = 1;
    int height = 1;
private:
    double _coeff = 1.0;
};

const static TDitherMatrix FloydSteinberg{
    TMatrix<double>{
        {0, 0, 7},
        {3, 5, 1}
    },
    {0, 1},
    16.0
};

int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("img", po::value<std::string>(), "image to dithering")
        ("bpp", po::value<uint>(), "how many bytes per pixel that takes values from 1 to 8")
        ("h", "prints help")
        ("help", "prints help")
    ;
    auto vmap = process_program_options(argc, argv, desc);
    if (vmap.contains("h") || vmap.contains("help")) {
        desc.print(std::cout);
    }
    else if (!vmap.contains("img") || !vmap.contains("bpp")) {
        throw std::runtime_error("Too few arguments");
    }
    sf::Image _img;

    if (!_img.loadFromFile(vmap["img"].as<std::string>())) {
        throw std::runtime_error("wrong first file");
    }

    const uint _BPP = 8;
    const uint userBPP = vmap["bpp"].as<uint>();
    if ( !(0 < userBPP && userBPP <= 8) ) {
        throw std::runtime_error("wrong bpp value");
    }
    const uint DITHER_V = 1 << (8 - userBPP);

    auto [width, height] = _img.getSize();
    sf::Image output{_img};

    for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
        sf::Color color = output.getPixel(x, y);
        TMatrix<sf::Color> quantErrMtrx = FloydSteinberg.applyTo(color, DITHER_V);

        int _width = FloydSteinberg.width;
        int _height = FloydSteinberg.height;
        // std::cout << "x: " << x << ' ' << "y: " << y << std::endl;
        for (int _x = 0; _x < _width; ++_x) {
        for (int _y = 0; _y < _height; ++_y) {
            sf::Vector2i delta = sf::Vector2i{_x, _y} - FloydSteinberg.center;
            sf::Vector2i resVec = sf::Vector2i{x, y} + delta;
            if (0 < resVec.x && resVec.x < width 
             && 0 < resVec.y && resVec.y < height) {
                if (delta == sf::Vector2i(0, 0))
                    output.setPixel(x, y, quantErrMtrx[_x][_y]);
                else {
                    sf::Color toGiveQuant = output.getPixel(resVec.x, resVec.y) + quantErrMtrx[_x][_y];
                    output.setPixel(resVec.x, resVec.y, toGiveQuant);
                }
            }
        }
        }

    }
    }

    if (!output.saveToFile("out.png")) {
        throw std::runtime_error("something went wrong during saving");
    }

    // sf::Color b = sf::Color::Black;

    // for (size_t x = 0; x < FloydSteinberg.size(); ++x) {
    //     for (size_t y = 0; y < FloydSteinberg[0].size(); ++y) {
    //         std::cout << FloydSteinberg[x][y] << ' ';
    //     }
    //     std::cout << std::endl;
    // }
    return 0;
}
