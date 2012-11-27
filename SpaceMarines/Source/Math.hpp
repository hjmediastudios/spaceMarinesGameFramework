#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <vector>

namespace SpaceMarines
{

namespace Math
{
	const unsigned int MaxUInt32 = 0xFFFFFFFF;
	const int MinInt32 = 0x80000000;
	const int MaxInt32 = 0x7FFFFFFF;
	const float MaxFloat = 3.402823466e+38F;
	const float MinPosFloat = 1.175494351e-38F;

	const float Pi = 3.141592654f;
	const float TwoPi = 2 * Pi;
	const float HalfPi = 0.5f * Pi;

	const float Epsilon = 0.000001f;
	const float ZeroEpsilon = 32.0f * MinPosFloat;
	const float NaN = *(float*)&MaxUInt32;

	//-------------------------------------------
	//			General math functions
	//-------------------------------------------
	static inline float degToRad(float degrees)
	{
		return degrees * 0.017453293f;
	}
	static inline float radToDeg(float radians)
	{
		return radians * 57.29577951f;
	}
	static inline float clamp(float f, float min, float max)
	{
		if (f < min)
			f = min;
		else if (f > max)
			f = max;
		return f;
	}
	static inline float min(float a, float b)
	{
		return a < b ? a : b;
	}
	static inline float max(float a, float b)
	{
		return a > b ? a : b;
	}
	static inline float select(float test, float a, float b)
	{
		return test >= 0 ? a : b;
	}

	enum ForceNoInitialization
	{
		NO_INITIALIZATION
	};

	static unsigned long hashString(const char* str)
	{
		unsigned long hash = 5381;
		int c;
		while (c = *str++)
		{
			hash = ((hash << 5) + hash) + c;
		}
		return hash;
	}

	static std::string textFileRead(const std::string &filename)
	{
		std::string file = std::string();
		std::string line = std::string();

		std::ifstream fileStream(filename);
		if (!fileStream.good())
		{
			std::cerr << "Error opening file \"" + filename + "\"";
			throw std::exception();
		}
		std::stringstream sstream;
		sstream << fileStream.rdbuf();
		fileStream.close();

		return sstream.str();
	}

	static inline bool textContains(char* text, char ch)
	{
		while (*text)
		{
			if (*text++ == ch)
				return true;
		}
		return false;
	}
	std::vector<std::string> splitString(const std::string &text, char* delims,
			unsigned short maxParts = 0);
	std::string firstLine(const std::string &text);
	std::string firstWord(const std::string &text);

	static inline float randomFloat()
	{
		return (float)rand() / (float)RAND_MAX;
	}

	static inline float randomFloatInRange(float min = 0.0f, float max = 1.0f)
	{
		return min + (float)rand()/((float)RAND_MAX / (max - min));
	}


//-------------------------------------------
//			Binary File input
//-------------------------------------------
	class BinaryFile
	{
	public:
		BinaryFile(const std::string &filename) : stream(filename.c_str(), std::ios::in | std::ios::binary)
		{
			/*if (!stream || stream.eof());*/
		}

		template<typename T>
		void read(T* reader)
		{
			if (stream.eof()) return;
			stream.read(reinterpret_cast<char*>(reader), sizeof(T));
		}

		virtual ~BinaryFile()
		{
			stream.close();
		}
	private:
		 std::ifstream stream;
	};

//-------------------------------------------
//			Conversion
//-------------------------------------------
	static inline int floatToInt_truncate (double val)
	{
		return (int) val;
	}

	static inline int floatToInt_fastRound (double val)
	{
		union
		{
			double dval;
			int ival[2];
		} u;

		u.dval = val + 6755399441055744.0;
		return u.ival[0]; // Should be [1] for big-endian systems
	}

	static std::string extractAppPath(const char* fullPath)
	{
		const std::string s(fullPath);
		if (s.find("/") != std::string::npos)
			return s.substr(0, s.rfind("/")) + "/";
		else if (s.find("\\") != std::string::npos)
			return s.substr(0, s.rfind("\\")) + "\\";
		else
			return "";
	}
}

//-------------------------------------------
//			Vectors
//-------------------------------------------

class Vector2
{
public:
	float x, y;
	Vector2(const float x, const float y) : x(x), y(y) {}
	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(Math::ForceNoInitialization){}
	Vector2(const float f) : x(f), y(f) {}
	float &operator[](unsigned short index){return *(&x + index);}
	bool operator==(const Vector2 &other)
	{
		return (x > other.x - Math::Epsilon && x < other.x + Math::Epsilon &&
				y > other.y - Math::Epsilon && y < other.y + Math::Epsilon);
	}
	bool operator!=(const Vector2 &other)
	{
		return (x < other.x - Math::Epsilon || x > other.x + Math::Epsilon ||
				y < other.y - Math::Epsilon || y > other.y + Math::Epsilon);
	}
	Vector2 operator-() const { return Vector2(-x, -y); }
	Vector2 operator+(const Vector2 &other) const { return Vector2(x + other.x, y + other.y); }
	Vector2 operator+(const float f) const { return Vector2(x + f, y + f); }
	Vector2 operator+=(const Vector2 &other) { return *this = *this + other; }
	Vector2 operator+=(const float f) { return *this = *this + f; }
	Vector2 operator-(const Vector2 &other) const { return Vector2(x - other.x, y - other.y); }
	Vector2 operator-(const float f) const { return Vector2(x - f, y - f); }
	Vector2 operator-=(const Vector2 &other) { return *this = *this - other; }
	Vector2 operator-=(const float f) { return *this = *this - f; }
	Vector2 operator*(const Vector2 &other) const { return Vector2(x * other.x, y * other.y); }
	Vector2 operator*(const float f) const { return Vector2(x * f, y * f); }
	Vector2 operator*=(const Vector2 &other) { return *this = *this * other; }
	Vector2 operator*=(const float f) { return *this = *this * f; }
	Vector2 operator/(const Vector2 &other) const { return Vector2(x / other.x, y / other.y); }
	Vector2 operator/(const float f) const { float fi = 1.0f / f; return Vector2(x*fi, y*fi); }
	Vector2 operator/=(const Vector2 &other) { return *this = *this / other; }
	Vector2 operator/=(const float f) { return *this = *this / f; }

