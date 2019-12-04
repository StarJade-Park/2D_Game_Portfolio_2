struct VTX_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD; // ÅØ½ºÃ³ ÁÂÇ¥
    float4 vCol : COLOR; // »ö±ò

    row_major matrix vMatWVP : WVP;
    float4 vSpriteUV : SPRITE_UV;
};

struct VTX_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD; // ÅØ½ºÃ³ ÁÂÇ¥
    float4 vCol : COLOR; // »ö±ò
};

VTX_OUT VS_TEX(VTX_IN _in)
{
    VTX_OUT Out = (VTX_OUT) 0.0f;
    Out.vCol = _in.vCol;

    Out.vUv.x = (_in.vUv.x * _in.vSpriteUV.z) + _in.vSpriteUV.x;
    Out.vUv.y = (_in.vUv.y * _in.vSpriteUV.w) + _in.vSpriteUV.y;

    //Out.vUv = _in.vUv;
    Out.vPos = _in.vPos;
    Out.vPos = mul(_in.vPos, _in.vMatWVP);

    return Out;
}