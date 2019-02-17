#pragma once

class Cell
{
	UInt32	m_X = 0;
	UInt32  m_Y = 0;
	std::unordered_map<UInt64, Rigidbody::SharedPtr>	m_ContainerRigidBodies;
};

class Grid
{
	std::unordered_map<UInt64, Rigidbody::SharedPtr>	m_ContainerRigidBodies;

};