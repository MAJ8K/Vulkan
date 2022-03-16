
struct VSOutput{
    float4 position: POSITION;
    float4 color: COLOR;
}

struct PSOutput{
    float4 color: COLOR;
}

PSOutput main(VSOutput input){
    PSOutput output;
    output.color = input.color;
    return output;
}