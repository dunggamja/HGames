//	공용으로 변경하는게 더 효율적이다.

#pragma once

//	Reference type
#define ASE_DECLARE_INSTANCE(CLASS)	\
private:CLASS m_##CLASS;			\
public:auto get##CLASS() -> decltype(m_##CLASS)* { return &m_##CLASS; }; \
public:void set(CLASS& s) {}

//	Pointer type
#define ASE_DECLARE_INSTANCE_PTR(CLASS)					\
private:CLASS* m_##CLASS = nullptr;						\
public:auto get##CLASS() { return m_##CLASS; }			\
public:void set(CLASS* s) { m_##CLASS = s; }

#define ASE_INSTANCE(THIS,CLASS)	THIS->get##CLASS()
#define ASE_THIS(CLASS)				this->get##CLASS()
#define ASE_INSTANCE_SET(THIS,s)	THIS->set(s);
#define ASE_THIS_SET(s)				this->set(s);

//#define ASE_INSTANCE2(THIS,CLASS1,CLASS2)	ASE_INSTANCE(ASE_INSTANCE(THIS,CLASS1),CLASS2)