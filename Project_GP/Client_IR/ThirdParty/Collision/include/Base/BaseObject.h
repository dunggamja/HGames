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
			// ID.. 老窜 措面 贸府...
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
