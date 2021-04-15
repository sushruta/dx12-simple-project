cbuffer ubo : register(b0)
{
    row_major float4x4 ubo_projectionMatrix : packoffset(c0);
    row_major float4x4 ubo_modelMatrix : packoffset(c4);
    row_major float4x4 ubo_viewMatrix : packoffset(c8);
    row_major float3x3 ubo_normalMatrix : packoffset(c12);
};

static float4 gl_Position;
static float3 outColor;
static float3 inNormal;
static float3 inPos;

struct SPIRV_Cross_Input
{
    float3 inPos : POSITION;
    float3 inNormal : NORMAL;
};

struct SPIRV_Cross_Output
{
    float3 outColor : COLOR;
    float4 gl_Position : SV_Position;
};

void vert_main()
{
    gl_Position = mul(float4(inPos, 1.0f), mul(ubo_modelMatrix, mul(ubo_viewMatrix, ubo_projectionMatrix)));

    float3 ambient = float3(0.2, 0.2, 0.2);
    float3 diffuse = float3(1, 0.25, 0.25);
    float3 specular = float3(1, 1, 1);
    float shininess = 65;

    float3 light_intensity = (0.5, 0.5, 1);

    float3 lightPos = float3(6, 6, -10);

    float4x4 modelviewMatrix = mul(ubo_modelMatrix, ubo_viewMatrix);

    float3 camera_space_pos = mul(float4(inPos, 1.0), modelviewMatrix).xyz;
    float3 N = normalize(mul(inNormal, ubo_normalMatrix));
    float3 L = normalize(lightPos - camera_space_pos);
    float3 E = -normalize(camera_space_pos);
    float3 H = normalize(E + L);

    float3 acon = ambient;
    float3 dcon = diffuse * light_intensity * max(dot(N, L), 0.0);
    float scon =
        specular * light_intensity * pow(max(dot(N, H), 0.0), shininess);
    
    float4 total = float4(acon + dcon + scon, 1.0);

    outColor = total;
    //outColor = float3(0.7, 0.5, 0.2);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    // inColor = float3(0.0);  // stage_input.inColor;
    inPos = stage_input.inPos;
    inNormal = stage_input.inNormal;
    vert_main();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.outColor = outColor;
    return stage_output;
}
