/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

//a struct that contains x and y ints
struct AVE_vector
{
	int x;
	int y;
};

//returns the magnitude between two vectors
float AVE_findM(AVE_vector v1, AVE_vector v2);

//sub opperator
AVE_vector operator-(AVE_vector v1, AVE_vector v2);

//add opperator
AVE_vector operator+(AVE_vector v1, AVE_vector v2);

//gets slope
float AVE_findSlope(AVE_vector v1, AVE_vector v2, int SCREEN_W);

#endif