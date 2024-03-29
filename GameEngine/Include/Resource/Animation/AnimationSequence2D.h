#pragma once
#include "../../Ref.h"
#include "../../Resource/Texture/Texture.h"

class CAnimationSequence2D :
    public CRef
{
    friend class CAnimationManager;
    friend class CAnimationSequence2DData;
public :
    CAnimationSequence2D();
    virtual ~CAnimationSequence2D() override;
private :
    class CScene* m_Scene;
    CSharedPtr<CTexture> m_Texture;
    std::vector<AnimationFrameData> m_vecFrameData;
    bool m_IsFrameReverse;
public :
    void SetFrameReverse(bool Reverse)
{
        m_IsFrameReverse = Reverse;
}
public :
    bool IsFrameReverse() const
{
        return m_IsFrameReverse;
}
    size_t GetFrameCount() const
    {
        return m_vecFrameData.size();
    }
    CScene* GetScene() const
{
        return m_Scene;
}
    CTexture* GetTexture() const
{
        return m_Texture;
}
    AnimationFrameData GetFrameData(int Index = 0)
{
        return m_vecFrameData[Index];
}
    void ClearFrame()
{
        m_vecFrameData.clear();
}
public:
    bool Init(CTexture* Texture);
    bool Init(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void AddFrameData(const Vector2& StartPos, const Vector2& Size);
public :
    void Save(FILE* pFile);
    void Load(FILE* pFile);
};

