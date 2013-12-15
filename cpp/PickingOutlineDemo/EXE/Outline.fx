///////////////////////////////////////////////////////////////////////////////
// Generate outline vertex of the silhouette edge
// how it work:
// the each input vertex has two faces normal, if they are the opposite direction
// from camera position, then the vertex is considered to be on edge vertex
// before going to this shader the degeneration triangle has been prepared
// two vertices in the same place
// one vertex has 0 normal, the other has a normal along with face normal direction
// if it is edge vertex, then move slightly a little bit along with the normal
// then a real triangle generated
///////////////////////////////////////////////////////////////////////////////


static float4 Color = {1.0f, 0.0f, 0.0f, 0.0f};

uniform extern float4x4 WorldViewMatrix;
uniform extern float4x4 ProjMatrix;


struct InputVS
{
    float4 position    : POSITION;
    float4 normal      : NORMAL0;
    float4 faceNormal1 : NORMAL1; 
    float4 faceNormal2 : NORMAL2;
};

struct OutputVS
{
    float4 position : POSITION;
    float4 diffuse  : COLOR;
};


OutputVS OutLineVS(InputVS input)
{
    OutputVS OUT;

    //transform position to view space
    input.position = mul(input.position, WorldViewMatrix);

    input.normal.w      = 0.0f;
    input.faceNormal1.w = 0.0f;
    input.faceNormal2.w = 0.0f;

    //transform normals to view space
    input.normal      = mul(input.normal,      WorldViewMatrix);
    input.faceNormal1 = mul(input.faceNormal1, WorldViewMatrix);
    input.faceNormal2 = mul(input.faceNormal2, WorldViewMatrix);

    float dot0 = dot(input.position, input.faceNormal1);
    float dot1 = dot(input.position, input.faceNormal2);

    //found the opposite direction! then it's on silhouette edge!
    if ((dot0 * dot1) < 0.0f)
    {
         input.position += 0.05f * input.normal;
    }

    //transform it back
    OUT.position = mul(input.position, ProjMatrix);

    //set the color
    OUT.diffuse = Color;
    
    return OUT;
}

//no PS needed

technique OutLineTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 OutLineVS();
    }
}
