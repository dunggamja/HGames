#pragma once


class Collide : std::enable_shared_from_this<Collide>
{
	SHARED_TYPEDEF(Collide)

public:
	enum class TYPE
	{
			NONE
		,	POLYGON		// �ٰ���
		,	SPHERE		// ��ü
		,	CYLINDER	// �����	
	};

public:
	virtual ~Collide() = default;


//======================================================================================================
// �浹 üũ�� ���� �Լ� �Դϴ�. 
//======================================================================================================
public:
	static gmtl::Vec3f									GetSupportPoint(const Collide& A, const Collide& B, gmtl::Vec3f& dir);
	static std::tuple<bool, std::array<gmtl::Vec3f, 4>>	CheckGJK(const Collide& A, const Collide& B);
	static bool											CheckSimplex(std::array<gmtl::Vec3f, 4>& simplexPoints, gmtl::Vec3f& dir, int& simplexCount);

	static Contact	CheckEPA(const Collide& A, const Collide& B, const std::array<gmtl::Vec3f, 4>& points);

protected:
	//================================================================
	// �浹ü���� �ش� �������� ���� ����Ǿ� �ִ� ���� �����ɴϴ�.
	//================================================================
	virtual gmtl::Vec3f	GetSupportPoint(gmtl::Vec3f dir) const = 0;
};