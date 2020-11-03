/* Created by Giacomo Bergami
	Edited by Samuel Buzz Appleby
			06/03/20
			170348069			*/
#pragma once




#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <ostream>

struct IntervalArithmetic {
	float x, y;
	/**
	 * Defining the interval [x, y]
	 * @param x         minimum value
	 * @param y         maximum value
	 */
	IntervalArithmetic(float x, float y);

	/**
	 * Defining the interval over one single point: [c, c]
	 * @param c
	 */
	IntervalArithmetic(float c);

	IntervalArithmetic& operator+=(const IntervalArithmetic& i);
	IntervalArithmetic& operator-=(const IntervalArithmetic& i);
	IntervalArithmetic& operator*=(const IntervalArithmetic& i);
	IntervalArithmetic& operator/=(const IntervalArithmetic& i);
	IntervalArithmetic operator+(const IntervalArithmetic& i) const;
	IntervalArithmetic operator-(const IntervalArithmetic& i) const;
	IntervalArithmetic operator*(const IntervalArithmetic& i) const;
	IntervalArithmetic operator/(const IntervalArithmetic& i) const;
	IntervalArithmetic pow(unsigned int pow);
	friend std::ostream& operator<<(std::ostream& os, const IntervalArithmetic& arithmetic);
};

