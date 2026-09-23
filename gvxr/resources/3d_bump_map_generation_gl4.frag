/**
********************************************************************************
*
*   @file       3d_bump_map_generation_gl4.frag
*
*   @brief      Fragment shader to create a bump map.
*               The shader is written in GLSL version 450, and is designed for
*               OpenGL 4.5.
*
*   @version    1.3
*
*   @date       26/02/2020
*
*   @author     Andréas Meuleman
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Feb 2020, 2020, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
// OpenGL 4.5
//******************************************************************************
#version 450 core
precision highp float; // Video card drivers require this line to function properly


//******************************************************************************
// constant variables
//******************************************************************************
//size of the arrays to initialize noises
const int TAB_SIZE = 64;


//******************************************************************************
// Input
//******************************************************************************
layout(location=0) smooth in vec3 g_object_position;

//******************************************************************************
// Output
//******************************************************************************
layout(location=0) out float frag_colour;


//******************************************************************************
// Uniform variables
//******************************************************************************
layout(location=0) uniform int g_has_thin_bumps;
layout(location=1) uniform int g_perm[TAB_SIZE];
layout(location=65) uniform float g_bump_frequency;
layout(location=66) uniform vec3 g_gradient_tab[TAB_SIZE];


//-------------------------------
int index(int ix, int iy, int iz)
//-------------------------------
{
    return g_perm[(ix + g_perm[(iy + g_perm[iz % TAB_SIZE]) % TAB_SIZE]) % TAB_SIZE];
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

//-------------
void main(void)
//-------------
{
    frag_colour = noise(g_object_position*g_bump_frequency*2)*0.1;
    frag_colour *= frag_colour;

    if(g_has_thin_bumps == 1)
    {
        frag_colour = 1 - frag_colour;
    }
}
