//
// Created by vlad on 23.06.20.
//
#include <Magick++.h>
#include <iostream>


void create_gif(int total_pictures_amount){
    std::string pictures = "";

    for (int i = 0; i < total_pictures_amount - 1; i++){
        pictures += " ../pictures/picture";
        pictures += std::to_string(i) + ".jpg";
    }

//    we are sorry for the way to get gif, but it was best possible for Magick++ library we could find
    std::string message = "convert -delay 10 " + pictures + " ../result.gif";
    std::system( message.c_str() );

//    comment next string in order to see pictures gif is made of (in directory pictures)
    system("exec rm -r ../pictures/*.jpg");

}

Magick::Color* define_color(double value){

    if (value < 1){
        return new Magick::Color("blue");
    } else if (value < 5) {
        return new Magick::Color("yellow");
    } else if (value < 10){
        return new Magick::Color("orange");
    } else {
        return new Magick::Color("red");
    }
}

void visualise_matrix(std::vector<double>* matrix, int height, int width, int number_of_image){

    std::string sizes = std::to_string(width) + "x" + std::to_string(height) + "!";
    Magick::Image inputImage;

    inputImage.read("../pictures/canvas.jpeg");
    inputImage.resize(Magick::Geometry(sizes));

    double current_temp;

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            current_temp = get_item(matrix, i, j, height, width);
            auto c = define_color(current_temp);

            inputImage.pixelColor(i, j, *c);
        }
    }

    std::string name = "../pictures/picture" + std::to_string(number_of_image) + ".jpg";
    inputImage.write(name);
}