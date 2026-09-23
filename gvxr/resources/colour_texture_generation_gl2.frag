/**
********************************************************************************
*
*	@file		colour_texture_generation_gl3.frag
*
*	@brief      fragment shader to create a texture.
*
*	@version	1.0
*
*	@date		01/08/2016
*
*	@author		Andréas Meuleman
*
********************************************************************************
*/



//******************************************************************************
//	OpenGL 2.0
//******************************************************************************
#version 110


//******************************************************************************
//	constant variables
//******************************************************************************
//size of the arrays to initialize noises
const int TAB_SIZE = 64;

//Frequence for fractal sum and turbulence
const float MINFREQ = 0.01;
const float MAXFREQ = 1.0;

//step for bump evaluation 
const float STEP = 0.00002;


//******************************************************************************
//	Varying variables
//******************************************************************************
varying vec3 g_normal;
varying vec3 g_object_position;
varying vec2 g_texture_coordinates;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform vec4 g_base_colour;
uniform vec4 g_second_colour;
uniform int g_has_tendon;
uniform int g_is_lung;
uniform float g_texture_frequency;
uniform sampler2D g_bump_map;
uniform int g_perm[TAB_SIZE];
uniform vec3 g_gradient_tab[TAB_SIZE];

//------------------------------------
int index(int ix, int iy, int iz)
//------------------------------------
{
    float tab_size = float(TAB_SIZE);
    int temp0 = int(mod(float(iz), tab_size));
    int temp1 = int(mod(float(iy+g_perm[temp0]), tab_size));
    int temp3 = int(mod(float(ix+g_perm[temp1]), tab_size));
    
	return g_perm[temp3];
}

//------------------------------------
float spline(float x, float knots[4])
//------------------------------------
{
	vec4 v_knots = vec4(knots[0], knots[1], knots[2], knots[3]);
					
	mat4 matrix = mat4(-0.5, 1., -0.5, 0.,
					1.5, -2.5, 0., 1.,
					-1.5, 2., 0.5, 0.,
					0.5, -0.5, 0., 0.);
					
	vec4 c = matrix*v_knots;
	
	x = clamp(x, 0.0, 1.0);
	
	return ((c.x*x + c.y)*x +c.z)*x +c.w;
}


//------------------------------------
float vlattice(int ix, int iy, int iz)
//------------------------------------
{
	return g_gradient_tab[index(ix,iy,iz)].z;
}

//Value noise
//------------------------------------
float vnoise(vec3 v)
//------------------------------------
{
	float x = v.x;
	float y = v.y;
	float z = v.z;

	int ix, iy, iz;
	int i, j, k;
	float fx, fy, fz;
	float xknots[4], yknots[4], zknots[4];
	
	ix = int(floor(x));
	fx = x - float(ix);
	
	iy = int(floor(y));
	fy = y - float(iy);

	iz = int(floor(z));
	fz = z - float(iz);

	for(k = -1; k <= 2; k++)
	{
		for(j = -1; j <= 2; j++)
		{
			for(i = -1; i <= 2; i++)
			{
				xknots[i+1] = vlattice(ix+i, iy+j, iz+k);
			}
			yknots[j+1] = spline(fx, xknots);
		}
		zknots[k+1] = spline(fy, yknots);
	}
	return spline(fz, zknots)*0.5+0.5;
}

//------------------------------------
float glattice(int ix, int iy, int iz, 
		float fx, float fy, float fz)
//------------------------------------
{
	int base_index = index(ix,iy,iz);
	return g_gradient_tab[base_index].x*fx + g_gradient_tab[base_index].y*fy + g_gradient_tab[base_index].z*fz;
}

//------------------------------------
float lerp(float t, float x0, float x1)
//------------------------------------
{
	return (x0 + t*(x1-x0));
}

//------------------------------------
float smoothStep(float x)
//------------------------------------
{
		return (x * x * (3.0 - 2.0 * x));
}

