#include "FastSIMD/InlInclude.h"

#include "Simplex.h"
#include "CoherentHelpers.inl"

template<typename FS>
class FS_T<FastNoise::Simplex, FS> : public virtual FastNoise::Simplex, public FS_T<FastNoise::Generator, FS>
{
    float32v FS_VECTORCALL Gen( int32v seed, float32v x, float32v y ) const
    {
        const float SQRT3 = 1.7320508075688772935274463415059f;
        const float F2 = 0.5f * (SQRT3 - 1.0f);
        const float G2 = (3.0f - SQRT3) / 6.0f;

        float32v f = float32v( F2 ) * (x + y);
        float32v x0 = FS_Floor_f32( x + f );
        float32v y0 = FS_Floor_f32( y + f );

        int32v i = FS_Convertf32_i32( x0 ) * int32v( Primes::X );
        int32v j = FS_Convertf32_i32( y0 ) * int32v( Primes::Y );

        float32v g = float32v( G2 ) * (x0 + y0);
        x0 = x - (x0 - g);
        y0 = y - (y0 - g);

        mask32v i1 = FS_GreaterThan_f32( x0, y0 );
        //mask32v j1 = ~i1; //NMasked funcs

        float32v x1 = FS_MaskedSub_f32( x0, float32v( 1.f ), i1 ) + float32v( G2 );
        float32v y1 = FS_NMaskedSub_f32( y0, float32v( 1.f ), i1 ) + float32v( G2 );
        float32v x2 = x0 + float32v( (G2 * 2) - 1 );
        float32v y2 = y0 + float32v( (G2 * 2) - 1 );

        float32v t0 = float32v( 0.5f ) - (x0 * x0) - (y0 * y0);
        float32v t1 = float32v( 0.5f ) - (x1 * x1) - (y1 * y1);
        float32v t2 = float32v( 0.5f ) - (x2 * x2) - (y2 * y2);

        t0 = FS_Max_f32( t0, float32v( 0 ) );
        t1 = FS_Max_f32( t1, float32v( 0 ) );
        t2 = FS_Max_f32( t2, float32v( 0 ) );

        t0 *= t0;
        t1 *= t1;
        t2 *= t2;

        float32v n0 = t0 * t0 * GetGradientDot( HashPrimes( seed, i, j ), x0, y0 );
        float32v n1 = t1 * t1 * GetGradientDot( HashPrimes( seed, FS_MaskedAdd_i32( i, int32v( Primes::X ), i1 ), FS_NMaskedAdd_i32( j, int32v( Primes::Y ), i1 ) ), x1, y1 );
        float32v n2 = t2 * t2 * GetGradientDot( HashPrimes( seed, i + int32v( Primes::X ), j + int32v( Primes::Y ) ), x2, y2 );

        return float32v( 75.3929901123046875f ) * (n0 + n1 + n2);
    }

