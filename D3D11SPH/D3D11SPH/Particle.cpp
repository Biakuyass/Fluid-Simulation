#include "Particle.h"

Particle::Particle()
{
	m_vPosRender = XMFLOAT3(0,0,0);
	m_vPosRealScale = XMFLOAT3(0,0,0);
	m_fViscosity = 0;
	m_fPressure = 0;
	m_fDensity = 0;
	m_vAcceleration = XMFLOAT3(0,0,0);
	m_vVelocity = XMFLOAT3(0,0,0);
}
     