//gradient noise using Perlin's method
//------------------------------------
float gnoise(vec3 v)
//------------------------------------
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	
	int ix, iy, iz;
	float fx0, fx1, fy0, fy1, fz0, fz1;
	float wx, wy, wz;
	float vx0, vx1, vy0, vy1, vz0, vz1;
 
	ix = int(floor(x));
	fx0 = x - float(ix);
	fx1 = fx0 - 1.0;
	wx = smoothStep(fx0);
	
	iy = int(floor(y));
	fy0 = y - float(iy);
	fy1 = fy0 - 1.0;
	wy = smoothStep(fy0);
	
	iz = int(floor(z));
	fz0 = z - float(iz);
	fz1 = fz0 - 1.0;
	wz = smoothStep(fz0);
	
	vx0 = glattice(ix, iy, iz, fx0, fy0, fz0);
	vx1 = glattice(ix+1, iy, iz, fx1, fy0, fz0);
	vy0 = lerp(wx, vx0, vx1);	
	vx0 = glattice(ix, iy+1, iz, fx0, fy1, fz0);
	vx1 = glattice(ix+1, iy+1, iz, fx1, fy1, fz0);
	vy1 = lerp(wx, vx0, vx1);
	vz0 = lerp(wy, vy0, vy1);
	vx0 = glattice(ix, iy, iz+1, fx0, fy0, fz1);
	vx1 = glattice(ix+1, iy, iz+1, fx1, fy0, fz1);
	vy0 = lerp(wx, vx0, vx1);
	vx0 = glattice(ix, iy+1, iz+1, fx0, fy1, fz1);
	vx1 = glattice(ix+1, iy+1, iz+1, fx1, fy1, fz1);
	vy1 = lerp(wx, vx0, vx1);
	vz1 = lerp(wy, vy0, vy1);
	
	return lerp(wz, vz0, vz1);
}

//combine value noise and gradient noise
//------------------------
float noise(vec3 v)
//------------------------
{
	return (gnoise(v) + vnoise(v))*0.5;
}

//fractal sum uses gradient noise
//------------------------
float fractalSum(vec3 Q)
//------------------------
{
	float value = 0.0;
	for(float f = MINFREQ; f < MAXFREQ; f *= 2.0)
	{
		value += gnoise(Q*f)/f;	
	}
	return value * 2.0 * MINFREQ + 0.5;
}

//turbulence uses value noise + gradient noise
//------------------------
float turbulence(vec3 Q)
//------------------------
{
	float value = 0.0;
	for (float f = MINFREQ; f < MAXFREQ; f *= 2.0)
	{
		value += abs(noise(Q*f))/f;
		return value * 2.0 * MINFREQ + 0.5;
	}
}

//------------------------
vec3 computeInterpoledNormalVector(vec3 first_vector, vec3 second_vector,float a,float b, float pos)
//------------------------
{
	float h = b-a;
	float first_rate = clamp(pos - a, 0.0, h);
	return (first_rate*first_vector+(h-first_rate)*second_vector)/h;
}


//------------------------
vec4 computeMuscleColour()
//------------------------
{
	float bump_value = 1.0 - clamp(length(texture2D(g_bump_map, g_texture_coordinates).rgb * 2. - 1.), 0.0, 1.0);
	//bump_value+=0.7;
	bump_value*=bump_value;
	
	return bump_value * g_base_colour + (1.0 - bump_value) * g_second_colour;
}


//------------------------
vec4 computeTendonColour()
//------------------------
{
    vec3 normal_vector   = normalize(g_normal);
	
	float dot_normal_vertical = abs(dot(normal_vector , vec3(0.0, 0.0, 1.0)));
	
	if(dot_normal_vertical>0.8)
	{
		//if on top or at the bottom, it is tendon
		return g_second_colour;
	}
	else 
	{
    	//modify the normal vector using the bump map   	
    	vec3 deltaNormal = texture2D(g_bump_map, g_texture_coordinates).rgb*2.-1.;
	    	    		
		vec3 bumped_normal_vector = normalize(g_normal+deltaNormal);
		
		//linear interpolation of the normal vectors
		float normal_rate = clamp(dot_normal_vertical-0.7, 0.0, 0.1);
		vec3 interpoled_normal_vector = (normal_rate*normal_vector+(0.1 - normal_rate) * bumped_normal_vector) * 10.0;
				
		dot_normal_vertical = abs(dot(interpoled_normal_vector , vec3(0.0, 0.0, 1.0)));
		
		//if on the sides, it is muscle and interpolate in between
		float tendon_rate = clamp(dot_normal_vertical - 0.7, 0.0, 0.1);
		return 10.0 * (computeMuscleColour()*(0.1-tendon_rate)+g_second_colour*tendon_rate);
	}	
}

//-------------
void main(void)
//-------------
{
	if(g_has_tendon==1)
	{
		gl_FragColor=computeTendonColour();
	}
	else
	{
		if(g_is_lung==1)
		{
			vec3 object_position = g_object_position * g_texture_frequency * 0.8;
			float noise_value = noise(object_position)*0.5+0.5;
			
			//lower the noise and make it sharper
			noise_value*=pow(noise_value,4.0);
			
			vec4 second_colour=g_second_colour*noise_value;
			gl_FragColor=(g_base_colour*(1.0-noise_value)+second_colour)*(turbulence(object_position)*0.4+0.6);
		}
		else
		{
			gl_FragColor=(g_base_colour*fractalSum(g_object_position));
		}
	}
}
