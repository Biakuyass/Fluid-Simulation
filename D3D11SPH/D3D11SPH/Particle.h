#ifndef PARTICLE_H_
#define PARTICLE_H_

#include<windows.h>
#include<xnamath.h>
#include<vector>
using namespace std;

class Particle
{
private:
	XMFLOAT3 m_vPosRender;
	XMFLOAT3 m_vPosRealScale;
	XMFLOAT3 m_vAcceleration;
	XMFLOAT3 m_vVelocity;
	float    m_fDensity;
	float    m_fPressure;
	float    m_fViscosity;

public:



	XMFLOAT3 PosRender() const { return m_vPosRender; }
	void     PosRender(XMFLOAT3 val) { m_vPosRender = val; }

	XMFLOAT3 PosRealScale() const { return m_vPosRealScale; }
	void     PosRealScale(XMFLOAT3 val) { m_vPosRealScale = val; }

	XMFLOAT3 Acceleration() const { return m_vAcceleration; }
	void     Acceleration(XMFLOAT3 val) { m_vAcceleration = val; }

	XMFLOAT3 Velocity() const { return m_vVelocity; }
	void     Velocity(XMFLOAT3 val) { m_vVelocity = val; }

	float Density() const { return m_fDensity; }
	void  Density(float val) { m_fDensity = val; }

	float Pressure() const { return m_fPressure; }
	void  Pressure(float val) { m_fPressure = val; }

	float Viscosity() const { return m_fViscosity; }
	void Viscosity(float val) { m_fViscosity = val; }

	vector<int> m_neightbortable;
	Particle();
	~Particle(){};
};






#endif