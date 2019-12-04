struct Vtx_In
{
    float4 mPos : POSITION;
    float2 mUv : TEXCOORD;
    float4 mCol : COLOR;
};

struct Vtx_Out
{
    float4 mPos : SV_POSITION;
    float2 mUv : TEXCOORD;
    float4 mCol : COLOR;
};

struct Pix_Out
{
    float4 mColor : SV_Target;
};

cbuffer MATDATA : register(b0)
{
    matrix WVP;
}

static matrix IdentityMatrix =
{
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
};

Vtx_Out VS_ColorRect(Vtx_In _in)
{
    Vtx_Out Out = (Vtx_Out) 0.0f;

    Out.mPos = mul(_in.mPos, WVP);
    Out.mCol = _in.mCol;
    Out.mUv = _in.mUv;

    return Out;
}

cbuffer COLOR : register(b1)
{
    float4 COLOR;
}

Pix_Out PS_ColorRect(Vtx_Out _in)
{
    Pix_Out Out = (Pix_Out) 0.0f;
    //Out.mColor = _in.mCol;
    Out.mColor = COLOR;
    // 1. 원그리기
    //float _x = _in.mUv.x - 0.5;
    //float _y = _in.mUv.y - 0.5;
    //if (pow(_x, 2.0f) + pow(_y, 2.0f) <= 0.25f)
    //{
    //    Out.mColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}


    // 2. 사각형
    //float _x = _in.mUv.x;
    //float _y = _in.mUv.y;
    //if ((_x) <= 0.001f)
    //{
    //    Out.mColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}

    //if ((_y) <= 0.003f)
    //{
    //    Out.mColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}

    //if ((_x) >= 0.999f)
    //{
    //    Out.mColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}

    //if ((_y) >= 0.997f)
    //{
    //    Out.mColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}


    // 3. 마트료시카 사각형
    //float _x = _in.mUv.x - 0.5f;
    //float _y = _in.mUv.y - 0.5f;

    //if ((_y + _x >= 0.1f) && (_y + _x <= 0.5f))
    //{
    //    Out.mColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}

    return Out;
}