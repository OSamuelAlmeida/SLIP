#include <iostream> 
#include <string>
#include <exception>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>

#include <Magick++.h> 

struct Point
{
    int x;
    int y;
};

struct RGB
{
    int R;
    int G;
    int B;
};

// std::less<T> specialization for RGB class
// Allows usage of RGB as a key on a std::map
namespace std
{
    template<> struct less<RGB>
    {
       bool operator() (const RGB& lhs, const RGB& rhs) const
       {
           return lhs.R < rhs.R || lhs.G < rhs.G || lhs.B < rhs.B;
       }
    };
}

std::map<RGB, std::vector<Point>> getColorMap(Magick::Image& image)
{
    int width = image.columns();
    int height = image.rows();
    int range = pow(2, image.modulusDepth()); 

    Magick::PixelPacket* pixels = image.getPixels(0, 0, width, height);

    std::map<RGB, std::vector<Point>> colorMap;

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            Magick::Color color = pixels[width * j + i];

            RGB rgbColor = { color.redQuantum() / range, color.greenQuantum() / range, color.blueQuantum() / range }; // Convert Magick++ Quantum range to 0-255


            colorMap[rgbColor].push_back({ i, j });
        }
    }

    return std::move(colorMap);
}

std::string generateSuperLogoCode(std::string& fileName, std::map<RGB, std::vector<Point>>& colorMap)
{
    std::stringstream stream;

    stream << "aprenda " << fileName << std::endl;

    for (auto pair : colorMap)
    {
        stream <<  "mudecl [ " << pair.first.R << " " << pair.first.G << " " << pair.first.B << " ]" << " ";

        for (auto point : pair.second)
        {
            stream << "un" << " ";
            stream << "mudexy " << point.x << " " << -point.y << " ";
            stream << "ul" << " ";
            stream << "pf 1" << std::endl;
        }
    }

    stream << "fim" << std::endl;

    return stream.str();
}

int main(int argc,char **argv) 
{ 
    if (argc < 2)
    {
        std::cerr << "Usage: SLIP image" << std::endl;;
        return -1;
    }

    Magick::InitializeMagick(*argv);

    Magick::Image image; 
    std::string imageFile = argv[1];

    try
    {
        std::cout << "Opening " << imageFile << "..." << std::endl;
        image.read(imageFile);
        
        std::cout << "Getting image pixels..." << std::endl;
        auto colorMap = getColorMap(image);

        std::cout << "Generating SuperLogo code..." << std::endl;
        std::string logoCode = generateSuperLogoCode(imageFile, colorMap);

        std::string logoFileName = imageFile + ".lgo";

        std::cout << "Outputting to " << logoFileName << "..." << std::endl;
        std::ofstream logoFile(logoFileName);

        if (logoFile.is_open())
        {
            logoFile << logoCode;
        }
        else
        {
            std::cerr << "Error opening " << logoFileName << " for writing!" << std::endl;
            return -2;
        }
    }
    catch (std::exception& exception)
    {
        std::cerr << "Error: " << exception.what() << std::endl;
    }
}
