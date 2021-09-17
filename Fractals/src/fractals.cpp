/* This code was created and edited by Aiden Mundy and Ethan Haarer
 * It generates Sierpinski Triangles and also can do floodfills.
 */

#include "fractals.h"
using namespace std;

void drawSierpinskiTriangle(GWindow& window, const double x, const double y, const double size, int order);
int floodFill(GWindow& window, const int x, const int y, const int color);
int recursiveFloodFill(GWindow& window, const int x, const int y, const int color, int originalColor);

void drawSierpinskiTriangle(GWindow& window, const double x, const double y, const double size, int order) {
    if (x < 0 || y < 0 || order <= 0){ // Throw/Error checking
        throw "Given X, Y, or Order is outside permitted range";
    }
    order--;
    if (order <= 0){ // Base case
        window.drawLine(x, y, x+size, y);
        window.drawLine(x+size, y, x+size/2, y+size*sqrt(3)/2);
        window.drawLine(x, y, x+size/2, y+size*sqrt(3)/2);
    }
    else { // Recursive case, if there is still another level to draw
        drawSierpinskiTriangle(window, x, y, size/2, order);
        drawSierpinskiTriangle(window, x+size/2, y, size/2, order);
        drawSierpinskiTriangle(window, x+size/4, y+size*sqrt(3)/4, size/2, order);
    }
}

int floodFill(GWindow& window, const int x, const int y, const int color) {
    if (!window.inCanvasBounds(x, y)){ // Ensures initial input is within the bounds of the graphics window.
        throw "Given X or Y is value outside premitted range";
    }
    return recursiveFloodFill(window, x, y, color, window.getPixel(x,y));
}

int recursiveFloodFill(GWindow& window, const int x, const int y, const int color, int originalColor){

    int numFilled = 0; //used to count the number of pixels colored
    //Base Case
    if (!window.inCanvasBounds(x, y) || originalColor != window.getPixel(x,y) || color == window.getPixel(x, y)){
        return 0;
    } else { //Recursive Case
        window.setPixel(x, y, color);
        numFilled += 1;

        numFilled += recursiveFloodFill(window, x+1, y, color, originalColor);
        numFilled += recursiveFloodFill(window, x-1, y, color, originalColor);
        numFilled += recursiveFloodFill(window, x, y+1, color, originalColor);
        numFilled += recursiveFloodFill(window, x, y-1, color, originalColor);

        return numFilled;
    }
}
