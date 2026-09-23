/**
********************************************************************************
*
*	@file		bump_map_generation_gl3.frag
*
*	@brief      fragment shader to create a bump map.
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
//	OpenGL 3.2
//******************************************************************************
#version 150


//******************************************************************************
//	constant variables
//******************************************************************************
//Frequence for fractal sum
const float MINFREQ=0.1;
const float MAXFREQ=1.;

//position of the line
const float LINE_POS = 0.35;
const float LINE_SHARP = 200.;

//coefitient of the perturbation
const float PERTURBATION_COEF = 0.005;

//frequency of the lines on the muscle (if has tendon)
const float LINE_FREQ = 50.;

//step for bump evaluation 
const float STEP=0.02;

//size of the arrays to initialize noises
const int TAB_SIZE = 64;


//******************************************************************************
//	Input
//******************************************************************************
smooth in vec3 g_normal;
smooth in vec3 g_object_position;

//******************************************************************************
//	Output
//******************************************************************************
out vec3 frag_colour;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform int g_has_tendon;
uniform int g_is_lung;
uniform int g_has_thin_bumps;
uniform mat4 g_matrix;
uniform mat4 g_right_line_matrix;
uniform mat4 g_side_lines_matrix;
uniform vec3 g_max_corner;
uniform vec3 g_min_corner;
uniform int g_perm[TAB_SIZE];
uniform vec3 g_gradient_tab[TAB_SIZE];

//------------------------------------
int index(int ix, int iy, int iz)
//------------------------------------
{
	return g_perm[(ix+g_perm[(iy+g_perm[iz%TAB_SIZE])%TAB_SIZE])%TAB_SIZE];	
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
	
	x = clamp(x, 0, 1);
	
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
	fx = x - ix;
	
	iy = int(floor(y));
	fy = y - iy;

	iz = int(floor(z));
	fz = z - iz;

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
		return (x*x*(3 - 2*x));
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
	fx0 = x - ix;
	fx1 = fx0 -1;
	wx = smoothStep(fx0);
	
	iy = int(floor(y));
	fy0 = y - iy;
	fy1 = fy0 -1;
	wy = smoothStep(fy0);
	
	iz = int(floor(z));
	fz0 = z - iz;
	fz1 = fz0 -1;
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

//fractal sum uses value noise + gradient noise
//------------------------
float fractalSum(vec3 Q)
//------------------------
{
	float value = 0;
	for(float f=MINFREQ; f<MAXFREQ; f*=2)
	{
		value += noise(Q*f)/f;	
	}
	return value*2*MINFREQ+0.5;
}

//calculate the height of the bump
//------------------------
float computeBumpValue(vec3 object_position)
//------------------------
{
	float noise_value;
	
	if(g_has_thin_bumps == 1)
	{
		noise_value = noise(object_position*2.)*0.5+0.5;
		noise_value *= 0.2;
	}
	else
	{
		noise_value = noise(object_position)*0.5+0.5;
	}
	
	//lower the noise
	float bump_value = noise_value*0.15;
	
	if(g_has_tendon == 1)
	{
		//create perturbation using noise and fractal sum
		float perturbation = noise_value + fractalSum(object_position);
	
		//lower the perturbation
		perturbation *= PERTURBATION_COEF;
		
		//angle of the point to the center
		float theta = atan(object_position.y,object_position.x+length(object_position.xy));
 
		float horizontal = clamp(abs(dot(normalize(g_normal),vec3(0,0,1))), 0., 1.);
		
		//reduce bump from noise
		bump_value *= horizontal*horizontal*horizontal;
		
		//Creates lines disturbed by fractal sum and noise
		bump_value+=0.1*(1-horizontal)*
			(1-(pow(abs(sin(5*LINE_FREQ*(theta+perturbation))),14)+
			0.6*pow(abs(sin(7*LINE_FREQ*(theta+perturbation))),10))+
			0.4*pow(abs(sin(9*LINE_FREQ*(theta+perturbation))),8));
	}
	
	if(g_is_lung == 1)
	{
		//create perturbation using fractal sum
		float perturbation = fractalSum(object_position);
	
		//lower the perturbation
		perturbation *= PERTURBATION_COEF;
		
		//change the global position
		perturbation += g_gradient_tab[0].z*0.05;
		
		vec3 rotated_for_right_line_position = (g_right_line_matrix*vec4(object_position,1.)).xyz;
		vec3 rotated_for_side_lines_position = (g_side_lines_matrix*vec4(object_position,1.)).xyz;
				
		float height;
		
		//if the fragment is on the right, the lung has a central line
		if((object_position.x - g_min_corner.x)/(g_max_corner.x - g_min_corner.x)>0.5)
		{
			//The height is 0 where the line is
			height = clamp(abs(LINE_SHARP*(rotated_for_right_line_position.z - g_min_corner.z)/(g_max_corner.z - g_min_corner.z)-
							LINE_SHARP*(LINE_POS+perturbation)),0.,1.);
						
			//add another change to the position of the line
			perturbation += g_gradient_tab[0].y*0.05+0.25;
					
			//draw the line on the side
			height *= clamp(abs(LINE_SHARP*(rotated_for_side_lines_position.z - g_min_corner.z)/(g_max_corner.z - g_min_corner.z)-
						LINE_SHARP*(LINE_POS+perturbation)),0.,1.);
			
		}
		//if not, it has only a line on the side
		else
		{
			//add another change to the position of the line and draw it higher
			perturbation -= g_gradient_tab[0].z*0.05-0.2;
								
			//draw the line on the side
			height = clamp(abs(LINE_SHARP*(rotated_for_side_lines_position.z - g_min_corner.z)/(g_max_corner.z - g_min_corner.z)-
						LINE_SHARP*(LINE_POS+perturbation)),0.,1.);
		}
		//square to make it sharper
		height = 1.-(1.-height)*(1.-height);

		bump_value *= bump_value;
		
		//add the line
		bump_value += clamp(height,0.,0.85);
	}
	return bump_value;
}


//-------------
void main(void)
//-------------
{
	vec3 U = cross(g_normal,vec3(1., 0., 0.));
	vec3 V = cross(g_normal,vec3(0., 1., 0.));
	vec3 W = cross(g_normal,vec3(0., 0., 1.));

	vec3 deltaNormal = U*(computeBumpValue(g_object_position-STEP*U)
			- computeBumpValue(g_object_position+STEP*U))/(STEP);

	deltaNormal += V*(computeBumpValue(g_object_position-STEP*V)
			- computeBumpValue(g_object_position+STEP*V))/(STEP);

	deltaNormal += W*(computeBumpValue(g_object_position-STEP*W)
			- computeBumpValue(g_object_position+STEP*W))/(STEP);

	frag_colour = deltaNormal*0.5+0.5;
}
