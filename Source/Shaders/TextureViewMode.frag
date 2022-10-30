#version 460 core
out vec4 FragColor;
in vec3 Normal;
in vec2 TextureCoord;
in vec3 WorldPosition;

uniform vec3 CameraPosition;

uniform vec3 lightPosition[3];
uniform vec3 lightColor[3];
uniform float lightIntensity[3];

uniform sampler2D texture_albedo;
uniform sampler2D texture_mr;
uniform sampler2D texture_normal;
uniform sampler2D texture_ao;

uniform int viewMode;
uniform bool texturesBoundToModel;
uniform sampler2D texture_metallic;
uniform sampler2D texture_roughness;

uniform int metallicChannel;
uniform int roughnessChannel;
uniform int aoChannel;
uniform vec2 texMultiplier;

vec3 getNormalFromMap(vec2 texCoord)
{
    vec3 tangentNormal = texture(texture_normal, texCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPosition);
    vec3 Q2  = dFdy(WorldPosition);
    vec2 st1 = dFdx(texCoord);
    vec2 st2 = dFdy(texCoord);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
    vec2 texCoord = TextureCoord;

    if(!texturesBoundToModel)
    {
        texCoord *= texMultiplier;
    }

    vec3 albedo = texture(texture_albedo, texCoord).rgb;
    vec3 metallicTex = texture(texture_mr, texCoord).rgb;
    vec3 roughnessTex = texture(texture_mr, texCoord).rgb;
    vec3 aoTex = texture(texture_ao, texCoord).rgb;
	vec3 N = getNormalFromMap(texCoord);

    float ao;
    float metallic;
    float roughness;

    if(texturesBoundToModel)
    {
        // This assumes that the texture setup automatically is ORM
        // Similar to the ModelTexture.frag scene
        ao = aoTex[0];
        metallic = metallicTex[1];
        roughness = roughnessTex[2];
    }
    else
    {
        // This assumes the textures differ from ORM
        // thus the user specified which channels they need to use
        ao = aoTex[aoChannel];

        metallicTex = texture(texture_metallic, texCoord).rgb;
        roughnessTex = texture(texture_roughness, texCoord).rgb;

        metallic = metallicTex[metallicChannel];
        roughness = roughnessTex[roughnessChannel];
    }

    vec3 color;

    switch(viewMode)
    {
    case 1: 
    color = albedo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 
    break;

    case 2:
    color = (N + 1.0) * 0.5;
    break;

    case 3:
    color = vec3(metallic, metallic, metallic);
    break;

    case 4:
    color = vec3(roughness, roughness, roughness);
    break;

    case 5:
    color = vec3(ao, ao, ao);
    break;
    }


    FragColor = vec4(color, 1.0);
}