    float32v FS_VECTORCALL Gen( int32v seed, float32v x, float32v y, float32v z ) const
    {
        const float F3 = 1.0f / 3.0f;
        const float G3 = 1.0f / 6.0f;

        float32v f = float32v( F3 ) * (x + y + z);
        float32v x0 = FS_Floor_f32( x + f );
        float32v y0 = FS_Floor_f32( y + f );
        float32v z0 = FS_Floor_f32( z + f );

        int32v i = FS_Convertf32_i32( x0 ) * int32v( Primes::X );
        int32v j = FS_Convertf32_i32( y0 ) * int32v( Primes::Y );
        int32v k = FS_Convertf32_i32( z0 ) * int32v( Primes::Z );

        float32v g = float32v( G3 ) * (x0 + y0 + z0);
        x0 = x - (x0 - g);
        y0 = y - (y0 - g);
        z0 = z - (z0 - g);

        mask32v x_ge_y = FS_GreaterEqualThan_f32( x0, y0 );
        mask32v y_ge_z = FS_GreaterEqualThan_f32( y0, z0 );
        mask32v x_ge_z = FS_GreaterEqualThan_f32( x0, z0 );

        mask32v i1 = x_ge_y & x_ge_z;
        mask32v j1 = ~x_ge_y & y_ge_z;
        mask32v k1 = ~y_ge_z & ~x_ge_z;

        mask32v i2 = x_ge_y | x_ge_z;
        mask32v j2 = ~(x_ge_y) | y_ge_z;
        mask32v k2 = mask32v( x_ge_z & y_ge_z ); //NMasked

        float32v x1 = FS_MaskedSub_f32( x0, float32v( 1 ), i1 ) + float32v( G3 );
        float32v y1 = FS_MaskedSub_f32( y0, float32v( 1 ), j1 ) + float32v( G3 );
        float32v z1 = FS_MaskedSub_f32( z0, float32v( 1 ), k1 ) + float32v( G3 );
        float32v x2 = FS_MaskedSub_f32( x0, float32v( 1 ), i2 ) + float32v( F3 );
        float32v y2 = FS_MaskedSub_f32( y0, float32v( 1 ), j2 ) + float32v( F3 );
        float32v z2 = FS_NMaskedSub_f32( z0, float32v( 1 ), k2 ) + float32v( F3 );
        float32v x3 = x0 + float32v( (G3 * 3) - 1 );
        float32v y3 = y0 + float32v( (G3 * 3) - 1 );
        float32v z3 = z0 + float32v( (G3 * 3) - 1 );

        float32v t0 = float32v( 0.6f ) - (x0 * x0) - (y0 * y0) - (z0 * z0);
        float32v t1 = float32v( 0.6f ) - (x1 * x1) - (y1 * y1) - (z1 * z1);
        float32v t2 = float32v( 0.6f ) - (x2 * x2) - (y2 * y2) - (z2 * z2);
        float32v t3 = float32v( 0.6f ) - (x3 * x3) - (y3 * y3) - (z3 * z3);

        t0 = FS_Max_f32( t0, float32v( 0 ) );
        t1 = FS_Max_f32( t1, float32v( 0 ) );
        t2 = FS_Max_f32( t2, float32v( 0 ) );
        t3 = FS_Max_f32( t3, float32v( 0 ) );

        t0 *= t0;
        t1 *= t1;
        t2 *= t2;
        t3 *= t3;

        float32v n0 = t0 * t0 * GetGradientDot( HashPrimes( seed, i, j, k ), x0, y0, z0 );
        float32v n1 = t1 * t1 * GetGradientDot( HashPrimes( seed, FS_MaskedAdd_i32( i, int32v( Primes::X ), i1 ), FS_MaskedAdd_i32( j, int32v( Primes::Y ), j1 ), FS_MaskedAdd_i32( k, int32v( Primes::Z ), k1 ) ), x1, y1, z1 );
        float32v n2 = t2 * t2 * GetGradientDot( HashPrimes( seed, FS_MaskedAdd_i32( i, int32v( Primes::X ), i2 ), FS_MaskedAdd_i32( j, int32v( Primes::Y ), j2 ), FS_NMaskedAdd_i32( k, int32v( Primes::Z ), k2 ) ), x2, y2, z2 );
        float32v n3 = t3 * t3 * GetGradientDot( HashPrimes( seed, i + int32v( Primes::X ), j + int32v( Primes::Y ), k + int32v( Primes::Z ) ), x3, y3, z3 );

        return float32v( 32 ) * (n0 + n1 + n2 + n3);
    }
};

