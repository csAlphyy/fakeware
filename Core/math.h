#pragma once

#include "../Core/SDK.h"

#include <DirectXMath.h>

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)
#define PI 3.14159265358979323846

enum
{
	PITCH,
	YAW,
	ROLL
};

namespace Math
{
	//--------------------------------------------------------------------------------
	inline bool InBox(int rx1, int ry1, int rx2, int ry2, int px, int py)
	{
		return px > rx1 && px < rx2 && py > ry1 && py < ry2;
	}
	//--------------------------------------------------------------------------------
	inline bool InBox(Vector2D r1, Vector2D r2, int px, int py)
	{
		return InBox(r1.x, r1.y, r2.x, r2.y, px, py);
	}
	//--------------------------------------------------------------------------------
	inline void SinCos(float radians, float* sine, float* cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);
	}
	//--------------------------------------------------------------------------------
	inline void NormalizeAngles(QAngle& angles)
	{
		for (auto i = 0; i < 3; i++)
		{
			while (angles[i] < -180.0f) angles[i] += 360.0f;
			while (angles[i] > 180.0f) angles[i] -= 360.0f;
		}
	}
	//--------------------------------------------------------------------------------
	inline void ClampAngles(QAngle& angles)
	{
		if (angles.pitch > 89.0f) angles.pitch = 89.0f;
		else if (angles.pitch < -89.0f) angles.pitch = -89.0f;

		if (angles.yaw > 180.0f) angles.yaw = 180.0f;
		else if (angles.yaw < -180.0f) angles.yaw = -180.0f;

		angles.roll = 0;
	}
	//--------------------------------------------------------------------------------
	inline void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
	{
		out[0] = in1.Dot(in2[0]) + in2[0][3];
		out[1] = in1.Dot(in2[1]) + in2[1][3];
		out[2] = in1.Dot(in2[2]) + in2[2][3];
	}
	//--------------------------------------------------------------------------------
	inline void AngleVectors(const QAngle &angles, Vector& forward)
	{
		float	sp, sy, cp, cy;

		DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
		DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));

		forward.x = cp*cy;
		forward.y = cp*sy;
		forward.z = -sp;
	}
	//--------------------------------------------------------------------------------
	inline void AngleVectors(const QAngle &angles, Vector& forward, Vector& right, Vector& up)
	{
		float sr, sp, sy, cr, cp, cy;

		DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
		DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));
		DirectX::XMScalarSinCos(&sr, &cr, DEG2RAD(angles[2]));

		forward.x = (cp * cy);
		forward.y = (cp * sy);
		forward.z = (-sp);
		right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right.y = (-1 * sr * sp * sy + -1 * cr *  cy);
		right.z = (-1 * sr * cp);
		up.x = (cr * sp * cy + -sr*-sy);
		up.y = (cr * sp * sy + -sr*cy);
		up.z = (cr * cp);
	}
	//--------------------------------------------------------------------------------
	inline void VectorAngles(const Vector& forward, QAngle& angles)
	{
		if (forward[1] == 0.0f && forward[0] == 0.0f)
		{
			angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
			angles[1] = 0.0f;
		} else
		{
			angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / PI;
			angles[1] = atan2(forward[1], forward[0]) * 180 / PI;

			if (angles[1] > 90) angles[1] -= 180;
			else if (angles[1] < 90) angles[1] += 180;
			else if (angles[1] == 90) angles[1] = 0;
		}

		angles[2] = 0.0f;
	}
	//--------------------------------------------------------------------------------
	inline static bool ScreenTransform(const Vector& in, Vector2D& out)
	{
		static auto& w2sMatrix = g_EngineClient->WorldToScreenMatrix();

		out.x = w2sMatrix.m[0][0] * in.x + w2sMatrix.m[0][1] * in.y + w2sMatrix.m[0][2] * in.z + w2sMatrix.m[0][3];
		out.y = w2sMatrix.m[1][0] * in.x + w2sMatrix.m[1][1] * in.y + w2sMatrix.m[1][2] * in.z + w2sMatrix.m[1][3];

		float w = w2sMatrix.m[3][0] * in.x + w2sMatrix.m[3][1] * in.y + w2sMatrix.m[3][2] * in.z + w2sMatrix.m[3][3];

		if (w < 0.001f)
		{
			out.x *= 100000;
			out.y *= 100000;
			return false;
		}

		out.x /= w;
		out.y /= w;

		return true;
	}
	//--------------------------------------------------------------------------------
	inline bool WorldToScreen(const Vector& in, Vector2D& out)
	{
		if (ScreenTransform(in, out))
		{
			int w, h;
			g_EngineClient->GetScreenSize(w, h);

			out.x = (w / 2.0f) + (out.x * w) / 2.0f;
			out.y = (h / 2.0f) - (out.y * h) / 2.0f;

			return true;
		}
		return false;
	}
	//--------------------------------------------------------------------------------
	inline QAngle CalcAngle(Vector Src, Vector Dst)
	{
		QAngle Angles;
		Vector Delta = Src - Dst;
		VectorAngles(Delta, Angles);
		Delta.Normalize();
		return Angles;
	}
	//--------------------------------------------------------------------------------
	inline void AngleVectors(const QAngle& angles, Vector* forward)
	{
		float sp, sy, cp, cy;

		SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
		SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}
	//--------------------------------------------------------------------------------
	inline float GetFOV(const QAngle& viewAngle, const QAngle& aimAngle)
	{
		Vector ang, aim;

		AngleVectors(viewAngle, &aim);
		AngleVectors(aimAngle, &ang);

		return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
	}
	//--------------------------------------------------------------------------------
	inline float RandomFloat(float a, float b)
	{
		float random = ((float) rand()) / (float) RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}
	//--------------------------------------------------------------------------------
	inline float DistancePointToLine(Vector Point, Vector LineOrigin, Vector Dir)
	{
		auto PointDir = Point - LineOrigin;

		auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
		if (TempOffset < 0.000001f)
			return FLT_MAX;

		auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

		return (Point - PerpendicularPoint).Length();
	}
	//--------------------------------------------------------------------------------
	inline void RandomSeed(int seed)
	{
		static auto fn = (decltype(&RandomSeed)) (GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed"));

		return fn(seed);
	}
}

