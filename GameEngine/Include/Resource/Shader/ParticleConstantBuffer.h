#pragma once
#include "ConstantBufferBase.h"

class CParticleConstantBuffer :
    public CConstantBufferBase
{
public:
    CParticleConstantBuffer();
    CParticleConstantBuffer(const CParticleConstantBuffer& Buffer);
    virtual ~CParticleConstantBuffer() override;
private:
	ParticleCBuffer m_BufferData;
public:
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CParticleConstantBuffer* Clone();
public:
    Vector3 GetStartMin() const
    {
        return m_BufferData.StartMin;
    }
    Vector3 GetStartMax() const
    {
        return m_BufferData.StartMax;
    }
    int GetSpawnCountMax() const
    {
        return m_BufferData.SpawnCountMax;
    }
public:
    void SetSpawnCountMax(int  SpawnCountMax)
    {
        m_BufferData.SpawnCountMax = SpawnCountMax;
    }
    void SetSpawnEnable(int SpawnEnable)
    {
        m_BufferData.SpawnEnable = SpawnEnable;
    }
    void SetStartMin(const Vector3& StartMin)
    {
        m_BufferData.StartMin = StartMin;
    }
    void SetStartMax(const Vector3& StartMax)
    {
        m_BufferData.StartMax = StartMax;
    }
    void SetScaleMin(const Vector3& ScaleMin)
    {
        m_BufferData.ScaleMin = ScaleMin;
    }
    void SetScaleMax(const Vector3& ScaleMax)
    {
        m_BufferData.ScaleMax = ScaleMax;
    }
    void SetColorMin(const Vector4& ColorMin)
    {
        m_BufferData.ColorMin = ColorMin;
    }
    void SetColorMax(const Vector4& ColorMax)
    {
        m_BufferData.ColorMax = ColorMax;
    }
    void SetLifeTimeMin(int  LifeTimeMin)
    {
        m_BufferData.LifeTimeMin = LifeTimeMin;
    }
    void SetLifeTimeMax(int  LifeTimeMax)
    {
        m_BufferData.LifeTimeMax = LifeTimeMax;
    }
    void SetSpeedMin(int  SpeedMin)
    {
        m_BufferData.SpeedMin = SpeedMin;
    }
    void SetSpeedMax(int  SpeedMax)
    {
        m_BufferData.SpeedMax = SpeedMax;
    }
    void SetMove(int Move)
    {
        m_BufferData.Move = Move;
    }
    void SetGravity (int Gravity)
    {
        m_BufferData.Gravity = Gravity;
    }
    void SetIs2D(bool Is2D)
    {
        m_BufferData.Is2D = Is2D;
    }
    void SetMoveAngle(const Vector3& Angle)
    {
        m_BufferData.MoveAngle = Angle;
    }
    void SetMoveDir(const Vector3& Dir)
    {
        m_BufferData.MoveDir = Dir;
    }
};

