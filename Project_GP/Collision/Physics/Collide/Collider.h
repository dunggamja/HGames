#pragma once


class Collider : std::enable_shared_from_this<Collider>
{
	SHARED_TYPEDEF(Collider)	

public:
	enum class TYPE
	{
			NONE
		,	POLYGON		// 다각형
		,	SPHERE		// 구체
		,	CYLINDER	// 원기둥	
	};

public:
	virtual ~Collider() = default;


//======================================================================================================
// 충돌 체크를 위한 함수 입니다. 
//======================================================================================================
public:
	static gmtl::Vec3f									GetSupportPoint(const Collider::SharedPtr A, const Collider::SharedPtr B, gmtl::Vec3f& dir);
	static std::tuple<bool, std::array<gmtl::Vec3f, 4>>	CheckGJK(const Collider::SharedPtr A, const Collider::SharedPtr B);
	static bool											ContainsOrigin(std::array<gmtl::Vec3f, 4>& simplexPoints, gmtl::Vec3f& dir, int& simplexCount);
	static bool											Triangle(std::array<gmtl::Vec3f, 4>& simplexPoints, gmtl::Vec3f& dir, int& simplexCount);
	static bool											Pyramid(std::array<gmtl::Vec3f, 4>& simplexPoints, gmtl::Vec3f& dir, int& simplexCount);

	static Contact	CheckEPA(const Collider::SharedPtr A, const Collider::SharedPtr B, const std::array<gmtl::Vec3f, 4>& points);

protected:
	//================================================================
	// 충돌체에서 해당 방향으로 가장 돌출되어 있는 점을 가져옵니다.
	//================================================================
	virtual gmtl::Vec3f	GetSupportPoint(gmtl::Vec3f dir) const = 0;

protected:
	static const Int32 GJK_MAX_ITER_NUM;
	static const Int32 EPA_MAX_ITER_NUM;
	static const float EPA_TOLERANCE;
	static const Int32 EPA_MAX_NUM_LOOSE_EDGES;
	static const Int32 EPA_MAX_NUM_FACES;
};