#define M_RADPI 57.295779513082f
static const constexpr auto PIRAD = 0.01745329251f;
inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandleW(L"vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}

inline void RandomSeed(int seed)
{
	static auto fn = (decltype(&RandomSeed))(GetProcAddress(GetModuleHandleW(L"vstdlib.dll"), "RandomSeed"));

	return fn(seed);
}
inline vec_t vector_lenght(const Vector& v)
{
	return (vec_t)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
inline vec_t vector_normalize(Vector& v)
{
	vec_t l = v.Length();

	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.y = 0.0f; v.z = 1.0f;
	}

	return l;
}
#define XM_2PI              6.283185307f

FORCEINLINE vec_t DotProduct(const Vector& a, const Vector& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

namespace math
{
	Vector angle_vector(QAngle meme);
	float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir);
	void normalize(Vector &vIn, Vector &vOut);
	void normalize_angles(QAngle& angles);
	void clamp_angles(QAngle& angles);
	bool sanitize_angles(QAngle &angles);
	void correct_movement(QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove);
	void angle_vectors2(const Vector &angles, Vector *forward, Vector *right, Vector *up);
	float get_distance(const Vector &start, const Vector &end);
	float get_fov(const QAngle &viewAngles, const QAngle &aimAngles);
	void compute_angle(const Vector &source, const Vector &destination, QAngle& angles);
	QAngle compute_angle(const Vector &source, const Vector &destination);
	void vector_transform(const Vector& in1, const matrix3x4_t& in2, Vector& out);
	void angle_vectors(const QAngle &angles, Vector& forward);
	void angle_vectors(const QAngle &angles, Vector& forward, Vector& right, Vector& up);
	void vector_angles(const Vector& forward, QAngle& angles);
	bool world_to_screen(const Vector& in, Vector& out);
}