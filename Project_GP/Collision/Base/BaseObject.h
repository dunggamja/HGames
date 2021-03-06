#pragma once

namespace Physics
{
	class BaseObject : public std::enable_shared_from_this<BaseObject>
	{
		SHARED_TYPEDEF(BaseObject)
	public:
		BaseObject()
		{
			static UInt64 instanceID = 0;

			//==========================
			// ID.. 일단 대충 처리...
			//==========================
			m_InstanceID = ++instanceID;
		}
		virtual ~BaseObject() = default;


	//=====================================
	// InstaceID 
	//=====================================	
	protected:
		UInt64	m_InstanceID = 0;

	public:
		UInt64	GetInstanceID() const { return m_InstanceID; }
	};
}
