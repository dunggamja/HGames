#pragma once
struct Simplex
{
	gmtl::Vec3f						m_Point;
	std::array<gmtl::Vec3f, 2>		m_OriginPoint;

	bool operator ==(const Simplex& O) const
	{
		return (m_Point == O.m_Point) && (m_OriginPoint == O.m_OriginPoint);
	}
};

struct SimplexArray
{
	std::array<Simplex, 4>		m_Simplex;
	int							m_Count = 0;
};

class Collider : std::enable_shared_from_this<Collider>
{
	SHARED_TYPEDEF(Collider)	

public:
	//enum class TYPE
	//{
	//		NONE
	//	,	POLYGON		// 다각형
	//	,	SPHERE		// 구체
	//	,	CYLINDER	// 원기둥	
	//};

public:
	virtual ~Collider() = default;


//======================================================================================================
// 충돌 체크를 위한 함수 입니다. 
//======================================================================================================
public:
	static Simplex									GetSupportPoint(const Collider::SharedPtr A, const Collider::SharedPtr B, gmtl::Vec3f& dir);
	static std::tuple<bool, SimplexArray>			CheckGJK(const Collider::SharedPtr A, const Collider::SharedPtr B);
	static bool										ContainsOrigin(SimplexArray& simplexPoints, gmtl::Vec3f& dir);
	static bool										Triangle(SimplexArray& simplexPoints, gmtl::Vec3f& dir);
	static bool										Pyramid(SimplexArray& simplexPoints, gmtl::Vec3f& dir);

	static Contact	CheckEPA(const Collider::SharedPtr A, const Collider::SharedPtr B, const SimplexArray& simplexArray);

protected:
	//================================================================
	// 충돌체에서 해당 방향으로 가장 돌출되어 있는 점을 가져옵니다.
	//================================================================
	virtual gmtl::Vec3f	GetFurthestPoint(gmtl::Vec3f dir) const		= 0;
	virtual	void		UpdateCollider(gmtl::Matrix44f& transform)	= 0;

protected:
	static const Int32 GJK_MAX_ITER_NUM;
	static const Int32 EPA_MAX_ITER_NUM;	
	static const Int32 EPA_MAX_NUM_LOOSE_EDGES;
	static const Int32 EPA_MAX_NUM_FACES;
	static const float FLOAT_TOLERANCE;
};