template<typename FS>
class FS_T<FastNoise::OpenSimplex, FS> : public virtual FastNoise::OpenSimplex, public FS_T<FastNoise::Simplex, FS>
{
    float32v FS_VECTORCALL Gen( int32v seed, float32v x, float32v y, float32v z ) const final
    {
        float32v f = float32v( 2.0f / 3.0f ) * (x + y + z);
        float32v xr = f - x;
        float32v yr = f - y;
        float32v zr = f - z;

        float32v val( 0 );
        for( size_t i = 0; i < 2; i++ )
        {
            float32v v0xr = FS_Floor_f32( xr + float32v( 0.5f ) );
            float32v v0yr = FS_Floor_f32( yr + float32v( 0.5f ) );
            float32v v0zr = FS_Floor_f32( zr + float32v( 0.5f ) );
            float32v d0xr = xr - v0xr;
            float32v d0yr = yr - v0yr;
            float32v d0zr = zr - v0zr;

            float32v score0xr = FS_Abs_f32( d0xr );
            float32v score0yr = FS_Abs_f32( d0yr );
            float32v score0zr = FS_Abs_f32( d0zr );
            mask32v dir0xr = FS_LessEqualThan_f32( FS_Max_f32( score0yr, score0zr ), score0xr );
            mask32v dir0yr = (~dir0xr) & FS_LessEqualThan_f32( FS_Max_f32( score0zr, score0xr ), score0yr ); // Can use AndNot
            mask32v dir0zr = ~(dir0xr | dir0yr);
            float32v v1xr = FS_MaskedAdd_f32( v0xr, FS_BitwiseOr_f32( float32v( 1 ), FS_BitwiseAnd_f32( d0xr, float32v( -1 ) ) ), dir0xr );
            float32v v1yr = FS_MaskedAdd_f32( v0yr, FS_BitwiseOr_f32( float32v( 1 ), FS_BitwiseAnd_f32( d0yr, float32v( -1 ) ) ), dir0yr );
            float32v v1zr = FS_MaskedAdd_f32( v0zr, FS_BitwiseOr_f32( float32v( 1 ), FS_BitwiseAnd_f32( d0zr, float32v( -1 ) ) ), dir0zr );
            float32v d1xr = xr - v1xr;
            float32v d1yr = yr - v1yr;
            float32v d1zr = zr - v1zr;

            int32v hv0xr = FS_Convertf32_i32( v0xr ) * int32v( Primes::X );
            int32v hv0yr = FS_Convertf32_i32( v0yr ) * int32v( Primes::Y );
            int32v hv0zr = FS_Convertf32_i32( v0zr ) * int32v( Primes::Z );
            int32v hv1xr = FS_Convertf32_i32( v1xr ) * int32v( Primes::X );
            int32v hv1yr = FS_Convertf32_i32( v1yr ) * int32v( Primes::Y );
            int32v hv1zr = FS_Convertf32_i32( v1zr ) * int32v( Primes::Z );

            float32v t0 = FS_FMulAdd_f32( -d0zr, d0zr, FS_FMulAdd_f32( -d0yr, d0yr, FS_FMulAdd_f32( -d0xr, d0xr, float32v( 0.6f ) ) ) ); // Use FNMulAdd
            float32v t1 = FS_FMulAdd_f32( -d1zr, d1zr, FS_FMulAdd_f32( -d1yr, d1yr, FS_FMulAdd_f32( -d1xr, d1xr, float32v( 0.6f ) ) ) );
            t0 = FS_Max_f32( t0, float32v( 0 ) );
            t1 = FS_Max_f32( t1, float32v( 0 ) );
            t0 = t0 * t0;
            t1 = t1 * t1;

            float32v v0 = t0 * t0 * GetGradientDot( HashPrimes( seed, hv0xr, hv0yr, hv0zr ), d0xr, d0yr, d0zr );
            float32v v1 = t1 * t1 * GetGradientDot( HashPrimes( seed, hv1xr, hv1yr, hv1zr ), d1xr, d1yr, d1zr );

            val += v0 + v1;

            if( i == 0 )
            {
                xr += float32v( 0.5f );
                yr += float32v( 0.5f );
                zr += float32v( 0.5f );
                seed -= int32v( -1 );
            }
        }
        return float32v( 32 ) * val;
    } 
};

