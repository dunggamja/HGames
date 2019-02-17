#pragma once

class BaseObject : public std::enable_shared_from_this<BaseObject>
{
	SHARED_TYPEDEF(BaseObject)
public:
	BaseObject()			= default;
	virtual ~BaseObject()	= default;

protected:
	UInt64	m_InstanceID = 0;
};