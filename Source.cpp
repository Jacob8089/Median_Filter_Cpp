#include <iostream>
#include <vector>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb_image.h"
#include"includes/lodepng.h"

std::vector<std::vector<uint8_t>> getPixelValues(const char* filename, int& width, int& height) {
    int channels;

    // Load the image file
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 1);

    // Check if the image was successfully loaded
    if (image == nullptr) {
        std::cout << "Failed to read the image file." << std::endl;
        return std::vector<std::vector<uint8_t>>();
    }

    // Create a 2D vector to store the pixel values
    std::vector<std::vector<uint8_t>> pixelValues(height, std::vector<uint8_t>(width));

    // Copy the pixel values to the 2D vector
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            pixelValues[i][j] = image[i * width + j];
        }
    }

    stbi_image_free(image);

    return pixelValues;
}

std::vector<std::vector<uint8_t>> medianFilter(const std::vector<std::vector<uint8_t>>& image, int filterSize) {
    int width = image[0].size();
    int height = image.size();

    std::vector<std::vector<uint8_t>> filteredImage(height, std::vector<uint8_t>(width));

    int filterOffset = filterSize / 2;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            std::vector<uint8_t> pixels;
            for (int filterY = -filterOffset; filterY <= filterOffset; filterY++) {
                for (int filterX = -filterOffset; filterX <= filterOffset; filterX++) {
                    int imageX = std::min(std::max(x + filterX, 0), width - 1);
                    int imageY = std::min(std::max(y + filterY, 0), height - 1);
                    pixels.push_back(image[imageY][imageX]);
                }
            }

            std::sort(pixels.begin(), pixels.end());
            uint8_t medianValue = pixels[pixels.size() / 2];
            filteredImage[y][x] = medianValue;
        }
    }

    return filteredImage;
}

void saveImage(const std::vector<std::vector<uint8_t>>& image, const std::string& filename) {
    int width = image[0].size();
    int height = image.size();
    std::vector<unsigned char> buffer(width * height);

    // Copy the pixel values to the buffer
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            buffer[y * width + x] = image[y][x];
        }
    }

    // Save the image in PNG format
    if (lodepng::encode(filename, buffer, width, height, LCT_GREY, 8) != 0) {
        std::cout << "Failed to save the image file." << std::endl;
    }
}

int main() {
    const char* filename = "C:\\Users\\jacob\\Desktop\\Sem2\\HSCD\\MedianFilter\\lenna4.jpeg";
    int width, height;

    // Get the pixel values for the image
    std::vector<std::vector<uint8_t>> pixelValues = getPixelValues(filename, width, height);

    int radius = 7;

    // Apply median filter to the image
    std::vector<std::vector<uint8_t>> filteredImage = medianFilter(pixelValues, radius);

    // Save the filtered image
    std::string outputFilename = "C:\\Users\\jacob\\Desktop\\Sem2\\HSCD\\MedianFilter\\lenna4_filtered.png";
    saveImage(filteredImage, outputFilename);

    std::cout << "Median filter applied and filtered image saved successfully." << std::endl;

    return 0;
}