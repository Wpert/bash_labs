#include <iostream>
#include <boost/program_options.hpp>
#include <SFML/Graphics.hpp>

#include "utils.hpp"

namespace po = boost::program_options;

class TDitherMatrix : public TMatrix<double> {
public:
    TDitherMatrix() = delete;

    TDitherMatrix(const TMatrix<double>& mtrx, const sf::Vector2i &center, double coeff = 0.0)
        : TMatrix<double>(mtrx), _coeff(coeff)
        , width(mtrx.size()), height(mtrx[0].size())
        , center(center)
    {
        if (coeff <= 1.0) {
            coeff = 0.0;
            for (size_t x = 0; x < width; ++x) {
            for (size_t y = 0; y < height; ++y) {
                coeff += _data[x][y];
            }
            }
        }
    }

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

const static TDitherMatrix FloydSteinbergDM{
    TMatrix<double>{
        {0, 0, 7},
        {3, 5, 1}
    },
    {0, 1},
    16.0
};

const static TDitherMatrix StuckiDM{
    TMatrix<double>{
     {0, 0, 0, 8, 4}, 
     {2, 4, 8, 4, 2},
     {1, 2, 4, 2, 1}
    },
    {0, 2}
};

sf::Image
ApplyDithering(
    const sf::Image &img,
    const TDitherMatrix &dMtrx, uint bpp
) {
    const uint MAX_BPP = 8;
    const uint MAX_COLORS_COUNT = (1 << (MAX_BPP - bpp)) - 1;
    const uint DITHER_COEFF = std::max(MAX_COLORS_COUNT, 1u);

    auto [width, height] = img.getSize();
    sf::Image output{img};

    for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
        sf::Color color = output.getPixel(x, y);
        TMatrix<sf::Color> quantErrMtrx = dMtrx.applyTo(color, DITHER_COEFF);

        int _width = dMtrx.width;
        int _height = dMtrx.height;
        for (int _x = 0; _x < _width; ++_x) {
        for (int _y = 0; _y < _height; ++_y) {
            sf::Vector2i delta = sf::Vector2i{_x, _y} - dMtrx.center;
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
    return output;
}

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

    const uint userBPP = vmap["bpp"].as<uint>();
    if ( !(0 <= userBPP && userBPP <= 8) ) {
        throw std::runtime_error("wrong bpp value");
    }

    sf::Image output{
        std::move(ApplyDithering(_img, StuckiDM, userBPP))
    };

    if (!output.saveToFile("out.png")) {
        throw std::runtime_error("something went wrong during saving");
    }
    
    auto [width, height] = _img.getSize();
    const auto* toCout = output.getPixelsPtr();
    for (size_t i = 0; i < width * height * 4; ++i) {
        std::cout << toCout[i];
    }

    return 0;
}
