#pragma once


class Collide : std::enable_shared_from_this<Collide>
{
	SHARED_TYPEDEF(Collide)

public:
	enum class TYPE
	{
			NONE
		,	POLYGON		// 다각형
		,	SPHERE		// 구체
		,	CYLINDER	// 원기둥	
	};

public:
	virtual ~Collide() = default;


//======================================================================================================
// 충돌 체크를 위한 함수 입니다. 
//======================================================================================================
public:
	static gmtl::Vec3f									GetSupportPoint(const Collide& A, const Collide& B, gmtl::Vec3f& dir);
	static std::tuple<bool, std::array<gmtl::Vec3f, 4>>	CheckGJK(const Collide& A, const Collide& B);
	static bool											CheckSimplex(std::array<gmtl::Vec3f, 4>& simplexPoints, gmtl::Vec3f& dir, int& simplexCount);

	static Contact	CheckEPA(const Collide& A, const Collide& B, const std::array<gmtl::Vec3f, 4>& points);

protected:
	//================================================================
	// 충돌체에서 해당 방향으로 가장 돌출되어 있는 점을 가져옵니다.
	//================================================================
	virtual gmtl::Vec3f	GetSupportPoint(gmtl::Vec3f dir) const = 0;
};