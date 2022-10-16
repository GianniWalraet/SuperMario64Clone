#pragma once

namespace MathHelper
{
	inline XMFLOAT3 QuaternionToEuler(XMFLOAT4 q)
	{
		XMFLOAT3 euler{};

		euler.y = atan2f(2.f * q.x * q.w + 2.f * q.y * q.z, 1.f - 2.f * (q.z * q.z + q.w * q.w));     // Yaw 
		euler.x = asinf(2.f * (q.x * q.z - q.w * q.y));                             // Pitch 
		euler.z = atan2f(2.f * q.x * q.y + 2.f * q.z * q.w, 1.f - 2.f * (q.y * q.y + q.z * q.z));      // Roll 

		return euler;
	}
	inline XMFLOAT4 EulerToQuaternion(XMFLOAT3 e)
	{
		XMFLOAT4 q{};
		q.x = sinf(e.z / 2) * cosf(e.y / 2) * cosf(e.x / 2) - cosf(e.z / 2) * sinf(e.y / 2) * sinf(e.x / 2);
		q.y = cosf(e.z / 2) * sinf(e.y / 2) * cosf(e.x / 2) + sinf(e.z / 2) * cosf(e.y / 2) * sinf(e.x / 2);
		q.z = cosf(e.z / 2) * cosf(e.y / 2) * sinf(e.x / 2) - sinf(e.z / 2) * sinf(e.y / 2) * cosf(e.x / 2);
		q.w = cosf(e.z / 2) * cosf(e.y / 2) * cosf(e.x / 2) + sinf(e.z / 2) * sinf(e.y / 2) * sinf(e.x / 2);

		return q;
	}

	inline float DistanceSquared(const XMFLOAT3& p1, const XMFLOAT3& p2)
	{
		return powf(p2.x - p1.x, 2.f) + powf(p2.y - p1.y, 2.f) + powf(p2.z - p1.z, 2.f);
	}
	inline float Distance(const XMFLOAT3& p1, const XMFLOAT3& p2)
	{
		return sqrtf(DistanceSquared(p1, p2));
	}
	inline float Magnitude(const XMFLOAT3& p)
	{
		return sqrtf(powf(p.x, 2.f) + powf(p.y, 2.f) + powf(p.z, 2.f));
	}

	inline float Dot(const XMFLOAT3& p1, const XMFLOAT3& p2)
	{
		return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
	}
	inline XMFLOAT3 Cross(const XMFLOAT3& p1, const XMFLOAT3& p2)
	{
		XMFLOAT3 p3{};
		p3.x = p1.y * p2.z - p1.z * p2.y;
		p3.y = p1.z * p2.x - p1.x * p2.z;
		p3.z = p1.x * p2.y - p1.y * p2.x;
		return p3;
	}

	inline XMFLOAT3 Normalized(const XMFLOAT3& p)
	{
		if (Magnitude(p) <= 1.f) return p;

		const float magnitude = Magnitude(p);
		return XMFLOAT3(p.x / magnitude, p.y / magnitude, p.z / magnitude);
	}
	inline XMFLOAT4 FromToRotation(const XMFLOAT3& from, const XMFLOAT3& to)
	{
		const XMFLOAT3 newAxis = Normalized(Cross(from, to));
		float angle = acos(Dot(from, to)) / (Magnitude(from) * Magnitude(to));

		XMFLOAT4 quat{};
		quat.x = newAxis.x * sin(angle / 2.f);
		quat.y = newAxis.y * sin(angle / 2.f);
		quat.z = newAxis.z * sin(angle / 2.f);
		quat.w = cos(angle / 2.f);

		return quat;
	}
	inline XMFLOAT3 ToFloat3(const PxVec3& p)
	{
		return XMFLOAT3(p.x, p.y, p.z);
	}

	inline float Angle(const XMFLOAT3& vec1, const XMFLOAT3& vec2)
	{
		return acosf(Dot(vec1, vec2) / Magnitude(vec1) * Magnitude(vec2));
	}

	inline XMFLOAT3 RotMatToEuler(const XMMATRIX& r)
	{
		//https://learnopencv.com/rotation-matrix-to-euler-angles/

		const float sy = sqrt(r.r[0].m128_f32[0] * r.r[0].m128_f32[0] + r.r[1].m128_f32[0] * r.r[1].m128_f32[0]);

		const bool singular = sy < 1e-6;

		float x, y, z;

		if (!singular)
		{
			x = atan2(r.r[2].m128_f32[1], r.r[2].m128_f32[2]);
			y = atan2(-r.r[2].m128_f32[0], sy);
			z = atan2(r.r[1].m128_f32[0], r.r[0].m128_f32[0]);
		}
		else
		{
			x = atan2(-r.r[1].m128_f32[2], r.r[2].m128_f32[2]);
			y = atan2(-r.r[2].m128_f32[0], sy);
			z = atan2(r.r[1].m128_f32[0], r.r[0].m128_f32[0]);
		}

		return { x,y,z };
	}

	inline bool XMFloat4Equals(const XMFLOAT4& a, const XMFLOAT4& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
	}
	inline bool XMFloat3Equals(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
	inline bool XMFloat2Equals(const XMFLOAT2& a, const XMFLOAT2& b)
	{
		return a.x == b.x && a.y == b.y;
	}

	inline float randF(float min, float max)
	{
		const float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		const float diff = max - min;
		const float r = random * diff;
		return min + r;
	}

	inline LONG BinaryClamp(LONG x)
	{
		if (x > 0) return 1;
		if (x < 0) return -1;
		return 0;
	}
	template<typename T>
	void Clamp(T& value, T hi, T lo)
	{
		if (value > hi)
			value = hi;

		if (value < lo)
			value = lo;
	}

	inline XMFLOAT3 SubtractFloat3(const XMFLOAT3& p1, const XMFLOAT3& p2)
	{
		XMFLOAT3 p{};
		p.x = p1.x - p2.x;
		p.y = p1.y - p2.y;
		p.z = p1.z - p2.z;

		return p;
	}

	inline float RandomFloat(float min, float max)
	{
		auto range = max - min;
		return (range * (float(rand()) / RAND_MAX)) + min;
	}

	inline int RandomInt(int min, int max)
	{
		return rand() % (max + 1 - min) + min;
	}
}