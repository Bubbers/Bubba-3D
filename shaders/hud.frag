#version 330


precision highp float;

in vec2 texCoord;
in vec2 location;

out vec4 fragmentColor;

uniform sampler2D sprite;
uniform bool isTexture;
uniform bool isColor;
uniform bool isFont;
uniform vec4 color;
uniform vec4 roundedCorners;
uniform float ratioWidthToHeight;
uniform vec4 border;
uniform vec4 borderColor;

float AA = 0.001;

void calculateCornersAndBorders();
float elipseRadius(float angleHorizontal, float horizontalRadius, float verticalRadius);
float antiAliase(float val, float shouldBe);
void calculateAlpha(float dist, float borderRadius, bool inCorner);

void main() {

    if(isTexture){
	    fragmentColor = texture(sprite, texCoord);
        calculateCornersAndBorders();
	}else if(isColor){
	    fragmentColor = color;
        calculateCornersAndBorders();
	}else if(isFont){
	    fragmentColor = vec4(1,1,1,texture(sprite,texCoord).x);
	}


}

void calculateCornersAndBorders(){

    vec2 distance = vec2(location.x < 0.5 ? location.x : 1.0-location.x,
                         location.y < 0.5 ? location.y : 1.0-location.y);
    /*
    layout:
     0 | 1
     - + -
     2 | 3
     */
    int quadrant;
    if(location.x > 0.5){
        quadrant = location.y < 0.5 ? 3 : 1;
    }else{
        quadrant = location.y < 0.5 ? 2 : 0;
    }

    float sizeX,sizeY;
    sizeY = roundedCorners[quadrant];
    sizeX = sizeY/ratioWidthToHeight;

    vec2 borderSize = vec2(mod(quadrant,2) == 0 ? border[3] : border[1], quadrant < 2 ? border[0] : border[2]);

    float dist, borderRadius;
    bool inCorner = false;

    if(distance.x <= sizeX && distance.y <= sizeY){
        inCorner = true;
        vec2 pointFromCenter = vec2(sizeX,sizeY)-distance;
        float angle = atan(pointFromCenter.x/pointFromCenter.y);
        float supposedRadius = elipseRadius(angle,sizeX,sizeY);
        if(sizeX > borderSize.x || sizeY > borderSize.y){
            borderRadius = elipseRadius(angle,sizeX-borderSize.x,sizeY-borderSize.y);
        }else{
            borderRadius = 0;
        }
        borderRadius = supposedRadius - borderRadius;
        dist = supposedRadius - length(pointFromCenter);
    }else{
        if(distance.x < borderSize.x || distance.y < borderSize.y){
            fragmentColor = borderColor;
        }
    }

    calculateAlpha(dist,borderRadius,inCorner);

}

float antiAliase(float val, float shouldBe){
    return (val - (shouldBe-AA))*(1/(AA*2));
}

float elipseRadius(float angleHorizontal, float horizontalRadius, float verticalRadius){
    return horizontalRadius*verticalRadius/sqrt(pow(horizontalRadius,2)*pow(cos(angleHorizontal),2) +
                                                pow(verticalRadius,2)*pow(sin(angleHorizontal),2));
}

void calculateAlpha(float dist, float borderRadius, bool inCorner){

    float epsilon = inCorner ? AA : 0;
    if(borderRadius > 0.0001 && dist < borderRadius+epsilon){
        if(dist > borderRadius-epsilon){
            float aa2 = antiAliase(dist,borderRadius);
            fragmentColor = fragmentColor*aa2 + borderColor*(1-aa2);
        }else{
            fragmentColor = borderColor;
        }
    }
    if(dist < epsilon){
        if(dist > -epsilon){
            fragmentColor.w *= antiAliase(dist,0);
        }else{
            discard;
        }
    }

}


/**
* Calculates the coordinate that y should be, given x, for it to be
* a circle. Based on the calculation for a half-circle: y=1-sqrt(1-x*x)
*
float supposedCircleCoordinate(float x, float xShift, float yShift){
    float x2 = x*xShift-1;
    return (1-sqrt(1-x2*x2))/yShift;
}*/

