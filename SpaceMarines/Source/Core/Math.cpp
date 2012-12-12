#include "Math.hpp"

namespace SpaceMarines
{

const Vector3 Vector3::ONE = Vector3(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::ZERO = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::UP = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::FORWARD = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::RIGHT = Vector3(1.0f, 0.0f, 0.0f);

const Matrix4 Matrix4::IDENTITY = Matrix4();

const Quaternion Quaternion::IDENTITY = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

const Vector4 Vector4::ONE = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

namespace Math
{
std::vector<std::string> splitString(const std::string &text, char* delims, unsigned short maxParts)
{
	std::vector<std::string> tokens;
	unsigned int start = 0;
	unsigned short parts = 0;
	for (unsigned int i = 0; text[i]; ++i)
	{
		while (text[i] && textContains(delims, text[i]))
			++i;

		start = i;

		while(text[i] && !textContains(delims, text[i]))
			++i;

		tokens.push_back(std::string(&text[start], &text[i]));
		parts++;

		if (maxParts > 0 && parts >= maxParts)
		{
			tokens.push_back(std::string(&text[i+1], &text[text.length() - 1]));
			return tokens;
		}
	}
	return tokens;
}

std::string firstLine(const std::string &text)
{
	std::stringstream ss(text);
	std::string s1;
	getline(ss, s1, '\n');
	return s1;
}
std::string firstWord(const std::string &text)
{
	return splitString(text, " \n", 1)[0];
}
}

}
