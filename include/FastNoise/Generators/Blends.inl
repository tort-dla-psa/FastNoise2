#include "FastSIMD/InlInclude.h"

#include "Blends.h"


template<typename FS>
class FS_T<FastNoise::Add, FS> : public virtual FastNoise::Add, public FS_T<FastNoise::Generator, FS>
{
public:
    FASTNOISE_IMPL_GEN_T;
    
    template<typename... P> 
    FS_INLINE float32v GenT( int32v seed, P... pos ) const
    {
        return this->GetSourceValue( mLHS, seed, pos... ) + this->GetSourceValue( mRHS, seed, pos... );
    }
};

template<typename FS>
class FS_T<FastNoise::Subtract, FS> : public virtual FastNoise::Subtract, public FS_T<FastNoise::Generator, FS>
{
public:
    FASTNOISE_IMPL_GEN_T;
    
    template<typename... P> 
    FS_INLINE float32v GenT( int32v seed, P... pos ) const
    {
        return this->GetSourceValue( mLHS, seed, pos... ) - this->GetSourceValue( mRHS, seed, pos... );
    }
};

template<typename FS>
class FS_T<FastNoise::Multiply, FS> : public virtual FastNoise::Multiply, public FS_T<FastNoise::Generator, FS>
{
public:
    FASTNOISE_IMPL_GEN_T;
    
    template<typename... P> 
    FS_INLINE float32v GenT( int32v seed, P... pos ) const
    {
        return this->GetSourceValue( mLHS, seed, pos... ) * this->GetSourceValue( mRHS, seed, pos... );
    }
};

template<typename FS>
class FS_T<FastNoise::Divide, FS> : public virtual FastNoise::Divide, public FS_T<FastNoise::Generator, FS>
{
public:
    FASTNOISE_IMPL_GEN_T;
    
    template<typename... P> 
    FS_INLINE float32v GenT( int32v seed, P... pos ) const
    {
        return this->GetSourceValue( mLHS, seed, pos... ) / this->GetSourceValue( mRHS, seed, pos... );
    }
};

template<typename FS>
class FS_T<FastNoise::Min, FS> : public virtual FastNoise::Min, public FS_T<FastNoise::Generator, FS>
{
public:
    FASTNOISE_IMPL_GEN_T;
    
    template<typename... P> 
    FS_INLINE float32v GenT( int32v seed, P... pos ) const
    {
        return FS_Min_f32( this->GetSourceValue( mLHS, seed, pos... ), this->GetSourceValue( mRHS, seed, pos... ) );
    }
};

template<typename FS>
class FS_T<FastNoise::Max, FS> : public virtual FastNoise::Max, public FS_T<FastNoise::Generator, FS>
{
public:
    FASTNOISE_IMPL_GEN_T;
    
    template<typename... P> 
    FS_INLINE float32v GenT( int32v seed, P... pos ) const
    {
        return FS_Max_f32( this->GetSourceValue( mLHS, seed, pos... ), this->GetSourceValue( mRHS, seed, pos... ) );
    }
};

template<typename FS>
class FS_T<FastNoise::Fade, FS> : public virtual FastNoise::Fade, public FS_T<FastNoise::Generator, FS>
{
public:
    FASTNOISE_IMPL_GEN_T;
    
    template<typename... P> 
    FS_INLINE float32v GenT( int32v seed, P... pos ) const
    {
        float32v fade = FS_Abs_f32( this->GetSourceValue( mFade, seed, pos... ) );

        return FS_FMulAdd_f32( this->GetSourceValue( mA, seed, pos... ), float32v( 1 ) - fade, this->GetSourceValue( mB, seed, pos... ) * fade );
    }
};