	float dot(const Vector2 &other) const { return x*other.x + y*other.y; }
	float length() const { return sqrtf(x*x + y*y); }
	float lengthSquared() const { return x*x + y*y; }
	float distance(const Vector2 &other) const { return (other - *this).length(); }
	float distanceSquared(const Vector2 &other) const { return (other - *this).lengthSquared(); }
	Vector2 normalized() const { float fi = 1.0f / sqrtf(x*x+y*y); return Vector2(x*fi, y*fi); }
	void normalize() { float fi = 1.0f / sqrtf(x*x+y*y); x *= fi; y *= fi; }

	friend std::ostream& operator<< (std::ostream &o, const Vector2 &v)
	{
		o << "<" << v.x << ", " << v.y << ">";
		return o;
	}
};

class Vector3
{
public:
	float x, y, z;
	//Vector3 constructors
	Vector3(const float f) : x(f), y(f), z(f) {}
	explicit Vector3(const Vector2 &vec2) : x(vec2.x), y(vec2.y), z(0.0f) {}
	Vector3() { x = 0.0f; y = 0.0f; z = 0.0f; }
	explicit Vector3(Math::ForceNoInitialization) {}
	Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	Vector3(const Vector3 &other) : x(other.x), y (other.y), z (other.z) {}
	Vector3(const btVector3 &v) : x(v.x()), y(v.y()), z(v.z()) {}
	//Vector3 accessors
	float &operator[] (unsigned short index) { return *(&x + index); }
	float operator[](unsigned short index) const { return *(&x + index); }
	bool operator==(const Vector3& other) const
	{
		return (x > other.x - Math::Epsilon && x < other.x + Math::Epsilon &&
				y > other.y - Math::Epsilon && y < other.y + Math::Epsilon &&
				z > other.z - Math::Epsilon && z < other.z + Math::Epsilon);
	}
	bool operator!=(const Vector3 &other) const
	{
		return (x < other.x - Math::Epsilon || x > other.x + Math::Epsilon ||
				y < other.y - Math::Epsilon || y > other.y + Math::Epsilon ||
				z < other.z - Math::Epsilon || z > other.z + Math::Epsilon);
	}
	//Vector3 arithmetic
	//Negation
	Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	//Addition
	Vector3 operator+(const Vector3 &other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator+ (const float f) const
	{
		return Vector3(x + f, y + f, z + f);
	}
	Vector3& operator+=(const Vector3 &v)
	{
		return *this = *this + v;
	}
	Vector3& operator+=(const float f)
	{
		return *this = *this + f;
	}

	//Subtraction
	Vector3 operator-(const Vector3 &v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	Vector3 operator-(const float f) const
	{
		return Vector3(x - f, y - f, z - f);
	}
	Vector3& operator-=(const Vector3 &v)
	{
		return *this = *this - v;
	}
	Vector3& operator-=(const float f)
	{
		return *this = *this - f;
	}

	//Multiplication
	Vector3 operator*(const Vector3 &v) const
	{
		return Vector3(x * v.x, y * v.y, z * v.z);
	}
	Vector3 operator*(const float f) const
	{
		return Vector3(x * f, y * f, z * f);
	}
	Vector3& operator*=(const Vector3& v)
	{
		return *this = *this * v;
	}
	Vector3& operator*=(const float f)
	{
		return *this = *this * f;
	}

	//Division
	Vector3 operator/(const Vector3 &v) const
	{
		return Vector3(x / v.x, y / v.y, z / v.z);
	}
	Vector3 operator/(const float f) const
	{
		return Vector3(x / f, y / f, z / f);
	}
	Vector3& operator/=(const Vector3 &v)
	{
		return *this = *this / v;
	}
	Vector3& operator/=(const float f)
	{
		return *this = *this / f;
	}

	//Special vector products
	float dot(const Vector3 &v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	Vector3 cross(const Vector3 &v) const
	{
		return Vector3( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
	}

	//Other functions
	float length() const
	{
		return sqrtf(x*x + y*y + z*z);
	}
	float lengthSquared() const
	{
		return x*x + y*y + z*z;
	}
	float distance(const Vector3 &other) const
	{
		return (other - *this).length();
	}
	float distanceSquared(const Vector3 &other) const
	{
		return (other - *this).lengthSquared();
	}


	Vector3 normalized() const
	{
		float invLength = 1.0f / length();
		return Vector3(x*invLength, y*invLength, z*invLength);
	}
	void normalize()
	{
		float invLength = 1.0f / length();
		x *= invLength;
		y *= invLength;
		z *= invLength;
	}
	Vector3 toRotation() const
	{
		//Assume unrotated vector is (0, 0, 1)
		Vector3 v;
		if (y != 0) v.x = atan2f(y, sqrtf(x*x + z*z));
		if (x != 0 || z != 0) v.y = atan2f(-x, -z);
		return v;
	}
	static Vector3 lerp(const Vector3 &a, const Vector3 &b, const float f)
	{
		return a + (b-a)*f;
	}

	static const Vector3 ONE;
	static const Vector3 ZERO;
	static const Vector3 RIGHT;
	static const Vector3 UP;
	static const Vector3 FORWARD;

	//Library conversion
	btVector3 bullet() const { return btVector3(x, y, z); }

	friend std::ostream& operator<< (std::ostream &o, const Vector3 &v)
	{
		o << "<" << v.x << ", " << v.y << ", " << v.z << ">";
		return o;
	}
};

class Vector4
{
public:
	float x, y, z, w;
	//Constructors
	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	explicit Vector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
	explicit Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(1.0f) {}
	Vector4 operator+(const Vector4 &v) const
	{
		return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	Vector4 operator*(const float f) const
	{
		return Vector4(x*f, y*f, z*f, w*f);
	}
	btVector4 bullet() { return btVector4(x, y, z, w); }
};

class Quaternion
{
public:
	float x, y, z, w;

	//Constructors
	Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	explicit Quaternion(const float x, const float y, const float z, const float w) :
			x(x), y(y), z(z), w(w) {}
	Quaternion(const Vector3 &vec)
	{
		Quaternion(vec.x, vec.y, vec.z);
	}
	Quaternion(const float eulerX, const float eulerY, const float eulerZ)
	{
		Quaternion roll( sinf( eulerX * 0.5f ), 0, 0, cosf( eulerX * 0.5f ) );
		Quaternion pitch( 0, sinf( eulerY * 0.5f ), 0, cosf( eulerY * 0.5f ) );
		Quaternion yaw( 0, 0, sinf( eulerZ * 0.5f ), cosf( eulerZ * 0.5f ) );
		// Order: y * x * z
		*this = pitch * roll * yaw;
	}
	Quaternion(const Vector3 &axis, const float angle)
	{
		Vector3 ax = axis * sinf(angle * 0.5f);
		x = ax.x; y = ax.y; z = ax.z; w = cosf(angle * 0.5f);
	}
	Quaternion(const btQuaternion &q)
	{
		x = q.x();
		y = q.y();
		z = q.z();
		w = q.w();
	}

	//Arithmetic with quaternions
	Quaternion operator*(const Quaternion &q) const
	{
		return Quaternion(
			y * q.z - z * q.y + q.x * w + x * q.w,
			z * q.x - x * q.z + q.y * w + y * q.w,
			x * q.y - y * q.x + q.z * w + z * q.w,
			w * q.w - (x * q.x + y * q.y + z * q.z) );
	}
	Quaternion& operator*=(const Quaternion &q)
	{
		return *this = *this * q;
	}
	Vector3 operator*(const Vector3 &v) const
	{
		Vector3 uv, uuv;
		Vector3 qvec(x, y, z);
		uv = qvec.cross(v);
		uuv = qvec.cross(uv);
		uv *= (2.0f * w);
		uuv *= 2.0f;
		return v + uv + uuv;
	}

	float norm() const
	{
		return x*x + y*y + z*z + w*w;
	}

	void normalize()
	{
		float len = norm();
		float factor = 1.0f / sqrtf(len);
		*this = *this * factor;
	}
	void setXYZ(const float x, const float y, const float z) { this->x = x, this->y = y; this->z = z; }

	//Other operations
	static Quaternion slerp(const Quaternion &a, const Quaternion &b, const float t)
	{
		// Spherical linear interpolation between two quaternions
		// Note: SLERP is not commutative

		Quaternion q1(a);

		// Calculate cosine
		float cosTheta = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

		// Use the shortest path
		if( cosTheta < 0 )
		{
			cosTheta = -cosTheta;
			q1.x = -b.x; q1.y = -b.y;
			q1.z = -b.z; q1.w = -b.w;
		}

		// Initialize with linear interpolation
		float scale0 = 1 - t, scale1 = t;

		// Use spherical interpolation only if the quaternions are not very close
		if( (1 - cosTheta) > 0.001f )
		{
			// SLERP
			float theta = acosf( cosTheta );
			float sinTheta = sinf( theta );
			scale0 = sinf( (1 - t) * theta ) / sinTheta;
			scale1 = sinf( t * theta ) / sinTheta;
		}

		// Calculate final quaternion
		return Quaternion( a.x * scale0 + q1.x * scale1, a.y * scale0 + q1.y * scale1,
						   a.z * scale0 + q1.z * scale1, a.w * scale0 + q1.w * scale1 );
	}

	static Quaternion nlerp(const Quaternion &a, const Quaternion &b, const float t)
	{
		/* Normalized linear quaternion interpolation;
		 * NLERP is faster than SLERP and commutative but does
		 * not yield constant velocity*/

		Quaternion qt;
		float cosTheta = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

		// Use the shortest path and interpolate linearly
		if( cosTheta < 0 )
			qt = Quaternion( a.x + (-b.x - a.x) * t, a.y + (-b.y - a.y) * t,
							 a.z + (-b.z - a.z) * t, a.w + (-b.w - a.w) * t );
		else
			qt = Quaternion( a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t,
							 a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t );

		// Return normalized quaternion
		float invLen = 1.0f / sqrtf( qt.x * qt.x + qt.y * qt.y + qt.z * qt.z + qt.w * qt.w );
		return Quaternion( qt.x * invLen, qt.y * invLen, qt.z * invLen, qt.w * invLen );
	}
	static Quaternion rotationBetweenVectors(const Vector3 &from, const Vector3 &to)
	{
		Quaternion q;
		Vector3 a = from.cross(to);
		q.setXYZ(a.x, a.y, a.z);

		float fromMag = from.length(); float toMag = to.length();
		q.w = sqrt((fromMag * fromMag) * (toMag * toMag)) + from.dot(to);
		q.normalize();
		return q;
	}
	static const Quaternion IDENTITY;

	//Library conversion
	btQuaternion bullet() const { return btQuaternion(x, y, z, w); }

	friend std::ostream& operator<<(std::ostream &os, const Quaternion &q)
	{
		os << "<" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ">";
		return os;
	}
};

class Matrix4
{
public:
	union
	{
		float mm[4][4]; //Column-major
		float m[16];
	};

	//Static construction methods
	static Matrix4 initTransMat(const Vector3 &v)
	{
		Matrix4 mat;
		mat.mm[3][0] = v.x;
		mat.mm[3][1] = v.y;
		mat.mm[3][2] = v.z;
		return mat;
	}
	static Matrix4 initTransMat(const float x, const float y, const float z)
	{
		return initTransMat(Vector3(x, y, z));
	}
	static Matrix4 initScaleMat(const Vector3 &v)
	{
		Matrix4 mat;
		mat.mm[0][0] = v.x;
		mat.mm[1][1] = v.y;
		mat.mm[2][2] = v.z;
		return mat;
	}
	static Matrix4 initScaleMat(const float x, const float y, const float z)
	{
		return initScaleMat(Vector3(x, y, z));
	}
	static Matrix4 initRotMat(const Vector3 &v)
	{
		//Rotation is in YXZ order
		return Matrix4(Quaternion(v.x, v.y, v.z));
	}
	static Matrix4 initRotMat(const float x, const float y, const float z)
	{
		//Rotation is in YXZ order
		return Matrix4(Quaternion(Vector3(x, y, z)));
	}
	static Matrix4 initRotMat(const Vector3 &axis, const float angle)
	{
		return Matrix4(Quaternion(axis, angle));
	}
	static Matrix4 initPerspMat(float l, float r, float b, float t, float n, float f)
	{
		Matrix4 mat;
		mat.m[0] = 2 * n / (r - l);
		mat.m[5] = 2 * n / (t - b);
		mat.m[8] = (r + l) / (r - l);
		mat.m[9] = (t + b) / (t - b);
		mat.m[10] = -(f + n) / (f - n);
		mat.m[11] = -1;
		mat.m[14] = -2 * f * n / (f - n);
		mat.m[15] = 0;
		return mat;
	}
	/**
	 * Updated to be Horde3D-compliant
	 */
	static Matrix4 initPerspMat(float degreeFOV, float aspect, float zNear, float zFar)
	{
		Matrix4 mat = Matrix4(0.0f);
		float zRange = zNear - zFar;
		float tanHalfFOV = tanf(Math::degToRad(degreeFOV) / 2.0f);

		mat.mm[0][0] = 1.0f / (tanHalfFOV * aspect);
		mat.mm[1][1] = 1.0f / tanHalfFOV;

		mat.mm[2][2] = (zNear + zFar) / zRange;
		mat.mm[3][2] = 2.0f * zFar * zNear / zRange;
		mat.mm[2][3] = -1.0f;

		return mat;
	}
	static Matrix4 initOrthoMat(float l, float r, float b, float t, float n, float f)
	{
		Matrix4 mat;
		mat.m[0] = 2 / (r - l);
		mat.m[5] = 2 / (t - b);
		mat.m[10] = -2 / (f - n);
		mat.m[12] = -(r + l) / (r - l);
		mat.m[13] = -(t + b) / (t - b);
		mat.m[14] = -(f + n) / (f - n);
		return mat;
	}

	static void fastMult43(Matrix4 &dst, const Matrix4 &m1, const Matrix4 &m2 )
	{
		// Note: dst may not be the same as m1 or m2
		float *dstx = dst.m;
		const float *m1x = m1.m;
		const float *m2x = m2.m;

		dstx[0] = m1x[0] * m2x[0] + m1x[4] * m2x[1] + m1x[8] * m2x[2];
		dstx[1] = m1x[1] * m2x[0] + m1x[5] * m2x[1] + m1x[9] * m2x[2];
		dstx[2] = m1x[2] * m2x[0] + m1x[6] * m2x[1] + m1x[10] * m2x[2];
		dstx[3] = 0.0f;

		dstx[4] = m1x[0] * m2x[4] + m1x[4] * m2x[5] + m1x[8] * m2x[6];
		dstx[5] = m1x[1] * m2x[4] + m1x[5] * m2x[5] + m1x[9] * m2x[6];
		dstx[6] = m1x[2] * m2x[4] + m1x[6] * m2x[5] + m1x[10] * m2x[6];
		dstx[7] = 0.0f;

		dstx[8] = m1x[0] * m2x[8] + m1x[4] * m2x[9] + m1x[8] * m2x[10];
		dstx[9] = m1x[1] * m2x[8] + m1x[5] * m2x[9] + m1x[9] * m2x[10];
		dstx[10] = m1x[2] * m2x[8] + m1x[6] * m2x[9] + m1x[10] * m2x[10];
		dstx[11] = 0.0f;

		dstx[12] = m1x[0] * m2x[12] + m1x[4] * m2x[13] + m1x[8] * m2x[14] + m1x[12] * m2x[15];
		dstx[13] = m1x[1] * m2x[12] + m1x[5] * m2x[13] + m1x[9] * m2x[14] + m1x[13] * m2x[15];
		dstx[14] = m1x[2] * m2x[12] + m1x[6] * m2x[13] + m1x[10] * m2x[14] + m1x[14] * m2x[15];
		dstx[15] = 1.0f;
	}

	//Constructors
	Matrix4(const float f)
	{
		for (int i=0; i < 16; i++)
		{
			m[i] = f;
		}
	}
	Matrix4(const Matrix4 &other)
	{
		for (int i=0; i < 16; i++)
		{
			m[i] = other.m[i];
		}
	}
	Matrix4()
	{
		mm[0][0] = 1; mm[1][0] = 0; mm[2][0] = 0; mm[3][0] = 0;
		mm[0][1] = 0; mm[1][1] = 1; mm[2][1] = 0; mm[3][1] = 0;
		mm[0][2] = 0; mm[1][2] = 0; mm[2][2] = 1; mm[3][2] = 0;
		mm[0][3] = 0; mm[1][3] = 0; mm[2][3] = 0; mm[3][3] = 1;
	}
	explicit Matrix4(Math::ForceNoInitialization) {}
	Matrix4(const float *floatArray16)
	{
		for( unsigned int j = 0; j < 4; ++j )
		{
			for( unsigned int i = 0; i < 4; ++i )
			{
				mm[i][j] = floatArray16[i * 4 + j];
			}
		}
	}
	Matrix4(const Quaternion &q)
	{
		// Calculate coefficients
		float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
		float xx = q.x * x2,  xy = q.x * y2,  xz = q.x * z2;
		float yy = q.y * y2,  yz = q.y * z2,  zz = q.z * z2;
		float wx = q.w * x2,  wy = q.w * y2,  wz = q.w * z2;

		mm[0][0] = 1 - (yy + zz);  mm[1][0] = xy - wz;
		mm[2][0] = xz + wy;        mm[3][0] = 0;
		mm[0][1] = xy + wz;        mm[1][1] = 1 - (xx + zz);
		mm[2][1] = yz - wx;        mm[3][1] = 0;
		mm[0][2] = xz - wy;        mm[1][2] = yz + wx;
		mm[2][2] = 1 - (xx + yy);  mm[3][2] = 0;
		mm[0][3] = 0;              mm[1][3] = 0;
		mm[2][3] = 0;              mm[3][3] = 1;
	}

	//Sums
	Matrix4 operator+(const Matrix4 &mat) const
	{
		Matrix4 mf(Math::NO_INITIALIZATION);
		mf.m[0] = m[0] + mat.m[0];
		mf.m[1] = m[1] + mat.m[1];
		mf.m[2] = m[2] + mat.m[2];
		mf.m[3] = m[3] + mat.m[3];
		mf.m[4] = m[4] + mat.m[4];
		mf.m[5] = m[5] + mat.m[5];
		mf.m[6] = m[6] + mat.m[6];
		mf.m[7] = m[7] + mat.m[7];
		mf.m[8] = m[8] + mat.m[8];
		mf.m[9] = m[9] + mat.m[9];
		mf.m[10] = m[10] + mat.m[10];
		mf.m[11] = m[11] + mat.m[11];
		mf.m[12] = m[12] + mat.m[12];
		mf.m[13] = m[13] + mat.m[13];
		mf.m[14] = m[14] + mat.m[14];
		mf.m[15] = m[15] + mat.m[15];
		return mf;
	}
	Matrix4 &operator+=(const Matrix4 &mat)
	{
		return *this = *this + mat;
	}

	float* ptr()
	{
		return &m[0];
	}

	//Multiplication
	Matrix4 operator*(const Matrix4 &mat) const
	{
		Matrix4 mf(Math::NO_INITIALIZATION);

		mf.m[0] = m[0] * mat.m[0] + m[4] * mat.m[1] + m[8] * mat.m[2] + m[12] * mat.m[3];
		mf.m[1] = m[1] * mat.m[0] + m[5] * mat.m[1] + m[9] * mat.m[2] + m[13] * mat.m[3];
		mf.m[2] = m[2] * mat.m[0] + m[6] * mat.m[1] + m[10] * mat.m[2] + m[14] * mat.m[3];
		mf.m[3] = m[3] * mat.m[0] + m[7] * mat.m[1] + m[11] * mat.m[2] + m[15] * mat.m[3];

		mf.m[4] = m[0] * mat.m[4] + m[4] * mat.m[5] + m[8] * mat.m[6] + m[12] * mat.m[7];
		mf.m[5] = m[1] * mat.m[4] + m[5] * mat.m[5] + m[9] * mat.m[6] + m[13] * mat.m[7];
		mf.m[6] = m[2] * mat.m[4] + m[6] * mat.m[5] + m[10] * mat.m[6] + m[14] * mat.m[7];
		mf.m[7] = m[3] * mat.m[4] + m[7] * mat.m[5] + m[11] * mat.m[6] + m[15] * mat.m[7];

		mf.m[8] = m[0] * mat.m[8] + m[4] * mat.m[9] + m[8] * mat.m[10] + m[12] * mat.m[11];
		mf.m[9] = m[1] * mat.m[8] + m[5] * mat.m[9] + m[9] * mat.m[10] + m[13] * mat.m[11];
		mf.m[10] = m[2] * mat.m[8] + m[6] * mat.m[9] + m[10] * mat.m[10] + m[14] * mat.m[11];
		mf.m[11] = m[3] * mat.m[8] + m[7] * mat.m[9] + m[11] * mat.m[10] + m[15] * mat.m[11];

		mf.m[12] = m[0] * mat.m[12] + m[4] * mat.m[13] + m[8] * mat.m[14] + m[12] * mat.m[15];
		mf.m[13] = m[1] * mat.m[12] + m[5] * mat.m[13] + m[9] * mat.m[14] + m[13] * mat.m[15];
		mf.m[14] = m[2] * mat.m[12] + m[6] * mat.m[13] + m[10] * mat.m[14] + m[14] * mat.m[15];
		mf.m[15] = m[3] * mat.m[12] + m[7] * mat.m[13] + m[11] * mat.m[14] + m[15] * mat.m[15];

		return mf;
	}

	Matrix4 operator*(const float f) const
	{
		Matrix4 mat( *this );
		mat.m[0]  *= f; mat.m[1]  *= f; mat.m[2]  *= f; mat.m[3]  *= f;
		mat.m[4]  *= f; mat.m[5]  *= f; mat.m[6]  *= f; mat.m[7]  *= f;
		mat.m[8]  *= f; mat.m[9]  *= f; mat.m[10] *= f; mat.m[11] *= f;
		mat.m[12] *= f; mat.m[13] *= f; mat.m[14] *= f; mat.m[15] *= f;
		return mat;
	}

	// ----------------------------
	// Vector-Matrix multiplication
	// ----------------------------
	Vector3 operator*(const Vector3 &v) const
	{
		return Vector3( v.x * mm[0][0] + v.y * mm[1][0] + v.z * mm[2][0] + mm[3][0],
		              v.x * mm[0][1] + v.y * mm[1][1] + v.z * mm[2][1] + mm[3][1],
		              v.x * mm[0][2] + v.y * mm[1][2] + v.z * mm[2][2] + mm[3][2] );
	}

	Vector4 operator*(const Vector4 &v) const
	{
		return Vector4( v.x * mm[0][0] + v.y * mm[1][0] + v.z * mm[2][0] + v.w * mm[3][0],
		              v.x * mm[0][1] + v.y * mm[1][1] + v.z * mm[2][1] + v.w * mm[3][1],
		              v.x * mm[0][2] + v.y * mm[1][2] + v.z * mm[2][2] + v.w * mm[3][2],
		              v.x * mm[0][3] + v.y * mm[1][3] + v.z * mm[2][3] + v.w * mm[3][3] );
	}

	Vector3 mult33Vec(const Vector3 &v) const
	{
		return Vector3( v.x * mm[0][0] + v.y * mm[1][0] + v.z * mm[2][0],
		              v.x * mm[0][1] + v.y * mm[1][1] + v.z * mm[2][1],
		              v.x * mm[0][2] + v.y * mm[1][2] + v.z * mm[2][2] );
	}

	// ---------------
	// Transformations
	// ---------------
	void translate( const float x, const float y, const float z )
	{
		*this = initTransMat(Vector3(x, y, z)) * *this;
	}

	void scale( const float x, const float y, const float z )
	{
		*this = initScaleMat( x, y, z ) * *this;
	}

	void rotate( const float x, const float y, const float z )
	{
		*this = initRotMat( x, y, z ) * *this;
	}

	Matrix4 transposed() const
	{
		Matrix4 mat(*this);
		for( unsigned int y = 0; y < 4; ++y )
		{
			for( unsigned int x = y + 1; x < 4; ++x )
			{
				float tmp = mat.mm[x][y];
				mat.mm[x][y] = mat.mm[y][x];
				mat.mm[y][x] = tmp;
			}
		}
		return mat;
	}

	float determinant() const
	{
		return
			mm[0][3]*mm[1][2]*mm[2][1]*mm[3][0] - mm[0][2]*mm[1][3]*mm[2][1]*mm[3][0] - mm[0][3]*mm[1][1]*mm[2][2]*mm[3][0] + mm[0][1]*mm[1][3]*mm[2][2]*mm[3][0] +
			mm[0][2]*mm[1][1]*mm[2][3]*mm[3][0] - mm[0][1]*mm[1][2]*mm[2][3]*mm[3][0] - mm[0][3]*mm[1][2]*mm[2][0]*mm[3][1] + mm[0][2]*mm[1][3]*mm[2][0]*mm[3][1] +
			mm[0][3]*mm[1][0]*mm[2][2]*mm[3][1] - mm[0][0]*mm[1][3]*mm[2][2]*mm[3][1] - mm[0][2]*mm[1][0]*mm[2][3]*mm[3][1] + mm[0][0]*mm[1][2]*mm[2][3]*mm[3][1] +
			mm[0][3]*mm[1][1]*mm[2][0]*mm[3][2] - mm[0][1]*mm[1][3]*mm[2][0]*mm[3][2] - mm[0][3]*mm[1][0]*mm[2][1]*mm[3][2] + mm[0][0]*mm[1][3]*mm[2][1]*mm[3][2] +
			mm[0][1]*mm[1][0]*mm[2][3]*mm[3][2] - mm[0][0]*mm[1][1]*mm[2][3]*mm[3][2] - mm[0][2]*mm[1][1]*mm[2][0]*mm[3][3] + mm[0][1]*mm[1][2]*mm[2][0]*mm[3][3] +
			mm[0][2]*mm[1][0]*mm[2][1]*mm[3][3] - mm[0][0]*mm[1][2]*mm[2][1]*mm[3][3] - mm[0][1]*mm[1][0]*mm[2][2]*mm[3][3] + mm[0][0]*mm[1][1]*mm[2][2]*mm[3][3];
	}

	Matrix4 inverted() const
	{
		Matrix4 mat(Math::NO_INITIALIZATION);

		float d = determinant();
		if( d == 0 ) return mat;
		d = 1.0f / d;

		mat.mm[0][0] = d * (mm[1][2]*mm[2][3]*mm[3][1] - mm[1][3]*mm[2][2]*mm[3][1] + mm[1][3]*mm[2][1]*mm[3][2] - mm[1][1]*mm[2][3]*mm[3][2] - mm[1][2]*mm[2][1]*mm[3][3] + mm[1][1]*mm[2][2]*mm[3][3]);
		mat.mm[0][1] = d * (mm[0][3]*mm[2][2]*mm[3][1] - mm[0][2]*mm[2][3]*mm[3][1] - mm[0][3]*mm[2][1]*mm[3][2] + mm[0][1]*mm[2][3]*mm[3][2] + mm[0][2]*mm[2][1]*mm[3][3] - mm[0][1]*mm[2][2]*mm[3][3]);
		mat.mm[0][2] = d * (mm[0][2]*mm[1][3]*mm[3][1] - mm[0][3]*mm[1][2]*mm[3][1] + mm[0][3]*mm[1][1]*mm[3][2] - mm[0][1]*mm[1][3]*mm[3][2] - mm[0][2]*mm[1][1]*mm[3][3] + mm[0][1]*mm[1][2]*mm[3][3]);
		mat.mm[0][3] = d * (mm[0][3]*mm[1][2]*mm[2][1] - mm[0][2]*mm[1][3]*mm[2][1] - mm[0][3]*mm[1][1]*mm[2][2] + mm[0][1]*mm[1][3]*mm[2][2] + mm[0][2]*mm[1][1]*mm[2][3] - mm[0][1]*mm[1][2]*mm[2][3]);
		mat.mm[1][0] = d * (mm[1][3]*mm[2][2]*mm[3][0] - mm[1][2]*mm[2][3]*mm[3][0] - mm[1][3]*mm[2][0]*mm[3][2] + mm[1][0]*mm[2][3]*mm[3][2] + mm[1][2]*mm[2][0]*mm[3][3] - mm[1][0]*mm[2][2]*mm[3][3]);
		mat.mm[1][1] = d * (mm[0][2]*mm[2][3]*mm[3][0] - mm[0][3]*mm[2][2]*mm[3][0] + mm[0][3]*mm[2][0]*mm[3][2] - mm[0][0]*mm[2][3]*mm[3][2] - mm[0][2]*mm[2][0]*mm[3][3] + mm[0][0]*mm[2][2]*mm[3][3]);
		mat.mm[1][2] = d * (mm[0][3]*mm[1][2]*mm[3][0] - mm[0][2]*mm[1][3]*mm[3][0] - mm[0][3]*mm[1][0]*mm[3][2] + mm[0][0]*mm[1][3]*mm[3][2] + mm[0][2]*mm[1][0]*mm[3][3] - mm[0][0]*mm[1][2]*mm[3][3]);
		mat.mm[1][3] = d * (mm[0][2]*mm[1][3]*mm[2][0] - mm[0][3]*mm[1][2]*mm[2][0] + mm[0][3]*mm[1][0]*mm[2][2] - mm[0][0]*mm[1][3]*mm[2][2] - mm[0][2]*mm[1][0]*mm[2][3] + mm[0][0]*mm[1][2]*mm[2][3]);
		mat.mm[2][0] = d * (mm[1][1]*mm[2][3]*mm[3][0] - mm[1][3]*mm[2][1]*mm[3][0] + mm[1][3]*mm[2][0]*mm[3][1] - mm[1][0]*mm[2][3]*mm[3][1] - mm[1][1]*mm[2][0]*mm[3][3] + mm[1][0]*mm[2][1]*mm[3][3]);
		mat.mm[2][1] = d * (mm[0][3]*mm[2][1]*mm[3][0] - mm[0][1]*mm[2][3]*mm[3][0] - mm[0][3]*mm[2][0]*mm[3][1] + mm[0][0]*mm[2][3]*mm[3][1] + mm[0][1]*mm[2][0]*mm[3][3] - mm[0][0]*mm[2][1]*mm[3][3]);
		mat.mm[2][2] = d * (mm[0][1]*mm[1][3]*mm[3][0] - mm[0][3]*mm[1][1]*mm[3][0] + mm[0][3]*mm[1][0]*mm[3][1] - mm[0][0]*mm[1][3]*mm[3][1] - mm[0][1]*mm[1][0]*mm[3][3] + mm[0][0]*mm[1][1]*mm[3][3]);
		mat.mm[2][3] = d * (mm[0][3]*mm[1][1]*mm[2][0] - mm[0][1]*mm[1][3]*mm[2][0] - mm[0][3]*mm[1][0]*mm[2][1] + mm[0][0]*mm[1][3]*mm[2][1] + mm[0][1]*mm[1][0]*mm[2][3] - mm[0][0]*mm[1][1]*mm[2][3]);
		mat.mm[3][0] = d * (mm[1][2]*mm[2][1]*mm[3][0] - mm[1][1]*mm[2][2]*mm[3][0] - mm[1][2]*mm[2][0]*mm[3][1] + mm[1][0]*mm[2][2]*mm[3][1] + mm[1][1]*mm[2][0]*mm[3][2] - mm[1][0]*mm[2][1]*mm[3][2]);
		mat.mm[3][1] = d * (mm[0][1]*mm[2][2]*mm[3][0] - mm[0][2]*mm[2][1]*mm[3][0] + mm[0][2]*mm[2][0]*mm[3][1] - mm[0][0]*mm[2][2]*mm[3][1] - mm[0][1]*mm[2][0]*mm[3][2] + mm[0][0]*mm[2][1]*mm[3][2]);
		mat.mm[3][2] = d * (mm[0][2]*mm[1][1]*mm[3][0] - mm[0][1]*mm[1][2]*mm[3][0] - mm[0][2]*mm[1][0]*mm[3][1] + mm[0][0]*mm[1][2]*mm[3][1] + mm[0][1]*mm[1][0]*mm[3][2] - mm[0][0]*mm[1][1]*mm[3][2]);
		mat.mm[3][3] = d * (mm[0][1]*mm[1][2]*mm[2][0] - mm[0][2]*mm[1][1]*mm[2][0] + mm[0][2]*mm[1][0]*mm[2][1] - mm[0][0]*mm[1][2]*mm[2][1] - mm[0][1]*mm[1][0]*mm[2][2] + mm[0][0]*mm[1][1]*mm[2][2]);

		return mat;
	}

	void decompose(Vector3 &trans, Vector3 &rot, Vector3 &scale ) const
	{
		// Getting translation is trivial
		trans = Vector3( mm[3][0], mm[3][1], mm[3][2] );

		// Scale is length of columns
		scale.x = sqrtf( mm[0][0] * mm[0][0] + mm[0][1] * mm[0][1] + mm[0][2] * mm[0][2] );
		scale.y = sqrtf( mm[1][0] * mm[1][0] + mm[1][1] * mm[1][1] + mm[1][2] * mm[1][2] );
		scale.z = sqrtf( mm[2][0] * mm[2][0] + mm[2][1] * mm[2][1] + mm[2][2] * mm[2][2] );

		if( scale.x == 0 || scale.y == 0 || scale.z == 0 ) return;

		// Detect negative scale with determinant and flip one arbitrary axis
		if( determinant() < 0 ) scale.x = -scale.x;

		// Combined rotation matrix YXZ
		//
		// Cos[y]*Cos[z]+Sin[x]*Sin[y]*Sin[z]   Cos[z]*Sin[x]*Sin[y]-Cos[y]*Sin[z]  Cos[x]*Sin[y]
		// Cos[x]*Sin[z]                        Cos[x]*Cos[z]                       -Sin[x]
		// -Cos[z]*Sin[y]+Cos[y]*Sin[x]*Sin[z]  Cos[y]*Cos[z]*Sin[x]+Sin[y]*Sin[z]  Cos[x]*Cos[y]

		rot.x = asinf( -mm[2][1] / scale.z );

		// Special case: Cos[x] == 0 (when Sin[x] is +/-1)
		float f = fabsf( mm[2][1] / scale.z );
		if( f > 0.999f && f < 1.001f )
		{
			// Pin arbitrarily one of y or z to zero
			// Mathematical equivalent of gimbal lock
			rot.y = 0;

			// Now: Cos[x] = 0, Sin[x] = +/-1, Cos[y] = 1, Sin[y] = 0
			// => m[0][0] = Cos[z] and m[1][0] = Sin[z]
			rot.z = atan2f( -mm[1][0] / scale.y, mm[0][0] / scale.x );
		}
		// Standard case
		else
		{
			rot.y = atan2f( mm[2][0] / scale.z, mm[2][2] / scale.z );
			rot.z = atan2f( mm[0][1] / scale.x, mm[1][1] / scale.y );
		}
	}

	Vector4 getCol( unsigned int col ) const
	{
		return Vector4( m[col * 4 + 0], m[col * 4 + 1], m[col * 4 + 2], m[col * 4 + 3] );
	}

	Vector4 getRow( unsigned int row ) const
	{
		return Vector4( m[row + 0], m[row + 4], m[row + 8], m[row + 12] );
	}

	Vector3 getTrans() const
	{
		return Vector3( mm[3][0], mm[3][1], mm[3][2] );
	}

	Vector3 getScale() const
	{
		Vector3 scale;
		// Scale is length of columns
		scale.x = sqrtf( mm[0][0] * mm[0][0] + mm[0][1] * mm[0][1] + mm[0][2] * mm[0][2] );
		scale.y = sqrtf( mm[1][0] * mm[1][0] + mm[1][1] * mm[1][1] + mm[1][2] * mm[1][2] );
		scale.z = sqrtf( mm[2][0] * mm[2][0] + mm[2][1] * mm[2][1] + mm[2][2] * mm[2][2] );
		return scale;
	}

	friend std::ostream& operator<<(std::ostream &stream, const Matrix4 &m)
	{
		for (int y=0; y<4; y++)
		{
			for (int x=0; x<4; x++)
			{
				stream << m.mm[y][x] << "   ";
			}
			stream << std::endl;
		}

		return stream;
	}

	static const Matrix4 IDENTITY;

};

// -------------------------------------------------------------------------------------------------
// Plane
// -------------------------------------------------------------------------------------------------

class Plane
{
public:
	Vector3 normal;
	float dist;

	// ------------
	// Constructors
	// ------------
	Plane()
	{
		normal.x = 0; normal.y = 0; normal.z = 0; dist = 0;
	};

	explicit Plane( const float a, const float b, const float c, const float d )
	{
		normal = Vector3( a, b, c );
		float invLen = 1.0f / normal.length();
		normal *= invLen;	// Normalize
		dist = d * invLen;
	}

	Plane( const Vector3 &v0, const Vector3 &v1, const Vector3 &v2 )
	{
		normal = v1 - v0;
		normal = normal.cross( v2 - v0 );
		normal.normalize();
		dist = -normal.dot( v0 );
	}

	// ----------------
	// Other operations
	// ----------------
	float distToPoint( const Vector3 &v ) const
	{
		return normal.dot( v ) + dist;
	}
};


// -------------------------------------------------------------------------------------------------
// Intersection
// -------------------------------------------------------------------------------------------------

inline bool rayTriangleIntersection( const Vector3 &rayOrig, const Vector3 &rayDir,
                                     const Vector3 &vert0, const Vector3 &vert1, const Vector3 &vert2,
                                     Vector3 &intsPoint )
{
	// Idea: Tomas Moeller and Ben Trumbore
	// in Fast, Minimum Storage Ray/Triangle Intersection

	// Find vectors for two edges sharing vert0
	Vector3 edge1 = vert1 - vert0;
	Vector3 edge2 = vert2 - vert0;

	// Begin calculating determinant - also used to calculate U parameter
	Vector3 pvec = rayDir.cross( edge2 );

	// If determinant is near zero, ray lies in plane of triangle
	float det = edge1.dot( pvec );


	// *** Culling branch ***
	/*if( det < Math::Epsilon )return false;

	// Calculate distance from vert0 to ray origin
	Vector3 tvec = rayOrig - vert0;

	// Calculate U parameter and test bounds
	float u = tvec.dot( pvec );
	if (u < 0 || u > det ) return false;

	// Prepare to test V parameter
	Vector3 qvec = tvec.cross( edge1 );

	// Calculate V parameter and test bounds
	float v = rayDir.dot( qvec );
	if (v < 0 || u + v > det ) return false;

	// Calculate t, scale parameters, ray intersects triangle
	float t = edge2.dot( qvec ) / det;*/


	// *** Non-culling branch ***
	if( det > -Math::Epsilon && det < Math::Epsilon ) return 0;
	float inv_det = 1.0f / det;

	// Calculate distance from vert0 to ray origin
	Vector3 tvec = rayOrig - vert0;

	// Calculate U parameter and test bounds
	float u = tvec.dot( pvec ) * inv_det;
	if( u < 0.0f || u > 1.0f ) return 0;

	// Prepare to test V parameter
	Vector3 qvec = tvec.cross( edge1 );

	// Calculate V parameter and test bounds
	float v = rayDir.dot( qvec ) * inv_det;
	if( v < 0.0f || u + v > 1.0f ) return 0;

	// Calculate t, ray intersects triangle
	float t = edge2.dot( qvec ) * inv_det;


	// Calculate intersection point and test ray length and direction
	intsPoint = rayOrig + rayDir * t;
	Vector3 vec = intsPoint - rayOrig;
	if( vec.dot( rayDir ) < 0 || vec.length() > rayDir.length() ) return false;

	return true;
}


inline bool rayAABBIntersection( const Vector3 &rayOrig, const Vector3 &rayDir,
                                 const Vector3 &mins, const Vector3 &maxs )
{
	// SLAB based optimized ray/AABB intersection routine
	// Idea taken from http://ompf.org/ray/

	float l1 = (mins.x - rayOrig.x) / rayDir.x;
	float l2 = (maxs.x - rayOrig.x) / rayDir.x;
	float lmin = Math::min( l1, l2 );
	float lmax = Math::max( l1, l2 );

	l1 = (mins.y - rayOrig.y) / rayDir.y;
	l2 = (maxs.y - rayOrig.y) / rayDir.y;
	lmin = Math::max( Math::min( l1, l2 ), lmin );
	lmax = Math::min( Math::max( l1, l2 ), lmax );

	l1 = (mins.z - rayOrig.z) / rayDir.z;
	l2 = (maxs.z - rayOrig.z) / rayDir.z;
	lmin = Math::max( Math::min( l1, l2 ), lmin );
	lmax = Math::min( Math::max( l1, l2 ), lmax );

	if( (lmax >= 0.0f) & (lmax >= lmin) )
	{
		// Consider length
		const Vector3 rayDest = rayOrig + rayDir;
		Vector3 rayMins( Math::min( rayDest.x, rayOrig.x), Math::min( rayDest.y, rayOrig.y ), Math::min( rayDest.z, rayOrig.z ) );
		Vector3 rayMaxs( Math::max( rayDest.x, rayOrig.x), Math::max( rayDest.y, rayOrig.y ), Math::max( rayDest.z, rayOrig.z ) );
		return
			(rayMins.x < maxs.x) && (rayMaxs.x > mins.x) &&
			(rayMins.y < maxs.y) && (rayMaxs.y > mins.y) &&
			(rayMins.z < maxs.z) && (rayMaxs.z > mins.z);
	}
	else
		return false;
}


inline float nearestDistToAABB( const Vector3 &pos, const Vector3 &mins, const Vector3 &maxs )
{
	const Vector3 center = (mins + maxs) * 0.5f;
	const Vector3 extent = (maxs - mins) * 0.5f;

	Vector3 nearestVec;
	nearestVec.x = Math::max( 0, fabsf( pos.x - center.x ) - extent.x );
	nearestVec.y = Math::max( 0, fabsf( pos.y - center.y ) - extent.y );
	nearestVec.z = Math::max( 0, fabsf( pos.z - center.z ) - extent.z );

	return nearestVec.length();
}

/**
 * Ray class
 */
struct Ray
{
	Vector3 origin;
	Vector3 direction;
};

}
