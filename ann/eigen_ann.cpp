/*
	Copyright (C) 2016 Matthew Lai

	Giraffe is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Giraffe is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "eigen_ann.h"

namespace
{

NNVector ReadVector(std::istream &is)
{
	int numDims;
	is >> numDims;
	assert(numDims == 1);
	int64_t elements;
	is >> elements;
	NNVector ret(elements);

	for (int64_t i = 0; i < elements; ++i)
	{
		is >> ret(i);
	}

	return ret;
}

NNMatrixRM ReadMatrix(std::istream &is)
{
	int numDims;
	is >> numDims;
	assert(numDims == 2);
	int64_t rows;
	is >> rows;
	int64_t cols;
	is >> cols;
	NNMatrixRM ret(rows, cols);

	for (int64_t row = 0; row < rows; ++row)
	{
		for (int64_t col = 0; col < cols; ++col)
		{
			is >> ret(row, col);
		}
	}

	return ret;
}

}

void EigenANN::FromString(const std::string &s)
{
	m_layers.clear();
	m_stringRep = s;

	std::stringstream ss(s);
	std::string token;

	// First we have to find the "EIGEN" marker
	do
	{
		ss >> token;
	}
	while (token != "EIGEN");

	int numLayers;
	ss >> numLayers;

	for (int i = 0; i < numLayers; ++i)
	{
		std::string layerType;
		ss >> layerType;

		if (layerType == "nn.Linear")
		{
			NNVector bias = ReadVector(ss);
			NNMatrix weight = ReadMatrix(ss);

			m_layers.push_back(std::unique_ptr<Layer>(new LinearLayer(bias, weight)));
		}
		else if (layerType == "nn.ReLU")
		{
			m_layers.push_back(std::unique_ptr<Layer>(new ReLULayer));
		}
		else if (layerType == "nn.Tanh")
		{
			m_layers.push_back(std::unique_ptr<Layer>(new TanhLayer));
		}
		else
		{
			std::cerr << "Layer type " << layerType << " not implemented!" << std::endl;
		}
	}
}
