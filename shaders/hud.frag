#version 330


precision highp float;

in vec2 texCoord;
in vec2 distance;

out vec4 fragmentColor;

uniform sampler2D sprite;
uniform bool isTexture;
uniform bool isColor;
uniform bool isFont;
uniform vec4 color;
uniform vec2 roundedCorners;

void calculateRoundedEdges(float sizeX,float sizeY, float antiAliasing);
float supposedCircleCoordinate(float x, float xShift, float yShift);
void calculateAlphaBasedOn(float y, float yShould, float antiAliasing);

void main() {

    if(isTexture){
	    fragmentColor = texture(sprite, texCoord);
        calculateRoundedEdges(roundedCorners.x,roundedCorners.y,0.001);
	}else if(isColor){
	    fragmentColor = color;
        calculateRoundedEdges(roundedCorners.x,roundedCorners.y,0.001);
	}else if(isFont){
	    fragmentColor = vec4(1,1,1,texture(sprite,texCoord).x);
	}


}

/**
* Discards pixels which are outside of the rounded corners.
* pixels which are +- antiAliasing from the border are given
* an antiAliased value.
*/
void calculateRoundedEdges(float sizeX, float sizeY, float antiAliasing){

    float distancex = distance.x < 0.5 ? distance.x : 1.0-distance.x;
    float distancey = distance.y < 0.5 ? distance.y : 1.0-distance.y;
    float xTerm = 1/sizeX;
    float yTerm = 1/sizeY;

    if(distancex < sizeX && distancey < sizeY){
        // If we always base the calculation on one axis, we could only
        // anti-alias in the other axis.
        if(distancey < distancex){
            float yShould = supposedCircleCoordinate(distancex,xTerm,yTerm);
            calculateAlphaBasedOn(distancey,yShould,antiAliasing);
        }else{
            float xShould = supposedCircleCoordinate(distancey,yTerm,xTerm);
            calculateAlphaBasedOn(distancex,xShould,antiAliasing);
        }
    }
}

/**
* Calculates the coordinate that y should be, given x, for it to be
* a circle. Based on the calculation for a half-circle: y=1-sqrt(1-x*x)
*/
float supposedCircleCoordinate(float x, float xShift, float yShift){
    float x2 = x*xShift-1;
    return (1-sqrt(1-x2*x2))/yShift;
}

/**
* Blends the alpha value to the desired value based on what the coordinate y
* should be and what it is.
*/
void calculateAlphaBasedOn(float y, float yShould, float antiAliasing){
    if(y < yShould+antiAliasing){
        if(y > yShould-antiAliasing){
            fragmentColor.w *= (y - (yShould-antiAliasing))*(1/(antiAliasing*2));
        }else{
            discard;
        }
    }
}