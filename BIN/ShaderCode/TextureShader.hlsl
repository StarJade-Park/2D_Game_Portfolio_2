struct VTX_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD; // 텍스처 좌표
    float4 vCol : COLOR; // 색깔
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD; // 텍스처 좌표
    float4 vCol : COLOR; // 색깔
};

cbuffer MATDATA : register(b0)
{
    matrix WVP;
}

cbuffer CUTDATA : register(b1)
{
    float4 CutUv;
}

cbuffer COLOR : register(b2)
{
    float4 COLOR;
}

VTX_OUT VS_TEX(VTX_IN _in)
{
    VTX_OUT Out = (VTX_OUT) 0.0f;
    Out.vCol = _in.vCol;

    Out.vUv.x = (_in.vUv.x * CutUv.z) + CutUv.x;
    Out.vUv.y = (_in.vUv.y * CutUv.w) + CutUv.y;

    //Out.vUv = _in.vUv;
    Out.vPos = _in.vPos;
    Out.vPos = mul(_in.vPos, WVP);

    return Out;
}


struct PIX_OUT
{
    float4 vOutColor : SV_Target;
};

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

PIX_OUT PS_TEX(VTX_OUT _in)
{
    PIX_OUT Out = (PIX_OUT) 0.0f;
    Out.vOutColor = Tex.Sample(Smp, _in.vUv) * COLOR;

    // 도트 이미지면 이걸로 blending 충분
    if (0.0f == Out.vOutColor.a)
    {
        clip(-1);
    }

    return Out;
}