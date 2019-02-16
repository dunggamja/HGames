#pragma once

class CSceneGame :
	public CSceneBase
{
	ASE_DECLARE_INSTANCE(CField);
public:
	void	InitInstance();
	virtual void Update(double tickCount) final;
	virtual void Render() final;

};