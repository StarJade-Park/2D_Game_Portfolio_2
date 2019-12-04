struct VTX_IN
{
    float4 vPos : POSITION;
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
};

cbuffer MATDATA : register(b0)
{
    matrix WVP;
}

VTX_OUT VS_DEBUGMESH(VTX_IN _In)
{
    VTX_OUT Out = (VTX_OUT) 0.0f;
    Out.vPos = mul(_In.vPos, WVP);

    return Out;
}

struct PIX_OUT
{
    float4 vOutColor : SV_Target;
};

cbuffer COLOR : register(b1)
{
    float4 COLOR;
}

PIX_OUT PS_DEBUGMESH(VTX_OUT _in)
{
    PIX_OUT Out = (PIX_OUT) 0.0f;
    Out.vOutColor = COLOR;

    return Out;
}