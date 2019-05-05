#pragma once

namespace Physics
{
	class ContactGenerator : public  std::enable_shared_from_this<ContactGenerator>
	{
		SHARED_TYPEDEF(ContactGenerator)
	public:
		using FUNC_CONTACT_GENERATOR = std::function<Contact(const Collider::SharedPtr, const Collider::SharedPtr)>;
	};
};